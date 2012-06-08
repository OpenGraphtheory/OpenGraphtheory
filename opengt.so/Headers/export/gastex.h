#ifndef __OPENGRAPHTHEORY_EXPORT_GASTEX_H
    #define __OPENGRAPHTHEORY_EXPORT_GASTEX_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterGASTEX : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterGastexRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                map<Graph::EdgeIterator, Visualization::Color>& edgecoloring);
            };

        }
    }

#endif
