
#include "../../../Headers/algorithms/hamilton/hamiltoniancycle.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmHAMILTONIANCYCLE::AlgorithmHamiltonianCycleRegistrator(
            &Algorithm::AlgorithmFactory, "hamiltoniancycle", new DefaultInstantiator<Algorithm, AlgorithmHAMILTONIANCYCLE>(
            "hamiltoniancycle", "Adds a hamiltonian cycle to the graph (if it exists)", "https://en.wikipedia.org/wiki/Hamiltonian_path"));


        bool AlgorithmHAMILTONIANCYCLE::FindHamiltonianCycle(Graph &G, EdgeSet& HamiltonianCycle)
        {
            return FindKCycle(G,G.NumberOfVertices(), HamiltonianCycle);
        }

        void AlgorithmHAMILTONIANCYCLE::AddHamiltonianCycle(Graph &G, string HamiltonianCycleName)
        {
            EdgeSet HamiltonianCycle;

            if(G.NumberOfVertices() <= 0)
                return;

            if(FindHamiltonianCycle(G, HamiltonianCycle))
                G.AddEdgeSet(HamiltonianCycle, HamiltonianCycleName);
        }

        void AlgorithmHAMILTONIANCYCLE::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                throw "Hamiltonian-Cycle Algorithm needs 1 parameter (name of result)";

            string HamiltonianCycleName =  parameters[0];
            AddHamiltonianCycle(G, HamiltonianCycleName);
        }

    }
}

