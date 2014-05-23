#include "../../Headers/export/graphml.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterGRAPHML::Export(Graph& G, ostream& os, VertexColoring& vertexcoloring,
                                         EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            StringTranslatorXML Translator;

            /// write header
            os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            os << "<!-- www.Open-Graphtheory.org -->\n";
            os << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\" xmlns:y=\"http://www.yworks.com/xml/graphml\">\n";
            os << "  <key for=\"node\" id=\"d6\" yfiles.type=\"nodegraphics\"/>\n";
            os << "  <graph id=\"G\" edgedefault=\"undirected\">\n";

            /// write vertices
            for(VertexIterator vi = G.BeginVertices(); vi != G.EndVertices(); vi++)
            {
                Vertex *v = *vi;
                os << "    <node id=\"n" << v->GetID() << "\">\n";
                os << "      <data key=\"d6\">\n";
                os << "        <y:ShapeNode>\n";
                vector<float> Coordinates = v->GetCoordinates();
                if(Coordinates.size() >= 2)
                {
                    os << "          <y:Geometry height=\"1.0\" width=\"1.0\" x=\"" << Coordinates[0] << "\" "
                                                                            << "y=\"" << Coordinates[1] << "\"/>\n";
                }
                os << "          <y:NodeLabel alignment=\"center\" autoSizePolicy=\"content\" fontFamily=\"Dialog\" fontSize=\"12\" fontStyle=\"plain\" hasBackgroundColor=\"false\" hasLineColor=\"false\" textColor=\"#000000\" visible=\"true\">"
                                 << Translator.Translate(v->GetLabel()) << "</y:NodeLabel>\n";
                os << "        </y:ShapeNode>\n";
                os << "      </data>\n";
                os << "    </node>\n";
            }

            /// write edges
            for(EdgeIterator ei = G.BeginEdges(); ei != G.EndEdges(); ei++)
            {
                Edge* e = *ei;
                if(e->IsEdge())
                {
                    os << "    <edge directed=\"false\" id=\"e" << e->GetID() << "\" source=\"n"
                       << e->From()->GetID() << "\" target=\"n" << e->To()->GetID() << "\"/>\n";
                }
                else if(e->IsArc())
                {
                    os << "    <edge directed=\"true\" id=\"a" << e->GetID() << "\" source=\"n"
                       << e->From()->GetID() << "\" target=\"n" << e->To()->GetID() << "\"/>\n";
                }
                else
                {
                    os << "    <hyperedge>\n";
                    for(VertexEdgeConnectionIterator conn = e->BeginConnections(); conn != e->EndConnections(); conn++)
                    {
                        os << "      <endpoint node=\"n" << (*conn)->GetVertex()->GetID() << "\" type=\"";
                        switch((*conn)->GetDirection())
                        {
                            case VertexEdgeConnection::EdgeToVertex: os << "out"; break;
                            case VertexEdgeConnection::Undirected: os << "undir"; break;
                            case VertexEdgeConnection::VertexToEdge: os << "in"; break;
                            default:
                                throw "Connection between Edge and Vertex has illegal value";
                        }
                        os << "\"/>\n";

                    }
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
