#ifndef __OPENGRAPHTHEORY_EXPORT_VRMLGRAPH_H
    #define __OPENGRAPHTHEORY_EXPORT_VRMLGRAPH_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterVRMLGRAPH : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterVrmlgraphRegistrator;
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                std::map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                std::map<Graph::EdgeIterator, Visualization::Color>& edgecoloring, float dpi, float edgewidth, float vertexradius);
            };

        }
    }

#endif
