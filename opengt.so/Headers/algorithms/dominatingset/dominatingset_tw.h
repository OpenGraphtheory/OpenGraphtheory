
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_DOMINATINGSET_DOMINATINGSET_TW_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_DOMINATINGSET_DOMINATINGSET_TW_H

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

                typedef std::set<std::pair<std::set<Graph::VertexIterator>, std::set<Graph::VertexIterator> > > DominatingSetTWData;

                class AlgorithmDOMINATINGSET_TW : public TreewidthBasedAlgorithm<DominatingSetTWData>
                {
                    protected:
                        DominatingSetTWData* HandleEmptyBag(Graph& G);

                        DominatingSetTWData* HandleIntroduceNode(Graph& G, DominatingSetTWData* SubtreeResult, std::set<Graph::VertexIterator>& Bag, Graph::VertexIterator Introduced);
                        DominatingSetTWData* HandleForgetNode(Graph& G, DominatingSetTWData* SubtreeResult, std::set<Graph::VertexIterator>& Bag, Graph::VertexIterator Introduced);
                        DominatingSetTWData* HandleJoinNode(Graph& G, DominatingSetTWData* Subtree1Result, DominatingSetTWData* Subtree2Result, std::set<Graph::VertexIterator>& Bag);

                        void HandleRootNode(Graph& G, DominatingSetTWData* RootResult, std::vector<std::string> parameters);


                        static FactoryRegistrator<Algorithm> AlgorithmDominatingSetTWRegistrator;
                    public:
                        AlgorithmDOMINATINGSET_TW();

                };
            }
        }
    }

#endif


