
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
            class AlgorithmVERTEXCOVER_APX2 : public ApproximationAlgorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    static void FindVertexCover(Graph& G, VertexSet& VertexCover);
                    static void AddVertexCover(Graph &G, std::string VertexCoverName);

                    bool CanGuaranteeApproximationDistance(const Graph& G, float MaxApproximationDistance);
                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmVertexCoverAPX2Registrator;
            };
        }
    }

#endif



