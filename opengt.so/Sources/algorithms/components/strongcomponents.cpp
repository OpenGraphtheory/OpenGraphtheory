
#include "../../../Headers/algorithms/components/strongcomponents.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmSTRONGCOMPONENTS::AlgorithmStrongComponentsRegistrator(
            &Algorithm::AlgorithmFactory, "strongcomponents", new DefaultInstantiator<Algorithm, AlgorithmSTRONGCOMPONENTS>(
            "strongcomponents", "determines the strongly connected components of the graph",
            "http://en.wikipedia.org/wiki/Strongly_connected_component"));



        // Tarjans Algorithm
        // https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
        void AlgorithmSTRONGCOMPONENTS::FindStrongComponents(Graph &G, map<Vertex*, int>& ComponentOfVertex,
                                                             vector<vector<Vertex*> >& VerticesInComponent,
                                                             VertexFilter *vertexfilter, EdgeFilter *edgefilter)
        {

            int numScc = 0;
            int index = 0;
            stack<Vertex*> tarStack;
            VertexSet onStack;
            map<Vertex*, int> dfs;
            map<Vertex*, int> lowlink;
            map<Vertex*, Vertex*> caller;
            map<Vertex*, VertexSet> nodeVector;
            map<Vertex*, VertexIterator> vindex;

            for (VertexIterator i = G.BeginVertices(); i != G.EndVertices(); i++)
            {
                Vertex* u = *i;
                if(vertexfilter != NULL && !vertexfilter->VertexAllowed(u))
                    continue;
                if (dfs.find(u) != dfs.end())
                    continue;

                dfs[u] = index;
                lowlink[u] = index;
                index++;
                nodeVector[u] = u->CollectNeighbors(1,1,0,1,1,0,0,0,0,vertexfilter,edgefilter);
                vindex[u] = nodeVector[u].begin();
                tarStack.push(u);
                caller[u] = NULL;
                onStack.insert(u);
                Vertex *last = u;

                while(last != NULL)
                {
                    if(vindex[last] != nodeVector[last].end())
                    {
                        Vertex *w = *(vindex[last]);
                        vindex[last]++;
                        if(dfs.find(w) == dfs.end())
                        {
                            caller[w] = last;
                            nodeVector[w] = w->CollectNeighbors(1,1,0,1,1,0,0,0,0,vertexfilter,edgefilter);
                            vindex[w] = nodeVector[w].begin();
                            dfs[w] = index;
                            lowlink[w] = index;
                            index++;
                            tarStack.push(w);
                            onStack.insert(w);
                            last = w;
                        }
                        else if(onStack.find(w) != onStack.end())
                        {
                            lowlink[last] = min(lowlink[last], dfs[w]);
                        }
                    }
                    else
                    {
                        if(lowlink[last] == dfs[last])
                        {
                            Vertex *top = NULL;
                            vector<Vertex*> scc;
                            while(top != last)
                            {
                                top = tarStack.top();
                                tarStack.pop();
                                onStack.erase(top);
                                scc.push_back(top);
                                ComponentOfVertex[top] = numScc;
                            }
                            VerticesInComponent.push_back(scc);
                            numScc++;
                        }

                        Vertex *newLast = caller[last]; //Go up one recursive call
                        if(newLast != NULL)
                            lowlink[newLast] = min(lowlink[newLast], lowlink[last]);
                        last = newLast;
                    }
                } // while(last != NULL)
            } // for (VertexIterator i = G.BeginVertices(); i != G.EndVertices(); i++)
        }


        void AlgorithmSTRONGCOMPONENTS::FindComponents(Graph &G, map<Vertex*, int>& ComponentOfVertex, vector<vector<Vertex*> >& VerticesInComponent,
                                                       VertexFilter *vertexfilter, EdgeFilter *edgefilter, bool StrongComponents)
        {
            int MaxComponent = 0;
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                if(vertexfilter != NULL && !vertexfilter->VertexAllowed(*v))
                    continue;

                if(ComponentOfVertex.find(*v) != ComponentOfVertex.end()) // component already assigned
                    continue;
cerr << "."; cerr.flush();


                // v is in a new component
                ComponentOfVertex[*v] = MaxComponent;
                VerticesInComponent.push_back(vector<Vertex*>(1,*v));

                // find rest of the new component
                AlgorithmPATH pathalgo;
                VertexSet ReachPlus;
                VertexSet ReachMinus;
                if(StrongComponents) // Strong Component of v = ReachPlus intersection ReachMinus
                {
                    ReachPlus = pathalgo.ForwardReachableVertices(G, *v, vertexfilter, edgefilter);
                    ReachMinus = pathalgo.BackwardReachableVertices(G, *v, vertexfilter, edgefilter);
                }
                else // Weak Component of v
                {
                    ReachPlus = pathalgo.WeaklyReachableVertices(G, *v, vertexfilter, edgefilter);
                    ReachMinus = ReachPlus;
                }

                for(VertexIterator w = ReachPlus.begin(); w != ReachPlus.end(); w++)
                {
                    if(ReachMinus.find(*w) != ReachMinus.end())
                    {
                        ComponentOfVertex[*w] = MaxComponent;
                        VerticesInComponent[MaxComponent].push_back(*w);
                    }
                }

                MaxComponent++;
            }
        }

/*
        void AlgorithmSTRONGCOMPONENTS::FindStrongComponents(Graph &G, map<Vertex*, int>& ComponentOfVertex, vector<vector<Vertex*> >& VerticesInComponent,
                                                             VertexFilter *vertexfilter, EdgeFilter *edgefilter)
        {
            FindComponents(G,ComponentOfVertex,VerticesInComponent,vertexfilter,edgefilter,true);
        }
*/
        void AlgorithmSTRONGCOMPONENTS::AddStrongComponents(Graph &G, std::string ComponentsName)
        {
            map<Vertex*, int> ComponentOfVertex;
            vector<vector<Vertex*> > VerticesInComponent;
            FindStrongComponents(G, ComponentOfVertex, VerticesInComponent, NULL, NULL);
            G.AddVertexPartitioning(ComponentOfVertex, ComponentsName);
        }

        void AlgorithmSTRONGCOMPONENTS::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() < 1)
                return;

            string ComponentsName = parameters[0];
            AddStrongComponents(G, ComponentsName);
        }

    }
}
