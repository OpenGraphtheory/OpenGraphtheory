
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_DIRECTEDTREEWIDTH_DIRECTEDTREEWIDTH_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_DIRECTEDTREEWIDTH_DIRECTEDTREEWIDTH_H

    #include "../../opengt.h"
    #include "../algorithm.h"
    #include <set>
    #include "../../set_helper.h"

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


                        bool FindVertexFlowAugmentingPath(Graph &G, Vertex* Source, Vertex* Drain, std::set<Edge*> EFlow,
                                                          std::set<Vertex*> VFlow, std::vector<Edge*>& AugmentingPath);
                        bool FindMaximumVertexFlow(Graph &G, Vertex* Source, Vertex* Drain, std::set<Edge*> EFlow);
                        bool FindMinimumVertexCut(Graph &G, std::set<VertexIterator>& X, std::set<VertexIterator>& Y, int maxsize);
                    public:
                        bool FindWeaklyBalancedWSeparation(Graph &G, std::set<VertexIterator>& X, std::set<VertexIterator>& S, std::set<VertexIterator>& Y);

                        void Run(Graph& G, std::vector<std::string> parameters);
                };

            }
        }
    }

#endif

