
#include <list>
#include <cmath>
#include "../opengt.h"
using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        Graph GenerateCircle(list<int> parameter)
        {
            int n = parameter.front();
            Graph result;
            for(int i = 0; i < n; i++)
                result.AddVertex(-100 * cos(i * (2 * M_PI / n) + M_PI/2), 100 * sin(i * (2 * M_PI / n) + M_PI/2));

            Graph::VertexIterator v1 = result.BeginVertices();
            Graph::VertexIterator v2 = v1;
            for(v2++; v2 != result.EndVertices(); v1++, v2++)
                result.AddEdge(v1, v2);
            result.AddEdge(result.BeginVertices(), v1);

            return result;
        }

    }
}
