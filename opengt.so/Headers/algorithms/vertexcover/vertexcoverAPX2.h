
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_VERTEXCOVER_VERTEXCOVERAPX2_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_VERTEXCOVER_VERTEXCOVERAPX2_H

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
            class AlgorithmVERTEXCOVER_APX2 : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    static void FindVertexCover(Graph& G, std::set<Graph::VertexIterator>& VertexCover);
                    static void AddVertexCover(Graph &G, std::string VertexCoverName);

                    float ApproximationQuality(const Graph& G);
                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmVertexCoverAPX2Registrator;
            };
        }
    }

#endif



