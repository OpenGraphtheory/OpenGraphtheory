
#include "../../Headers/import/rgml.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::XML;

namespace OpenGraphtheory
{
    namespace Import
    {

        FactoryRegistrator<ImportFilter> ImportFilterRGML::ImportFilterRGMLRegistrator(
            &ImportFilter::ImportFilterFactory, "rgml", new DefaultInstantiator<ImportFilter, ImportFilterRGML>(
                "rgml", "Resource Description Framework (RDF) Graph Modeling Language",
                "http://www.cs.rpi.edu/research/groups/pb/punin/public_html/RGML/"));


        Graph ImportFilterRGML::Import(istream& is)
        {
			OpenGraphtheory::XML::XML* root = new OpenGraphtheory::XML::XML;
			is >> (*root);

		    list<OpenGraphtheory::XML::XML*> rdfnodes = root->FindChildren("rdf:RDF");
			if(rdfnodes.size() > 1)
				throw "XML Document must have exactly 1 top element \"rdf:RDF\"";
			if(rdfnodes.size() < 1)
				throw "XML Document contains no element \"rdf:RDF\" (document possibly empty)";
            OpenGraphtheory::XML::XML* rdfnode = rdfnodes.front();


            // The RGML Standard doesn't define a default value
            bool Directed = false;
		    list<OpenGraphtheory::XML::XML*> graphnodes = rdfnode->FindChildren("Graph");
		    if(graphnodes.size() > 0)
                Directed = graphnodes.front()->GetAttribute("rgml:directed", "false") == "true";


            Graph result;
            map<string, VertexIterator*> Vertex_XML_ID_to_pointer;

			/// load vertices
			list<OpenGraphtheory::XML::XML*> nodes = rdfnode->FindChildren("Node");
			for(list<OpenGraphtheory::XML::XML*>::iterator node = nodes.begin(); node != nodes.end(); node++)
			{
				/// create vertex
				VertexIterator *vi = new VertexIterator(result.AddVertex());
				Vertex* v = **vi;

                v->SetLabel((*node)->GetAttribute("rgml:label",""));

				/// assign XML-ID
				string id = (*node)->GetAttribute("rdf:ID", "");
				if(id == "")
					throw "Illegal Structure"; // illegal or no ID
                id = "#" + id;
				if(Vertex_XML_ID_to_pointer.find(id) != Vertex_XML_ID_to_pointer.end())
					throw "multiple nodes with same id"; // same ID twice
				Vertex_XML_ID_to_pointer[id] = vi;
			}


			/// load edges
			list<OpenGraphtheory::XML::XML*> edges = rdfnode->FindChildren("Edge");
			for(list<OpenGraphtheory::XML::XML*>::iterator edge = edges.begin(); edge != edges.end(); edge++)
			{
				string xmlFrom = "";
				string xmlTo = "";
				OpenGraphtheory::XML::XML* hyperedge_nodes = NULL;
				for(list<OpenGraphtheory::XML::XML_Element*>::iterator child = (*edge)->children.begin(); child != (*edge)->children.end(); child++)
				{
                    OpenGraphtheory::XML::XML* xchild = dynamic_cast<OpenGraphtheory::XML::XML*>(*child);
                    if(xchild != NULL)
                    {
                        if(xchild->name == "source")
                            xmlFrom = xchild->GetAttribute("rdf:resource","");
                        if(xchild->name == "target")
                            xmlTo = xchild->GetAttribute("rdf:resource","");
                        if(xchild->name == "nodes")
                            hyperedge_nodes = xchild;
                    }
                }

                EdgeIterator e;
                if(hyperedge_nodes == NULL)
                {
                    map<string,VertexIterator*>::iterator from = Vertex_XML_ID_to_pointer.find(xmlFrom);
                    map<string,VertexIterator*>::iterator to = Vertex_XML_ID_to_pointer.find(xmlTo);
                    if(from == Vertex_XML_ID_to_pointer.end() || to == Vertex_XML_ID_to_pointer.end())
                        throw "edge with reference to nonexisting node-id";


                    bool isdirected = Directed;
                    string sisdirected = (*edge)->GetAttribute("directed", "");
                    if(sisdirected != "")
                        isdirected = sisdirected == "true";

                    /// create edge
                    if(isdirected)
                        e = result.AddArc(*(from->second), *(to->second));
                    else
                        e = result.AddEdge(*(from->second), *(to->second));
                }
                else // Hyperedge
                {
                    list<VertexIterator> IncidentVertices;
                    list<OpenGraphtheory::XML::XML*> seqs = hyperedge_nodes->FindChildren("rdf:Seq");
                    for(list<OpenGraphtheory::XML::XML*>::iterator seq = seqs.begin(); seq != seqs.end(); seq++)
                    {
                        list<OpenGraphtheory::XML::XML*> lis = (*seq)->FindChildren("rdf:li");
                        for(list<OpenGraphtheory::XML::XML*>::iterator li = lis.begin(); li != lis.end(); li++)
                        {
                            string nodeid = (*li)->GetAttribute("rdf:resource","");
                            map<string,VertexIterator*>::iterator nodeit = Vertex_XML_ID_to_pointer.find(nodeid);
                            if(nodeit == Vertex_XML_ID_to_pointer.end())
                                throw "edge with reference to nonexisting node-id";
                            IncidentVertices.push_back(*(nodeit->second));
                        }
                    }

                    e = result.AddEdge(IncidentVertices);
                }

                (*e)->SetLabel((*edge)->GetAttribute("rgml:label",""));
            }

            for(map<string, VertexIterator*>::iterator i = Vertex_XML_ID_to_pointer.begin(); i != Vertex_XML_ID_to_pointer.end(); i++)
                delete i->second;
			delete root;
			return result;
        }

    }
}
