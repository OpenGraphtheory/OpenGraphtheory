
#include <iostream>
#include "../opengt.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportToGASTEX(Graph& G, ostream& os)
        {
            if(G.IsHypergraph())
                throw "The GasTeX fileformat doesn\'t support hypergraphs\n";

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                if(coordinates.size() < 2)
                    throw "Vertex with less than 2 coordinates found";
            }


            os << "% www.Open-Graphtheory.org\n";
            os << "\\begin{picture}(100,100)(0,0)\n";

            /// write vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                os << "\t\\node(n" << v.GetID() << ")(" << coordinates[0] << "," << coordinates[1] << "){" << v.GetLabel() << "}\n";
            }

            /// write edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                os << "\t\\drawedge" << (e.IsEdge()?"[AHnb=0]":"")
                   << "(n" << e.From().GetID() << ",n" << e.To().GetID() << "){" << e.GetLabel() << "}\n";

            os << "\\end{picture}\n";
        }

    }
}
