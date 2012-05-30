#ifndef __OPENGRAPHTHEORY_EXPORT_SVG_H
    #define __OPENGRAPHTHEORY_EXPORT_SVG_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterSVG : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterSvgRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os);
            };

        }
    }

#endif
