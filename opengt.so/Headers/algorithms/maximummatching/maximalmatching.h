
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMMATCHING_MAXIMALMATCHING_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_MAXIMUMMATCHING_MAXIMALMATCHING_H

    #include "../algorithms.h"
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
                    static void FindMaximalMatching(Graph& G, EdgeSet& Matching, VertexSet& MatchedVertices);
                    static void FindMaximalMatching(Graph& G, EdgeSet& Matching);
                    static void AddMaximalMatching(Graph &G, std::string MatchingName);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmMaximalMatchingRegistrator;
            };
        }
    }

#endif


