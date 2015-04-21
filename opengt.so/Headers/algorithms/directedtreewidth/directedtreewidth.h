
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_DIRECTEDTREEWIDTH_DIRECTEDTREEWIDTH_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_DIRECTEDTREEWIDTH_DIRECTEDTREEWIDTH_H

    #include "../../opengt.h"
    #include "../algorithm.h"
    #include <set>
    #include "../../set_helper.h"
    #include "../vertexseparator/vertexseparator.h"

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            namespace DirectedTreewidth
            {

                class AlgorithmDIRECTEDTREEDECOMPOSITION : public Algorithm
                {
                    protected:
                        static MultiFactoryRegistrator<Algorithm> AlgorithmDirectedTreeDecompositionRegistrator;

                    public:
                        bool FindWeaklyBalancedWSeparation(VertexSet& X, VertexSet& S, VertexSet& Y);

                        void Run(Graph& G, std::vector<std::string> parameters);
                };

            }
        }
    }

#endif

