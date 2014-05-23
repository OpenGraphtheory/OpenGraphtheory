
#include "../../../Headers/algorithms/clique/clique_tw.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Algorithms;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        namespace Treewidth
        {

            MultiFactoryRegistrator<Algorithm> AlgorithmCLIQUE_TW::AlgorithmCliqueTWRegistrator(
                &Algorithm::AlgorithmFactory, "clique", new DefaultInstantiator<Algorithm, AlgorithmCLIQUE_TW>(
                "clique", "Adds a clique to the graph, using the treewidth-based algorithm", "http://en.wikipedia.org/wiki/Clique_(graph_theory)"));


            AlgorithmCLIQUE_TW::AlgorithmCLIQUE_TW()
                : TreewidthBasedAlgorithm<VertexSet>()
            {
            }

            bool BagClique(VertexSet& Candidates, VertexIterator candidate, VertexSet& temp, int k)
            {
                if(k<=0)
                    return true;

                for(; candidate != Candidates.end(); candidate++)
                {
                    bool legal = true;
                    for(VertexIterator i = temp.begin(); i != temp.end(); i++)
                        if(!(*candidate)->UnderlyingAdjacent(*i))
                        {
                            legal=false;
                            break;
                        }
                    if(!legal)
                        continue;

                    temp.insert(*candidate);
                    VertexIterator candidate1 = candidate;
                    candidate1++;

                    if(BagClique(Candidates, candidate1, temp, k-1))
                        return true;

                    temp.erase(*candidate);
                }
                return false;
            }

            VertexSet* AlgorithmCLIQUE_TW::HandleLeafNode(Graph& G, VertexIterator IntroducedNode)
            {
                VertexSet* result = new VertexSet;
                result->insert(*IntroducedNode);
                return result;
            }

            VertexSet* AlgorithmCLIQUE_TW::HandleIntroduceNode(Graph& G, VertexSet* SubtreeResult, VertexSet& Bag, VertexIterator IntroducedNode)
            {
                VertexSet Candidates;
                for(VertexIterator i = Bag.begin(); i != Bag.end(); i++)
                    if( (*i)->UnderlyingAdjacent(*IntroducedNode) )
                        Candidates.insert(*i);

                if(SubtreeResult->size() >= Candidates.size())
                    return SubtreeResult;


                VertexSet* BagResult = new VertexSet;
                VertexSet temp;
                temp.insert(*IntroducedNode);
                for(unsigned int k = SubtreeResult->size(); k < Candidates.size(); k++)
                    if(BagClique(Candidates, Candidates.begin(), temp, k))
                        (*BagResult) = temp;
                    else
                        break;


                VertexSet* result = SubtreeResult->size() > BagResult->size() ? SubtreeResult : BagResult;
                delete (result == SubtreeResult ? BagResult : SubtreeResult);
                return result;
            }

            VertexSet* AlgorithmCLIQUE_TW::HandleForgetNode(Graph& G, VertexSet* SubtreeResult, VertexSet& Bag, VertexIterator ForgottenNode)
            {
                return SubtreeResult;
            }

            VertexSet* AlgorithmCLIQUE_TW::HandleJoinNode(Graph& G, VertexSet* SubtreeResult1, VertexSet* SubtreeResult2, VertexSet& Bag)
            {
                VertexSet* result = SubtreeResult1->size() > SubtreeResult2->size() ? SubtreeResult1 : SubtreeResult2;
                delete (result == SubtreeResult1 ? SubtreeResult2 : SubtreeResult1);
                return result;
            }

            void AlgorithmCLIQUE_TW::HandleRootNode(Graph& G, VertexSet* RootResult, vector<string> parameters)
            {
                if(parameters.size() < 1)
                    throw "Clique Algorithm needs 1 parameter (name of result)";
                G.AddVertexSet(*RootResult, parameters[0]);
                delete RootResult;
            }

        }
    }
}

