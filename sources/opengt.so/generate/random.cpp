
#include "../opengt.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <set>
using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        Graph GenerateRandom(list<int> parameters)
        {

            int n = parameters.front();
            parameters.pop_front();
            int m = parameters.front();

            if(m > n*(n-1)/2)
                throw "cannot have more than n*(n-1)/2 edges";

            Graph G(n);

            // initialize vector "allindices"
            vector<int> allindices(n*(n-1)/2);
            for(int i = (n*(n-1)/2)-1; i >= 0; i--)
                allindices[i] = i;

            // randomize vector "allindices"
            srand(time(NULL));
            for(int i = (n*(n-1)/2)-1; i > 0; i--)
            {
                int rand_idx = rand() % (i+1);
                int temp = allindices[i];
                allindices[i] = allindices[rand_idx];
                allindices[rand_idx] = temp;
            }

            // select first m indices from randomized "allindices"
            set<int> selected_indices;
            for(int i = m-1; i >= 0; i--)
                selected_indices.insert(allindices[i]);

            // insert the edges with the selected indices into the graph
            int i = 0;
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                Graph::VertexIterator v1 = v;
                for(v1++; v1 != G.EndVertices(); v1++)
                {
                    if(selected_indices.find(i) != selected_indices.end())
                        G.AddEdge(v,v1);
                    i++;
                }
            }

            return G;
        }

    }
}
