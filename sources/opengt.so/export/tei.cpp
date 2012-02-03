
#include <iostream>
#include "../opengt.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportToTEI(Graph& G, ostream& os)
        {
            if(G.IsHypergraph())
                throw "The TEI fileformat doesn\'t support hypergraphs\n";
            if(G.IsMixed())
                throw "The TEI fileformat doesn\'t support mixed graphs\n";
            bool directed = G.IsDirected();

            os << "<!-- www.Open-Graphtheory.org -->\n";
            os << "<graph xml:id=\"" << G.GetID() << "\" type=\"" << (directed?"":"un") << "directed\">\n";
            os << "  <label>" << G.GetLabel() << "</label>\n";

            /// write vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                os << "  <node xml:id=\"" << v.GetID() << "\">\n";
                os << "    <label>" << v.GetLabel() << "</label>\n";
                os << "  </node>\n";
            }

            /// write edges or arcs (distinguishable only by the "type" attribute of the graph!)
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << "  <arc from=\"#" << e.From().GetID() << "\" to=\"#" << e.To().GetID() << "\">\n";
                os << "    <label>" << e.GetLabel() << "</label>\n";
                os << "  </arc>\n";
            }

            os << "</graph>\n";
        }

    }
}
