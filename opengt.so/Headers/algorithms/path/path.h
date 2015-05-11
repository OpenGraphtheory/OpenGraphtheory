
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_PATH_PATH_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_PATH_PATH_H

    #include "../algorithms.h"
    #include "../../VertexEdgeFilter.h"
    #include<string>
    #include<map>
    #include<vector>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmPATH : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    void AddPath(Graph &G, Vertex* from, Vertex* to, std::string pathname);
                    std::list<std::pair<Vertex*, Edge*>* > FindPath(Graph &G, Vertex* from, Vertex* to,
                                                                    VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);
                    VertexSet ForwardReachableVertices(Graph& G, Vertex* from, VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);
                    VertexSet BackwardReachableVertices(Graph& G, Vertex* from, VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);
                    VertexSet WeaklyReachableVertices(Graph& G, Vertex* from, VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmPathRegistrator;
                    VertexSet ReachableVertices(Graph& G, Vertex* from, VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL, bool positive=true, bool negative=false);
            };
        }
    }

#endif

