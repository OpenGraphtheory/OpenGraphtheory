
#include "GraphWindow.h"
#include "../opengt.h"
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Visualization
    {

        int defaultcolors[] = { 0xE50000, // red
                               0xF3D510, // yellow
                               0x34cd20, // green
                               0x1C23EB, // dark blue
                               0xF37D23, // orange
                               0x15CDC7, // light blue
                               0x8722E5, // purple
                               0xFF33FF // pink
                             };

        GraphWindow::GraphWindow(int width, int height, Graph* G, string Caption, string vertexcoloring, string edgecoloring, int gridsize, bool ZoomToFit)
            : DisplayWindow(width, height, Caption)
        {
            ModelLeft = 0;
            ModelTop = 0;
            ModelWidth = width;
            ModelHeight = height;
            this -> ZoomToFit = ZoomToFit;
            this -> VertexColoring = vertexcoloring;
            this -> EdgeColoring = edgecoloring;
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
                if(ZoomToFit)
                    UpdateModelDimensions();

                for(Graph::EdgeIterator e = DisplayedGraph->BeginEdges(); e != DisplayedGraph->EndEdges(); e++)
                {
                    float x1 = e.From().GetX();
                    float y1 = e.From().GetY();
                    float x2 = e.To().GetX();
                    float y2 = e.To().GetY();

                    ModelToScreen(x1, y1);
                    ModelToScreen(x2, y2);

                    if(e.Attributes().HasIntAttribute(EdgeColoring))
                    {
                        int color = defaultcolors[e.Attributes().GetIntAttribute(EdgeColoring)];
                        SetColor((short)(color / 65536), (short)((color / 256) % 256), (short)(color % 256));
                    }
                    else
                        SetColor(0,0,0);

                    Line(x1,y1,x2,y2);
                }

                for(Graph::VertexIterator v = DisplayedGraph->BeginVertices(); v != DisplayedGraph->EndVertices(); v++)
                {
                    float x = v.GetX();
                    float y = v.GetY();

                    ModelToScreen(x, y);

                    if(v.Attributes().HasIntAttribute(VertexColoring))
                    {
                        int color = defaultcolors[v.Attributes().GetIntAttribute(VertexColoring)];
                        SetColor((unsigned short)(color / 65536), (unsigned short)((color / 256) % 256), (unsigned short)(color % 256));
                    }
                    else
                        SetColor(0,0,0);

                    FillCircle(x, y, 7);
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

        void GraphWindow::ScreenToModel(float &x, float &y)
        {
            float scaleX = width / ModelWidth;
            float scaleY = height / ModelHeight;
            float scale = scaleX < scaleY ? scaleX : scaleY;

            x = x / scale + ModelLeft;
            y = y / scale + ModelTop;
        }

    }
}
