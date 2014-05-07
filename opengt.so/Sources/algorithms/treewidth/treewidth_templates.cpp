 #ifndef __OPENGRAPHTHEORY_ALGORITHMS_TREEWIDTH_TREEWIDTHTEMPLATES_CPP
    #define __OPENGRAPHTHEORY_ALGORITHMS_TREEWIDTH_TREEWIDTHTEMPLATES_CPP

    #include "../../../Headers/algorithms/treewidth/treewidth.h"

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            namespace Treewidth
            {

                template<class T> TreewidthBasedAlgorithm<T>::TreewidthBasedAlgorithm()
                {
                }

                template<class T> TreewidthBasedAlgorithm<T>::~TreewidthBasedAlgorithm()
                {
                }

                template<class T> T* TreewidthBasedAlgorithm<T>::HandleEmptyBag(Graph& G)
                {
                    return NULL;
                }

                template<class T> T* TreewidthBasedAlgorithm<T>::HandleLeafNode(Graph& G, Graph::VertexIterator Introduced)
                {
                    T* EmptyBagResult = HandleEmptyBag(G);
                    std::set<Graph::VertexIterator> Bag;
                    Bag.insert(Introduced);
                    return HandleIntroduceNode(G, EmptyBagResult, Bag, Introduced);
                }

                template<class T> std::pair<T*, std::set<Graph::VertexIterator> > TreewidthBasedAlgorithm<T>::Run(Graph& G, NiceTreeDecomposition* Decomposition)
                {
                    IntroduceNode* inode = dynamic_cast<IntroduceNode*>(Decomposition);
                    ForgetNode* fnode = dynamic_cast<ForgetNode*>(Decomposition);
                    JoinNode* jnode = dynamic_cast<JoinNode*>(Decomposition);

                    if(inode != NULL)
                    {
                        if(inode->Subtree == NULL)
                        {
                            std::set<Graph::VertexIterator> Bag;
                            Bag.insert(inode->IntroducedNode);
                            T* result = this->HandleLeafNode(G, inode->IntroducedNode);
                            return std::pair<T*, std::set<Graph::VertexIterator> >(result, Bag);
                        }
                        else
                        {
                            std::pair<T*, std::set<Graph::VertexIterator> > subtreeresult = Run(G, inode->Subtree);
                            subtreeresult.second.insert(inode->IntroducedNode);
                            T* result = this->HandleIntroduceNode(G, subtreeresult.first, subtreeresult.second, inode->IntroducedNode);
                            return std::pair<T*, std::set<Graph::VertexIterator> >(result, subtreeresult.second);
                        }
                    }

                    if(fnode != NULL)
                    {
                        std::pair<T*, std::set<Graph::VertexIterator> > subtreeresult = Run(G, fnode->Subtree);
                        subtreeresult.second.erase(fnode->ForgottenNode);
                        T* result = this->HandleForgetNode(G, subtreeresult.first, subtreeresult.second, fnode->ForgottenNode);
                        return std::pair<T*, std::set<Graph::VertexIterator> >(result, subtreeresult.second);
                    }

                    if(jnode != NULL)
                    {
                        std::pair<T*, std::set<Graph::VertexIterator> > subtree1result = Run(G, jnode->Subtree1);
                        std::pair<T*, std::set<Graph::VertexIterator> > subtree2result = Run(G, jnode->Subtree2);
                        T* result = this->HandleJoinNode(G, subtree1result.first, subtree2result.first, subtree1result.second);
                        return std::pair<T*, std::set<Graph::VertexIterator> >(result, subtree1result.second);
                    }

                    throw "Type mismatch: parameter \"Decomposition\" is neither of type IntroduceNode, ForgetNode nor JoinNode";
                }

                template<class T> void TreewidthBasedAlgorithm<T>::Run(Graph& G, std::vector<std::string> parameters)
                {
                    std::set<Graph::VertexIterator> VertexCover;
                    AlgorithmVERTEXCOVER_APX2::FindVertexCover(G, VertexCover);

                    NiceTreeDecomposition* treedecomp = NULL;
                    for(std::set<Graph::VertexIterator>::iterator i = VertexCover.begin(); i != VertexCover.end(); i++)
                        treedecomp = new IntroduceNode(treedecomp, *i);

                    for(Graph::VertexIterator i = G.BeginVertices(); i != G.EndVertices(); i++)
                        if(VertexCover.find(i) == VertexCover.end())
                            treedecomp = new ForgetNode(new IntroduceNode(treedecomp, i), i);

                    std::pair<T*, std::set<Graph::VertexIterator> > RootResult = Run(G, treedecomp);
                    this->HandleRootNode(G, RootResult.first, parameters);

                    delete treedecomp;
                }

            }
        }
    }

#endif
