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
            ImageHeight = HeightInCm;
        }

        void GraphRendererMPOST::EndRenderingGraph()
        {
            (*os) << "endfig;\n";
            (*os) << "bye\n";
        }

        void GraphRendererMPOST::DeclareVertex(int vertex_id, float x, float y, float radius, string text, Color color)
        {
            (*os) << "z" << vertex_id << " = (" << x << "cm, " << ImageHeight-y << "cm);\n";
        }

        void GraphRendererMPOST::SetPenColor(Visualization::Color color)
        {
            PenColor = color;
        }

        void GraphRendererMPOST::SetBrushColor(Visualization::Color color)
        {
            BrushColor = color;
        }

        void GraphRendererMPOST::SetLineWidth(float width)
        {
            (*os) << "pickup pencircle scaled " << width << "cm;\n";
        }

        void GraphRendererMPOST::RenderEdge(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, string text, Color color)
        {
            SetLineWidth(width);
            SetPenColor(color);
            (*os) << " draw z" << from_id << " -- z" << to_id
                  << " withrgbcolor (" << (PenColor.Red/256.0f) << ","<< (PenColor.Green/256.0f) << "," << (PenColor.Blue/256.0f) << ");\n";
        }

        void GraphRendererMPOST::RenderArc(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, string text, Color color)
        {
            SetLineWidth(width);
            SetPenColor(color);
            (*os) << " drawarrow z" << from_id << " -- z" << to_id
                  << " withrgbcolor (" << (PenColor.Red/256.0f) << ","<< (PenColor.Green/256.0f) << "," << (PenColor.Blue/256.0f) << ");\n";
        }


        void GraphRendererMPOST::RenderVertex(int vertex_id, float x, float y, float radius, string text, Color color)
        {
            SetBrushColor(color);
            (*os) << "draw z" << vertex_id << " withpen pencircle scaled " << 2*radius << "cm"
                  << " withrgbcolor (" << (BrushColor.Red/256.0f) << ","<< (BrushColor.Green/256.0f) << "," << (BrushColor.Blue/256.0f) << ");\n";
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
