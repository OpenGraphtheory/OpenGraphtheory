
#include <string>
#include "../opengt.h"

namespace OpenGraphtheory
{
    namespace Import
    {

        class ImportFilter
        {
            public:
                string name;
                string description;
                string URL;
            private:
                Graph (*ImportFunction)(istream& is);

            public:
                static map<string, ImportFilter> GetImportFilters();
                ImportFilter();
                ImportFilter(string name, string description, string URL, Graph ImportFunction(istream& is));

                Graph Import(istream& is);
                Graph Import(string FileName);
                static Graph Import(istream& is, string format);
                static Graph Import(string FileName, string format);
        };

        Graph ImportFromGXL(istream& is);

        Graph ImportFromTGF(istream& is);
        //Graph ImportFromVRMLGRAPH(istream& is);

        //Graph ImportFromGRAPHML(istream& is);
        //Graph ImportFromTEI(istream& is);
        //Graph ImportFromXGMML(istream& is);
        //Graph ImportFromSVG(istream& is);

        //Graph ImportFromDOT(istream& is);
        //Graph ImportFromGASTEX(istream& is);
        //Graph ImportFromGML(istream& is);
        //Graph ImportFromMPOST(istream& is);
        //Graph ImportFromRGML(istream& is);
    }
}

