#include <iostream>
#include "../opengt.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportToMPOST(Graph& G, ostream& os)
        {
            if(G.IsHypergraph())
                throw "The MPOST fileformat does not support hypergraphs\n";

            /// header
            os << "% www.Open-Graphtheory.org\n";
            os << "transform pagecoords;\n";
            os << "pagecoords:=identity scaled 100mm;\n";
            os << "beginfig(1);\n\n";

            /// declare vertices
            float width=100, height=100;
            Graph::VertexIterator v1 = G.BeginVertices();
            int maxx = v1.GetX(), minx = v1.GetX(), maxy = v1.GetY(), miny = v1.GetY();
            for(v1++; v1 != G.EndVertices(); v1++)
            {
                if(v1.GetX() < minx)
                    minx = v1.GetX();
                if(v1.GetX() > maxx)
                    maxx = v1.GetX();
                if(v1.GetY() < miny)
                    miny = v1.GetY();
                if(v1.GetY() > maxy)
                    maxy = v1.GetY();
            }

            float scaleX = width / (maxx-minx);
            float scaleY = height / (maxy-miny);
            float scale = scaleX < scaleY ? scaleX : scaleY;
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                os << "z" << v.GetID() << " = (" << scale * (v.GetX() - minx) << ","
                                                   << scale * (v.GetY() - miny) << ");\n";
            os << "\n";

            /// draw vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                os << "drawdot z" << v.GetID() << " withpen pencircle"
                   << " scaled " << 2*v.GetWeight() << ";\n";
                os << " label.ulft(btex " << v.GetLabel() << " etex , z" << v.GetID() << ");\n";
            }

            /// draw edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << " draw" << (e.IsEdge()?"":"arrow") << " z" << e.From().GetID() << " -- z" << e.To().GetID() << ";\n";
                os << "  label.ulft(btex "<< e.GetLabel() << " etex, (z"<<e.From().GetID()<<"+z"<<e.To().GetID()<<")/2);\n";
            }

            os << "endfig;\n";
            os << "bye\n";
        }

    }
}
