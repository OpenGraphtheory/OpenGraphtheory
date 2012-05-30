
#ifndef __OPENGT_EXPORT_EXPORT_H
    #define __OPENGT_EXPORT_EXPORT_H

    #include <string>
    #include <ostream>
    #include "../factory.h"
    #include "../opengt.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilter
            {
                public:
                    static Factory<ExportFilter> ExportFilterFactory;
                    virtual ~ExportFilter();

                    virtual void Export(OpenGraphtheory::Graph& G, std::ostream& os) = 0;
                    void Export(OpenGraphtheory::Graph& G, std::string FileName);
                    string Export(OpenGraphtheory::Graph& G);
                    static void Export(OpenGraphtheory::Graph& G, std::ostream& os, std::string format);
                    static void Export(OpenGraphtheory::Graph& G, std::string FileName, std::string format);
            };

        }
    }

#endif
