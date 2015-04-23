
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_VERTEXDISJOINTPATHS_VERTEXDISJOINTPATHS_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_VERTEXDISJOINTPATHS_VERTEXDISJOINTPATHS_H

    #include "../../opengt.h"
    #include "../algorithm.h"
    #include <set>
    #include "../../set_helper.h"

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {

            class AlgorithmVERTEXDISJOINTPATHS : public Algorithm
            {
                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmVertexDisjointPathsRegistrator;

                    bool FindAugmentingPathOrSeparator(Graph &G, Vertex* Source, Vertex* Drain, EdgeSet& EFlow,
                                                      VertexSet& VFlow, std::vector<Edge*>& AugmentingPath, VertexSet& Separator);
                    bool FindDisjointPathsAndSeparator(Graph &G, Vertex* Source, Vertex* Drain, EdgeSet& DisjointPaths, VertexSet& Separator);
                    bool FindDisjointPaths(Graph &G, Vertex* Source, Vertex* Drain, EdgeSet& EFlow);
                public:
                    void AddVertexDisjointPaths(Graph &G, Vertex* Source, Vertex* Drain, std::string DisjointPathsName);
                    void Run(Graph& G, std::vector<std::string> parameters);
            };

        }
    }

#endif

