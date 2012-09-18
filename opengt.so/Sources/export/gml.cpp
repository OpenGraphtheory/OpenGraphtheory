
#include "../../Headers/export/gml.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterGML::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring, map<Graph::EdgeIterator, Color>& edgecoloring, float dpi)
        {
            if(G.IsHypergraph())
                throw "The GML fileformat doesn\'t support hypergraphs\n";

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                if(coordinates.size() < 2)
                    throw "Vertex with less than 2 coordinates found";
            }

            os << "# www.Open-Graphtheory.org\n";
            os << "graph [ id " << G.GetID() << " label \"" << G.GetLabel() << "\"\n";

            /// write vertices: node [ id label graphics [ x y ] ]
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                os << "  node [ id " << v.GetID() << " label \"" << v.GetLabel() << "\"\n";
                os << "    graphics [ x " << coordinates[0] << " y " << coordinates[1] << " ]\n";
                os << "  ]\n";
            }

            /// write edges: edge [ source target label graphics [ type line ] ]
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << "  edge [ source " << e.From().GetID() << " target " << e.To().GetID()
                   << " label \"" << e.GetLabel() << "\"\n";
                os << "    graphics [ type " << (e.IsEdge()?"line":"arc") << " ]\n";
                os << "  ]\n";
            }

            os << "]\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterGML::ExportFilterGmlRegistrator(&ExportFilter::ExportFilterFactory, "gml",
            new DefaultInstantiator<ExportFilter, ExportFilterGML>("gml", "Graph Modeling Language",
                "http://www.fim.uni-passau.de/en/fim/faculty/chairs/theoretische-informatik/projects.html"));

    }
}
