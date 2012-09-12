
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_COLOR_COLOR_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_COLOR_COLOR_H

    #include "../algorithm.h"
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

                    bool CompleteColoring(Graph& G, map<Graph::VertexIterator, int>& PreColoring, int k);
                    map<Graph::VertexIterator, int> FindColoring(Graph &G);
                    bool AddColoring(Graph& G, int k, string ColoringName);
                    void AddColoring(Graph& G, string ColoringName);

                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmColoringRegistrator;

            };
        }
    }

#endif

