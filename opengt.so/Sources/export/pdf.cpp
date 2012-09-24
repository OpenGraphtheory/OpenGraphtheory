#include "../../Headers/export/pdf.h"
#include <iomanip>

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterPDF::Begin(ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI)
        {
            ImageHeight = HeightInCm;
            ResolutionDPCM = ResolutionDPI / 2.54;

            obj=1;
            obj_offsets.clear();
            temp_os.clear();
            oss.clear();

            temp_os << "%PDF-1.0\n";

            // Object 1 for Meta-Infos of the file
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\n";
            temp_os << "<<\n";
            temp_os << "/Creator (www.Open-Graphtheory.org)\n";
            temp_os << ">>\n";
            temp_os << "endobj\n";
            obj++;

            // Object 2 for the "Catalog"
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\n";
            temp_os << "<<\n";
            temp_os << "/Type /Catalog\n";
            temp_os << "/Pages " << obj+1 << " 0 R\n"; // object 3 is the Page-Tree
            //os << "/Outlines 4 0 R\n";
            //os << "/PageMode /UseOutlines\n";
            temp_os << ">>\n";
            temp_os << "endobj\n";
            obj++;

            // Object 3 is the Page-Tree
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\n";
            temp_os << "<<\n";
            temp_os << "/Type /Pages\n";
            temp_os << "/Kids [" << obj+1 << " 0 R]\n"; // Page-Tree contains 1 Page
            temp_os << "/Count 1\n";
            temp_os << ">>\n";
            temp_os << "endobj\n";
            obj++;

            // Object 4 is the declaration of Page 1
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\n";
            temp_os << "<<\n";
            temp_os << "/Type /Page\n";
            temp_os << "/Parent " << obj-1 << " 0 R\n";
            temp_os << "/Resources " << obj+2 << " 0 R\n";
            temp_os << "/Contents " << obj+1 << " 0 R\n";
            temp_os << ">>\n";
            temp_os << "endobj\n";
            obj++;


            // Object 5 is the content of Page 1
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\n";

            stringstream oss;
            oss << "BX /GS1 gs EX\n";
            oss << "q\n";
            oss << "2 w\n";
            oss << "0 0 0 rg\n";
            oss << "0 0 0 RG\n";

            ImageHeight = HeightInCm;
        }

        void ExportFilterPDF::End(ostream &os)
        {
            oss << "Q\n";

            temp_os << "<< /Length " << oss.tellp() << " >>\n";
            temp_os << "stream\n";
            temp_os << oss.str() ;
            temp_os << "endstream\n";
            temp_os << "endobj\n";
            obj++;

            // Object 6 is the Ressources of Page 1
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\n";
            temp_os << "<<\n";
            temp_os << "/ProcSet [/PDF /ImageC ]\n";
            temp_os << "/Font << /F4 " << obj+2 << " 0 R >>\n";
            temp_os << "/ExtGState << /GS1 " << obj+1 << " 0 R >>\n";
            temp_os << ">>\n";
            temp_os << "endobj\n";
            obj++;

            // Object 7 is the "ExtGState" of Page 1 (whatever that is...)
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\n";
            temp_os << "<<\n";
            temp_os << "/Type /ExtGState\n";
            temp_os << "/SA false\n";
            temp_os << "/OP false\n";
            temp_os << "/HT /default\n";
            temp_os << ">>\n";
            temp_os << "endobj\n";
            obj++;

            // Object 8 is the (unique) Font that is used
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\n";
            temp_os << "<<\n";
            temp_os << "/Type /Font\n";
            temp_os << "/Subtype /Type1\n";
            temp_os << "/Name F4\n";
            temp_os << "/BaseFont Times-Roman\n";
            temp_os << ">>\n";
            temp_os << "endobj\n";
            obj++;




            // xrefs
            // Adobe, you are morons! No wonder you have one 0-day after another!
            int xref_offset = temp_os.tellp();
            temp_os << "xref\n";
            temp_os << "1 " << obj_offsets.size() << "\n";
            for(list<int>::iterator i = obj_offsets.begin(); i != obj_offsets.end(); i++)
            temp_os << setfill('0') << setw(10) << (*i) << " 00001 n\r\n";



            // trailer
            temp_os << "trailer\n";
            temp_os << "<<\n";
            temp_os << "/Size " << obj_offsets.size() << "\n"; // there are 3 entries in the xref table
            temp_os << "/Root 2 0 R\n"; // object 2 is the catalog
            temp_os << "/Info 1 0 R\n"; // object 1 contains the meta infos
            temp_os << ">>\n";
            temp_os << "startxref\n";
            temp_os << xref_offset << "\n";
            temp_os << "%%EOF\n";

            os << temp_os.str();
        }

        void ExportFilterPDF::SetPenColor(ostream &os, Visualization::Color color)
        {
            oss << (color.Red/256.0f) << " " << (color.Green/256.0f) << " " << (color.Blue/256.0f) << " RG\n";
        }

        void ExportFilterPDF::SetBrushColor(ostream &os, Visualization::Color color)
        {
            oss << (color.Red/256.0f) << " " << (color.Green/256.0f) << " " << (color.Blue/256.0f) << " rg\n";
        }

        void ExportFilterPDF::SetLineWidth(ostream &os, float width)
        {
            oss << (int)(width*ResolutionDPCM) << " w\n";
        }

        void ExportFilterPDF::Line(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2)
        {
            oss << (int)(x1*ResolutionDPCM) << " " << (int)((ImageHeight-y1)*ResolutionDPCM) << " m "
                << (int)(x2*ResolutionDPCM) << " " << (int)((ImageHeight-y2)*ResolutionDPCM) << " l s\n";
        }


        void ExportFilterPDF::Circle(ostream &os, int node_id, float x, float y, float Radius)
        {
            // http://forums.adobe.com/message/2312083
            // WTF, Adobe!? Seriously, WTF!?

            int radius = Radius * ResolutionDPCM;
            int h = 0.5522422 * Radius * ResolutionDPCM; // sine of 33 deg 31 sec

            oss << "q 1 0 0 1 " << x*ResolutionDPCM << " " << (ImageHeight-y)*ResolutionDPCM << " cm\n"
                << "0 " << -radius << " m\n"

                << h << " " << -radius << " " << radius << " " << -h << " " << radius << " " << 0 << "c\n"
                << radius << " " << h << " " << h << " " << radius << " " << 0 << " " << radius << "c\n"
                << -h << " " << radius << " " << -radius << " " << h << " " << -radius << " " << 0 << "c\n"
                << -radius << " " << -h << " " << -h << " " << -radius << " " << 0 << " " << -radius << "c\n"
                << "f Q\n";
        }

        void ExportFilterPDF::PutText(ostream &os, float x, float y, string text)
        {
        }

        FactoryRegistrator<ExportFilter> ExportFilterPDF::ExportFilterPdfRegistrator(&ExportFilter::ExportFilterFactory, "pdf",
            new DefaultInstantiator<ExportFilter, ExportFilterPDF>("pdf", "Portable Document Format", "http://en.wikipedia.org/wiki/Portable_Document_Format"));

    }
}
