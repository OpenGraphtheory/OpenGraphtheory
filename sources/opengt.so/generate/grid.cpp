
#include <list>
#include "../opengt.h"
using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        Graph GenerateGrid(list<int> parameter)
        {
            Graph result;
            int w = parameter.front(); parameter.pop_front();
            int h = parameter.front();


            list<Graph::VertexIterator> CurrentColumn;
            list<Graph::VertexIterator> LastColumn;
            list<Graph::VertexIterator>::iterator LastColumnIterator;
            for(int x = 0; x < w; x++)
            {
                Graph::VertexIterator LastVertex;
                for(int y = 0; y < h; y++)
                {
                    Graph::VertexIterator CurrentVertex = result.AddVertex(x*25, y*25);
                    if(y > 0)
                        result.AddEdge(LastVertex, CurrentVertex);

                    if(x > 0)
                    {
                        if(y == 0)
                            LastColumnIterator = LastColumn.begin();
                        else
                            LastColumnIterator++;

                        result.AddEdge(*LastColumnIterator, CurrentVertex);
                    }

                    CurrentColumn.push_back(CurrentVertex);
                    LastVertex = CurrentVertex;
                }

                LastColumn = CurrentColumn;
                CurrentColumn.clear();
            }


            return result;
        }


    }
}

