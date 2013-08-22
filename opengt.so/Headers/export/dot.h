#ifndef __OPENGRAPHTHEORY_EXPORT_DOT_H
    #define __OPENGRAPHTHEORY_EXPORT_DOT_H

    #include <ostream>
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
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                std::map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                std::map<Graph::EdgeIterator, Visualization::Color>& edgecoloring, float dpi, float edgewidth, float vertexradius);
            };

        }
    }

#endif
