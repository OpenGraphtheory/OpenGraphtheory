
#include "../../../Headers/algorithms/hamilton/hamiltonianpath_tw.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Algorithms;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        namespace Treewidth
        {

            MultiFactoryRegistrator<Algorithm> AlgorithmHAMILTONIANPATH_TW::AlgorithmHamiltonianPathTWRegistrator(
                &Algorithm::AlgorithmFactory, "hamiltonianpath", new DefaultInstantiator<Algorithm, AlgorithmHAMILTONIANPATH_TW>(
                "hamiltonianpath", "Adds a hamiltonian path to the graph using the treewidth-based method", "https://en.wikipedia.org/wiki/Hamiltonian_path"));

            AlgorithmHAMILTONIANPATH_TW::AlgorithmHAMILTONIANPATH_TW()
                : TreewidthBasedAlgorithm()
            {
            }


            HamiltonianPathTWData* AlgorithmHAMILTONIANPATH_TW::HandleEmptyBag(Graph& G)
            {
                return NULL;
            }


            HamiltonianPathTWData* AlgorithmHAMILTONIANPATH_TW::HandleIntroduceNode(Graph& G, HamiltonianPathTWData* SubtreeResult, VertexSet& Bag, VertexIterator IntroducedNode)
            {
                return NULL;
            }


            HamiltonianPathTWData* AlgorithmHAMILTONIANPATH_TW::HandleForgetNode(Graph& G, HamiltonianPathTWData* SubtreeResult, VertexSet& Bag, VertexIterator ForgottenNode)
            {
                return NULL;
            }


            HamiltonianPathTWData* AlgorithmHAMILTONIANPATH_TW::HandleJoinNode(Graph& G, HamiltonianPathTWData* SubtreeResult1, HamiltonianPathTWData* SubtreeResult2, VertexSet& Bag)
            {
                return NULL;
            }


            void AlgorithmHAMILTONIANPATH_TW::HandleRootNode(Graph& G, HamiltonianPathTWData* RootResult, vector<string> parameters)
            {
                if(parameters.size() < 1)
                    throw "Hamiltonian-Path Algorithm needs 1 parameter (name of result)";

                EdgeSet result;
                G.AddEdgeSet(result, parameters[0]);
                delete RootResult;
            }


        }
    }
}

