
#include "../../Headers/export/tgf.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterTGF::Export(Graph& G, ostream& os, VertexColoring& vertexcoloring,
                                     EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            if(G.IsHypergraph())
                throw "The TGF fileformat doesn\'t support hypergraphs\n";
            if(G.IsMixed())
                throw "The TGF fileformat doesn\'t support mixed graphs\n";

            /// write vertices
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                os << (*v)->GetID() << " " << (*v)->GetLabel() << "\n";

            os << "#\n";

            /// write edges or arcs
            for(EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                os << (*e)->From()->GetID() << " " << (*e)->To()->GetID() << " " << (*e)->GetLabel() << "\n";

        }

        FactoryRegistrator<ExportFilter> ExportFilterTGF::ExportFilterTgfRegistrator(&ExportFilter::ExportFilterFactory, "tgf",
            new DefaultInstantiator<ExportFilter, ExportFilterTGF>("tgf", "Trivial Graph Format",
                "http://en.wikipedia.org/wiki/Trivial_Graph_Format"));


    }
}
