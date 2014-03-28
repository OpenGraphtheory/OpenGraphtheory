#include "../../Headers/export/graphml.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterGRAPHML::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring, map<Graph::EdgeIterator, Color>& edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                if(e.IsHyperedge() && !e.IsUndirected())
                    throw "The GraphML fileformat doesn\'t support hyperedges that aren't completely undirected";

            /// write header
            os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            os << "<!-- www.Open-Graphtheory.org -->\n";
            os << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\" xmlns:y=\"http://www.yworks.com/xml/graphml\">\n";
            os << "  <key for=\"node\" id=\"d6\" yfiles.type=\"nodegraphics\"/>\n";
            os << "  <graph id=\"g" << G.GetID() << "\">\n";

            /// write vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                os << "    <node id=\"n" << v.GetID() << "\">\n";
                os << "      <data key=\"d6\">\n";
                os << "        <y:ShapeNode>\n";
                vector<float> Coordinates = v.GetCoordinates();
                if(Coordinates.size() >= 2)
                {
                    os << "          <y:Geometry height=\"1.0\" width=\"1.0\" x=\"" << Coordinates[0] << "\" "
                                                                            << "y=\"" << Coordinates[1] << "\"/>\n";
                }
                os << "          <y:NodeLabel alignment=\"center\" autoSizePolicy=\"content\" fontFamily=\"Dialog\" fontSize=\"12\" fontStyle=\"plain\" hasBackgroundColor=\"false\" hasLineColor=\"false\" textColor=\"#000000\" visible=\"true\">" << v.GetLabel() << "</y:NodeLabel>\n";
                os << "        </y:ShapeNode>\n";
                os << "      </data>\n";
                os << "    </node>\n";
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
