
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
            class AlgorithmVERTEXCOVER : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    static bool FindVertexCover(Graph& G, std::set<Graph::VertexIterator>& VertexCover, unsigned int k);
                    static void FindMinimumVertexCover(Graph& G, std::set<Graph::VertexIterator>& VertexCover);
                    static void AddVertexCover(Graph &G, std::string VertexCoverName);

                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmVertexCoverRegistrator;
                    static bool TestVertexCover(Graph &G,
                                                std::set<Graph::EdgeIterator>& Uncovered,
                                                std::set<Graph::VertexIterator>& VertexCover,
                                                unsigned int k);
            };
        }
    }

#endif



