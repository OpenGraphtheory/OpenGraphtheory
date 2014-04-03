#ifndef __OPENGRAPHTHEORY_IMPORT_XGMML_H
    #define __OPENGRAPHTHEORY_IMPORT_XGMML_H

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

            class ImportFilterXGMML : public ImportFilter
            {
                protected:
                    static FactoryRegistrator<ImportFilter> ImportFilterXGMMLRegistrator;
                public:
                    Graph Import(std::istream& is);
            };

        }
    }

#endif
