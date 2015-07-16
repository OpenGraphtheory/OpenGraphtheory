
#include "../../../Headers/algorithms/hamilton/hamiltonianpath.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmHAMILTONIANPATH::AlgorithmHamiltonianPathRegistrator(
            &Algorithm::AlgorithmFactory, "hamiltonianpath", new DefaultInstantiator<Algorithm, AlgorithmHAMILTONIANPATH>(
            "hamiltonianpath", "Adds a hamiltonian path to the graph (if it exists)", "https://en.wikipedia.org/wiki/Hamiltonian_path"));


        bool AlgorithmHAMILTONIANPATH::FindHamiltonianPath(Graph &G, EdgeSet& HamiltonianPath)
        {
            return FindKPath(G,G.NumberOfVertices()-1, HamiltonianPath);
        }

        void AlgorithmHAMILTONIANPATH::AddHamiltonianPath(Graph &G, string HamiltonianPathName)
        {
            EdgeSet HamiltonianPath;

            if(G.NumberOfVertices() <= 0)
                return;

            if(FindHamiltonianPath(G, HamiltonianPath))
                G.AddEdgeSet(HamiltonianPath, HamiltonianPathName);
            // else ... nothing
        }

        void AlgorithmHAMILTONIANPATH::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                throw "Hamiltonian-Path Algorithm needs 1 parameter (name of result)";

            string HamiltonianPathName =  parameters[0];
            AddHamiltonianPath(G, HamiltonianPathName);
        }

    }
}

