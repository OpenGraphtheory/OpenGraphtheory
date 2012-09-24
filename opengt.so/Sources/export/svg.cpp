
#include "../../Headers/export/svg.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

// arcs get no arrow yet

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterSVG::Begin(ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI)
        {
            ResolutionDPCM = ResolutionDPI / 2.54;

            os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            os << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
            os << "<svg xmlns=\"http://www.w3.org/2000/svg\"\n";
            os << "     xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n";
            os << "     xmlns:ev=\"http://www.w3.org/2001/xml-events\"\n";
            os << "     version=\"1.1\" baseProfile=\"full\"\n";
            os << "     width=\"" << WidthInCm << "cm\" height=\"" << HeightInCm << "cm\"\n";
            os << "     viewBox=\"" << 0 << " " << 0 << " " << (int)(WidthInCm*ResolutionDPCM) << " " << (int)(HeightInCm*ResolutionDPCM) << "\"";
            os << ">\n\n";

            os << "<!-- www.Open-Graphtheory.org -->\n\n";
        }

        void ExportFilterSVG::End(ostream &os)
        {
            os << "</svg>\n";
        }

        void ExportFilterSVG::SetPenColor(ostream &os, Visualization::Color color)
        {
            PenColor = color;
        }

        void ExportFilterSVG::SetBrushColor(ostream &os, Visualization::Color color)
        {
            BrushColor = color;
        }

        void ExportFilterSVG::SetLineWidth(ostream &os, float width)
        {
            LineWidth = width;
        }

        void ExportFilterSVG::Line(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2)
        {
                os << "  <line x1=\"" << (int)(x1*ResolutionDPCM) << "\" y1=\"" << (int)(y1*ResolutionDPCM)
                   << "\" x2=\"" << (int)(x2*ResolutionDPCM) << "\" y2=\"" << (int)(y2*ResolutionDPCM) << "\""
                   << " stroke=\"rgb(" << PenColor.Red << "," << PenColor.Green << "," << PenColor.Blue << ")\""
                   << " stroke-width=\"" << LineWidth*ResolutionDPCM << "\" />\n";
        }

        void ExportFilterSVG::Circle(ostream &os, int node_id, float x, float y, float radius)
        {
                os << "  <circle cx=\"" << (int)(x*ResolutionDPCM) << "\" cy=\"" << (int)(y*ResolutionDPCM)
                   << "\" r=\"" << (int)(radius*ResolutionDPCM) << "\" fill=\"rgb(" << BrushColor.Red << ","
                   << BrushColor.Green << "," << BrushColor.Blue << ")\"/>\n";
        }

        void ExportFilterSVG::PutText(ostream &os, float x, float y, string text)
        {
                //os << "  <text />\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterSVG::ExportFilterSvgRegistrator(&ExportFilter::ExportFilterFactory, "svg",
            new DefaultInstantiator<ExportFilter, ExportFilterSVG>("svg", "Scalable Vector Graphics",
                "http://de.wikipedia.org/wiki/Scalable_Vector_Graphics"));


    }
}
