
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_DIRECTEDTREEWIDTH_DIRECTEDTREEWIDTH_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_DIRECTEDTREEWIDTH_DIRECTEDTREEWIDTH_H

    #include "../../opengt.h"
    #include "../algorithms.h"
    #include <set>
    #include "../../set_helper.h"
    #include "../vertexseparator/vertexseparator.h"
    #include "../components/strongcomponents.h"

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            namespace DirectedTreewidth
            {

                class DirectedTreeDecomposition
                {
                    public:
                        DirectedTreeDecomposition();
                        ~DirectedTreeDecomposition();

                        VertexSet Bag;
                        std::vector<DirectedTreeDecomposition*> Children;
                };

                class AlgorithmDIRECTEDTREEDECOMPOSITION : public Algorithm
                {
                    protected:
                        static MultiFactoryRegistrator<Algorithm> AlgorithmDirectedTreeDecompositionRegistrator;

                    public:
                        bool FindWeaklyBalancedWSeparation(Graph& G, VertexSet& V, VertexSet& W, VertexIterator WIt,
                                                           VertexSet& X, VertexSet& S, VertexSet& Y, size_t k);
                        DirectedTreeDecomposition* FindDirectedTreeDecomposition(Graph& G, VertexSet& V, VertexSet& W, size_t k);
                        DirectedTreeDecomposition* FindDirectedTreeDecomposition(Graph& G, size_t k);
                        DirectedTreeDecomposition* FindDirectedTreeDecomposition(Graph& G);

                        void PrintDirectedTreeDecomposition(DirectedTreeDecomposition* d, size_t indent);

                        void Run(Graph& G, std::vector<std::string> parameters);
                };

            }
        }
    }

#endif

