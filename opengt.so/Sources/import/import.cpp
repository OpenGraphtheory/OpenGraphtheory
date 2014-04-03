
#include "../../Headers/import/import.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Import
    {

        Factory<ImportFilter> ImportFilter::ImportFilterFactory;

        ImportFilter::ImportFilter()
        {

        }

        ImportFilter::~ImportFilter()
        {

        }

        Graph ImportFilter::Import(string FileName)
        {
            ifstream is(FileName.c_str());
            return Import(is);
            // is.close(); done in the destructor anyways. no need to copy the result of Import(is)
        }

        Graph ImportFilter::Import(istream& is, string format)
        {
            ImportFilter* import = ImportFilterFactory.Produce(format);
            if(import == NULL)
                throw "unknown import-format";
            Graph result = import->Import(is);
            delete import;
            return result;
        }

        Graph ImportFilter::Import(string FileName, string format)
        {
            ImportFilter* import = ImportFilterFactory.Produce(format);
            if(import == NULL)
                throw "unknown import-format";
            Graph result = import->Import(FileName);
            delete import;
            return result;
        }

    }
}

