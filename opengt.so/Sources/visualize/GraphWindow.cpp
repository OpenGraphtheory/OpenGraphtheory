
#include "../../Headers/visualize/GraphWindow.h"
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Visualization
    {

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

                    if(e.Attributes().HasAttribute(EdgeColoring))
                    {
                        IntAttribute* attrEdgeColoring = dynamic_cast<IntAttribute*>(e.Attributes().GetAttribute(EdgeColoring));
                        if(attrEdgeColoring != NULL)
                            SetColor(Color::DefaultColors[attrEdgeColoring->Value].Red, Color::DefaultColors[attrEdgeColoring->Value].Green, Color::DefaultColors[attrEdgeColoring->Value].Blue);
                        BoolAttribute* attrEdgeSet = dynamic_cast<BoolAttribute*>(e.Attributes().GetAttribute(EdgeColoring));
                        if(attrEdgeSet != NULL && attrEdgeSet->Value)
                            SetColor(Color::DefaultColors[0].Red, Color::DefaultColors[0].Green, Color::DefaultColors[0].Blue);
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

                    if(v.Attributes().HasAttribute(VertexColoring))
                    {
                        IntAttribute* attrVertexColoring = dynamic_cast<IntAttribute*>(v.Attributes().GetAttribute(VertexColoring));
                        if(attrVertexColoring != NULL)
                            SetColor(Color::DefaultColors[attrVertexColoring->Value].Red, Color::DefaultColors[attrVertexColoring->Value].Green, Color::DefaultColors[attrVertexColoring->Value].Blue);
                        BoolAttribute* attrVertexSet = dynamic_cast<BoolAttribute*>(v.Attributes().GetAttribute(VertexColoring));
                        if(attrVertexSet != NULL && attrVertexSet->Value)
                            SetColor(Color::DefaultColors[0].Red, Color::DefaultColors[0].Green, Color::DefaultColors[0].Blue);

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
