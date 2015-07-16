
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_KPATH_KPATH_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_KPATH_KPATH_H

    #include "../algorithms.h"
    #include<string>
    #include<set>
    #include<vector>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmKPATH : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    bool FindKPath(Graph& G, int k, EdgeSet& KPath);
                    void AddKPath(Graph &G, int k, std::string KPathName);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmKPathRegistrator;
                    bool TestKPath(Graph &G,
                                   int k,
                                   Vertex* source,
                                   Vertex* target,
                                   VertexSet& Visited,
                                   EdgeSet& Path,
                                   bool cyclic);
                    // source must be != NULL
                    // target CAN be NULL if cyclic=false, but must be != NULL if cyclic=true
                    bool DoTestKPath(Graph &G,
                                   int k,
                                   Vertex* source,
                                   Vertex* target,
                                   VertexSet& Visited,
                                   EdgeSet& Path,
                                   bool cyclic);
            };
        }
    }

#endif


