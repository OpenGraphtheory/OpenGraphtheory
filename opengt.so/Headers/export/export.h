
#ifndef __OPENGT_EXPORT_EXPORT_H
    #define __OPENGT_EXPORT_EXPORT_H

    #include <string>
    #include <ostream>
    #include "../factory.h"
    #include "../opengt.h"
    #include "../visualize/color.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilter
            {
                public:
                    static Factory<ExportFilter> ExportFilterFactory;
                    virtual ~ExportFilter();

                    virtual void Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                map<Graph::EdgeIterator, Visualization::Color>& edgecoloring) = 0;
                    void DoExport(OpenGraphtheory::Graph& G, ostream& os, std::string vertexcoloring="", std::string edgecoloring="");
                    void DoExport(OpenGraphtheory::Graph& G, std::string FileName, std::string vertexcoloring="", std::string edgecoloring="");
                    string DoExport(OpenGraphtheory::Graph& G, std::string vertexcoloring="", std::string edgecoloring="");
                    static void Export(OpenGraphtheory::Graph& G, std::ostream& os, std::string format, std::string vertexcoloring="", std::string edgecoloring="");
                    static void Export(OpenGraphtheory::Graph& G, std::string FileName, std::string format, std::string vertexcoloring="", std::string edgecoloring="");


            };

        }
    }

#endif
