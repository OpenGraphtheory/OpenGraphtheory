
#include "../../Headers/visualize/GraphWindow.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Visualization
    {

        GraphWindow::GraphWindow(int width, int height, Graph* G, string Caption, string vertexcoloring,
                                 string edgecoloring, float EdgeWidth, float VertexRadius)
            : DisplayWindow(width, height, Caption)
        {
            this->VertexColoring = vertexcoloring;
            this->EdgeColoring = edgecoloring;
            this->EdgeWidth = EdgeWidth;
            this->VertexRadius = VertexRadius;
            sem_init(&GUpdateSemaphore, 0, 1);

            Display(G);
        }

        GraphWindow::~GraphWindow()
        {
            sem_destroy(&GUpdateSemaphore);
        }

        void GraphWindow::Display(Graph* G)
        {
            sem_wait(&GUpdateSemaphore);
            DisplayedGraph = G;
            sem_post(&GUpdateSemaphore);
            Update();
        }

        void GraphWindow::Update()
        {
            sem_wait(&GUpdateSemaphore);
            Clear();
            if(DisplayedGraph != NULL)
            {
                map<VertexIterator, int> vcoloringInt = DisplayedGraph->GetVertexColoring(VertexColoring);
                map<VertexIterator, Color> vcoloring;
                for(map<VertexIterator, int>::iterator i = vcoloringInt.begin(); i != vcoloringInt.end(); i++)
                    vcoloring[i->first] = Color::DefaultColors[i->second < Color::NumDefaultColors ? i->second : 0];

                map<EdgeIterator, Color> ecoloring;
                map<EdgeIterator, int> ecoloringInt = DisplayedGraph->GetEdgeColoring(EdgeColoring);
                for(map<EdgeIterator, int>::iterator i = ecoloringInt.begin(); i != ecoloringInt.end(); i++)
                    ecoloring[i->first] = Color::DefaultColors[i->second < Color::NumDefaultColors ? i->second : 0];


                Visualization::GraphWindowRenderingContext* context = new Visualization::GraphWindowRenderingContext(this);
                context->RenderGraph(*DisplayedGraph, vcoloring, ecoloring, 75, EdgeWidth, VertexRadius);
                delete context;
            }
            Flush();
            sem_post(&GUpdateSemaphore);
        }



        GraphWindowRenderingContext::GraphWindowRenderingContext(GraphWindow* window)
            : GraphRenderingContext(),
            PenColor(0,0,0),
            BrushColor(0,0,0)
        {
            this->window = window;
            this->LineWidth = 0.1;
        }

        void GraphWindowRenderingContext::SetPenColor(Color color)
        {
            PenColor = color;
        }

        void GraphWindowRenderingContext::SetBrushColor(Color color)
        {
            BrushColor = color;
        }

        void GraphWindowRenderingContext::SetLineWidth(float width)
        {
            LineWidth = width;
        }

        void GraphWindowRenderingContext::Line(float x1, float y1, float x2, float y2)
        {
            float scaleX = window->Width() / ModelWidth;
            float scaleY = window->Height() / ModelHeight;
            float scale = scaleX < scaleY ? scaleX : scaleY;

            ModelToScreen(x1, y1);
            ModelToScreen(x2, y2);
            window->SetColor(PenColor.Red, PenColor.Green, PenColor.Blue);
            window->Line((int)x1, (int)y1, (int)x2, (int)y2, (int)(LineWidth*scale));
        }

        void GraphWindowRenderingContext::Circle(float x, float y, float radius)
        {
            float scaleX = window->Width() / ModelWidth;
            float scaleY = window->Height() / ModelHeight;
            float scale = scaleX < scaleY ? scaleX : scaleY;

            ModelToScreen(x,y);
            window->SetColor(BrushColor.Red, BrushColor.Green, BrushColor.Blue);
            window->FillCircle((int)x,(int)y,(int)(radius*scale));
        }

        void GraphWindowRenderingContext::PutText(float x, float y, std::string text)
        {
            // ....
        }

        void GraphWindowRenderingContext::BeginRenderingGraph(float WidthInCm, float HeightInCm, float ResolutionDPI)
        {
            ModelWidth = WidthInCm;
            ModelHeight = HeightInCm;
        }

        void GraphWindowRenderingContext::ModelToScreen(float &x, float &y)
        {
            float scaleX = window->Width() / ModelWidth;
            float scaleY = window->Height() / ModelHeight;
            float scale = scaleX < scaleY ? scaleX : scaleY;

            x *= scale;
            y *= scale;
        }

        void GraphWindowRenderingContext::ScreenToModel(float &x, float &y)
        {
            float scaleX = window->Width() / ModelWidth;
            float scaleY = window->Height() / ModelHeight;
            float scale = scaleX < scaleY ? scaleX : scaleY;

            x /= scale;
            y /= scale;
        }

    }
}
