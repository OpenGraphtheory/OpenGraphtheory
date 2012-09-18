#include "../../Headers/export/mpost.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterMPOST::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring, map<Graph::EdgeIterator, Color>& edgecoloring, float dpi)
        {
            float radius = 0.1;

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
            os << "beginfig(1);\n\n";

            /// declare vertices
            Graph::VertexIterator v1 = G.BeginVertices();
            vector<float> coordinates = v1.GetCoordinates();
            float minx = coordinates[0], maxy = coordinates[1], miny = coordinates[1];
            for(v1++; v1 != G.EndVertices(); v1++)
            {
                coordinates = v1.GetCoordinates();
                if(coordinates[0] < minx)
                    minx = coordinates[0];
                if(coordinates[1] < miny)
                    miny = coordinates[1];
                if(coordinates[1] > maxy)
                    maxy = coordinates[1];
            }
            maxy -= miny;


            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                coordinates = v.GetCoordinates();
                os << "z" << v.GetID() << " = (" << (coordinates[0] - minx) + radius << "cm, "
                                                 << (maxy - (coordinates[1] - miny)) + radius << "cm);\n";
            }
            os << "\n";

            /// draw edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << " draw" << (e.IsEdge()?"":"arrow") << " z" << e.From().GetID() << " -- z" << e.To().GetID();
                if(edgecoloring.find(e) != edgecoloring.end())
                    os << " withrgbcolor (" << (edgecoloring[e].Red/256.0f) << ","<< (edgecoloring[e].Green/256.0f) << "," << (edgecoloring[e].Blue/256.0f) << ");\n";
                else
                    os << " withcolor black;\n";
                os << "  label.ulft(btex "<< e.GetLabel() << " etex, (z"<<e.From().GetID()<<"+z"<<e.To().GetID()<<")/2);\n";
            }

            /// draw vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                os << "draw z" << v.GetID() << " withpen pencircle scaled " << radius << "cm";
                if(vertexcoloring.find(v) != vertexcoloring.end())
                    os << " withcolor (" << (vertexcoloring[v].Red/256.0f) << ","<< (vertexcoloring[v].Green/256.0f) << "," << (vertexcoloring[v].Blue/256.0f) << ");\n";
                else
                    os << " withcolor black;\n";
                os << " label.ulft(btex " << v.GetLabel() << " etex , z" << v.GetID() << ");\n";
            }

            os << "endfig;\n";
            os << "bye\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterMPOST::ExportFilterMpostRegistrator(&ExportFilter::ExportFilterFactory, "mpost",
            new DefaultInstantiator<ExportFilter, ExportFilterMPOST>("mpost", "MetaPost", "http://de.wikipedia.org/wiki/MetaPost"));

    }
}
