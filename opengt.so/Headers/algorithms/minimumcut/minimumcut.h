
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_MINIMUMCUT_MINIMUMCUT_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_MINIMUMCUT_MINIMUMCUT_H

    #include "../algorithm.h"
    #include "../maximumflow/maximumflow.h"
    #include<string>
    #include<set>
    #include<vector>
    #include<map>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmMINIMUMCUT : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    static void FindMinimumCut(Graph& G, Graph::VertexIterator Source, Graph::VertexIterator Drain,
                                               std::map<Graph::EdgeIterator, float>& Capacities, std::set<Graph::EdgeIterator>& MinimumCut);
                    static void FindMinimumCut(Graph& G, std::map<Graph::EdgeIterator, float>& Capacities, std::set<Graph::EdgeIterator>& MinimumCut);
                    static void FindMinimumCut(Graph& G, std::set<Graph::EdgeIterator>& MinimumCut);

                    static void AddMinimumCut(Graph &G, Graph::VertexIterator Source, Graph::VertexIterator Drain,
                                              std::map<Graph::EdgeIterator, float>& Capacities, std::string CutName);
                    static void AddMinimumCut(Graph &G, std::map<Graph::EdgeIterator, float>& Capacities, std::string CutName);
                    static void AddMinimumCut(Graph &G, std::string CutName);

                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmMinimumCutRegistrator;
            };
        }
    }

#endif


