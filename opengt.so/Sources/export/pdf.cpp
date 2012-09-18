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

            /// header
            os << "%PDF-1.0\n";

            // Object 1 for Meta-Infos of the file
            int offset10 = os.tellp();
            os << "1 0 obj\n";
            os << "<<\n";
            os << "/Creator (www.Open-Graphtheory.org)\n";
            os << ">>\n";
            os << "endobj\n";

            // Object 2 for the "Catalog"
            int offset20 = os.tellp();
            os << "2 0 obj\n";
            os << "<<\n";
            os << "/Type /Catalog\n";
            os << "/Pages 3 0 R\n"; // object 3 is the pages tree
            //os << "/Outlines 4 0 R\n";
            //os << "/PageMode /UseOutlines\n";
            os << ">>\n";
            os << "endobj\n";

            // Object 3 is the Page-Tree
            int offset30 = os.tellp();
            os << "3 0 obj\n";
            os << "<<\n";
            os << "/Type /Pages\n";
            os << "/Kids [4 0 R]\n"; // object 4 is the only page of the document
            os << "/Count 1\n";
            os << ">>\n";
            os << "endobj\n";

            // The actual graphics data
            int offset40 = os.tellp();
            os << "4 0 obj\n";
            os << "stream\n";

            stringstream sstream;


/*
            os << "pagecoords:=identity scaled 100mm;\n";
            os << "beginfig(1);\n\n";

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

                os << (int)from[0] << " " << (int)from[1] << " m " << (int)to[0] << " " << (int)to[1] << " l s\n";
                /*
                if(edgecoloring.find(e) != edgecoloring.end())
                    os << " withrgbcolor (" << (edgecoloring[e].Red/256.0f) << ","<< (edgecoloring[e].Green/256.0f) << "," << (edgecoloring[e].Blue/256.0f) << ");\n";
                else
                    os << " withcolor Black;\n";
                */

                //os << "("<< e.GetLabel() << ")\n";
            }

            os << "endstream\n";
            os << "endobj\n";



            // xrefs
            // Adobe, you are morons! No wonder you have one 0-day after another!
            int xref_offset = os.tellp();
            os << "xref\n";
            os << "1 4\n";
            os << setw(10) << offset10 << " 00001 n\r\n";
            os << setw(10) << offset20 << " 00001 n\r\n";
            os << setw(10) << offset30 << " 00001 n\r\n";
            os << setw(10) << offset40 << " 00001 n\r\n";



            // trailer
            os << "trailer\n";
            os << "<<\n";
            os << "/Size 4\n"; // there are 3 entries in the xref table
            os << "/Root 2 0 R\n"; // object 2 is the catalog
            os << "/Info 1 0 R\n"; // object 1 contains the meta infos
            os << ">>\n";
            os << "startxref\n";
            os << xref_offset << "\n";
            os << "%%EOF\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterPDF::ExportFilterPdfRegistrator(&ExportFilter::ExportFilterFactory, "pdf",
            new DefaultInstantiator<ExportFilter, ExportFilterPDF>("pdf", "Portable Document Format", "http://en.wikipedia.org/wiki/Portable_Document_Format"));

    }
}
