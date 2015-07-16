
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_HAMILTON_HAMILTONIANPATH_TW_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_HAMILTON_HAMILTONIANPATH_TW_H

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

                typedef std::set< std::pair<VertexSet, std::set<std::pair<Vertex*, Vertex*> > > > HamiltonianPathTWData;

                class AlgorithmHAMILTONIANPATH_TW : public TreewidthBasedAlgorithm<HamiltonianPathTWData>
                {
                    protected:
                        HamiltonianPathTWData* HandleEmptyBag(Graph& G);

                        HamiltonianPathTWData* HandleIntroduceNode(Graph& G, HamiltonianPathTWData* SubtreeResult, VertexSet& Bag, VertexIterator Introduced);
                        HamiltonianPathTWData* HandleForgetNode(Graph& G, HamiltonianPathTWData* SubtreeResult, VertexSet& Bag, VertexIterator Introduced);
                        HamiltonianPathTWData* HandleJoinNode(Graph& G, HamiltonianPathTWData* Subtree1Result, HamiltonianPathTWData* Subtree2Result, VertexSet& Bag);

                        void HandleRootNode(Graph& G, HamiltonianPathTWData* RootResult, std::vector<std::string> parameters);


                        static MultiFactoryRegistrator<Algorithm> AlgorithmHamiltonianPathTWRegistrator;
                    public:
                        AlgorithmHAMILTONIANPATH_TW();

                };
            }
        }
    }

#endif


