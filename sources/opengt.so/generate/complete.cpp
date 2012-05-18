
#include <cmath>
#include <list>
#include "../opengt.h"
using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        Graph GenerateComplete(list<int> parameter)
        {
            Graph result;
            if(parameter.size() <= 1)
            {
                int n = parameter.front();
                for(int i = 0; i < n; i++)
                    result.AddVertex(-100 * cos(i * (2 * M_PI / n) + M_PI/2), 100 * sin(i * (2 * M_PI / n) + M_PI/2));

                for(Graph::VertexIterator v1 = result.BeginVertices(); v1 != result.EndVertices(); v1++)
                    for(Graph::VertexIterator v2 = v1 + 1; v2 != result.EndVertices(); v2++)
                        result.AddEdge(v1, v2);
            }
            else
            {
                Graph::VertexIterator LastPartitionLastAdded;
                int n = parameter.front();
                for(int i = 0; i < n; i++)
                    LastPartitionLastAdded = result.AddVertex();

                list<int>::iterator i = parameter.begin();
                for(i++; i != parameter.end(); i++)
                {
                    n = *i;
                    if(n > 0)
                    {
                        Graph::VertexIterator CurrentPartitionLastAdded;

                        for(int j = 0; j < n; j++)
                        {
                            CurrentPartitionLastAdded = result.AddVertex();
                            for(Graph::VertexIterator k = result.BeginVertices(); ; k++)
                            {
                                result.AddEdge(CurrentPartitionLastAdded, k);
                                if(k == LastPartitionLastAdded)
                                    break;
                            }
                        }

                        LastPartitionLastAdded = CurrentPartitionLastAdded;
                    }
                }

            }
            return result;
        }


    }
}

