
#ifndef __OPENGRAPHTHEORY_EXPORT_EXPORT_TEMPLATES_CPP
    #define __OPENGRAPHTHEORY_EXPORT_EXPORT_TEMPLATES_CPP

    #include "../../Headers/export/export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            template<class T> void GraphicalExportFilter<T>::Export(OpenGraphtheory::Graph& G, std::ostream& os, VertexColoring& vertexcoloring,
                                                                    EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius)
            {
                OpenGraphtheory::Visualization::GraphRenderingContext* context = new T(os);
                context->RenderGraph(G, vertexcoloring, edgecoloring, dpi, edgewidth, vertexradius);
                delete(context);
            }

        }
    }

#endif
