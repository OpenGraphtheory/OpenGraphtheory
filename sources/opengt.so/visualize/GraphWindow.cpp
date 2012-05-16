
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
                    vector<float> FromCoordinates = e.From().GetCoordinates();
                    vector<float> ToCoordinates = e.To().GetCoordinates();
                    float x1 = FromCoordinates[0];
                    float y1 = FromCoordinates[1];
                    float x2 = ToCoordinates[0];
                    float y2 = ToCoordinates[1];

                    ModelToScreen(x1, y1);
                    ModelToScreen(x2, y2);

                    if(e.Attributes().HasAttribute<IntAttribute>(EdgeColoring))
                    {
                        int color = defaultcolors[e.Attributes().GetAttribute<IntAttribute>(EdgeColoring)->Value];
                        SetColor((short)(color / 65536), (short)((color / 256) % 256), (short)(color % 256));
                    }
                    else
                        SetColor(0,0,0);

                    Line(x1,y1,x2,y2);
                }

                for(Graph::VertexIterator v = DisplayedGraph->BeginVertices(); v != DisplayedGraph->EndVertices(); v++)
                {
                    vector<float> coordinates = v.GetCoordinates();
                    float x = coordinates[0];
                    float y = coordinates[1];

                    ModelToScreen(x, y);

                    if(v.Attributes().HasAttribute<IntAttribute>(VertexColoring))
                    {
                        int color = defaultcolors[v.Attributes().GetAttribute<IntAttribute>(VertexColoring)->Value];
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
            vector<float> coordinates = v.GetCoordinates();
            float minx = coordinates[0];
            float miny = coordinates[1];
            float maxx = minx;
            float maxy = miny;
            for(v++; v != DisplayedGraph->EndVertices(); v++)
            {
                coordinates = v.GetCoordinates();

                if(coordinates[0] < minx)
                    minx = coordinates[0];
                else if(coordinates[0] > maxx)
                    maxx = coordinates[0];

                if(coordinates[1] < miny)
                    miny = coordinates[1];
                else if(coordinates[1] > maxy)
                    maxy = coordinates[1];
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
