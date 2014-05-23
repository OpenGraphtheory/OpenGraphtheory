
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_PATH_PATH_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_PATH_PATH_H

    #include "../algorithm.h"
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
                    std::list<std::pair<Vertex*, Edge*>* > FindShortestPath(Graph &G, Vertex* from, Vertex* to,
                                                                            VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);
                    std::list<std::pair<Vertex*, Edge*>* > FindPath(Graph &G, Vertex* from, Vertex* to,
                                                                    VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);
                    VertexSet ReachableVertices(Graph& G, Vertex* from, VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmPathRegistrator;

                    void DijkstraInit(Graph& G, VertexSet &Q, VertexWeighting& distance, Vertex* from);
                    void DijkstraUpdate(Vertex* v, Edge* e, Vertex* u, VertexWeighting &distance,
                            std::map<Vertex*, std::pair<Vertex*, Edge*>* > &path);
                    std::list<std::pair<Vertex*, Edge*>* > DijkstraExtract(Vertex* from, Vertex* to,
                                                        std::map<Vertex*, std::pair<Vertex*, Edge*>* > &path);
            };
        }
    }

#endif

