
#include "../../Headers/export/vrmlgraph.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterVRMLGRAPH::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring, map<Graph::EdgeIterator, Color>& edgecoloring, float dpi)
        {
            if(G.IsHypergraph())
                throw "The VRMLgraph fileformat doesn\'t support hypergraphs\n";
            if(G.IsMixed())
                throw "The VRMLgraph fileformat doesn\'t support mixed graphs\n";
            if(G.IsDirected())
                throw "The VRMLgraph fileformat doesn\'t support directed graphs\n";

            os << "# www.Open-Graphtheory.org\n";
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                os << e.From().GetID() << " " << e.To().GetID() << "\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterVRMLGRAPH::ExportFilterVrmlgraphRegistrator(&ExportFilter::ExportFilterFactory, "vrmlgraph",
            new DefaultInstantiator<ExportFilter, ExportFilterVRMLGRAPH>("vrmlgraph", "Virtual Reality Modeling Language",
                "http://vrmlgraph.i-scream.org.uk"));


    }
}