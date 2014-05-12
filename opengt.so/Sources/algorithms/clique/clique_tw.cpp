
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
                : TreewidthBasedAlgorithm<set<Graph::VertexIterator> >()
            {
            }

            bool BagClique(set<Graph::VertexIterator>& Candidates, set<Graph::VertexIterator>::iterator candidate, set<Graph::VertexIterator>& temp, int k)
            {
                if(k<=0)
                    return true;

                for(; candidate != Candidates.end(); candidate++)
                {
                    bool legal = true;
                    for(set<Graph::VertexIterator>::iterator i = temp.begin(); i != temp.end(); i++)
                        if(!candidate->UnderlyingAdjacent(*i))
                        {
                            legal=false;
                            break;
                        }
                    if(!legal)
                        continue;

                    temp.insert(*candidate);
                    set<Graph::VertexIterator>::iterator candidate1 = candidate;
                    candidate1++;

                    if(BagClique(Candidates, candidate1, temp, k-1))
                        return true;

                    temp.erase(*candidate);
                }
                return false;
            }

            set<Graph::VertexIterator>* AlgorithmCLIQUE_TW::HandleLeafNode(Graph& G, Graph::VertexIterator IntroducedNode)
            {
                set<Graph::VertexIterator>* result = new set<Graph::VertexIterator>;
                result->insert(IntroducedNode);
                return result;
            }

            set<Graph::VertexIterator>* AlgorithmCLIQUE_TW::HandleIntroduceNode(Graph& G, set<Graph::VertexIterator>* SubtreeResult, set<Graph::VertexIterator>& Bag, Graph::VertexIterator IntroducedNode)
            {
                set<Graph::VertexIterator> Candidates;
                for(set<Graph::VertexIterator>::iterator i = Bag.begin(); i != Bag.end(); i++)
                    if( i->UnderlyingAdjacent(IntroducedNode) )
                        Candidates.insert(*i);

                if(SubtreeResult->size() >= Candidates.size())
                    return SubtreeResult;


                set<Graph::VertexIterator>* BagResult = new set<Graph::VertexIterator>;
                set<Graph::VertexIterator> temp;
                temp.insert(IntroducedNode);
                for(unsigned int k = SubtreeResult->size(); k < Candidates.size(); k++)
                    if(BagClique(Candidates, Candidates.begin(), temp, k))
                        (*BagResult) = temp;
                    else
                        break;


                set<Graph::VertexIterator>* result = SubtreeResult->size() > BagResult->size() ? SubtreeResult : BagResult;
                delete (result == SubtreeResult ? BagResult : SubtreeResult);
                return result;
            }

            set<Graph::VertexIterator>* AlgorithmCLIQUE_TW::HandleForgetNode(Graph& G, set<Graph::VertexIterator>* SubtreeResult, set<Graph::VertexIterator>& Bag, Graph::VertexIterator ForgottenNode)
            {
                return SubtreeResult;
            }

            set<Graph::VertexIterator>* AlgorithmCLIQUE_TW::HandleJoinNode(Graph& G, set<Graph::VertexIterator>* SubtreeResult1, set<Graph::VertexIterator>* SubtreeResult2, set<Graph::VertexIterator>& Bag)
            {
                set<Graph::VertexIterator>* result = SubtreeResult1->size() > SubtreeResult2->size() ? SubtreeResult1 : SubtreeResult2;
                delete (result == SubtreeResult1 ? SubtreeResult2 : SubtreeResult1);
                return result;
            }

            void AlgorithmCLIQUE_TW::HandleRootNode(Graph& G, set<Graph::VertexIterator>* RootResult, vector<string> parameters)
            {
                if(parameters.size() < 1)
                    throw "Clique Algorithm needs 1 parameter (name of result)";
                G.AddVertexSet(*RootResult, parameters[0]);
                delete RootResult;
            }

        }
    }
}

