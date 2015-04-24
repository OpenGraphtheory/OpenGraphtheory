
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_DOMINATINGSET_DOMINATINGSET_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_DOMINATINGSET_DOMINATINGSET_H

    #include "../algorithms.h"
    #include<string>
    #include<set>
    #include<vector>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmDOMINATINGSET : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    bool FindDominatingSet(Graph& G, VertexSet& DominatingSet, unsigned int k, bool VerticesCanDominateThemselves = true);
                    void AddDominatingSet(Graph &G, std::string DominatingSetName, bool VerticesCanDominateThemselves = true);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmDominatingSetRegistrator;
                    bool TestDominatingSet(Graph &G,
                                           std::map<Vertex*,int>& Dominators, VertexSet& Undominated,
                                           VertexSet& Excluded, VertexSet& DominatingSet,
                                           unsigned int k, bool VerticesCanDominateThemselves = true);
            };
        }
    }

#endif


