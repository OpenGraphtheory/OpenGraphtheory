#include "../../Headers/export/postscript.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterPOSTSCRIPT::Begin(ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI)
        {
            os << "%!PS-Adobe-3.0\n";
            os << "% www.Open-Graphtheory.org\n";
            os << "gsave\n";

            os << "/Helvetica-Bold findfont\n";
            os << "7 scalefont\n";
            os << "setfont\n";

            ImageHeight = HeightInCm;
            ResolutionDPCM = ResolutionDPI / 2.54;
        }

        void ExportFilterPOSTSCRIPT::End(ostream &os)
        {
            os << "grestore\n";
            os << "showpage\n";
            os << "%%EOF\n";
        }

        void ExportFilterPOSTSCRIPT::SetPenColor(ostream &os, Visualization::Color color)
        {
            os << (color.Red/256.0f) << " "<< (color.Green/256.0f) << " " << (color.Blue/256.0f) << " setrgbcolor\n";
        }

        void ExportFilterPOSTSCRIPT::SetBrushColor(ostream &os, Visualization::Color color)
        {
            os << (color.Red/256.0f) << " "<< (color.Green/256.0f) << " " << (color.Blue/256.0f) << " setrgbcolor\n";
        }

        void ExportFilterPOSTSCRIPT::SetLineWidth(ostream &os, float width)
        {
            os << width*ResolutionDPCM << " setlinewidth\n";
        }

        void ExportFilterPOSTSCRIPT::Line(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2)
        {
            os << (int)(x1*ResolutionDPCM) << " " << (int)((ImageHeight-y1)*ResolutionDPCM) << " moveto "
               << (int)(x2*ResolutionDPCM) << " " << (int)((ImageHeight-y2)*ResolutionDPCM) << " lineto stroke\n";
        }

        void ExportFilterPOSTSCRIPT::Circle(ostream &os, int node_id, float x, float y, float radius)
        {
            os << (int)(x*ResolutionDPCM) << " " << (int)((ImageHeight-y)*ResolutionDPCM) << " " << (int)(radius*ResolutionDPCM) << " 0 360 arc fill\n";
        }

        void ExportFilterPOSTSCRIPT::PutText(ostream &os, float x, float y, string text)
        {
            os << "0 0 0 setrgbcolor\n";
            os << (int)(x*ResolutionDPCM) << " " << (int)((ImageHeight-y)*ResolutionDPCM) << " moveto\n";
            os << "(" << text << ") show\n";
            os << "newpath\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterPOSTSCRIPT::ExportFilterPostscriptRegistrator(&ExportFilter::ExportFilterFactory, "postscript",
            new DefaultInstantiator<ExportFilter, ExportFilterPOSTSCRIPT>("postscript", "PostScript", "http://en.wikipedia.org/wiki/PostScript"));

    }
}
