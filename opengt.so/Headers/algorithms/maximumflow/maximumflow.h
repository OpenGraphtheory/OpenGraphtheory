
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMFLOW_MAXIMUMFLOW_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMFLOW_MAXIMUMFLOW_H

    #include "../algorithm.h"
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
                    static void FindMaximumFlow(Graph& G, Graph::VertexIterator Source, Graph::VertexIterator Drain,
                                         std::map<Graph::EdgeIterator, float>& Capacities, std::map<Graph::EdgeIterator, float>& Flow);
                    static void AddMaximumFlow(Graph &G, Graph::VertexIterator Source, Graph::VertexIterator Drain,
                                         std::map<Graph::EdgeIterator, float>& Capacities, std::string FlowName);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmMaximumFlowRegistrator;
                    static bool FindAugmentingPath(Graph& G, Graph::VertexIterator Source, Graph::VertexIterator Drain,
                                         std::map<Graph::EdgeIterator, float>& Capacities, std::map<Graph::EdgeIterator, float>& Flow,
                                         std::vector<Graph::EdgeIterator>& AugmentingPath);
            };
        }
    }

#endif


