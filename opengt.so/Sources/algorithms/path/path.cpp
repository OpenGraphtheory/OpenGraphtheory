
#include "../../../Headers/algorithms/path/path.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        // Dijkstras Algorithm
        // see http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

        MultiFactoryRegistrator<Algorithm> AlgorithmPATH::AlgorithmPathRegistrator(
            &Algorithm::AlgorithmFactory, "path", new DefaultInstantiator<Algorithm, AlgorithmPATH>(
            "path", "Adds a path to the graph", "http://en.wikipedia.org/wiki/Path_%28graph_theory%29"));


        list<pair<Vertex*, Edge*>* > AlgorithmPATH::FindPath(Graph &G, Vertex* from, Vertex* to,
                                                             VertexFilter *vertexfilter, EdgeFilter *edgefilter)
        {
            list<pair<Vertex*, Edge*>* > result;
            if(from == to)
                return result;

            VertexSet Q;
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                Q.insert(*v);
            VertexWeighting distance;
            distance[from] = 0;

            map<Vertex*, pair<Vertex*,Edge*>* > dfstree;

            while(!Q.empty())
            {
                // select vertex v from Q with smallest distance to start-vertex
                VertexIterator v = Q.end();
                float vDistance = 0;
                bool FirstHit = true;
                for(VertexIterator vCandidate = Q.begin(); vCandidate != Q.end(); vCandidate++)
                    if(distance.find(*vCandidate) != distance.end())
                        if((distance[*vCandidate] < vDistance) || FirstHit)
                        {
                            FirstHit = false;
                            v = vCandidate;
                            vDistance = distance[*v];
                        }

                if(v == Q.end()) // this can happen if the graph is not connected
                    break;
                if(*v == to) // shortest path to target-vertex found.
                    break;

                Vertex* pv = *v;
                // erase v from Q (this means dfstree[v] now contains the final predecessor and edge for v)
                Q.erase(pv);

                // update distances of v's neighbors (only the ones which are still in Q)
                EdgeSet IncidentEdges = pv->CollectIncidentEdges(1,1,0);
                for(EdgeIterator e = IncidentEdges.begin(); e != IncidentEdges.end(); e++)
                {
                    if(edgefilter != NULL && !edgefilter->EdgeAllowed(*e))
                        continue;

                    VertexSet vNeighbors = (*e)->CollectIncidentVertices(1,1,0);
                    for(VertexIterator n = vNeighbors.begin(); n != vNeighbors.end(); n++)
                    {
                        if(Q.contains(*n) ) // && (vertexfilter == NULL || vertexfilter->VertexAllowed(*n))
                        {
                            if((distance.find(*n) == distance.end()) || ((*e)->GetWeight() + distance[pv] < distance[*n]))
                            {
                                distance[*n] = distance[pv] + (*e)->GetWeight();
                                if(dfstree.find(*n) == dfstree.end())
                                    dfstree[*n] = new pair<Vertex*, Edge*>(pv, *e);
                                dfstree[*n]->first = pv;
                                dfstree[*n]->second = *e;
                            }
                        }
                    }
                }
            }

            if(dfstree.find(to) != dfstree.end()) // path found
            {
                Vertex* i2;
                for(Vertex* i = to; i != from; i = i2)
                {
                    i2 = dfstree[i]->first;
                    result.push_front(dfstree[i]);
                    dfstree[i] = NULL;
                }
            }
            else // no path found
            {
                result.push_back(NULL);
            }

            // erase dfs-tree
            for(map<Vertex*, pair<Vertex*,Edge*>* >::iterator i = dfstree.begin(); i != dfstree.end(); i++)
                delete i->second;

            return result;
        }



        VertexSet AlgorithmPATH::ReachableVertices(Graph& G, Vertex* from, VertexFilter *vertexfilter, EdgeFilter *edgefilter, bool positive, bool negative)
        {
            VertexSet result;
            result.insert(from);
            vector<Vertex*> queue(G.NumberOfVertices()+1);
            queue[0] = from;
            int head=1, tail=0;

            while(tail < head)
            {
                Vertex* v = queue[tail++];

                EdgeSet IncidentEdges = v->CollectIncidentEdges(1,positive,negative);
                for(EdgeIterator e = IncidentEdges.begin(); e != IncidentEdges.end(); e++)
                {
                    if(edgefilter != NULL && !edgefilter->EdgeAllowed(*e))
                        continue;

                    VertexSet ReachableVertices = (*e)->CollectIncidentVertices(1,positive,negative);
                    for(VertexIterator u = ReachableVertices.begin(); u != ReachableVertices.end(); u++)
                    {
                        if(result.contains(*u) || (vertexfilter != NULL && !vertexfilter->VertexAllowed(*u)))
                            continue;
                        result.insert(*u);
                        queue[head++] = *u;
                    }
                }
            }

            return result;
        }

        VertexSet AlgorithmPATH::ForwardReachableVertices(Graph& G, Vertex* from, VertexFilter *vertexfilter, EdgeFilter *edgefilter)
        {
            return ReachableVertices(G,from,vertexfilter,edgefilter,1,0);
        }

        VertexSet AlgorithmPATH::BackwardReachableVertices(Graph& G, Vertex* from, VertexFilter *vertexfilter, EdgeFilter *edgefilter)
        {
            return ReachableVertices(G,from,vertexfilter,edgefilter,0,1);
        }

        VertexSet AlgorithmPATH::WeaklyReachableVertices(Graph& G, Vertex* from, VertexFilter *vertexfilter, EdgeFilter *edgefilter)
        {
            return ReachableVertices(G,from,vertexfilter,edgefilter,1,1);
        }


        void AlgorithmPATH::AddPath(Graph &G, Vertex* from, Vertex* to, std::string PathName)
        {
            list<pair<Vertex*, Edge*>* > path = FindPath(G, from, to);

            if(path.size() != 1 || *(path.begin()) != NULL)
            {
                EdgeSet pathedges;
                for(list<pair<Vertex*, Edge*>* >::iterator i = path.begin(); i != path.end(); i++)
                {
                    pathedges.insert((*i)->second);
                    delete *i;
                }
                G.AddEdgeSet(pathedges, PathName);
            }
        }


        void AlgorithmPATH::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() < 3)
                throw "Path Algorithm needs 3 parameters (start, end, result name)";

            string FromName =  parameters[0];
            string ToName = parameters[1];
            string PathName = parameters[2];
            VertexIterator from = G.EndVertices();
            VertexIterator to = G.EndVertices();

            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                if(FromName == (*v)->GetLabel())
                    from = v;
                if(ToName == (*v)->GetLabel())
                    to = v;
            }

            if(from == G.EndVertices())
                throw "Path Algorithm: start vertex not found";
            if(to == G.EndVertices())
                throw "Path Algorithm: end vertex not found";

            AddPath(G, *from, *to, PathName);
        }

    }
}
