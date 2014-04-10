#ifndef __OPENGRAPHTHEORY_IMPORT_TEI_H
    #define __OPENGRAPHTHEORY_IMPORT_TEI_H

    #include <iostream>
    #include <sstream>
    #include <map>
    #include <string>
    #include "import.h"
    #include "../simple_xml.h"

    namespace OpenGraphtheory
    {
        namespace Import
        {

            class ImportFilterTEI : public ImportFilter
            {
                protected:
                    static FactoryRegistrator<ImportFilter> ImportFilterTEIRegistrator;
                public:
                    Graph Import(std::istream& is);
            };

        }
    }

#endif
