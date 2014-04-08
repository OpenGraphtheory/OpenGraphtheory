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

            class GraphRendererSVG : public Visualization::GraphRenderingContextStream
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterSvgRegistrator;
                    Visualization::Color BrushColor;
                    Visualization::Color PenColor;
                    float ResolutionDPCM;
                    float LineWidth;

                public:
                    GraphRendererSVG(std::ostream& os);
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
