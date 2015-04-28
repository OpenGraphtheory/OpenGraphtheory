#ifndef __OPENGRAPHTHEORY_EXPORT_TGF_H
    #define __OPENGRAPHTHEORY_EXPORT_TGF_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            /// \ingroup ExportFilter

            class ExportFilterTGF : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterTgfRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os, VertexColoring& vertexcoloring,
                                EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius);
            };

        }
    }

#endif
