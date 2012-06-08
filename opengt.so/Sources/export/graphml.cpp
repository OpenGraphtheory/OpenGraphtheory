#include "../../Headers/export/graphml.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterGRAPHML::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring, map<Graph::EdgeIterator, Color>& edgecoloring)
        {
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                if(e.IsHyperedge() && !e.IsUndirected())
                    throw "The GraphML fileformat doesn\'t support hyperedges that aren't completely undirected";

            /// write header
            os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            os << "<!-- www.Open-Graphtheory.org -->\n";
            os << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\">\n";
            os << "  <graph id=\"g" << G.GetID() << "\">\n";

            /// write vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                os << "    <node id=\"n" << v.GetID() << "\"/>\n";
            }

            /// write edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                if(e.IsEdge())
                {
                    os << "    <edge directed=\"false\" id=\"e" << e.GetID() << "\" source=\"n"
                       << e.From().GetID() << "\" target=\"n" << e.To().GetID() << "\"/>\n";
                }
                else if(e.IsArc())
                {
                    os << "    <edge directed=\"true\" id=\"a" << e.GetID() << "\" source=\"n"
                       << e.From().GetID() << "\" target=\"n" << e.To().GetID() << "\"/>\n";
                }
                else
                {
                    os << "    <hyperedge>\n";
                    for(Graph::VertexIterator inc = e.BeginIncidentVertices(); inc != e.EndIncidentVertices(); inc++)
                        os << "      <endpoint node=\"n" << inc.GetID() << "\"/>\n";
                    os << "    </hyperedge>\n";
                }
            }

            /// write footer
            os << "  </graph>\n";
            os << "</graphml>\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterGRAPHML::ExportFilterGraphmlRegistrator(&ExportFilter::ExportFilterFactory, "graphml",
            new DefaultInstantiator<ExportFilter, ExportFilterGRAPHML>("graphml", "GraphML", "http://graphml.graphdrawing.org/"));


    }
}
