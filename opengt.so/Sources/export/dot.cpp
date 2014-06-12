
#include "../../Headers/export/dot.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterDOT::Export(Graph& G, ostream& os, VertexColoring& vertexcoloring,
                                     EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            if(G.IsHypergraph())
                throw "The DOT fileformat doesn\'t support hypergraphs\n";

            StringTranslator Translator; // should be replaced with an appropriate translator

            os << "// www.open-graphtheory.org\n";
            os << "graph gxl2dot {\n";

            /// write vertices
            os << std::setfill('0');
            for(VertexIterator vi = G.BeginVertices(); vi != G.EndVertices(); vi++)
            {
                Vertex* v = *vi;

                Visualization::Color col = Visualization::Color(0,0,0);
                if(vertexcoloring.find(v) != vertexcoloring.end())
                    col = vertexcoloring[v];

                os << "\tv" << v->GetID() << " [ label=\"" << Translator.Translate(v->GetLabel()) << "\","
                                         << " width=\"" << (vertexradius >= 0 ? (vertexradius/2.54) : (v->GetWeight()/2.54)) << "\"," // dot uses inches, gxl uses cm
                                         << " color=\"#" << std::hex << std::setw(2) << col.Red
                                                                     << std::setw(2) << col.Green
                                                                     << std::setw(2) << col.Blue << std::dec << "\"";
                Coordinates coordinates = v->GetCoordinates(0);
                if(coordinates.size() > 1)
                {
                    os << ", pos=\"" << coordinates[0];
                    for(unsigned int i = 1; i < coordinates.size(); i++)
                        os << "," << coordinates[i];
                    os << "\"";
                }

                os << " ];\n";
            }

            /// write edges
            for(EdgeIterator ei = G.BeginEdges(); ei != G.EndEdges(); ei++)
            {
                Edge* e = *ei;
                Visualization::Color col = Visualization::Color(0,0,0);
                if(edgecoloring.find(e) != edgecoloring.end())
                    col = edgecoloring[e];

                if(e->IsEdge())
                {
                    os << "\tv" << e->From()->GetID() << " -- v" << e->To()->GetID()
                       << " [ label=\"" << Translator.Translate(e->GetLabel())
                       << "\" penwidth=\"" << (edgewidth>=0 ? (edgewidth/2.54) : (e->GetWeight()/2.54))
                       << "\" color=\"#" << std::hex << std::setw(2) << col.Red
                                                     << std::setw(2) << col.Green
                                                     << std::setw(2) << col.Blue << std::dec
                       << "\" dir=\"none\" ];\n";
                }
                else
                {
                    os << "\tv" << e->From()->GetID() << " -> v" << e->To()->GetID()
                       << " [ label=\"" << Translator.Translate(e->GetLabel())
                       << "\" penwidth=\"" << (edgewidth>=0 ? (edgewidth/2.54) : (e->GetWeight()/2.54))
                       << "\" color=\"#" << std::hex << std::setw(2) << col.Red
                                                     << std::setw(2) << col.Green
                                                     << std::setw(2) << col.Blue << std::dec
                       << "\" dir=\"forward\" ];\n";
                }
            }

            os << "}\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterDOT::ExportFilterDotRegistrator(&ExportFilter::ExportFilterFactory, "dot",
            new DefaultInstantiator<ExportFilter, ExportFilterDOT>("dot", "Fileformat of the Graphviz project", "http://www.graphviz.org/doc/info/lang.html"));

    }
}
