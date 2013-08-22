#ifndef __OPENGRAPHTHEORY_EXPORT_PDF_H
    #define __OPENGRAPHTHEORY_EXPORT_PDF_H

    #include <ostream>
    #include <iomanip>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterPDF : public GraphicalExportFilter
            {
                protected:
                    float ImageHeight;
                    float ResolutionDPCM;

                    int obj;
                    std::list<int> obj_offsets;
                    // unfortunately, this indirect writing is necessary. Otherwise tellp fails, if os is stdout and
                    // not redirected to a file (tellp then always returns -1, which would write a corrupted pdf)
                    std::stringstream temp_os;
                    std::stringstream oss;


                    static FactoryRegistrator<ExportFilter> ExportFilterPdfRegistrator;
                public:
                    void Begin(std::ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI);
                    void End(std::ostream &os);
                    void SetPenColor(std::ostream &os, Visualization::Color);
                    void SetBrushColor(std::ostream &os, Visualization::Color);
                    void SetLineWidth(std::ostream &os, float Width);
                    void Line(std::ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2);
                    void Circle(std::ostream &os, int node_id, float x, float y, float radius);
                    void PutText(std::ostream &os, float x, float y, std::string text);
            };

        }
    }

#endif
