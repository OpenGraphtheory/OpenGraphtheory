#ifndef __OPENGRAPHTHEORY_IMPORT_RGML_H
    #define __OPENGRAPHTHEORY_IMPORT_RGML_H

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

            class ImportFilterRGML : public ImportFilter
            {
                protected:
                    static FactoryRegistrator<ImportFilter> ImportFilterRGMLRegistrator;
                public:
                    Graph Import(std::istream& is);
            };

        }
    }

#endif
