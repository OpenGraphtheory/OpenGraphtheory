
#include <iostream>
#include "../opengt.h"
#include "export.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterTGF::Export(Graph& G, ostream& os)
        {
            if(G.IsHypergraph())
                throw "The TGF fileformat doesn\'t support hypergraphs\n";
            if(G.IsMixed())
                throw "The TGF fileformat doesn\'t support mixed graphs\n";

            /// write vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                os << v.GetID() << " " << v.GetLabel() << "\n";

            os << "#\n";

            /// write edges or arcs
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                os << e.From().GetID() << " " << e.To().GetID() << " " << e.GetLabel() << "\n";

        }

        FactoryRegistrator<ExportFilter> ExportFilterTgfRegistrator(&ExportFilter::ExportFilterFactory, "tgf",
            new DefaultInstantiator<ExportFilter, ExportFilterTGF>("tgf", "Trivial Graph Format",
                "http://en.wikipedia.org/wiki/Trivial_Graph_Format"));


    }
}
