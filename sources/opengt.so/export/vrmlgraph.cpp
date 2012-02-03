
#include <iostream>
#include "../opengt.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportToVRMLGRAPH(Graph& G, ostream& os)
        {
            if(G.IsHypergraph())
                throw "The VRMLgraph fileformat doesn\'t support hypergraphs\n";
            if(G.IsMixed())
                throw "The VRMLgraph fileformat doesn\'t support mixed graphs\n";
            if(G.IsDirected())
                throw "The VRMLgraph fileformat doesn\'t support directed graphs\n";

            os << "# www.Open-Graphtheory.org\n";
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                os << e.From().GetID() << " " << e.To().GetID() << "\n";
        }

    }
}
