
#include "../../Headers/export/gml.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterGML::Export(Graph& G, ostream& os, VertexColoring& vertexcoloring,
                                     EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            if(G.IsHypergraph())
                throw "The GML fileformat doesn\'t support hypergraphs\n";

            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = (*v)->GetCoordinates();
                if(coordinates.size() < 2)
                    throw "Vertex with less than 2 coordinates found";
            }

            StringTranslator Translator; // should be replaced with an appropriate translator.

            os << "graph [ label \"" << Translator.Translate(G.GetLabel()) << "\"\n";
            os << "  comment \"www.Open-Graphtheory.org\"\n";

            /// write vertices: node [ id label graphics [ x y ] ]
            for(VertexIterator vi = G.BeginVertices(); vi != G.EndVertices(); vi++)
            {
                Vertex* v = *vi;
                vector<float> coordinates = v->GetCoordinates();
                os << "  node [ id " << v->GetID() << " label \"" << Translator.Translate(v->GetLabel()) << "\"\n";
                os << "    graphics [ x " << coordinates[0] << " y " << coordinates[1] << " ]\n";
                os << "  ]\n";
            }

            /// write edges: edge [ source target label graphics [ type line ] ]
            for(EdgeIterator ei = G.BeginEdges(); ei != G.EndEdges(); ei++)
            {
                Edge* e = *ei;
                os << "  edge [ source " << e->From()->GetID() << " target " << e->To()->GetID()
                   << " label \"" << Translator.Translate(e->GetLabel()) << "\"\n";
                os << "    graphics [ type \"" << (e->IsEdge()?"line":"arc") << "\" ]\n";
                os << "  ]\n";
            }

            os << "]\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterGML::ExportFilterGmlRegistrator(&ExportFilter::ExportFilterFactory, "gml",
            new DefaultInstantiator<ExportFilter, ExportFilterGML>("gml", "Graph Modelling Language",
                "http://www.fim.uni-passau.de/en/fim/faculty/chairs/theoretische-informatik/projects.html"));

    }
}
