#include "../../Headers/export/mpost.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {


        void ExportFilterMPOST::Begin(ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI)
        {
            os << "% www.Open-Graphtheory.org\n";
            os << "beginfig(1);\n\n";
            ImageHeight = HeightInCm;
        }

        void ExportFilterMPOST::End(ostream &os)
        {
            os << "endfig;\n";
            os << "bye\n";
        }

        void ExportFilterMPOST::DeclareNode(ostream &os, int node_id, float x, float y)
        {
            os << "z" << node_id << " = (" << x << "cm, " << ImageHeight-y << "cm);\n";
        }

        void ExportFilterMPOST::SetPenColor(ostream &os, Visualization::Color color)
        {
            PenColor = color;
        }

        void ExportFilterMPOST::SetBrushColor(ostream &os, Visualization::Color color)
        {
            BrushColor = color;
        }

        void ExportFilterMPOST::SetLineWidth(ostream &os, float width)
        {
            os << "pickup pencircle scaled " << width << "cm;\n";
        }

        void ExportFilterMPOST::Line(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2)
        {
                os << " draw z" << from_id << " -- z" << to_id
                   << " withrgbcolor (" << (PenColor.Red/256.0f) << ","<< (PenColor.Green/256.0f) << "," << (PenColor.Blue/256.0f) << ");\n";
        }

        void ExportFilterMPOST::Arrow(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2)
        {
                os << " drawarrow z" << from_id << " -- z" << to_id
                   << " withrgbcolor (" << (PenColor.Red/256.0f) << ","<< (PenColor.Green/256.0f) << "," << (PenColor.Blue/256.0f) << ");\n";
        }


        void ExportFilterMPOST::Circle(ostream &os, int node_id, float x, float y, float radius)
        {
                os << "draw z" << node_id << " withpen pencircle scaled " << 2*radius << "cm"
                   << " withrgbcolor (" << (BrushColor.Red/256.0f) << ","<< (BrushColor.Green/256.0f) << "," << (BrushColor.Blue/256.0f) << ");\n";
        }

        void ExportFilterMPOST::PutText(ostream &os, float x, float y, string text)
        {
            os << "  label.ulft(btex "<< text << " etex, ("<<x<<"cm,"<<ImageHeight-y<<"cm));\n";
        }

        void ExportFilterMPOST::PutEdgeText(ostream &os, int from_id, int to_id, float x, float y, string text)
        {
            os << "  label.ulft(btex "<< text << " etex, (z"<<from_id<<"+z"<<to_id<<")/2);\n";
        }

        void ExportFilterMPOST::PutVertexText(ostream &os, int node_id, float x, float y, string text)
        {
            os << " label.ulft(btex " << text << " etex , z" << node_id << ");\n";
        }


        FactoryRegistrator<ExportFilter> ExportFilterMPOST::ExportFilterMpostRegistrator(&ExportFilter::ExportFilterFactory, "mpost",
            new DefaultInstantiator<ExportFilter, ExportFilterMPOST>("mpost", "MetaPost", "http://de.wikipedia.org/wiki/MetaPost"));

    }
}
