#include "../../Headers/export/postscript.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterPOSTSCRIPT::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring, map<Graph::EdgeIterator, Color>& edgecoloring, float dpi)
        {
            float Radius = 0.5; // cm

            if(G.IsHypergraph())
                throw "The POSTSCRIPT fileformat does not support hypergraphs\n";

            vector<float> FirstCoordinates = G.BeginVertices().GetCoordinates();
            float MaxYCoordinate = FirstCoordinates[1];
            float MinXCoordinate = FirstCoordinates[0];

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                if(coordinates.size() < 2)
                    throw "Vertex with less than 2 coordinates found";
                if(MaxYCoordinate < coordinates[1])
                    MaxYCoordinate = coordinates[1];
                if(MinXCoordinate > coordinates[0])
                    MinXCoordinate = coordinates[0];
            }
            MinXCoordinate -= Radius;
            MaxYCoordinate += Radius;

            /// header
            os << "%!PS-Adobe-3.0\n";
            os << "% www.Open-Graphtheory.org\n";
            os << "gsave\n";
            os << "0.2 setlinewidth\n";

            os << "/Helvetica-Bold findfont\n";
            os << "7 scalefont\n";
            os << "setfont\n";

            /// draw edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                vector<float> FromCoordinates = e.From().GetCoordinates();
                vector<float> ToCoordinates = e.To().GetCoordinates();

                if(edgecoloring.find(e) != edgecoloring.end())
                    os << (edgecoloring[e].Red/256.0f) << " "<< (edgecoloring[e].Green/256.0f) << " " << (edgecoloring[e].Blue/256.0f) << " setrgbcolor\n";
                else
                    os << "0 0 0 setrgbcolor\n";

                os << (int)((FromCoordinates[0]-MinXCoordinate)* dpi/2.54) << " " << (int)((MaxYCoordinate - FromCoordinates[1])* dpi/2.54) << " moveto "
                   << (int)((ToCoordinates[0]-MinXCoordinate)* dpi/2.54) << " " << (int)((MaxYCoordinate - ToCoordinates[1])*dpi/2.54) << " lineto stroke\n";
            }

            /// draw vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> Coordinates = v.GetCoordinates();
                if(vertexcoloring.find(v) != vertexcoloring.end())
                    os << (vertexcoloring[v].Red/256.0f) << " "<< (vertexcoloring[v].Green/256.0f) << " " << (vertexcoloring[v].Blue/256.0f) << " setrgbcolor\n";
                else
                    os << "0 0 0 setrgbcolor\n";
                os << (int)((Coordinates[0]-MinXCoordinate)* dpi/2.54) << " " << (int)((MaxYCoordinate - Coordinates[1])* dpi/2.54) << " " << (int)(Radius*dpi/2.54) << " 0 360 arc fill\n";

                if(v.GetLabel() != "")
                {
                    os << "0 0 0 setrgbcolor\n";
                    os << (int)((Coordinates[0] - MinXCoordinate + Radius)* dpi/2.54) << " " << (int)((MaxYCoordinate - Coordinates[1] + Radius)* dpi/2.54) << " moveto\n";
                    os << "(" << v.GetLabel() << ") show\n";
                    os << "newpath\n";
                }
            }

            os << "grestore\n";
            os << "showpage\n";
            os << "%%EOF\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterPOSTSCRIPT::ExportFilterPostscriptRegistrator(&ExportFilter::ExportFilterFactory, "postscript",
            new DefaultInstantiator<ExportFilter, ExportFilterPOSTSCRIPT>("postscript", "PostScript", "http://en.wikipedia.org/wiki/PostScript"));

    }
}
