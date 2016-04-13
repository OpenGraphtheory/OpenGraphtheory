
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_VERTEXCOVER_VERTEXCOVER_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_VERTEXCOVER_VERTEXCOVER_H

    #include "../algorithms.h"
    #include "../maximummatching/maximummatching.h"
    #include<string>
    #include<set>
    #include "../../set_helper.h"
    #include<vector>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmVERTEXCOVER : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    bool FindVertexCover(Graph& G, VertexSet& VertexCover, unsigned int k);
                    void FindMinimumVertexCover(Graph& G, VertexSet& VertexCover);
                    void AddVertexCover(Graph &G, std::string VertexCoverName);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmVertexCoverRegistrator;
                    bool TestVertexCover(Graph &G,
                                                EdgeSet& Uncovered,
                                                VertexSet& VertexCover,
                                                int k);
            };
        }
    }

#endif



