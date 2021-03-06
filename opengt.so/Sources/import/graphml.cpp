
#include "../../Headers/import/graphml.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::XML;

namespace OpenGraphtheory
{
    namespace Import
    {

        FactoryRegistrator<ImportFilter> ImportFilterGRAPHML::ImportFilterGRAPHMLRegistrator(
            &ImportFilter::ImportFilterFactory, "graphml", new DefaultInstantiator<ImportFilter, ImportFilterGRAPHML>(
                "graphml", "GraphML", "http://graphml.graphdrawing.org/"));


        // hyperedge/endpoint type="in" or type="out" or type="undir" (default: undir)


        Graph ImportFilterGRAPHML::Import(istream& is)
        {
            OpenGraphtheory::XML::XML* root = new OpenGraphtheory::XML::XML;
            is >> (*root);

            list<OpenGraphtheory::XML::XML*> graphmlnodes = root->FindChildren("graphml");
            if(graphmlnodes.size() > 1)
                throw "XML Document must have exactly 1 top element \"graphml\"";
            if(graphmlnodes.size() < 1)
                throw "XML Document contains no element \"graphml\" (document possibly empty)";
            OpenGraphtheory::XML::XML* graphmlnode = graphmlnodes.front();
            list<OpenGraphtheory::XML::XML*> graphnodes = graphmlnode->FindChildren("graph");
            OpenGraphtheory::XML::XML* graphnode = graphnodes.front();


            Graph result;
            map<string, Vertex*> Vertex_XML_ID_to_pointer;

            // 0 is the default for directed, by the XGMML standard
            string edgedefault = graphnode->GetAttribute("edgedefault", "");
            bool defaultdirected;
            if(edgedefault == "directed")
                defaultdirected = true;
            else if(edgedefault == "undirected")
                defaultdirected = false;
            else
                throw "graph attribute \"edgedefault\" must be \"directed\" or \"undirected\"";



            /// load vertices
            list<OpenGraphtheory::XML::XML*> nodes = graphnode->FindChildren("node");
            for(list<OpenGraphtheory::XML::XML*>::iterator node = nodes.begin(); node != nodes.end(); node++)
            {
                /// create vertex
                Vertex* v = *(result.AddVertex());

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
                map<string,Vertex*>::iterator from = Vertex_XML_ID_to_pointer.find(xmlFrom);
                string xmlTo = (*edge)->GetAttribute("target", "");
                map<string,Vertex*>::iterator to = Vertex_XML_ID_to_pointer.find(xmlTo);
                if(from == Vertex_XML_ID_to_pointer.end() || to == Vertex_XML_ID_to_pointer.end())
                    throw "edge with reference to nonexisting node-id";

                /// create edge
                bool Directed = defaultdirected;
                string directed = (*edge)->GetAttribute("directed", "");
                if(directed == "true")
                    Directed = true;
                else if(directed == "false")
                    Directed = false;

                EdgeIterator e;
                if(Directed)
                    e = result.AddArc(from->second, to->second);
                else
                    e = result.AddEdge(from->second, to->second);

                /// assign attributes
                /*
                e.SetLabel((*edge)->GetAttribute("label",""));
                */
            }

            delete root;
            return result;
        }

    }
}
