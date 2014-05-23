
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
            VertexPartitioning vcoloringInt = G.GetVertexPartitioning(vertexcoloring);
            VertexColoring vcoloring;
            for(VertexPartitioning::iterator i = vcoloringInt.begin(); i != vcoloringInt.end(); i++)
                vcoloring[i->first] = Color::DefaultColors[i->second < Color::NumDefaultColors ? i->second : 0];

            EdgePartitioning ecoloringInt = G.GetEdgePartitioning(edgecoloring);
            EdgeColoring ecoloring;
            for(EdgePartitioning::iterator i = ecoloringInt.begin(); i != ecoloringInt.end(); i++)
                ecoloring[i->first] = Color::DefaultColors[i->second < Color::NumDefaultColors ? i->second : 0];

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
