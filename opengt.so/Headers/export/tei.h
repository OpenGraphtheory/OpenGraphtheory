#ifndef __OPENGRAPHTHEORY_EXPORT_TEI_H
    #define __OPENGRAPHTHEORY_EXPORT_TEI_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterTEI : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterTeiRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os);
            };

        }
    }

#endif
