#include "../../Headers/export/pdf.h"
#include <iomanip>

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterPDF::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring, map<Graph::EdgeIterator, Color>& edgecoloring, float dpi)
        {
            if(G.IsHypergraph())
                throw "The PDF export filter does not support hypergraphs\n";

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                if(coordinates.size() < 2)
                    throw "Vertex with less than 2 coordinates found";
            }
            int obj = 1;
            list<int> obj_offsets;

            // unfortunately, this indirect writing is necessary. Otherwise tellp fails, if os is stdout and
            // not redirected to a file (tellp then always returns -1, which would write a corrupted pdf)
            stringstream temp_os;

            /// header
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


/*
            /// draw vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                os << "drawdot z" << v.GetID() << " withpen pencircle"
                   << " scaled " << 2*v.GetWeight();
                if(vertexcoloring.find(v) != vertexcoloring.end())
                    os << " withcolor (" << (vertexcoloring[v].Red/256.0f) << ","<< (vertexcoloring[v].Green/256.0f) << "," << (vertexcoloring[v].Blue/256.0f) << ");\n";
                else
                    os << " withcolor Black;\n";
                os << "(" << v.GetLabel() << ") \n";
            }

            /// draw edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << " draw" << (e.IsEdge()?"":"arrow") << " z" << e.From().GetID() << " -- z" << e.To().GetID();
                if(edgecoloring.find(e) != edgecoloring.end())
                    os << " withrgbcolor (" << (edgecoloring[e].Red/256.0f) << ","<< (edgecoloring[e].Green/256.0f) << "," << (edgecoloring[e].Blue/256.0f) << ");\n";
                else
                    os << " withcolor Black;\n";
                os << "  label.ulft(btex "<< e.GetLabel() << " etex, (z"<<e.From().GetID()<<"+z"<<e.To().GetID()<<")/2);\n";
            }
*/


            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                vector<float> from = e.From().GetCoordinates();
                vector<float> to = e.To().GetCoordinates();

                oss << (int)(from[0]*dpi/2.54) << " " << (int)(from[1]*dpi/2.54) << " m " << (int)(to[0]*dpi/2.54) << " " << (int)(to[1]*dpi/2.54) << " l s\n";
                /*
                if(edgecoloring.find(e) != edgecoloring.end())
                    os << " withrgbcolor (" << (edgecoloring[e].Red/256.0f) << ","<< (edgecoloring[e].Green/256.0f) << "," << (edgecoloring[e].Blue/256.0f) << ");\n";
                else
                    os << " withcolor Black;\n";
                */

                //os << "("<< e.GetLabel() << ")\n";
            }


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

        FactoryRegistrator<ExportFilter> ExportFilterPDF::ExportFilterPdfRegistrator(&ExportFilter::ExportFilterFactory, "pdf",
            new DefaultInstantiator<ExportFilter, ExportFilterPDF>("pdf", "Portable Document Format", "http://en.wikipedia.org/wiki/Portable_Document_Format"));

    }
}
