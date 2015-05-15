
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_VERTEXDISJOINTPATHS_VERTEXDISJOINTPATHS_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_VERTEXDISJOINTPATHS_VERTEXDISJOINTPATHS_H

    #include "../../opengt.h"
    #include "../../VertexEdgeFilter.h"
    #include "../algorithms.h"
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

                    bool FindAugmentingPathOrSeparator(Graph &G, VertexSet& Sources, VertexSet& Drains, EdgeSet& EFlow,
                                                                 VertexSet& VFlow, std::vector<Edge*>& AugmentingPath, VertexSet& Separator,
                                                                 VertexFilter* vertexfilter = NULL, EdgeFilter* edgefilter = NULL);
                    bool FindDisjointPathsAndSeparator(Graph &G, VertexSet& Sources, VertexSet& Drains, EdgeSet& DisjointPaths, VertexSet& Separator,
                                                                 VertexFilter* vertexfilter = NULL, EdgeFilter* edgefilter = NULL);
                public:
                    bool FindDisjointPaths(Graph &G, VertexSet& Sources, VertexSet& Drain, EdgeSet& EFlow,
                                                     VertexFilter* vertexfilter = NULL, EdgeFilter* edgefilter = NULL);
                    bool FindDisjointPaths(Graph &G, Vertex* Source, Vertex* Drain, EdgeSet& EFlow,
                                                     VertexFilter* vertexfilter = NULL, EdgeFilter* edgefilter = NULL);

                    void AddVertexDisjointPaths(Graph &G, VertexSet& Sources, VertexSet& Drain, std::string DisjointPathsName);
                    void AddVertexDisjointPaths(Graph &G, Vertex* Source, Vertex* Drain, std::string DisjointPathsName);
                    void Run(Graph& G, std::vector<std::string> parameters);
            };

        }
    }

#endif

