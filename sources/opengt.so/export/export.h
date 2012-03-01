
#include <string>
#include "../opengt.h"

namespace OpenGraphtheory
{
    namespace Export
    {

        class ExportFilter
        {
            public:
                string name;
                string description;
                string URL;
            private:
                void (*ExportFunction)(Graph& G, ostream& os);

            public:
                static map<string, ExportFilter> GetExportFilters();
                ExportFilter();
                ExportFilter(string name, string description, string URL, void ExportFunction(Graph& G, ostream& os));

                void Export(Graph& G, ostream& os);
                void Export(Graph& G, string FileName);
                string Export(Graph& G);
                static void Export(Graph& G, ostream& os, string format);
                static void Export(Graph& G, string FileName, string format);
        };


        void ExportToDOT(Graph& G, ostream& os);
        void ExportToGASTEX(Graph& G, ostream& os);
        void ExportToGML(Graph& G, ostream& os);
        void ExportToGRAPHML(Graph& G, ostream& os);
        void ExportToMPOST(Graph& G, ostream& os);
        void ExportToRGML(Graph& G, ostream& os);
        void ExportToSVG(Graph& G, ostream& os);
        void ExportToTEI(Graph& G, ostream& os);
        void ExportToTGF(Graph& G, ostream& os);
        void ExportToVRMLGRAPH(Graph& G, ostream& os);
        void ExportToXGMML(Graph& G, ostream& os);
        void ExportToGXL(Graph& G, ostream& os);
        void ExportToPOVRAY(Graph& G, ostream& os);
    }
}
