#ifndef __OPENGRAPHTHEORY_EXPORT_TIKZ_H
    #define __OPENGRAPHTHEORY_EXPORT_TIKZ_H

    #include <ostream>
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class GraphRendererTIKZ : public Visualization::GraphRenderingContextStream
            {
                protected:

                    static FactoryRegistrator<ExportFilter> ExportFilterTikzRegistrator;
                    float ImageHeight;
                    float LineWidth;
                    bool RenderingVertices;
                    bool RenderingEdges;

                public:
                    GraphRendererTIKZ(std::ostream& os);
                    void BeginRenderingGraph(float WidthInCm, float HeightInCm, float ResolutionDPI);
                    void EndRenderingGraph();

                    void SetPenColor(Visualization::Color);
                    void SetBrushColor(Visualization::Color);
                    void SetLineWidth(float Width);

                    void DeclareVertex(int vertex_id, float x, float y, float radius, std::string text, Visualization::Color);

                    void BeginRenderingVertices();
                    void EndRenderingVertices();

                    void BeginRenderingEdges();
                    void RenderEdge(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, std::string text, Visualization::Color color);
                    void RenderArc(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, std::string text, Visualization::Color color);
                    void EndRenderingEdges();

                    void PutText(float x, float y, std::string text);
            };

        }
    }

#endif
