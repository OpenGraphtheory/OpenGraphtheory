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

            /// \ingroup GraphicalExportFilter

            class GraphRendererPDF : public Visualization::GraphRenderingContextStream
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
                    GraphRendererPDF(std::ostream& os);
                    void BeginRenderingGraph(float WidthInCm, float HeightInCm, float ResolutionDPI);
                    void EndRenderingGraph();
                    void SetPenColor(Visualization::Color);
                    void SetBrushColor(Visualization::Color);
                    void SetLineWidth(float Width);
                    void Line(float x1, float y1, float x2, float y2);
                    void Circle(float x, float y, float radius);
                    void PutText(float x, float y, std::string text);
            };

        }
    }

#endif
