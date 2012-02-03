
#include "GraphWindow.h"
#include "../opengt.h"
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Visualization
    {

        GraphWindow::GraphWindow(int width, int height, Graph* G, string Caption, int gridsize)
            : DisplayWindow(width, height, Caption)
        {
            Display(G);
        }

        void GraphWindow::Display(Graph* G)
        {
            DisplayedGraph = G;
            Update();
        }

        void GraphWindow::Update()
        {
            Clear();
            if(DisplayedGraph != NULL)
            {
                UpdateModelDimensions();

                for(Graph::EdgeIterator e = DisplayedGraph->BeginEdges(); e != DisplayedGraph->EndEdges(); e++)
                {
                    float x1 = e.From().GetX();
                    float y1 = e.From().GetY();
                    float x2 = e.To().GetX();
                    float y2 = e.To().GetY();

                    ModelToScreen(x1, y1);
                    ModelToScreen(x2, y2);
                    Line(x1,y1,x2,y2);
                }

                for(Graph::VertexIterator v = DisplayedGraph->BeginVertices(); v != DisplayedGraph->EndVertices(); v++)
                {
                    float x = v.GetX();
                    float y = v.GetY();

                    ModelToScreen(x, y);
                    FillCircle(x, y, 15);
                }
            }
            Flush();
        }

        void GraphWindow::UpdateModelDimensions()
        {
            Graph::VertexIterator v = DisplayedGraph->BeginVertices();
            float minx = v.GetX();
            float miny = v.GetY();
            float maxx = minx;
            float maxy = miny;
            for(v++; v != DisplayedGraph->EndVertices(); v++)
            {
                if(v.GetX() < minx)
                    minx = v.GetX();
                else if(v.GetX() > maxx)
                    maxx = v.GetX();

                if(v.GetY() < miny)
                    miny = v.GetY();
                else if(v.GetY() > maxy)
                    maxy = v.GetY();
            }
            UpdateModelDimensions(minx, miny, maxx, maxy);
        }

        void GraphWindow::UpdateModelDimensions(float left, float top, float right, float bottom)
        {
            ModelLeft = left;
            ModelWidth = right - left;
            ModelTop = top;
            ModelHeight = bottom - top;
        }

        void GraphWindow::ModelToScreen(float &x, float &y)
        {
            float scaleX = width / ModelWidth;
            float scaleY = height / ModelHeight;
            float scale = scaleX < scaleY ? scaleX : scaleY;

            x = (x - ModelLeft) * scale;
            y = (y - ModelTop) * scale;
        }

    }
}
