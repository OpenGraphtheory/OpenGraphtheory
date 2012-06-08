
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_CLIQUE_CLIQUE_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_CLIQUE_CLIQUE_H

    #include "../algorithm.h"
    #include<string>
    #include<set>
    #include<vector>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmCLIQUE : public Algorithm
            {
                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmCliqueRegistrator;
                    void Run(Graph &G, std::vector<std::string> parameters);

                    bool Clique(Graph& G, std::set<Graph::VertexIterator>& Clique, int k);
                    void AddClique(Graph &G, std::string CliqueName);
            };
        }
    }

#endif


