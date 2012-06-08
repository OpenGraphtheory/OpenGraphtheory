#ifndef __OPENGRAPHTHEORY_EXPORT_GML_H
    #define __OPENGRAPHTHEORY_EXPORT_GML_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterGML : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterGmlRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                map<Graph::EdgeIterator, Visualization::Color>& edgecoloring);
            };

        }
    }

#endif
