
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
                VertexSet emptyset;
                DominatingSetTWData* result = new DominatingSetTWData;
                result->insert(DominatingSetTWData::value_type(emptyset, emptyset));
                return result;
            }


            DominatingSetTWData* AlgorithmDOMINATINGSET_TW::HandleIntroduceNode(Graph& G, DominatingSetTWData* SubtreeResult, VertexSet& Bag, VertexIterator IntroducedNode)
            {
                DominatingSetTWData* result = new DominatingSetTWData;
                VertexSet N = (*IntroducedNode)->CollectNeighbors(1,0,1,0,0,0,1,1,0);

                for(DominatingSetTWData::iterator i = SubtreeResult->begin(); i != SubtreeResult->end(); i++)
                    if(!N.intersection(i->first).empty())
                    {
                        // current set already dominates the introduced node
                        result->insert(DominatingSetTWData::value_type(i->first,i->second));
                    }
                    else
                    {
                        VertexSet D1 = i->first;
                        D1.insert(*IntroducedNode);
                        VertexSet U1 = i->second;
                        U1.insert(*IntroducedNode);
                        result->insert(DominatingSetTWData::value_type(D1,i->second));
                        result->insert(DominatingSetTWData::value_type(i->first,U1));
                    }

                delete SubtreeResult;
                return result;
            }


            DominatingSetTWData* AlgorithmDOMINATINGSET_TW::HandleForgetNode(Graph& G, DominatingSetTWData* SubtreeResult, VertexSet& Bag, VertexIterator ForgottenNode)
            {
                DominatingSetTWData* result = new DominatingSetTWData;
                for(DominatingSetTWData::iterator i = SubtreeResult->begin(); i != SubtreeResult->end(); i++)
                    if(!i->second.contains(*ForgottenNode)) // only populate vertex-sets that dominate the forgotten node
                                                            // (only verts in the current bag are allowed to be undominated)
                        result->insert(DominatingSetTWData::value_type(i->first,i->second));

                delete SubtreeResult;
                return result;
            }


            DominatingSetTWData* AlgorithmDOMINATINGSET_TW::HandleJoinNode(Graph& G, DominatingSetTWData* SubtreeResult1, DominatingSetTWData* SubtreeResult2, VertexSet& Bag)
            {
                DominatingSetTWData* result = new DominatingSetTWData;

                for(DominatingSetTWData::iterator d2 = SubtreeResult2->begin(); d2 != SubtreeResult2->end(); d2++)
                {
                    bool IsInIntersection = false;
                    VertexSet D;
                    for(DominatingSetTWData::iterator d1 = SubtreeResult1->begin(); d1 != SubtreeResult1->end(); d1++)
                    {
                        VertexSet D1 = d1->first;
                        VertexSet D2 = d2->first;
                        VertexSet U1 = d1->second;
                        VertexSet U2 = d2->second;
                        if((U1 == U2)
                        && (D1.intersection(Bag) == D2.intersection(Bag)))
                        {
                            IsInIntersection = true;
                            D = D1+D2;
                            break;
                        }
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
                VertexSet result;
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

