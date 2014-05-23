#ifndef __OPENGRAPHTHEORY_EXPORT_DOT_H
    #define __OPENGRAPHTHEORY_EXPORT_DOT_H

    #include <ostream>
    #include <iomanip>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterDOT : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterDotRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os, VertexColoring& vertexcoloring,
                                EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius);
            };

        }
    }

#endif
