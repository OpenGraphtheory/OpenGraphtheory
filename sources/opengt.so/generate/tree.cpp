
#include <list>
#include <cmath>
#include "../opengt.h"
using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        void GenerateTree(Graph* G, list<int>* parameter, list<int>::iterator* it, Graph::VertexIterator v)
        {
            if(*it == parameter->end())
                return;

            for(int i = **it; i > 0; --i)
            {
                Graph::VertexIterator u = G->AddVertex();
                G->AddEdge(v,u);

                if(*it != parameter->end())
                    (*it)++;
                GenerateTree(G, parameter, it, u);
            }
        }

        Graph GenerateTree(list<int> parameter)
        {
            Graph result;
            Graph::VertexIterator root = result.AddVertex();
            list<int>::iterator i = parameter.begin();
            GenerateTree(&result, &parameter, &i, root);
            return result;
        }

    }
}

