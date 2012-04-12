
#include <iostream>
#include "../opengt.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportToGML(Graph& G, ostream& os)
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
                os << "\tnode [ id " << v.GetID() << " label \"" << v.GetLabel() << "\"\n";
                os << "\t\tgraphics [ x " << coordinates[0] << " y " << coordinates[1] << " ]\n";
                os << "\t]\n";
            }

            /// write edges: edge [ source target label graphics [ type line ] ]
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << "\tedge [ source " << e.From().GetID() << " target " << e.To().GetID()
                   << " label \"" << e.GetLabel() << "\"\n";
                os << "\t\tgraphics [ type " << (e.IsEdge()?"line":"arc") << " ]\n";
                os << "\t]\n";
            }

            os << "]\n";
        }

    }
}
