
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_TREEWIDTH_TREEWIDTH_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_TREEWIDTH_TREEWIDTH_H

    #include "../../opengt.h"
    #include "../algorithm.h"
    #include "../vertexcover/vertexcoverAPX2.h"
    #include <set>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            namespace Treewidth
            {

                class NiceTreeDecomposition
                {
                    public:
                        NiceTreeDecomposition();
                        virtual ~NiceTreeDecomposition();
                        virtual int Width() = 0;
                        virtual int BagSize() = 0;
                };

                class IntroduceNode : public NiceTreeDecomposition
                {
                    public:
                        NiceTreeDecomposition* Subtree;
                        Graph::VertexIterator IntroducedNode;

                        IntroduceNode(NiceTreeDecomposition* Subtree, Graph::VertexIterator IntroducedNode);
                        ~IntroduceNode();
                        int Width();
                        int BagSize();
                };

                class ForgetNode : public NiceTreeDecomposition
                {
                    public:
                        NiceTreeDecomposition* Subtree;
                        Graph::VertexIterator ForgottenNode;

                        ForgetNode(NiceTreeDecomposition* Subtree, Graph::VertexIterator ForgottenNode);
                        ~ForgetNode();
                        int Width();
                        int BagSize();
                };

                class JoinNode : public NiceTreeDecomposition
                {
                    public:
                        NiceTreeDecomposition* Subtree1;
                        NiceTreeDecomposition* Subtree2;

                        JoinNode(NiceTreeDecomposition* Subtree1, NiceTreeDecomposition* Subtree2);
                        ~JoinNode();
                        int Width();
                        int BagSize();
                };

                class NiceTreeDecompositionAlgorithm
                {
                    public:
                        virtual NiceTreeDecomposition* FindNiceTreeDecomposition(Graph& G);
                };



                template<class T> class TreewidthBasedAlgorithm : public Algorithm
                {
                    protected:

                        virtual T* HandleEmptyBag(Graph& G);
                        virtual T* HandleLeafNode(Graph& G, Graph::VertexIterator Introduced);

                        virtual T* HandleIntroduceNode(Graph& G, T* SubtreeResult, std::set<Graph::VertexIterator>& Bag, Graph::VertexIterator Introduced) = 0;
                        virtual T* HandleForgetNode(Graph& G, T* SubtreeResult, std::set<Graph::VertexIterator>& Bag, Graph::VertexIterator Introduced) = 0;
                        virtual T* HandleJoinNode(Graph& G, T* Subtree1Result, T* Subtree2Result, std::set<Graph::VertexIterator>& Bag) = 0;

                        virtual void HandleRootNode(Graph& G, T* RootResult, std::vector<std::string> parameters) = 0;
                        std::pair<T*,std::set<Graph::VertexIterator> > Run(Graph& G, NiceTreeDecomposition* Decomposition);

                    public:
                        TreewidthBasedAlgorithm();
                        virtual ~TreewidthBasedAlgorithm();

                        void Run(Graph& G, std::vector<std::string> parameters);
                };

            }
        }
    }

    #include "../../../Sources/algorithms/treewidth/treewidth_templates.cpp"

#endif

