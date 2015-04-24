
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_COLOR_COLOR_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_COLOR_COLOR_H

    #include "../algorithms.h"
    #include<string>
    #include<map>
    #include<vector>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmCOLORING : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);

                    bool CompleteColoring(Graph& G, VertexPartitioning& PreColoring, int k);
                    VertexPartitioning FindColoring(Graph &G);
                    bool AddColoring(Graph& G, int k, std::string ColoringName);
                    void AddColoring(Graph& G, std::string ColoringName);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmColoringRegistrator;

            };
        }
    }

#endif

