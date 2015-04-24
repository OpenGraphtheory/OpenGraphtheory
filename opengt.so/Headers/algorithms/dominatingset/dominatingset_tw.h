
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_DOMINATINGSET_DOMINATINGSET_TW_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_DOMINATINGSET_DOMINATINGSET_TW_H

    #include "../algorithms.h"
    #include "../../set_helper.h"
    #include "../treewidth/treewidth.h"
    #include<string>
    #include<set>
    #include<vector>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            namespace Treewidth
            {

                typedef std::set< std::pair<VertexSet, VertexSet> > DominatingSetTWData;

                class AlgorithmDOMINATINGSET_TW : public TreewidthBasedAlgorithm<DominatingSetTWData>
                {
                    protected:
                        DominatingSetTWData* HandleEmptyBag(Graph& G);

                        DominatingSetTWData* HandleIntroduceNode(Graph& G, DominatingSetTWData* SubtreeResult, VertexSet& Bag, VertexIterator Introduced);
                        DominatingSetTWData* HandleForgetNode(Graph& G, DominatingSetTWData* SubtreeResult, VertexSet& Bag, VertexIterator Introduced);
                        DominatingSetTWData* HandleJoinNode(Graph& G, DominatingSetTWData* Subtree1Result, DominatingSetTWData* Subtree2Result, VertexSet& Bag);

                        void HandleRootNode(Graph& G, DominatingSetTWData* RootResult, std::vector<std::string> parameters);


                        static MultiFactoryRegistrator<Algorithm> AlgorithmDominatingSetTWRegistrator;
                    public:
                        AlgorithmDOMINATINGSET_TW();

                };
            }
        }
    }

#endif


