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

            class ExportFilterGRAPHML : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterGraphmlRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                map<Graph::EdgeIterator, Visualization::Color>& edgecoloring);
            };

        }
    }

#endif
