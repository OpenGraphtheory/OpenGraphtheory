
#include "../../../Headers/algorithms/components/weakcomponents.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmWEAKCOMPONENTS::AlgorithmWeakComponentsRegistrator(
            &Algorithm::AlgorithmFactory, "weakcomponents", new DefaultInstantiator<Algorithm, AlgorithmWEAKCOMPONENTS>(
            "weakcomponents", "determines the weakly connected components of the graph",
            "http://mathworld.wolfram.com/WeaklyConnectedComponent.html"));

        
        void AlgorithmWEAKCOMPONENTS::FindWeakComponents(Graph &G, map<Vertex*, int>& ComponentOfVertex, vector<vector<Vertex*> >& VerticesInComponent,
                                                         VertexFilter *vertexfilter, EdgeFilter *edgefilter)
        {
            FindComponents(G,ComponentOfVertex,VerticesInComponent,vertexfilter,edgefilter,false);
        }
        
        void AlgorithmWEAKCOMPONENTS::AddWeakComponents(Graph &G, std::string ComponentsName)
        {
            map<Vertex*, int> ComponentOfVertex;
            vector<vector<Vertex*> > VerticesInComponent;
            FindWeakComponents(G, ComponentOfVertex, VerticesInComponent, NULL, NULL);
            G.AddVertexPartitioning(ComponentOfVertex, ComponentsName);
        }
        
        void AlgorithmWEAKCOMPONENTS::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() < 1)
                return;

            string ComponentsName = parameters[0];
            AddWeakComponents(G, ComponentsName);
        }

    }
}
