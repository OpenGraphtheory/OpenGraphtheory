#ifndef __OPENGRAPHTHEORY_EXPORT_RGML_H
    #define __OPENGRAPHTHEORY_EXPORT_RGML_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterRGML : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterRgmlRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                map<Graph::EdgeIterator, Visualization::Color>& edgecoloring, float dpi, float edgewidth, float vertexradius);
            };

        }
    }

#endif
