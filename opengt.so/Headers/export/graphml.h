#ifndef __OPENGRAPHTHEORY_EXPORT_GRAPHML_H
    #define __OPENGRAPHTHEORY_EXPORT_GRAPHML_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            /// \ingroup ExportFilter

            class ExportFilterGRAPHML : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterGraphmlRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os, VertexColoring& vertexcoloring,
                                EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius);
            };

        }
    }

#endif
