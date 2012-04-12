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

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                if(coordinates.size() < 2)
                    throw "Vertex with less than 2 coordinates found";
            }

            /// header
            os << "% www.Open-Graphtheory.org\n";
            os << "transform pagecoords;\n";
            os << "pagecoords:=identity scaled 100mm;\n";
            os << "beginfig(1);\n\n";

            /// declare vertices
            float width=100, height=100;
            Graph::VertexIterator v1 = G.BeginVertices();
            vector<float> coordinates = v1.GetCoordinates();
            int maxx = coordinates[0], minx = coordinates[0], maxy = coordinates[1], miny = coordinates[1];
            for(v1++; v1 != G.EndVertices(); v1++)
            {
                coordinates = v1.GetCoordinates();
                if(coordinates[0] < minx)
                    minx = coordinates[0];
                if(coordinates[0] > maxx)
                    maxx = coordinates[0];
                if(coordinates[1] < miny)
                    miny = coordinates[1];
                if(coordinates[1] > maxy)
                    maxy = coordinates[1];
            }

            float scaleX = width / (maxx-minx);
            float scaleY = height / (maxy-miny);
            float scale = scaleX < scaleY ? scaleX : scaleY;
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                coordinates = v.GetCoordinates();
                os << "z" << v.GetID() << " = (" << scale * (coordinates[0] - minx) << ","
                                                 << scale * (coordinates[1] - miny) << ");\n";
            }
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
