#ifndef __OPENGRAPHTHEORY_EXPORT_POVRAY_H
    #define __OPENGRAPHTHEORY_EXPORT_POVRAY_H

    #include <ostream>
    #include <math.h>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterPOVRAY : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterPovrayRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os, VertexColoring& vertexcoloring,
                                EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius);
            };

        }
    }

#endif
