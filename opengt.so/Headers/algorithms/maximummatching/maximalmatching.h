
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMMATCHING_MAXIMALMATCHING_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMMATCHING_MAXIMALMATCHING_H

    #include "../algorithm.h"
    #include "../../set_helper.h"
    #include<string>
    #include<vector>
    #include<set>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmMAXIMALMATCHING : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    static void FindMaximalMatching(Graph& G, std::set<Graph::EdgeIterator>& Matching);
                    static void AddMaximalMatching(Graph &G, std::string MatchingName);

                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmMaximalMatchingRegistrator;
            };
        }
    }

#endif


