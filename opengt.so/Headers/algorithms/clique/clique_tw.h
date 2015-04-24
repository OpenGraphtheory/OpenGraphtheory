
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_CLIQUE_CLIQUE_TW_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_CLIQUE_CLIQUE_TW_H

    #include "../algorithms.h"
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
                class AlgorithmCLIQUE_TW : public TreewidthBasedAlgorithm<VertexSet>
                {
                    protected:
                        static MultiFactoryRegistrator<Algorithm> AlgorithmCliqueTWRegistrator;

                        VertexSet* HandleLeafNode(Graph& G, VertexIterator Introduced);

                        VertexSet* HandleIntroduceNode(Graph& G, VertexSet* SubtreeResult, VertexSet& Bag, VertexIterator Introduced);
                        VertexSet* HandleForgetNode(Graph& G, VertexSet* SubtreeResult, VertexSet& Bag, VertexIterator Introduced);
                        VertexSet* HandleJoinNode(Graph& G, VertexSet* Subtree1Result, VertexSet* Subtree2Result, VertexSet& Bag);

                        void HandleRootNode(Graph& G, VertexSet* RootResult, std::vector<std::string> parameters);

                    public:
                        AlgorithmCLIQUE_TW();
                };
            }
        }
    }

#endif


