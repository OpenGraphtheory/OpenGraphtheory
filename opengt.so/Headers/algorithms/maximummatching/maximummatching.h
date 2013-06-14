
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMMATCHING_MAXIMUMMATCHING_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMMATCHING_MAXIMUMMATCHING_H

    #include "../algorithm.h"
    #include "../../set_helper.h"
    #include<string>
    #include<set>
    #include<vector>
    #include<map>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmMAXIMUMMATCHING : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    static void FindMaximumMatching(Graph& G, std::set<Graph::EdgeIterator>& Matching);
                    static void AddMaximumMatching(Graph &G, std::string MatchingName);

                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmMaximumMatchingRegistrator;
                    static bool FindAugmentingPath(Graph& G, Graph::VertexIterator Source, std::set<Graph::EdgeIterator>& Matching,
                                         std::set<Graph::VertexIterator>& MatchedVertices, std::vector<Graph::EdgeIterator>& AugmentingPath);
                    static bool FindAugmentingPath(Graph& G, std::set<Graph::EdgeIterator>& Matching,
                                         std::set<Graph::VertexIterator>& MatchedVertices,
                                         std::vector<Graph::EdgeIterator>& AugmentingPath);
                    static bool FindAugmentingPath(Graph& G, std::set<Graph::EdgeIterator>& Matching,
                                         std::vector<Graph::EdgeIterator>& AugmentingPath);
            };
        }
    }

#endif


