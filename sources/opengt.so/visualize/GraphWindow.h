
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
                Graph* DisplayedGraph;

                float ModelLeft, ModelWidth, ModelTop, ModelHeight;
                void UpdateModelDimensions();
                void UpdateModelDimensions(float left, float top, float right, float bottom);
                void ModelToScreen(float &x, float& y);

            public:

                GraphWindow(int width, int height, Graph* G, string Caption = "http://www.Open-Graphtheory.org", int gridsize = 1);

                void Display(Graph* G);
                void Update();

                void (*OnVertexMouseDown) (Graph::VertexIterator v, MouseButton Button,   unsigned short ButtonStates);
                void (*OnVertexMouseUp)   (Graph::VertexIterator v, MouseButton Button,   unsigned short ButtonStates);

        };


    }
}


