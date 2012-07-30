
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
                    bool Colorize(Graph& G, int k, std::string ColorizationName);
                    void Colorize(Graph &G, std::string ColorizationName);

                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmColoringRegistrator;

                    bool Colorize(Graph& G, std::map<Graph::VertexIterator, int>& Colors, int k);
            };
        }
    }

#endif

