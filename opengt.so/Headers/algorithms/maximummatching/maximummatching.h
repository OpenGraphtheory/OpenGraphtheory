
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMMATCHING_MAXIMUMMATCHING_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMMATCHING_MAXIMUMMATCHING_H

    #include "maximalmatching.h"
    #include "../algorithms.h"
    #include "../../set_helper.h"
    #include "../oddcircle/oddcircle.h"
    #include<string>
    #include<set>
    #include<vector>
    #include<map>
    #include<queue>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmMAXIMUMMATCHING : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    static void FindMaximumMatching(Graph& G, EdgeSet& Matching);
                    static void AddMaximumMatching(Graph &G, std::string MatchingName);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmMaximumMatchingRegistrator;

                    static bool Augment(Graph& G, EdgeSet& Matching, VertexPartitioning& ContractionEquivalenceClass);
            };
        }
    }

#endif


