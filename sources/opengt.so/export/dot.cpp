
#include <iostream>
#include "../opengt.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportToDOT(Graph& G, ostream& os)
        {
            if(G.IsHypergraph())
                throw "The DOT fileformat doesn\'t support hypergraphs\n";

            os << "// www.open-graphtheory.org\n";
            os << "graph ogt2dot_" << G.GetID() << " {\n";

            /// write vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                os << "\tv" << v.GetID() << " [ label=\"" << v.GetLabel() << "\", "
                   << "pos=\"" << v.GetX() << "," << v.GetY() << "\" ];\n";
            }

            /// write edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                if(e.IsEdge())
                {
                    os << "\tv" << e.From().GetID() << " -- v" << e.To().GetID()
                       << " [ label=\"" << e.GetLabel() << "\" dir=\"none\" ];\n";
                }
                else
                {
                    os << "\tv" << e.From().GetID() << " -> v" << e.To().GetID()
                       << " [ label=\"" << e.GetLabel() << "\" dir=\"forward\" ];\n";
                }
            }

            os << "}\n";
        }

    }
}
