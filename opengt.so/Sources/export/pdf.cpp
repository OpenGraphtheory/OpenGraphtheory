#include "../../Headers/export/pdf.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {
        GraphRendererPDF::GraphRendererPDF(ostream& os)
            : GraphRenderingContextStream(os)
        {
        }

        void GraphRendererPDF::BeginRenderingGraph(float WidthInCm, float HeightInCm, float ResolutionDPI)
        {
            ImageHeight = HeightInCm;
            ResolutionDPCM = ResolutionDPI / 2.54;

            obj=1;
            obj_offsets.clear();
            temp_os.clear();
            oss.clear();

            temp_os << "%PDF-1.0\r\n";

            // Object 1 for Meta-Infos of the file
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\r\n";
            temp_os << "<<\r\n";
            temp_os << "/Creator (www.Open-Graphtheory.org)\r\n";
            temp_os << ">>\r\n";
            temp_os << "endobj\r\n";
            obj++;

            // Object 2 for the "Catalog"
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\r\n";
            temp_os << "<<\r\n";
            temp_os << "/Type /Catalog\r\n";
            temp_os << "/Pages " << obj+1 << " 0 R\r\n"; // object 3 is the Page-Tree
            //os << "/Outlines 4 0 R\r\n";
            //os << "/PageMode /UseOutlines\r\n";
            temp_os << ">>\r\n";
            temp_os << "endobj\r\n";
            obj++;

            // Object 3 is the Page-Tree
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\r\n";
            temp_os << "<<\r\n";
            temp_os << "/Type /Pages\r\n";
            temp_os << "/Kids [" << obj+1 << " 0 R]\r\n"; // Page-Tree contains 1 Page
            temp_os << "/Count 1\r\n";
            temp_os << ">>\r\n";
            temp_os << "endobj\r\n";
            obj++;

            // Object 4 is the declaration of Page 1
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\r\n";
            temp_os << "<<\r\n";
            temp_os << "/Type /Page\r\n";
            temp_os << "/Parent " << obj-1 << " 0 R\r\n";
            temp_os << "/Resources " << obj+2 << " 0 R\r\n";
            temp_os << "/Contents " << obj+1 << " 0 R\r\n";
            temp_os << ">>\r\n";
            temp_os << "endobj\r\n";
            obj++;


            // Object 5 is the content of Page 1
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\r\n";

            //stringstream oss;
            oss << "BX /GS1 gs EX\r\n";
            oss << "q\r\n";
            oss << "2 w\r\n";
            oss << "0 0 0 rg\r\n";
            oss << "0 0 0 RG\r\n";

            ImageHeight = HeightInCm;
        }

        void GraphRendererPDF::EndRenderingGraph()
        {
            oss << "Q\r\n";

            temp_os << "<< /Length " << oss.tellp() << " >>\r\n";
            temp_os << "stream\r\n";
            temp_os << oss.str() ;
            temp_os << "endstream\r\n";
            temp_os << "endobj\r\n";
            obj++;

            // Object 6 is the Ressources of Page 1
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\r\n";
            temp_os << "<<\r\n";
            temp_os << "/ProcSet [/PDF /ImageC ]\r\n";
            temp_os << "/Font << /F4 " << obj+2 << " 0 R >>\r\n";
            temp_os << "/ExtGState << /GS1 " << obj+1 << " 0 R >>\r\n";
            temp_os << ">>\r\n";
            temp_os << "endobj\r\n";
            obj++;

            // Object 7 is the "ExtGState" of Page 1 (whatever that is...)
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\r\n";
            temp_os << "<<\r\n";
            temp_os << "/Type /ExtGState\r\n";
            temp_os << "/SA false\r\n";
            temp_os << "/OP false\r\n";
            temp_os << "/HT /default\r\n";
            temp_os << ">>\r\n";
            temp_os << "endobj\r\n";
            obj++;

            // Object 8 is the (unique) Font that is used
            obj_offsets.push_back(temp_os.tellp());
            temp_os << obj << " 0 obj\r\n";
            temp_os << "<<\r\n";
            temp_os << "/Type /Font\r\n";
            temp_os << "/Subtype /Type1\r\n";
            temp_os << "/Name F4\r\n";
            temp_os << "/BaseFont Times-Roman\r\n";
            temp_os << ">>\r\n";
            temp_os << "endobj\r\n";
            obj++;




            // xrefs
            // Adobe, you are morons! No wonder you have one 0-day after another!
            int xref_offset = temp_os.tellp();
            temp_os << "xref\r\n";
            temp_os << "1 " << obj_offsets.size() << "\r\n";
            for(list<int>::iterator i = obj_offsets.begin(); i != obj_offsets.end(); i++)
            temp_os << setfill('0') << setw(10) << (*i) << " 00001 n\r\n";



            // trailer
            temp_os << "trailer\r\n";
            temp_os << "<<\r\n";
            temp_os << "/Size " << obj_offsets.size() << "\r\n"; // there are 3 entries in the xref table
            temp_os << "/Root 2 0 R\r\n"; // object 2 is the catalog
            temp_os << "/Info 1 0 R\r\n"; // object 1 contains the meta infos
            temp_os << ">>\r\n";
            temp_os << "startxref ";
            temp_os << (xref_offset+1) << "\r\n";
            temp_os << "%%EOF\r\n";

            (*os) << temp_os.str();
        }

        void GraphRendererPDF::SetPenColor(Visualization::Color color)
        {
            oss << (color.Red/256.0f) << " " << (color.Green/256.0f) << " " << (color.Blue/256.0f) << " RG\r\n";
        }

        void GraphRendererPDF::SetBrushColor(Visualization::Color color)
        {
            oss << (color.Red/256.0f) << " " << (color.Green/256.0f) << " " << (color.Blue/256.0f) << " rg\r\n";
        }

        void GraphRendererPDF::SetLineWidth(float width)
        {
            oss << (int)(width*ResolutionDPCM) << " w\r\n";
        }

        void GraphRendererPDF::Line(float x1, float y1, float x2, float y2)
        {
            oss << (int)(x1*ResolutionDPCM) << " " << (int)((ImageHeight-y1)*ResolutionDPCM) << " m "
                << (int)(x2*ResolutionDPCM) << " " << (int)((ImageHeight-y2)*ResolutionDPCM) << " l s\r\n";
        }


        void GraphRendererPDF::Circle(float x, float y, float Radius)
        {
            // http://forums.adobe.com/message/2312083
            // WTF, Adobe!? Seriously, WTF!?

            int radius = Radius * ResolutionDPCM;
            int h = 0.5522422 * Radius * ResolutionDPCM; // sine of 33 deg 31 sec

            oss << "q 1 0 0 1 " << x*ResolutionDPCM << " " << (ImageHeight-y)*ResolutionDPCM << " cm\r\n"
                << "0 " << -radius << " m\r\n"

                << h << " " << -radius << " " << radius << " " << -h << " " << radius << " " << 0 << "c\r\n"
                << radius << " " << h << " " << h << " " << radius << " " << 0 << " " << radius << "c\r\n"
                << -h << " " << radius << " " << -radius << " " << h << " " << -radius << " " << 0 << "c\r\n"
                << -radius << " " << -h << " " << -h << " " << -radius << " " << 0 << " " << -radius << "c\r\n"
                << "f Q\r\n";
        }

        void GraphRendererPDF::PutText(float x, float y, string text)
        {
        }

        FactoryRegistrator<ExportFilter> GraphRendererPDF::ExportFilterPdfRegistrator(&ExportFilter::ExportFilterFactory, "pdf",
            new DefaultInstantiator<ExportFilter, GraphicalExportFilter<GraphRendererPDF> >("pdf", "Portable Document Format", "http://en.wikipedia.org/wiki/Portable_Document_Format"));

    }
}
