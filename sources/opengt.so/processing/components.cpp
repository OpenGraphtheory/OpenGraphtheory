
#include <set>
#include <queue>
#include "../opengt.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Processing
    {

        set<set<Graph::VertexIterator>*> Components(Graph G)
        {
            if(!G.IsUndirected())
                throw "Components are only defined for undirected graphs. For mixed- and directed graphs, use StrongComponents!";

            map<Graph::VertexIterator, int> Component;
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                Component[v] = -1;

            set<set<Graph::Graph::VertexIterator>*> result;// = new set<set<Graph::Graph::VertexIterator>*>();
            int currentcomponent = 0;
            queue<Graph::VertexIterator> que;

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                if(Component[v] != -1)
                    continue;

                set<Graph::VertexIterator> *CurrentComponent = new set<Graph::VertexIterator>();

                Component[v] = currentcomponent;
                CurrentComponent->insert(v);
                que.push(v);

                while(!que.empty())
                {
                    Graph::VertexIterator u = que.front();

                    /*
                    Graph::VertexSet Neighbors = Neighborhood(G, u);
                    for(Graph::VertexIterator w = Neighbors.Begin(); w != Neighbors.End(); w++)
                    {
                        if(Component[w] != -1)
                            continue;

                        Component[w] = currentcomponent;
                        CurrentComponent->insert(w);
                        que.push(w);
                    }
                    */
                    que.pop();
                }

                result.insert(CurrentComponent);
                currentcomponent++;
            }


            return result;
        }

    }
}
