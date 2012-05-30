
#include <string>
#include <iostream>
#include "../opengt.h"

namespace OpenGraphtheory
{
    namespace Import
    {

        class ImportFilter
        {
            protected:
                static Factory<ImportFilter> ImportFilterFactory;
            public:
                ImportFilter();
                virtual ~ImportFilter();

                virtual Graph Import(istream& is) = 0;

                Graph Import(string FileName);
                static Graph Import(istream& is, string format);
                static Graph Import(string FileName, string format);
        };

    }
}

