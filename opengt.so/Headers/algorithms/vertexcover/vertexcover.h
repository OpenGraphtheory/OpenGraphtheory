
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_VERTEXCOVER_VERTEXCOVER_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_VERTEXCOVER_VERTEXCOVER_H

    #include "../algorithm.h"
    #include "../maximummatching/maximummatching.h"
    #include<string>
    #include<set>
    #include "../../set_helper.h"
    #include<vector>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmVERTEXCOVER : public ExactAlgorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    static bool FindVertexCover(Graph& G, VertexSet& VertexCover, unsigned int k);
                    static void FindMinimumVertexCover(Graph& G, VertexSet& VertexCover);
                    static void AddVertexCover(Graph &G, std::string VertexCoverName);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmVertexCoverRegistrator;
                    static bool TestVertexCover(Graph &G,
                                                EdgeSet& Uncovered,
                                                VertexSet& VertexCover,
                                                unsigned int k);
            };
        }
    }

#endif



