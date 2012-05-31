#include "../../Headers/export/postscript.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterPOSTSCRIPT::Export(Graph& G, ostream& os)
        {
            if(G.IsHypergraph())
                throw "The POSTSCRIPT fileformat does not support hypergraphs\n";

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                if(coordinates.size() < 2)
                    throw "Vertex with less than 2 coordinates found";
            }

            /// header
            os << "%!PS-Adobe-3.0\n";
            os << "% www.Open-Graphtheory.org\n";
            os << "72 2.54 div 30 div dup scale\n";
            os << "0.2 setlinewidth\n";

            /// draw vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> Coordinates = v.GetCoordinates();
                os << (int)(Coordinates[0]+0.5) << " " << (int)(Coordinates[1]+0.5) << " 2 0 360 arc fill\n";
            }

            /// draw edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                vector<float> FromCoordinates = e.From().GetCoordinates();
                vector<float> ToCoordinates = e.To().GetCoordinates();
                os << (int)(FromCoordinates[0]+0.5) << " " << (int)(FromCoordinates[1]+0.5) << " moveto "
                   << (int)(ToCoordinates[0]+0.5) << " " << (int)(ToCoordinates[1]+0.5) << " lineto stroke\n";
            }

            os << "showpage\n";
            os << "%%EOF\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterPOSTSCRIPT::ExportFilterPostscriptRegistrator(&ExportFilter::ExportFilterFactory, "postscript",
            new DefaultInstantiator<ExportFilter, ExportFilterPOSTSCRIPT>("postscript", "PostScript", "http://en.wikipedia.org/wiki/PostScript"));

    }
}
