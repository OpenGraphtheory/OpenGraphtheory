#ifndef __OPENGRAPHTHEORY_IMPORT_LGF_H
    #define __OPENGRAPHTHEORY_IMPORT_LGF_H

    #include <iostream>
    #include <sstream>
    #include <map>
    #include <string>
    #include "import.h"

    namespace OpenGraphtheory
    {
        namespace Import
        {

            class ImportFilterLGF : public ImportFilter
            {
                protected:
                    static FactoryRegistrator<ImportFilter> ImportFilterLgfRegistrator;
                public:
                    Graph Import(std::istream& is);
            };

            class LGFSection
            {
                public:
                    std::string name;
                    std::vector<std::string> columnnames;
                    std::map<std::string, int> column_number;
                    std::list<std::list<std::string> > rows;
            };

        }
    }

#endif

