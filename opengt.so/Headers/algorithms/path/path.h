
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
                    list<pair<Graph::VertexIterator, Graph::EdgeIterator>* > FindShortestPath(Graph &G, Graph::VertexIterator from, Graph::VertexIterator to,
                                                                                              VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);
                    list<pair<Graph::VertexIterator, Graph::EdgeIterator>* > FindPath(Graph &G, Graph::VertexIterator from, Graph::VertexIterator to,
                                                                                              VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);
                    set<Graph::VertexIterator> ReachableVertices(Graph& G, Graph::VertexIterator from,
                                                                 VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);

                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmPathRegistrator;

                    void DijkstraInit(Graph& G, set<Graph::VertexIterator> &Q, map<Graph::VertexIterator, float>& distance, Graph::VertexIterator from);
                    void DijkstraUpdate(Graph::VertexIterator v, Graph::EdgeIterator e, Graph::VertexIterator u, map<Graph::VertexIterator, float> &distance,
                            map<Graph::VertexIterator, pair<Graph::VertexIterator, Graph::EdgeIterator>* > *path);
                    list<pair<Graph::VertexIterator, Graph::EdgeIterator>* > DijkstraExtract(Graph::VertexIterator from, Graph::VertexIterator to,
                                                        map<Graph::VertexIterator, pair<Graph::VertexIterator, Graph::EdgeIterator>* > *path);
            };
        }
    }

#endif

