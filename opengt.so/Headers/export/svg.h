#ifndef __OPENGRAPHTHEORY_EXPORT_SVG_H
    #define __OPENGRAPHTHEORY_EXPORT_SVG_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterSVG : public GraphicalExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterSvgRegistrator;
                    Visualization::Color BrushColor;
                    Visualization::Color PenColor;
                    float ResolutionDPCM;
                    float LineWidth;

                public:
                    void Begin(ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI);
                    void End(ostream &os);
                    void SetPenColor(ostream &os, Visualization::Color);
                    void SetBrushColor(ostream &os, Visualization::Color);
                    void SetLineWidth(ostream &os, float Width);
                    void Line(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2);
                    void Circle(ostream &os, int node_id, float x, float y, float radius);
                    void PutText(ostream &os, float x, float y, string text);
            };
        }
    }

#endif
