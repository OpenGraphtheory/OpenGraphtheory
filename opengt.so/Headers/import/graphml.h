#ifndef __OPENGRAPHTHEORY_IMPORT_GRAPHML_H
    #define __OPENGRAPHTHEORY_IMPORT_GRAPHML_H

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

            class ImportFilterGRAPHML : public ImportFilter
            {
                protected:
                    static FactoryRegistrator<ImportFilter> ImportFilterGRAPHMLRegistrator;
                public:
                    Graph Import(std::istream& is);
            };

        }
    }

#endif
