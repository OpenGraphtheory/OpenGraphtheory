#ifndef __OPENGRAPHTHEORY_EXPORT_MPOST_H
    #define __OPENGRAPHTHEORY_EXPORT_MPOST_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterMPOST : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterMpostRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os);
            };

        }
    }

#endif
