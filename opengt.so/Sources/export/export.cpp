
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include "../../Headers/export/export.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        Factory<ExportFilter> ExportFilter::ExportFilterFactory;

        ExportFilter::~ExportFilter()
        {

        }

        void ExportFilter::Export(Graph& G, string FileName)
        {
            ofstream os(FileName.c_str());
            Export(G, os);
            os.close();
        }

        string ExportFilter::Export(Graph& G)
        {
            stringstream sstream;
            Export(G, sstream);
            return sstream.str();
        }

        void ExportFilter::Export(Graph& G, ostream& os, string format)
        {
            ExportFilter* exportfilter = ExportFilter::ExportFilterFactory.Produce(format);
            if(exportfilter == NULL)
                throw "unknown export-format";

            exportfilter->Export(G, os);
            delete exportfilter;
        }

        void ExportFilter::Export(Graph& G, string FileName, string format)
        {
            ExportFilter* exportfilter = ExportFilter::ExportFilterFactory.Produce(format);
            if(exportfilter == NULL)
                throw "unknown export-format";

            exportfilter->Export(G, FileName);
            delete exportfilter;
        }



    }
}
