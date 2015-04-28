
#include "../../Headers/import/tei.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::XML;

namespace OpenGraphtheory
{
    namespace Import
    {

        FactoryRegistrator<ImportFilter> ImportFilterTEI::ImportFilterTEIRegistrator(
            &ImportFilter::ImportFilterFactory, "tei", new DefaultInstantiator<ImportFilter, ImportFilterTEI>(
                "tei", "Text Encoding Initiative", "http://www.tei-c.org/release/doc/tei-p5-doc/de/html/GD.html"));


        Graph ImportFilterTEI::Import(istream& is)
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
            map<string, Vertex*> Vertex_XML_ID_to_pointer;

            // the TEI standard doesn't define a default value for "type", although
            // it is an optional parameter.
            bool Directed = (graphnode->GetAttribute("type", "undirected") == "directed");

            /// load vertices
            list<OpenGraphtheory::XML::XML*> nodes = graphnode->FindChildren("node");
            for(list<OpenGraphtheory::XML::XML*>::iterator node = nodes.begin(); node != nodes.end(); node++)
            {
                /// create vertex
                Vertex* v = *(result.AddVertex());

                /// assign attributes
                for(list<OpenGraphtheory::XML::XML_Element*>::iterator child = (*node)->children.begin(); child != (*node)->children.end(); child++)
                {
                    OpenGraphtheory::XML::XML* xchild = dynamic_cast<OpenGraphtheory::XML::XML*>(*child);
                    if(xchild != NULL && xchild->name == "label")
                        v->SetLabel(xchild->InnerText());
                }

                /// assign XML-ID
                string id = (*node)->GetAttribute("xml:id", "");
                if(id == "")
                    throw "Illegal Structure"; // illegal or no ID
                id = "#" + id;
                if(Vertex_XML_ID_to_pointer.find(id) != Vertex_XML_ID_to_pointer.end())
                    throw "multiple nodes with same id"; // same ID twice
                Vertex_XML_ID_to_pointer[id] = v;
            }


            /// load edges
            list<OpenGraphtheory::XML::XML*> edges = graphnode->FindChildren("arc");
            for(list<OpenGraphtheory::XML::XML*>::iterator edge = edges.begin(); edge != edges.end(); edge++)
            {
                string xmlFrom = (*edge)->GetAttribute("from", "");
                map<string,Vertex*>::iterator from = Vertex_XML_ID_to_pointer.find(xmlFrom);
                string xmlTo = (*edge)->GetAttribute("to", "");
                map<string,Vertex*>::iterator to = Vertex_XML_ID_to_pointer.find(xmlTo);
                if(from == Vertex_XML_ID_to_pointer.end() || to == Vertex_XML_ID_to_pointer.end())
                    throw "edge with reference to nonexisting node-id";

                /// create edge
                EdgeIterator e;
                if(Directed)
                    e = result.AddArc(from->second, to->second);
                else
                    e = result.AddEdge(from->second, to->second);

                /// assign attributes
                for(list<OpenGraphtheory::XML::XML_Element*>::iterator child = (*edge)->children.begin(); child != (*edge)->children.end(); child++)
                {
                    OpenGraphtheory::XML::XML* xchild = dynamic_cast<OpenGraphtheory::XML::XML*>(*child);
                    if(xchild != NULL && xchild->name == "label")
                        (*e)->SetLabel(xchild->InnerText());
                }

            }

            delete root;
            return result;
        }

    }
}
