
#include "../../../Headers/algorithms/directedtreewidth/directedtreewidth.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Algorithms;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        namespace DirectedTreewidth
        {
            MultiFactoryRegistrator<Algorithm> AlgorithmDIRECTEDTREEDECOMPOSITION::AlgorithmDirectedTreeDecompositionRegistrator(
                &Algorithm::AlgorithmFactory, "directedtreedecomposition", new DefaultInstantiator<Algorithm, AlgorithmDIRECTEDTREEDECOMPOSITION>(
                "directedtreedecomposition", "returns a directed tree-decomposition of the graph", "http://dx.doi.org/10.1006/jctb.2000.2031"));

/*


            bool AlgorithmDIRECTEDTREEDECOMPOSITION::FindWeaklyBalancedWSeparation(Graph& G, VertexSet& W, VertexIterator WIt, VertexSet& X, VertexSet& S, VertexSet& Y)
            {
                if(WIt == W.end())
                {
                    AlgorithmVERTEXSEPARATOR separatoralgorithm;
                    VertexSet Sep = separatoralgorithm.FindMinimumVertexSeparator(G, X, Y, S);
                    return S.size() + Sep.size() <= k;
                }
                else
                {
                    VertexIterator WItNext = WIt;
                    WItNext++;

                    X.insert(WIt);                                             // Put WIt into X
                    if(FindWeaklyBalancedWSeparation(G,W,WItNext, X,S,Y))
                        return true;
                    X.erase(WIt);

                    Y.insert(WIt);                                             // Put WIt into Y
                    if(FindWeaklyBalancedWSeparation(G,W,WItNext, X,S,Y))
                        return true;
                    Y.erase(WIt);

                    S.insert(WIt);                                             // Put WIt into S
                    if(FindWeaklyBalancedWSeparation(G,W,WItNext, X,S,Y))
                        return true;
                    S.erase(WIt);

                    return false;
                }
            }



            bool AlgorithmDIRECTEDTREEDECOMPOSITION::FindDirectedTreeDecomposition(Graph& G, int k)
            {
                VertexSet W;
                VertexIterator v = G.BeginVertices();
                for(int i = 0; i < 2*k+2 && v != G.EndVertices(); i++)
                {
                    W.insert(*v);
                    v++;
                }

                VertexSet X, S, Y;
                if(!FindWeaklyBalancedWSeparation(G, W, W.begin(), X,S,Y))
                    return false;


                AlgorithmSTRONGCOMPONENTS strongcomponentalgorithm;
                VertexPartitioning components = strongcomponentalgorithm.FindStronglyConnectedComponents(G, ___S, )

                // Sort the Components

                // Distribute Components in a balanced way

                // Recurse on Components
            }



            bool AlgorithmDIRECTEDTREEDECOMPOSITION::FindDirectedTreeDecomposition(Graph& G)
            {
                for(int k = 1; ; k++)
                    if(FindDirectedTreeDecomposition(G, k))
                        return true;
                // return false;
            }
            */



            void AlgorithmDIRECTEDTREEDECOMPOSITION::Run(Graph& G, std::vector<std::string> parameters)
            {
                if(parameters.size() < 1)
                    throw "directed treewidth algorithm needs 1 parameter (result name)";
                string ResultName = parameters[0];

                //FindDirectedTreeDecomposition(G);
            }

        }
    }
}
