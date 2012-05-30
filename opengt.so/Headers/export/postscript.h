#ifndef __OPENGRAPHTHEORY_EXPORT_POSTSCRIPT_H
    #define __OPENGRAPHTHEORY_EXPORT_POSTSCRIPT_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterPOSTSCRIPT : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterPostscriptRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os);
            };

        }
    }

#endif
