
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
                    void AddPath(Graph &G, Graph::VertexIterator from, Graph::VertexIterator to, std::string pathname);
                    std::list<std::pair<Graph::VertexIterator, Graph::EdgeIterator>* > FindShortestPath(Graph &G, Graph::VertexIterator from, Graph::VertexIterator to,
                                                                                              VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);
                    std::list<std::pair<Graph::VertexIterator, Graph::EdgeIterator>* > FindPath(Graph &G, Graph::VertexIterator from, Graph::VertexIterator to,
                                                                                              VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);
                    std::set<Graph::VertexIterator> ReachableVertices(Graph& G, Graph::VertexIterator from,
                                                                 VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmPathRegistrator;

                    void DijkstraInit(Graph& G, std::set<Graph::VertexIterator> &Q, std::map<Graph::VertexIterator, float>& distance, Graph::VertexIterator from);
                    void DijkstraUpdate(Graph::VertexIterator v, Graph::EdgeIterator e, Graph::VertexIterator u, std::map<Graph::VertexIterator, float> &distance,
                            std::map<Graph::VertexIterator, std::pair<Graph::VertexIterator, Graph::EdgeIterator>* > *path);
                    std::list<std::pair<Graph::VertexIterator, Graph::EdgeIterator>* > DijkstraExtract(Graph::VertexIterator from, Graph::VertexIterator to,
                                                        std::map<Graph::VertexIterator, std::pair<Graph::VertexIterator, Graph::EdgeIterator>* > *path);
            };
        }
    }

#endif

