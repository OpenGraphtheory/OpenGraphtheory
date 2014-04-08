#ifndef __OPENGRAPHTHEORY_EXPORT_POSTSCRIPT_H
    #define __OPENGRAPHTHEORY_EXPORT_POSTSCRIPT_H

    #include <ostream>
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class GraphRendererPOSTSCRIPT : public Visualization::GraphRenderingContextStream
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterPostscriptRegistrator;
                    float ImageHeight;
                    float ResolutionDPCM;

                public:
                    GraphRendererPOSTSCRIPT(std::ostream& os);
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
