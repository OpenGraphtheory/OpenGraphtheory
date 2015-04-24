
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_MINIMUMCUT_MINIMUMCUT_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_MINIMUMCUT_MINIMUMCUT_H

    #include "../algorithms.h"
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
                    static void FindMinimumCut(Graph& G, Vertex* Source, Vertex* Drain,
                                               EdgeWeighting& Capacities, EdgeSet& MinimumCut);
                    static void FindMinimumCut(Graph& G, EdgeWeighting& Capacities, EdgeSet& MinimumCut);
                    static void FindMinimumCut(Graph& G, EdgeSet& MinimumCut);

                    static void AddMinimumCut(Graph &G, Vertex* Source, Vertex* Drain,
                                              EdgeWeighting& Capacities, std::string CutName);
                    static void AddMinimumCut(Graph &G, EdgeWeighting& Capacities, std::string CutName);
                    static void AddMinimumCut(Graph &G, std::string CutName);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmMinimumCutRegistrator;
            };
        }
    }

#endif


