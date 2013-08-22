
#include <string>
#include <iostream>
#include "../opengt.h"

namespace OpenGraphtheory
{
    namespace Import
    {

        class ImportFilter
        {
            public:
                static Factory<ImportFilter> ImportFilterFactory;
                ImportFilter();
                virtual ~ImportFilter();

                virtual Graph Import(std::istream& is) = 0;

                Graph Import(std::string FileName);
                static Graph Import(std::istream& is, std::string format);
                static Graph Import(std::string FileName, std::string format);
        };

    }
}

