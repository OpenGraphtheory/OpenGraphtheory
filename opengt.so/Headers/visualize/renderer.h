#ifndef __OPENGRAPHTHEORY_VISUALIZE_RENDERER_H
    #define __OPENGRAPHTHEORY_VISUALIZE_RENDERER_H

    #include "../opengt.h"
    #include "color.h"
    #include "vector2d.h"
    #include "../export/stringtranslator.h"
    #include <string>
    #include <math.h>

    namespace OpenGraphtheory
    {
        namespace Visualization
        {

            class GraphicsContext
            {
                public:
                    GraphicsContext();
                    virtual ~GraphicsContext();

                    virtual void SetPenColor(Color color);
                    virtual void SetBrushColor(Color color);
                    virtual void SetLineWidth(float width);

                    virtual void Line(float x1, float y1, float x2, float y2);
                    virtual void Arrow(float x1, float y1, float x2, float y2);
                    virtual void Circle(float x, float y, float radius);
                    virtual void PutText(float x, float y, std::string text);
            };

            class GraphRenderingContext : public GraphicsContext
            {
                protected:
                    Color LastPenColor;
                    Color LastBrushColor;
                    float LastLineWidth;
                    Export::StringTranslator* Translator;

                    virtual void BeginRenderingGraph(float WidthInCm, float HeightInCm, float ResolutionDPI);
                    virtual void EndRenderingGraph();

                    virtual void BeginDeclaringVertices();
                    virtual void DeclareVertex(int vertex_id, float x, float y, float radius, std::string text, Color color);
                    virtual void EndDeclaringVertices();

                    virtual void BeginRenderingEdges();
                    virtual void RenderEdge(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, std::string text, Color color);
                    virtual void RenderArc(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, std::string text, Color color);
                    virtual void EndRenderingEdges();

                    virtual void BeginRenderingVertices();
                    virtual void RenderVertex(int vertex_id, float x, float y, float radius, std::string text, Color color);
                    virtual void EndRenderingVertices();

                public:
                    GraphRenderingContext();
                    virtual ~GraphRenderingContext();

                    virtual void RenderGraph(Graph& G, std::map<Graph::VertexIterator, Color>& vertexcoloring,
                                             std::map<Graph::EdgeIterator, Color>& edgecoloring, float dpi,
                                             float edgewidth, float vertexradius);
            };

            class GraphRenderingContextStream : public GraphRenderingContext
            {
                protected:
                    std::ostream* os;
                public:
                    GraphRenderingContextStream(std::ostream& os);
            };

        }
    }

#endif

