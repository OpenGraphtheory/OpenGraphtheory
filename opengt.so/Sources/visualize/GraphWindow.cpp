
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
            this->vertexColoring = vertexcoloring;
            this->edgeColoring = edgecoloring;
            this->EdgeWidth = EdgeWidth;
            this->VertexRadius = VertexRadius;

            this->MustUpdate = true;
            sem_init(&GUpdateSemaphore, 0, 1);
            sem_init(&MustUpdateSemaphore, 0, 1);

            Display(G);

            TerminateUpdateThread = false;
            if( pthread_create(&UpdateThread,NULL,RunUpdateThread,this) )
                throw "error creating window updating thread\n";

        }

        GraphWindow::~GraphWindow()
        {
            sem_destroy(&GUpdateSemaphore);
            sem_destroy(&MustUpdateSemaphore);
            TerminateUpdateThread = true;
            pthread_join(UpdateThread, NULL);
        }

        void GraphWindow::Display(Graph* G)
        {
            sem_wait(&GUpdateSemaphore);
            DisplayedGraph = G;
            sem_post(&GUpdateSemaphore);
            UpdateGraph();
        }

        void GraphWindow::UpdateGraph()
        {
            sem_wait(&GUpdateSemaphore);
            Clear();
            if(DisplayedGraph != NULL)
            {
                /*
                VertexPartitioning vcoloringInt = DisplayedGraph->GetVertexPartitioning(vertexColoring);
                VertexColoring vcoloring;
                for(VertexPartitioning::iterator i = vcoloringInt.begin(); i != vcoloringInt.end(); i++)
                    vcoloring[i->first] = Color::DefaultColors[i->second < Color::NumDefaultColors ? i->second : 0];

                EdgePartitioning ecoloringInt = DisplayedGraph->GetEdgePartitioning(edgeColoring);
                EdgeColoring ecoloring;
                for(EdgePartitioning::iterator i = ecoloringInt.begin(); i != ecoloringInt.end(); i++)
                    ecoloring[i->first] = Color::DefaultColors[i->second < Color::NumDefaultColors ? i->second : 0];
                */

                Visualization::GraphWindowRenderingContext* context = new Visualization::GraphWindowRenderingContext(this);
                context->RenderGraph(*DisplayedGraph, vertexColoring, edgeColoring, 75, EdgeWidth, VertexRadius);
                delete context;
            }
            Flush();
            sem_post(&GUpdateSemaphore);
        }



        bool GraphWindow::GetAndSetMustUpdate(bool newValue)
        {
            bool result;
            sem_wait(&MustUpdateSemaphore);
            result = MustUpdate;
            MustUpdate = newValue;
            sem_post(&MustUpdateSemaphore);
            return result;
        }

        void GraphWindow::RequestUpdate(Graph* G)
        {
            sem_wait(&MustUpdateSemaphore);
            MustUpdate = true;
            DisplayedGraph = G;
            sem_post(&MustUpdateSemaphore);
        }

        void* RunUpdateThread(void* GraphWin)
        {
            GraphWindow* GWin = (GraphWindow*)GraphWin;

            timespec SleepTime;
            SleepTime.tv_sec = 0;
            SleepTime.tv_nsec = 1000000 / 60;
            while(!GWin->TerminateUpdateThread)
            {
                if(GWin->GetAndSetMustUpdate(false))
                    GWin->UpdateGraph();
                nanosleep(&SleepTime, NULL);
            }

            return NULL;
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
