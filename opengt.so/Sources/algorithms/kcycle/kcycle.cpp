
#include "../../../Headers/algorithms/kcycle/kcycle.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmKCYCLE::AlgorithmKCycleRegistrator(
            &Algorithm::AlgorithmFactory, "kcycle", new DefaultInstantiator<Algorithm, AlgorithmKCYCLE>(
            "kcycle", "Adds a k-cycle to the graph (if it exists)", ""));


        bool AlgorithmKCYCLE::FindKCycle(Graph &G, int k, EdgeSet& KCycle)
        {
            VertexSet Visited;
            return TestKPath(G, k, NULL, NULL, Visited, KCycle, true);
        }


        void AlgorithmKCYCLE::AddKCycle(Graph &G, int k, string KCycleName)
        {
            if(G.NumberOfVertices() <= 0 || G.NumberOfVertices() < k)
                return;

            EdgeSet KCycle;
            if(FindKCycle(G, k, KCycle))
                G.AddEdgeSet(KCycle, KCycleName);
        }


        void AlgorithmKCYCLE::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 1)
                throw "Hamiltonian-Path Algorithm needs 2 parameters (k and name of result)";

            int k;
            stringstream s;
            s << parameters[0];
            s >> k;
            string KCycleName =  parameters[1];
            AddKCycle(G, k, KCycleName);
        }

    }
}

