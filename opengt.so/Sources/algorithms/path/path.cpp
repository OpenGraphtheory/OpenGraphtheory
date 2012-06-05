
#include "../../../Headers/algorithms/path/path.h"
#include<map>

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        // Diijkstras Algorithm
        // see http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

        FactoryRegistrator<Algorithm> AlgorithmPATH::AlgorithmPathRegistrator(
            &Algorithm::AlgorithmFactory, "path", new DefaultInstantiator<Algorithm, AlgorithmPATH>(
            "path", "Adds a path to the graph", "http://en.wikipedia.org/wiki/Path_%28graph_theory%29"));


        list<pair<Graph::VertexIterator, Graph::EdgeIterator>* > AlgorithmPATH::FindPath(Graph &G, Graph::VertexIterator from, Graph::VertexIterator to)
        {
            map<Graph::VertexIterator, float> distance;
            set<Graph::VertexIterator> Q;
            map<Graph::VertexIterator, pair<Graph::VertexIterator,Graph::EdgeIterator>* > path;

            DijkstraInit(G, Q, distance, from);
            while(!Q.empty())
            {
                // select vertex u from Q with smallest distance to start-vertex
                Graph::VertexIterator v = G.EndVertices();
                float vDistance = 0;
                bool FirstHit = true;
                for(set<Graph::VertexIterator>::iterator vCandidate = Q.begin(); vCandidate != Q.end(); vCandidate++)
                    if(distance.find(*vCandidate) != distance.end())
                        if(distance[*vCandidate] < vDistance || FirstHit)
                        {
                            FirstHit = false;
                            v = *vCandidate;
                            vDistance = distance[v];
                        }

                if(v == G.EndVertices()) // this can happen if the graph is not connected
                    break;
                if(v == to) // shortest path to target-vertex found.
                    break;

                Q.erase(v);

                for(Graph::EdgeIterator e = v.BeginIncidentEdges(); e != v.EndIncidentEdges(); e++)
                {
                    for(Graph::VertexIterator u = e.BeginIncidentVertices(); u != e.EndIncidentVertices(); u++)
                        if(Q.find(u) != Q.end())
                            DijkstraUpdate(v, e, u, distance, &path);
                    for(Graph::VertexIterator u = e.BeginPositiveIncidentVertices(); u != e.EndPositiveIncidentVertices(); u++)
                        if(Q.find(u) != Q.end())
                            DijkstraUpdate(v, e, u, distance, &path);
                }

                for(Graph::EdgeIterator e = v.BeginPositiveIncidentEdges(); e != v.EndPositiveIncidentEdges(); e++)
                {
                    for(Graph::VertexIterator u = e.BeginIncidentVertices(); u != e.EndIncidentVertices(); u++)
                        if(Q.find(u) != Q.end())
                            DijkstraUpdate(v, e, u, distance, &path);
                    for(Graph::VertexIterator u = e.BeginPositiveIncidentVertices(); u != e.EndPositiveIncidentVertices(); u++)
                        if(Q.find(u) != Q.end())
                            DijkstraUpdate(v, e, u, distance, &path);
                }

            }

            return DijkstraExtract(from, to, &path);
        }

        void AlgorithmPATH::DijkstraInit(Graph& G, set<Graph::VertexIterator> &Q, map<Graph::VertexIterator, float>& distance, Graph::VertexIterator from)
        {
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                Q.insert(v);
            distance[from] = 0;
        }

        void AlgorithmPATH::DijkstraUpdate(Graph::VertexIterator v, Graph::EdgeIterator e, Graph::VertexIterator u, map<Graph::VertexIterator, float> &distance,
                            map<Graph::VertexIterator, pair<Graph::VertexIterator, Graph::EdgeIterator>* > *path)
        {
            if((distance.find(u) == distance.end()) || (e.GetWeight() + distance[v] < distance[u]))
            {
                distance[u] = distance[v] + e.GetWeight();
                // MEMORY LEAK!!!
                (*path)[u] = new pair<Graph::VertexIterator, Graph::EdgeIterator>(v, e);
            }
        }

        list<pair<Graph::VertexIterator, Graph::EdgeIterator>* > AlgorithmPATH::DijkstraExtract(Graph::VertexIterator from, Graph::VertexIterator to,
                                                        map<Graph::VertexIterator, pair<Graph::VertexIterator, Graph::EdgeIterator>* > *path)
        {
            list<pair<Graph::VertexIterator, Graph::EdgeIterator>* > result;
            for(Graph::VertexIterator i = to; i != from; i = (*path)[i]->first)
                result.push_front((*path)[i]);
            return result;
        }



        void AlgorithmPATH::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string FromName =  parameters[0];
            string ToName = parameters[1];
            string PathName = parameters[2];
            Graph::VertexIterator from = G.EndVertices();
            Graph::VertexIterator to = G.EndVertices();

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                if(FromName == v.GetLabel())
                    from = v;
                if(ToName == v.GetLabel())
                    to = v;
            }

            if(from != G.EndVertices() && to != G.EndVertices())
            {
                list<pair<Graph::VertexIterator, Graph::EdgeIterator>* > path = FindPath(G, from, to);

                for(list<pair<Graph::VertexIterator, Graph::EdgeIterator>* >::iterator i = path.begin(); i != path.end(); i++)
                {
                    (*i)->second.Attributes().Add(PathName, "int");
                    Attribute* attr = (*i)->second.Attributes().GetAttribute(PathName);
                    IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
                    if(iattr != NULL)
                        iattr->Value = 0;
                }
            }
        }

    }
}
