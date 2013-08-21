
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMMATCHING_MAXIMUMMATCHING_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMMATCHING_MAXIMUMMATCHING_H

    #include "maximalmatching.h"
    #include "../algorithm.h"
    #include "../../set_helper.h"
    #include "../oddcircle/oddcircle.h"
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
                    static void FindMaximumMatching(Graph& G, std::set<Graph::EdgeIterator>& Matching, string MatchingName);
                    static void AddMaximumMatching(Graph &G, std::string MatchingName);

                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmMaximumMatchingRegistrator;

                    static void FindMaximumMatching(Graph& G, std::set<Graph::EdgeIterator>& Matching, std::set<Graph::VertexIterator>& MatchedVertices, std::string MatchingName);
                    static void FindMaximumMatchingBipartite(Graph& G, std::set<Graph::EdgeIterator>& Matching, std::set<Graph::VertexIterator>& MatchedVertices);

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


