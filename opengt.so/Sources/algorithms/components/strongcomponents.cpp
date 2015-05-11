
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
        

        void AlgorithmSTRONGCOMPONENTS::FindStrongComponents(Graph &G, map<Vertex*, int>& ComponentOfVertex, vector<vector<Vertex*> >& VerticesInComponent,
                                                             VertexFilter *vertexfilter, EdgeFilter *edgefilter)
        {
            FindComponents(G,ComponentOfVertex,VerticesInComponent,vertexfilter,edgefilter,true);
        }
        
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
