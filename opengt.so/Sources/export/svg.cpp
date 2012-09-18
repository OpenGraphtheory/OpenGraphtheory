
#include "../../Headers/export/svg.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

// arcs get no arrow yet

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterSVG::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring, map<Graph::EdgeIterator, Color>& edgecoloring, float dpi)
        {
            float radius = 0.2;
            if(G.IsHypergraph())
                throw "The SVG fileformat does not support hypergraphs\n";

            Graph::VertexIterator v1 = G.BeginVertices();
            vector<float> coordinates = v1.GetCoordinates();
            float maxx = coordinates[0], minx = coordinates[0], maxy = coordinates[1], miny = coordinates[1];
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

            /// header
            os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            os << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
            os << "<svg xmlns=\"http://www.w3.org/2000/svg\"\n";
            os << "     xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n";
            os << "     xmlns:ev=\"http://www.w3.org/2001/xml-events\"\n";
            os << "     version=\"1.1\" baseProfile=\"full\"\n";
            os << "     width=\"" << (maxx-minx+2*radius) << "cm\" height=\"" << (maxy-miny+2*radius) << "5cm\"\n";
            os << "     viewBox=\"" << 0 << " " << 0 << " " << 100*(int)(maxx-minx+2*radius) << " " << 100*(int)(maxy-miny+2*radius) << "\"";
            os << ">\n\n";

            os << "<!-- www.Open-Graphtheory.org -->\n\n";

            /// draw vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                coordinates = v.GetCoordinates();
                os << "  <circle cx=\"" << (int)(100*coordinates[0]-minx+radius) << "\" cy=\"" << (int)(100*(coordinates[1]-miny+radius)) << "\" r=\""<<(int)(100*radius)<<"\"/>\n"; // << 2*v.GetWeight() << "\"/>\n";
                //os << "  <text />\n";
            }

            /// draw edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                vector<float> FromCoordinates = e.From().GetCoordinates();
                vector<float> ToCoordinates = e.To().GetCoordinates();
                os << "  <line x1=\"" << (int)(100*(FromCoordinates[0]-minx+radius)) << "\" y1=\"" << (int)(100*(FromCoordinates[1]-miny+radius))
                   << "\" x2=\"" << (int)(100*(ToCoordinates[0]-minx+radius)) << "\" y2=\"" << (int)(100*(ToCoordinates[1]-miny+radius)) << "\""
                   << " stroke=\"black\""
                   << " stroke-width=\"5\" />\n"; //" << e.GetWeight() << "\"/>\n";
            }

            /// footer
            os << "</svg>\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterSVG::ExportFilterSvgRegistrator(&ExportFilter::ExportFilterFactory, "svg",
            new DefaultInstantiator<ExportFilter, ExportFilterSVG>("svg", "Scalable Vector Graphics",
                "http://de.wikipedia.org/wiki/Scalable_Vector_Graphics"));


    }
}
