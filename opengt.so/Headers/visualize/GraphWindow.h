
#include "DisplayWindow.h"
#include "../opengt.h"
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Visualization
    {

        class GraphWindow : public DisplayWindow
        {
            protected:
                int gridsize;
                bool ZoomToFit;
                Graph* DisplayedGraph;

                string VertexColoring;
                string EdgeColoring;

                float ModelLeft, ModelWidth, ModelTop, ModelHeight;
                void UpdateModelDimensions();
                void UpdateModelDimensions(float left, float top, float right, float bottom);
                void ModelToScreen(float &x, float& y);
                void ScreenToModel(float &x, float& y);

            public:

                GraphWindow(int width, int height, Graph* G, string Caption = "http://www.Open-Graphtheory.org", string vertexcoloring = "",
                            string edgecoloring = "", int gridsize = 1, bool ZoomToFit = false);

                void Display(Graph* G);
                void Update();

                void (*OnVertexMouseDown) (Graph::VertexIterator v, MouseButton Button,   unsigned short ButtonStates);
                void (*OnVertexMouseUp)   (Graph::VertexIterator v, MouseButton Button,   unsigned short ButtonStates);

        };


    }
}

