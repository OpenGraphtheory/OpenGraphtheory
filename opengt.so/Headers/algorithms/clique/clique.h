
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
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    bool FindClique(Graph& G, std::set<Graph::VertexIterator>& Clique, unsigned int k);
                    void AddClique(Graph &G, std::string CliqueName);

                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmCliqueRegistrator;
                    bool TestClique(std::set<Graph::VertexIterator>& Clique, std::set<Graph::VertexIterator>& Candidates, unsigned int k, bool CliqueOrIndependentSet = true);
            };
        }
    }

#endif


