
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMFLOW_MAXIMUMFLOW_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMFLOW_MAXIMUMFLOW_H

    #include "../algorithms.h"
    #include<string>
    #include<set>
    #include<vector>
    #include<map>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmMAXIMUMFLOW : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    static void FindMaximumFlow(Graph& G, Vertex* Source, Vertex* Drain,
                                         EdgeWeighting& Capacities, EdgeWeighting& Flow);
                    static void AddMaximumFlow(Graph &G, Vertex* Source, Vertex* Drain,
                                         EdgeWeighting& Capacities, std::string FlowName);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmMaximumFlowRegistrator;
                    static bool FindAugmentingPath(Graph &G, Vertex* Source, Vertex* Drain, EdgeWeighting& Capacities,
                                                   EdgeWeighting& Flow, std::vector<Edge*>& AugmentingPath);
            };
        }
    }

#endif


