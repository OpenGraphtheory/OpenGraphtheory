
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

            Graph::VertexIterator v1 = result.BeginVertices();
            Graph::VertexIterator v2 = v1;
            for(v2++; v2 != result.EndVertices(); v1++, v2++)
                result.AddEdge(v1, v2);

            return result;
        }

    }
}

