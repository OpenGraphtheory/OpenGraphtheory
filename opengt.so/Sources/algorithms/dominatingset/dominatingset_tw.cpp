
#include "../../../Headers/algorithms/dominatingset/dominatingset_tw.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Algorithms;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        namespace Treewidth
        {

            MultiFactoryRegistrator<Algorithm> AlgorithmDOMINATINGSET_TW::AlgorithmDominatingSetTWRegistrator(
                &Algorithm::AlgorithmFactory, "dominatingset", new DefaultInstantiator<Algorithm, AlgorithmDOMINATINGSET_TW>(
                "dominatingset", "Adds a dominating set to the graph using the treewidth-based method", "http://en.wikipedia.org/wiki/Dominating_set"));

            AlgorithmDOMINATINGSET_TW::AlgorithmDOMINATINGSET_TW()
                : TreewidthBasedAlgorithm()
            {
            }


            DominatingSetTWData* AlgorithmDOMINATINGSET_TW::HandleEmptyBag(Graph& G)
            {
                set<Graph::VertexIterator> emptyset;
                DominatingSetTWData* result = new DominatingSetTWData;
                result->insert(DominatingSetTWData::value_type(emptyset, emptyset));
                return result;
            }


            DominatingSetTWData* AlgorithmDOMINATINGSET_TW::HandleIntroduceNode(Graph& G, DominatingSetTWData* SubtreeResult, set<Graph::VertexIterator>& Bag, Graph::VertexIterator IntroducedNode)
            {
                DominatingSetTWData* result = new DominatingSetTWData;
                set<Graph::VertexIterator> N = IntroducedNode.CollectNeighbors(1,0,1,0,0,0,1,1,0);

                for(DominatingSetTWData::iterator i = SubtreeResult->begin(); i != SubtreeResult->end(); i++)
                    if(!SetHelper::IntersectionEmpty(N, i->first))
                    {
                        result->insert(DominatingSetTWData::value_type(i->first,i->second));
                    }
                    else
                    {
                        set<Graph::VertexIterator> D1 = i->first;
                        D1.insert(IntroducedNode);
                        set<Graph::VertexIterator> U1 = i->second;
                        U1.insert(IntroducedNode);
                        result->insert(DominatingSetTWData::value_type(D1,i->second));
                        result->insert(DominatingSetTWData::value_type(i->first,U1));
                    }

                delete SubtreeResult;
                return result;
            }


            DominatingSetTWData* AlgorithmDOMINATINGSET_TW::HandleForgetNode(Graph& G, DominatingSetTWData* SubtreeResult, set<Graph::VertexIterator>& Bag, Graph::VertexIterator ForgottenNode)
            {
                DominatingSetTWData* result = new DominatingSetTWData;
                for(DominatingSetTWData::iterator i = SubtreeResult->begin(); i != SubtreeResult->end(); i++)
                    if(i->second.find(ForgottenNode) == i->second.end()) // only populate vertex-sets that dominate the forgotten node
                                                                         // (only verts in the current bag are allowed to be undominated)
                        result->insert(DominatingSetTWData::value_type(i->first,i->second));

                delete SubtreeResult;
                return result;
            }


            DominatingSetTWData* AlgorithmDOMINATINGSET_TW::HandleJoinNode(Graph& G, DominatingSetTWData* SubtreeResult1, DominatingSetTWData* SubtreeResult2, set<Graph::VertexIterator>& Bag)
            {
                DominatingSetTWData* result = new DominatingSetTWData;

                for(DominatingSetTWData::iterator d2 = SubtreeResult2->begin(); d2 != SubtreeResult2->end(); d2++)
                {
                    bool IsInIntersection = false;
                    set<Graph::VertexIterator> D;
                    for(DominatingSetTWData::iterator d1 = SubtreeResult1->begin(); d1 != SubtreeResult1->end(); d1++)
                        if(SetHelper::SetsEqual(d1->second, d2->second)
                        && SetHelper::SetsEqual(SetHelper::Intersection(d1->first, Bag), SetHelper::Intersection(d2->first, Bag)))
                        {
                            IsInIntersection = true;
                            D = SetHelper::Union(d2->first, d1->first);
                            break;
                        }
                    if(IsInIntersection)
                        result->insert(DominatingSetTWData::value_type(D,d2->second));
                }

                delete SubtreeResult1;
                delete SubtreeResult2;
                return result;
            }


            void AlgorithmDOMINATINGSET_TW::HandleRootNode(Graph& G, DominatingSetTWData* RootResult, vector<string> parameters)
            {
                if(parameters.size() < 1)
                    throw "Dominating-Set Algorithm needs 1 parameter (name of result)";

                unsigned int minSize = G.NumberOfVertices()+1;
                set<Graph::VertexIterator> result;
                // pick a pair (D,U) from RootResult with minimal |D|, such that U = empty
                // (U is a set of vertices which are not dominated by D, so if U =/= empty,
                // then D isn't a dominating set)
                for(DominatingSetTWData::iterator i = RootResult->begin(); i != RootResult->end(); i++)
                    if(i->second.empty() && i->first.size() < minSize)
                    {
                        minSize = i->first.size();
                        result = i->first;
                    }

                G.AddVertexSet(result, parameters[0]);
                delete RootResult;
            }


        }
    }
}

