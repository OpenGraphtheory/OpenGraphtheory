#include "../../Headers/export/mpost.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        GraphRendererMPOST::GraphRendererMPOST(std::ostream& os)
            : GraphRenderingContextStream(os)
        {
            Translator = new StringTranslatorLatex;
        }

        void GraphRendererMPOST::BeginRenderingGraph(float WidthInCm, float HeightInCm, float ResolutionDPI)
        {
            (*os) << "% www.Open-Graphtheory.org\n";
            (*os) << "beginfig(1);\n\n";
            (*os) << "  def circlepath = ((0.5,0)..(0,0.5)..(-0.5,0)..(0,-0.5)..cycle) enddef;\n";
            (*os) << "  color brushcolor;\n";
            (*os) << "  color pencolor;\n";
            (*os) << "\n\n";
            ImageHeight = HeightInCm;
        }

        void GraphRendererMPOST::EndRenderingGraph()
        {
            (*os) << "\nendfig;\n";
            (*os) << "bye\n";
        }

        void GraphRendererMPOST::DeclareVertex(int vertex_id, float x, float y, float radius, string text, Color color)
        {
            (*os) << "  z" << vertex_id << " = (" << x << "cm, " << ImageHeight-y << "cm);\n";
            (*os) << "  radius" << vertex_id << " = " << radius << "cm;\n";
        }

        void GraphRendererMPOST::BeginDeclaringVertices()
        {
            DeclaringVertices = true;
        }

        void GraphRendererMPOST::EndDeclaringVertices()
        {
            DeclaringVertices = false;
            (*os) << "\n\n";
        }

        void GraphRendererMPOST::EndRenderingEdges()
        {
            (*os) << "\n\n";
        }

        void GraphRendererMPOST::SetPenColor(Visualization::Color color)
        {
            (*os) << "  pencolor := (" << (color.Red/256.0f) << ","<< (color.Green/256.0f) << "," << (color.Blue/256.0f) << ");\n";
        }

        void GraphRendererMPOST::SetBrushColor(Visualization::Color color)
        {
            if(!DeclaringVertices)
                (*os) << "  brushcolor := (" << (color.Red/256.0f) << ","<< (color.Green/256.0f) << "," << (color.Blue/256.0f) << ");\n";
        }

        void GraphRendererMPOST::SetLineWidth(float width)
        {
            (*os) << "  linewidth := " << width << "cm;\n";
        }

        void GraphRendererMPOST::RenderEdge(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, string text, Color color)
        {
            (*os) << "  draw z" << from_id << " -- z" << to_id << " withrgbcolor pencolor withpen pencircle scaled linewidth;\n";
        }

        void GraphRendererMPOST::RenderArc(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, string text, Color color)
        {
            (*os) << "  drawarrow (z" << from_id << " -- z" << to_id
                  << ") cutafter (circlepath scaled (2*radius" << to_id << ") shifted z" << to_id << ")\n"
                  << "            withrgbcolor pencolor withpen pencircle scaled linewidth;\n";
        }


        void GraphRendererMPOST::RenderVertex(int vertex_id, float x, float y, float radius, string text, Color color)
        {
            (*os) << "  draw z" << vertex_id << " withpen pencircle scaled (2*radius" << vertex_id << ")" << " withrgbcolor brushcolor;\n";
        }

        void GraphRendererMPOST::PutText(float x, float y, string text)
        {
            (*os) << "  label.ulft(btex "<< text << " etex, ("<<x<<"cm,"<<ImageHeight-y<<"cm));\n";
        }

/*
        void GraphRendererMPOST::PutEdgeText(int from_id, int to_id, float x, float y, string text)
        {
            (*os) << "  label.ulft(btex "<< text << " etex, (z"<<from_id<<"+z"<<to_id<<")/2);\n";
        }

        void GraphRendererMPOST::PutVertexText(int node_id, float x, float y, string text)
        {
            (*os) << " label.ulft(btex " << text << " etex , z" << node_id << ");\n";
        }
*/

        FactoryRegistrator<ExportFilter> GraphRendererMPOST::ExportFilterMpostRegistrator(&ExportFilter::ExportFilterFactory, "mpost",
            new DefaultInstantiator<ExportFilter, GraphicalExportFilter<GraphRendererMPOST> >("mpost", "MetaPost", "http://de.wikipedia.org/wiki/MetaPost"));

    }
}
