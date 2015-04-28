
#ifndef __OPENGRAPHTHEORY_EXPORT_EXPORT_H
    #define __OPENGRAPHTHEORY_EXPORT_EXPORT_H

    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <list>
    #include <math.h>
    #include <string>

    #include "stringtranslator.h"
    #include "../factory.h"
    #include "../opengt.h"
    #include "../visualize/color.h"
    #include "../visualize/vector2d.h"
    #include "../visualize/renderer.h"

    namespace OpenGraphtheory
    {

        /// \defgroup Export Exporting
        /// @{

        namespace Export
        {

            /// \brief Base Class for all Export Filters
            class ExportFilter
            {
                public:
                    static Factory<ExportFilter> ExportFilterFactory;
                    virtual ~ExportFilter();

                    virtual void Export(OpenGraphtheory::Graph& G, std::ostream& os, VertexColoring& vertexcoloring,
                                        EdgeColoring& edgecoloring, float dpi=72, float edgewidth=-1, float vertexradius=-1) = 0;
                    void DoExport(OpenGraphtheory::Graph& G, std::ostream& os, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72, float edgewidth=-1, float vertexradius=-1);
                    void DoExport(OpenGraphtheory::Graph& G, std::string FileName, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72, float edgewidth=-1, float vertexradius=-1);
                    std::string DoExport(OpenGraphtheory::Graph& G, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72, float edgewidth=-1, float vertexradius=-1);
                    static void Export(OpenGraphtheory::Graph& G, std::ostream& os, std::string format, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72, float edgewidth=-1, float vertexradius=-1);
                    static void Export(OpenGraphtheory::Graph& G, std::string FileName, std::string format, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72,float edgewidth=-1, float vertexradius=-1);
            };

            /// \defgroup ExportFilter
            /// \ingroup Export
            /// @{


            /// \brief Base Class for all Export Filters that produce graphical output
            template<class T> class GraphicalExportFilter : public ExportFilter
            {
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os, VertexColoring& vertexcoloring,
                                EdgeColoring& edgecoloring, float dpi=72, float edgewidth=-1, float vertexradius=-1);
            };

            /// \defgroup GraphicalExportFilter
            /// \ingroup ExportFilter

            /// @}

        }

        /// @}
    }

    #include "../../Sources/export/export_templates.cpp"

#endif
