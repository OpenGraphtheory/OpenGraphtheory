#ifndef __OPENGRAPHTHEORY_IMPORT_VRMLGRAPH_H
    #define __OPENGRAPHTHEORY_IMPORT_VRMLGRAPH_H

    #include <iostream>
    #include <sstream>
    #include <map>
    #include <string>
    #include "import.h"

    namespace OpenGraphtheory
    {
        namespace Import
        {

            class ImportFilterVRMLGRAPH : public ImportFilter
            {
                protected:
                    static FactoryRegistrator<ImportFilter> ImportFilterVRMLGRAPHRegistrator;
                public:
                    Graph Import(std::istream& is);
            };

        }
    }

#endif

