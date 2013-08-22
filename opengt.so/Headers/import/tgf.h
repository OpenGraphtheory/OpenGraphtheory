
#include <string>
#include "import.h"

namespace OpenGraphtheory
{
    namespace Import
    {

        class ImportFilterTGF : public ImportFilter
        {
            protected:
                static FactoryRegistrator<ImportFilter> ImportFilterTgfRegistrator;
            public:
                Graph Import(std::istream& is);
        };

    }
}

