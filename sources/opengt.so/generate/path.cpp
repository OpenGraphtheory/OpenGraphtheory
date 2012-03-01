
#include <list>
#include "../opengt.h"
using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        Graph GeneratePath(list<int> parameter)
        {
            int n = parameter.front();
            Graph result;
            for(int i = 0; i < n; i++)
                result.AddVertex(0, 50 * i);

            for(Graph::VertexIterator v = result.BeginVertices(); v + 1 != result.EndVertices(); v++)
                result.AddEdge(v, v+1);

            return result;
        }

    }
}

