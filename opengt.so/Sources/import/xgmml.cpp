
#include "../../Headers/import/xgmml.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::XML;

namespace OpenGraphtheory
{
    namespace Import
    {

        FactoryRegistrator<ImportFilter> ImportFilterXGMML::ImportFilterXGMMLRegistrator(
            &ImportFilter::ImportFilterFactory, "xgmml", new DefaultInstantiator<ImportFilter, ImportFilterXGMML>(
                "xgmml", "eXtensible Graph Markup and Modeling Language","http://cgi5.cs.rpi.edu/research/groups/pb/punin/public_html/XGMML/"));


        Graph ImportFilterXGMML::Import(istream& is)
        {
			OpenGraphtheory::XML::XML* root = new OpenGraphtheory::XML::XML;
			is >> (*root);

		    list<OpenGraphtheory::XML::XML*> graphnodes = root->FindChildren("graph");
			if(graphnodes.size() > 1)
				throw "XML Document must have exactly 1 top element \"graph\"";
			if(graphnodes.size() < 1)
				throw "XML Document contains no element \"graph\" (document possibly empty)";
            OpenGraphtheory::XML::XML* graphnode = graphnodes.front();


            Graph result;
            map<string, Graph::VertexIterator*> Vertex_XML_ID_to_pointer;

            // 0 is the default for directed, by the XGMML standard
            bool Directed = (graphnode->GetAttribute("directed", "0") == "1");

			/// load vertices
			list<OpenGraphtheory::XML::XML*> nodes = graphnode->FindChildren("node");
			for(list<OpenGraphtheory::XML::XML*>::iterator node = nodes.begin(); node != nodes.end(); node++)
			{
				/// create vertex
				Graph::VertexIterator *v = new Graph::VertexIterator(result.AddVertex());

				v->SetLabel((*node)->GetAttribute("label",""));
				string sWeight = (*node)->GetAttribute("weight","0");
				stringstream s;
				s << sWeight;
				float Weight = 0;
				s >> Weight;
				v->SetWeight(Weight);

				/// assign attributes
				list<OpenGraphtheory::XML::XML*> attrs = (*node)->FindChildren("att");
				for(list<OpenGraphtheory::XML::XML*>::iterator attr = attrs.begin(); attr != attrs.end(); attr++)
				{
                    string name = (*attr)->GetAttribute("name", "");
                    string value = (*attr)->GetAttribute("value", "");
                    v->Attributes().Unset(name);
                    v->Attributes().Add(name, "string");

                    Attribute* pAttr = v->Attributes().GetAttribute(name);
                    StringAttribute* sAttr = dynamic_cast<StringAttribute*>(pAttr);
                    if(sAttr != NULL)
                        sAttr->Value = value;
                }

				/// assign XML-ID
				string id = (*node)->GetAttribute("id", "");
				if(id == "")
					throw "Illegal Structure"; // illegal or no ID
				if(Vertex_XML_ID_to_pointer.find(id) != Vertex_XML_ID_to_pointer.end())
					throw "multiple nodes with same id"; // same ID twice
				Vertex_XML_ID_to_pointer[id] = v;
			}


			/// load edges
			list<OpenGraphtheory::XML::XML*> edges = graphnode->FindChildren("edge");
			for(list<OpenGraphtheory::XML::XML*>::iterator edge = edges.begin(); edge != edges.end(); edge++)
			{
				string xmlFrom = (*edge)->GetAttribute("source", "");
                map<string,Graph::VertexIterator*>::iterator from = Vertex_XML_ID_to_pointer.find(xmlFrom);
				string xmlTo = (*edge)->GetAttribute("target", "");
                map<string,Graph::VertexIterator*>::iterator to = Vertex_XML_ID_to_pointer.find(xmlTo);
                if(from == Vertex_XML_ID_to_pointer.end() || to == Vertex_XML_ID_to_pointer.end())
					throw "edge with reference to nonexisting node-id";

				/// create edge
				Graph::EdgeIterator e;
				if(Directed)
                    e = result.AddArc(*(from->second), *(to->second));
                else
                    e = result.AddEdge(*(from->second), *(to->second));

                /// assign attributes
				e.SetLabel((*edge)->GetAttribute("label",""));
            }

            for(map<string, Graph::VertexIterator*>::iterator i = Vertex_XML_ID_to_pointer.begin(); i != Vertex_XML_ID_to_pointer.end(); i++)
                delete i->second;
			delete root;
			return result;
        }

    }
}
