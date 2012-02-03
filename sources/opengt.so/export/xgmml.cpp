
#include <iostream>
#include "../opengt.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportToXGMML(Graph& G, ostream& os)
        {
            if(G.IsHypergraph())
                throw "The XGMML fileformat does not support hypergraphs\n";

            os << "<?xml version=\"1.0\"?>\n";
            os << "<!DOCTYPE graph PUBLIC \"-//John Punin//DTD graph description//EN\" \"http://www.cs.rpi.edu/~puninj/XGMML/xgmml.dtd\">\n";
            os << "<!-- www.Open-Graphtheory.org -->\n";

            os << "<graph id=\"" << G.GetID() << "\" label=\"" << G.GetLabel() << "\">\n";

            /// write vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                os << "\t<node id=\"" << v.GetID() << "\" label=\"" << v.GetLabel()
                   << "\" weight=\"" << v.GetWeight() << "\">\n";
                os << "\t\t<graphics x=\"" << v.GetX() << "\" y=\"" << v.GetY() << "\"/>\n";
                os << "\t</node>\n";
            }

            /// write edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << "\t<edge source=\"" << e.From().GetID() << "\" target=\""<< e.To().GetID()
                   << "\" weight=\"" << e.GetWeight() << "\" label=\"" << e.GetLabel() << "\">\n";
                os << "\t\t<graphics type=\"" << (e.IsEdge()?"line":"arc") << "\"/>\n";
                os << "\t</edge>\n";
            }

            os << "</graph>\n";
        }

    }
}
