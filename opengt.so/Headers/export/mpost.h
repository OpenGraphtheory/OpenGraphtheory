#ifndef __OPENGRAPHTHEORY_EXPORT_MPOST_H
    #define __OPENGRAPHTHEORY_EXPORT_MPOST_H

    #include <ostream>
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class GraphRendererMPOST : public Visualization::GraphRenderingContextStream
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterMpostRegistrator;

                    float ImageHeight;
                    bool DeclaringVertices;
                public:
                    GraphRendererMPOST(std::ostream& os);

                    void BeginRenderingGraph(float WidthInCm, float HeightInCm, float ResolutionDPI);
                    void EndRenderingGraph();

                    void BeginDeclaringVertices();
                    void DeclareVertex(int vertex_id, float x, float y, float radius, std::string text, Visualization::Color color);
                    void EndDeclaringVertices();
                    void EndRenderingEdges();

                    void RenderEdge(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, std::string text, Visualization::Color color);
                    void RenderArc(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, std::string text, Visualization::Color color);
                    void RenderVertex(int vertex_id, float x, float y, float radius, std::string text, Visualization::Color color);
                    void PutText(float x, float y, std::string text);

                    void SetPenColor(Visualization::Color color);
                    void SetBrushColor(Visualization::Color color);
                    void SetLineWidth(float width);
            };

        }
    }

#endif
