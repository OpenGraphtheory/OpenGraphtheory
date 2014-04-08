
#include "../../Headers/export/export.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        Factory<ExportFilter> ExportFilter::ExportFilterFactory;

        ExportFilter::~ExportFilter()
        {

        }


        void ExportFilter::DoExport(Graph& G, ostream& os, std::string vertexcoloring, std::string edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            map<Graph::VertexIterator, Color> vcoloring;
            map<Graph::EdgeIterator, Color> ecoloring;

            if(vertexcoloring != "")
            {
                for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                {
                    Attribute* attr = v.Attributes().GetAttribute(vertexcoloring);
                    IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
                    if(iattr != NULL)
                        vcoloring[v] = Color::DefaultColors[iattr->Value < 7 ? iattr->Value : 0];
                    else
                    {
                        BoolAttribute* battr = dynamic_cast<BoolAttribute*>(attr);
                        if(battr != NULL && battr->Value)
                            vcoloring[v] = Color::DefaultColors[0];
                    }
                }
            }

            if(edgecoloring != "")
            {
                for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                {
                    Attribute* attr = e.Attributes().GetAttribute(edgecoloring);
                    IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
                    if(iattr != NULL)
                        ecoloring[e] = Color::DefaultColors[iattr->Value < 7 ? iattr->Value : 0];
                    else
                    {
                        BoolAttribute* battr = dynamic_cast<BoolAttribute*>(attr);
                        if(battr != NULL && battr->Value)
                            ecoloring[e] = Color::DefaultColors[0];
                    }
                }
            }

            Export(G, os, vcoloring, ecoloring, dpi, edgewidth, vertexradius);
        }


        void ExportFilter::DoExport(Graph& G, string FileName, std::string vertexcoloring, std::string edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            ofstream os(FileName.c_str());
            DoExport(G, os, vertexcoloring, edgecoloring, dpi, edgewidth, vertexradius);
            os.close();
        }

        string ExportFilter::DoExport(Graph& G, std::string vertexcoloring, std::string edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            stringstream sstream;
            DoExport(G, sstream, vertexcoloring, edgecoloring, dpi, edgewidth, vertexradius);
            return sstream.str();
        }



        void ExportFilter::Export(Graph& G, ostream& os, string format, std::string vertexcoloring, std::string edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            ExportFilter* exportfilter = ExportFilter::ExportFilterFactory.Produce(format);
            if(exportfilter == NULL)
                throw "unknown export-format";

            exportfilter->DoExport(G, os, vertexcoloring, edgecoloring, dpi, edgewidth, vertexradius);
            delete exportfilter;
        }

        void ExportFilter::Export(Graph& G, string FileName, string format, std::string vertexcoloring, std::string edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            ExportFilter* exportfilter = ExportFilter::ExportFilterFactory.Produce(format);
            if(exportfilter == NULL)
                throw "unknown export-format";

            exportfilter->DoExport(G, FileName, vertexcoloring, edgecoloring, dpi, edgewidth, vertexradius);
            delete exportfilter;
        }
    }
}
