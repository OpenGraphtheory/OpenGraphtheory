
#include "../../Headers/export/lgf.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {
        FactoryRegistrator<ExportFilter> ExportFilterLGF::ExportFilterLgfRegistrator(&ExportFilter::ExportFilterFactory, "lgf",
            new DefaultInstantiator<ExportFilter, ExportFilterLGF>("lgf", "LEMON Graph Format",
                "http://lemon.cs.elte.hu/pub/doc/latest/a00002.html"));

        void ExportFilterLGF::Export(Graph& G, ostream& os, VertexColoring& vertexcoloring,
                                     EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            if(G.IsHypergraph())
                throw "The LGF fileformat doesn\'t support hypergraphs\n";
            if(G.IsMixed())
                throw "The LGF fileformat doesn\'t support mixed graphs\n";

            /// write vertices
            os << "@nodes\nlabel\tcoordinates\tsize\ttitle\n";
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                Coordinates coordinates = (*v)->GetCoordinates(2);
                os << (*v)->GetID() << "\t("
                   << coordinates[0] << "," << coordinates[1] << ")\t"
                   << (*v)->GetWeight() << "\t"
                   << "\"" << (*v)->GetLabel() << "\"\n";
            }

            /// write edges or arcs
            os << "@edges\n\t\tcapacity\ttitle\n";
            for(EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                os << (*e)->From()->GetID() << "\t" << (*e)->To()->GetID()
                   << "\t" << (*e)->GetWeight() << "\t\"" << (*e)->GetLabel() << "\"\n";
        }
    }
}
