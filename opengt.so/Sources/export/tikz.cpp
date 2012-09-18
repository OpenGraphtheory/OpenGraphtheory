#include "../../Headers/export/tikz.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterTIKZ::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring, map<Graph::EdgeIterator, Color>& edgecoloring, float dpi)
        {
            float radius = 0.5;

            if(G.IsHypergraph())
                throw "The TIKZ export filter does not support hypergraphs\n";

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                if(coordinates.size() < 2)
                    throw "Vertex with less than 2 coordinates found";
            }

            /// header
            os << "% www.Open-Graphtheory.org\n";
            os << "% \\usepackage{tikz}\n";
            os << "\\begin{tikzpicture}\n";

            os << "\\tikzstyle{vertex}=[shape=circle, draw, inner sep=" << radius << "cm]\n";

            /// declare vertices
            Graph::VertexIterator v1 = G.BeginVertices();
            vector<float> coordinates = v1.GetCoordinates();
            float minx = coordinates[0], maxy = coordinates[1];
            for(v1++; v1 != G.EndVertices(); v1++)
            {
                coordinates = v1.GetCoordinates();
                if(coordinates[0] < minx)
                    minx = coordinates[0];
                if(coordinates[1] > maxy)
                    maxy = coordinates[1];
            }
            minx -= radius;
            maxy += radius;

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                coordinates = v.GetCoordinates();
                os << "  \\definecolor{currentcolor}{rgb}{" << (vertexcoloring[v].Red/256.0f) << ","<< (vertexcoloring[v].Green/256.0f) << "," << (vertexcoloring[v].Blue/256.0f) << "}\n";
                os << "  \\node[vertex,fill=currentcolor] (n" << v.GetID() << ") at (" << (coordinates[0] - minx) << "cm, "
                                                                                  << (maxy-coordinates[1]-radius) << "cm){"
                                                                                  << v.GetLabel() << "};\n";
            }
            os << "\n";

            /// draw edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << "  \\definecolor{currentcolor}{rgb}{" << (edgecoloring[e].Red/256.0f) << ","<< (edgecoloring[e].Green/256.0f) << "," << (edgecoloring[e].Blue/256.0f) << "}\n";
                os << "  \\draw[-" << (e.IsEdge() ? "" : ">") << ",currentcolor] (n" << e.From().GetID() << ") -- (n" << e.To().GetID() << ");\n";

                //os << "  label.ulft(btex "<< e.GetLabel() << " etex, (z"<<e.From().GetID()<<"+z"<<e.To().GetID()<<")/2);\n";
            }

            os << "\\end{tikzpicture}\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterTIKZ::ExportFilterTikzRegistrator(&ExportFilter::ExportFilterFactory, "tikz",
            new DefaultInstantiator<ExportFilter, ExportFilterTIKZ>("tikz", "TikZ graphics for LaTeX", "http://en.wikipedia.org/wiki/PGF/TikZ"));

    }
}
