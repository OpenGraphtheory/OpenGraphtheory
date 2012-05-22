
#include <string>
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

                virtual void Export(Graph& G, ostream& os) = 0;
                void Export(Graph& G, string FileName);
                string Export(Graph& G);
                static void Export(Graph& G, ostream& os, string format);
                static void Export(Graph& G, string FileName, string format);
        };


        class ExportFilterDOT : public ExportFilter
        {
            public:
                void Export(Graph& G, ostream& os);
        };

        class ExportFilterGASTEX : public ExportFilter
        {
            public:
                void Export(Graph& G, ostream& os);
        };

        class ExportFilterGML : public ExportFilter
        {
            public:
                void Export(Graph& G, ostream& os);
        };

        class ExportFilterGRAPHML : public ExportFilter
        {
            public:
                void Export(Graph& G, ostream& os);
        };

        class ExportFilterMPOST : public ExportFilter
        {
            public:
                void Export(Graph& G, ostream& os);
        };

        class ExportFilterRGML : public ExportFilter
        {
            public:
                void Export(Graph& G, ostream& os);
        };

        class ExportFilterSVG : public ExportFilter
        {
            public:
                void Export(Graph& G, ostream& os);
        };

        class ExportFilterTEI : public ExportFilter
        {
            public:
                void Export(Graph& G, ostream& os);
        };

        class ExportFilterTGF : public ExportFilter
        {
            public:
                void Export(Graph& G, ostream& os);
        };

        class ExportFilterVRMLGRAPH : public ExportFilter
        {
            public:
                void Export(Graph& G, ostream& os);
        };

        class ExportFilterXGMML : public ExportFilter
        {
            public:
                void Export(Graph& G, ostream& os);
        };

        class ExportFilterPOVRAY : public ExportFilter
        {
            public:
                void Export(Graph& G, ostream& os);
        };

    }
}
