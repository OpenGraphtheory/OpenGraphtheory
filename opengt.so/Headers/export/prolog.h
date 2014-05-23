#ifndef __OPENGRAPHTHEORY_EXPORT_PROLOG_H
    #define __OPENGRAPHTHEORY_EXPORT_PROLOG_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterPROLOG : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterPrologRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os, VertexColoring& vertexcoloring,
                                EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius);
            };

        }
    }

#endif
