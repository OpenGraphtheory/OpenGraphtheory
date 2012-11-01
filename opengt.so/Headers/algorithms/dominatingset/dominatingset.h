
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_DOMINATINGSET_DOMINATINGSET_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_DOMINATINGSET_DOMINATINGSET_H

    #include "../algorithm.h"
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
                    bool FindDominatingSet(Graph& G, std::set<Graph::VertexIterator>& DominatingSet, unsigned int k, bool VerticesCanDominateThemselves = true);
                    void AddDominatingSet(Graph &G, std::string DominatingSetName, bool VerticesCanDominateThemselves = true);

                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmDominatingSetRegistrator;
                    bool TestDominatingSet(Graph &G,
                                           std::map<Graph::VertexIterator,int>& Dominators, std::set<Graph::VertexIterator>& Undominated,
                                           std::set<Graph::VertexIterator>& Excluded, std::set<Graph::VertexIterator>& DominatingSet,
                                           unsigned int k, bool VerticesCanDominateThemselves = true);
            };
        }
    }

#endif


