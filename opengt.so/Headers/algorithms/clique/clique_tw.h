
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_CLIQUE_CLIQUE_TW_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_CLIQUE_CLIQUE_TW_H

    #include "../algorithm.h"
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
                class AlgorithmCLIQUE_TW : public TreewidthBasedAlgorithm<std::set<Graph::VertexIterator> >
                {
                    protected:
                        static FactoryRegistrator<Algorithm> AlgorithmCliqueTWRegistrator;

                        std::set<Graph::VertexIterator>* HandleIntroduceNode(Graph& G, std::set<Graph::VertexIterator>* SubtreeResult, std::set<Graph::VertexIterator>& Bag, Graph::VertexIterator Introduced);
                        std::set<Graph::VertexIterator>* HandleForgetNode(Graph& G, std::set<Graph::VertexIterator>* SubtreeResult, std::set<Graph::VertexIterator>& Bag, Graph::VertexIterator Introduced);
                        std::set<Graph::VertexIterator>* HandleJoinNode(Graph& G, std::set<Graph::VertexIterator>* Subtree1Result, std::set<Graph::VertexIterator>* Subtree2Result, std::set<Graph::VertexIterator>& Bag);

                        void HandleRootNode(Graph& G, std::set<Graph::VertexIterator>* RootResult, std::vector<std::string> parameters);

                    public:
                        AlgorithmCLIQUE_TW();
                };
            }
        }
    }

#endif


