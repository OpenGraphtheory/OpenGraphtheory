
#include "../../../Headers/algorithms/path/path.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        // Diijkstras Algorithm
        // see http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

        MultiFactoryRegistrator<Algorithm> AlgorithmPATH::AlgorithmPathRegistrator(
            &Algorithm::AlgorithmFactory, "path", new DefaultInstantiator<Algorithm, AlgorithmPATH>(
            "path", "Adds a path to the graph", "http://en.wikipedia.org/wiki/Path_%28graph_theory%29"));


        list<pair<Vertex*, Edge*>* > AlgorithmPATH::FindPath(Graph &G, Vertex* from, Vertex* to,
                                                             VertexFilter *vertexfilter, EdgeFilter *edgefilter)
        {
            VertexWeighting distance;
            VertexSet Q;
            map<Vertex*, pair<Vertex*,Edge*>* > path;

            DijkstraInit(G, Q, distance, from);
            while(!Q.empty())
            {
                // select vertex u from Q with smallest distance to start-vertex
                VertexIterator v = G.EndVertices();
                float vDistance = 0;
                bool FirstHit = true;
                for(VertexIterator vCandidate = Q.begin(); vCandidate != Q.end(); vCandidate++)
                    if(distance.find(*vCandidate) != distance.end())
                        if(distance[*vCandidate] < vDistance || FirstHit)
                        {
                            FirstHit = false;
                            v = vCandidate;
                            vDistance = distance[*v];
                        }

                if(v == G.EndVertices()) // this can happen if the graph is not connected
                    break;
                if(*v == to) // shortest path to target-vertex found.
                    break;

                Q.erase(*v);

                EdgeSet IncidentEdges = (*v)->CollectIncidentEdges(1,1,0);
                for(EdgeIterator e = IncidentEdges.begin(); e != IncidentEdges.end(); e++)
                {
                    if(edgefilter != NULL)
                        if(!edgefilter->EdgeAllowed(*e))
                            continue;

                    VertexSet ReachableVertices = (*e)->CollectIncidentVertices(1,1,0);
                    for(VertexIterator u = ReachableVertices.begin(); u != ReachableVertices.end(); u++)
                        if(Q.contains(*u) && !(vertexfilter != NULL && !vertexfilter->VertexAllowed(*u)))
                            DijkstraUpdate(*v, *e, *u, distance, path);
                }
            }

            list<pair<Vertex*, Edge*>* > result = DijkstraExtract(from, to, path);
            for(map<Vertex*, pair<Vertex*,Edge*>* >::iterator i = path.begin(); i != path.end(); i++)
                delete i->second;
            return result;
        }

        void AlgorithmPATH::DijkstraInit(Graph& G, VertexSet &Q, VertexWeighting& distance, Vertex* from)
        {
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                Q.insert(*v);
            distance[from] = 0;
        }

        void AlgorithmPATH::DijkstraUpdate(Vertex* v, Edge* e, Vertex* u, VertexWeighting &distance,
                            map<Vertex*, pair<Vertex*, Edge*>* > &path)
        {
            if((distance.find(u) == distance.end()) || (e->GetWeight() + distance[v] < distance[u]))
            {
                distance[u] = distance[v] + e->GetWeight();
                path[u] = new pair<Vertex*, Edge*>(v, e);
            }
        }

        list<pair<Vertex*, Edge*>* > AlgorithmPATH::DijkstraExtract(Vertex* from, Vertex* to,
                                                        map<Vertex*, pair<Vertex*, Edge*>* >& path)
        {
            list<pair<Vertex*, Edge*>* > result;
            for(Vertex* i = to; i != from; i = path[i]->first)
                result.push_front(new pair<Vertex*, Edge*>(path[i]->first, path[i]->second));
            return result;
        }


        void AlgorithmPATH::AddPath(Graph &G, Vertex* from, Vertex* to, std::string PathName)
        {
            list<pair<Vertex*, Edge*>* > path = FindPath(G, from, to);

            EdgeSet pathedges;
            for(list<pair<Vertex*, Edge*>* >::iterator i = path.begin(); i != path.end(); i++)
            {
                pathedges.insert((*i)->second);
                delete *i;
            }
            G.AddEdgeSet(pathedges, PathName);
        }


        void AlgorithmPATH::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

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

            if(from != G.EndVertices() && to != G.EndVertices())
                AddPath(G, *from, *to, PathName);
        }

    }
}
