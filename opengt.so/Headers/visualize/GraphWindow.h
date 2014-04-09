#ifndef __OPENGRAPHTHEORY_VISUALIZE_GRAPHWINDOW_H
    #define __OPENGRAPHTHEORY_VISUALIZE_GRAPHWINDOW_H

    #include "DisplayWindow.h"
    #include "renderer.h"
    #include "color.h"
    #include "../opengt.h"

    namespace OpenGraphtheory
    {
        namespace Visualization
        {

            class GraphWindow : public DisplayWindow
            {
                protected:
                    int gridsize;
                    Graph* DisplayedGraph;

                    std::map<Graph::VertexIterator, Color> VertexColoring;
                    std::map<Graph::EdgeIterator, Color> EdgeColoring;
                    float EdgeWidth;
                    float VertexRadius;

                public:

                    GraphWindow(int width, int height, Graph* G, std::string Caption = "http://www.Open-Graphtheory.org",
                                std::string vertexcoloring = "", std::string edgecoloring = "", int gridsize = 1,
                                float EdgeWidth = -1, float VertexRadius = -1);

                    void Display(Graph* G);
                    void Update();

                    void (*OnVertexMouseDown) (Graph::VertexIterator v, MouseButton Button,   unsigned short ButtonStates);
                    void (*OnVertexMouseUp)   (Graph::VertexIterator v, MouseButton Button,   unsigned short ButtonStates);

            };

            class GraphWindowRenderingContext : public GraphRenderingContext
            {
                protected:
                    GraphWindow* window;
                    float LineWidth;
                    Color PenColor;
                    Color BrushColor;

                    float ModelWidth;
                    float ModelHeight;
                    void ModelToScreen(float &x, float& y);
                    void ScreenToModel(float &x, float& y);

                public:
                    GraphWindowRenderingContext(GraphWindow* window);
                    void BeginRenderingGraph(float WidthInCm, float HeightInCm, float ResolutionDPI);

                    void SetPenColor(Color color);
                    void SetBrushColor(Color color);
                    void SetLineWidth(float width);

                    void Line(float x1, float y1, float x2, float y2);
                    void Circle(float x, float y, float radius);
                    void PutText(float x, float y, std::string text);
            };

        }
    }

#endif
