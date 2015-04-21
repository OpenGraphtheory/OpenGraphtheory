
#include "../../../Headers/algorithms/oddcircle/oddcircle.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmODDCIRCLE::AlgorithmOddCircleRegistrator(
            &Algorithm::AlgorithmFactory, "oddcircle", new DefaultInstantiator<Algorithm, AlgorithmODDCIRCLE>(
            "oddcircle", "Adds an odd circle to the graph", "http://en.wikipedia.org/wiki/Bipartite_graph#Characterization"));

        // need to store the edges between a vertex and its predecessor

        bool AlgorithmODDCIRCLE::FindOddCircle(Graph &G, list<Vertex*>& OddCircle)
        {
            VertexSet Q;
            map<Vertex*, pair<Vertex*,Edge*>* > path;
            VertexPartitioning coloring;
            list<Vertex*> queue;

            bool ClashFound = false;
            Vertex *BipartitionClash1=NULL, *BipartitionClash2=NULL, *BipartitionClashRoot=NULL;


            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                Q.insert(*v);
                coloring[*v] = 0;
            }


            while(!Q.empty() && !ClashFound)
            {
                queue.push_back(*(Q.begin()));
                coloring[*(Q.begin())] = 1;
                path[*(Q.begin())] = NULL;
                Q.erase(*(Q.begin()));

                while(queue.size() > 0 && !ClashFound)
                {
                    Vertex* front = *(queue.begin());
                    queue.pop_front();
                    int neighbor_color = (coloring[front] == 1 ? 2 : 1);

                    VertexSet neighbors = front->CollectNeighbors(1,1,1,1,1,1,1,1,1);
                    neighbors.erase(front); // just in case
                    for(VertexIterator n = neighbors.begin(); n != neighbors.end() && !ClashFound; n++)
                    {
                        if(coloring[*n] == 0)
                        {
                            coloring[*n] = neighbor_color;
                            path[*n] = new pair<Vertex*,Edge*>(front, *G.EndEdges());
                            queue.push_back(*n);
                            Q.erase(*n);
                        }
                        else if(coloring[*n] != neighbor_color)
                        {
                            BipartitionClash1 = front;
                            BipartitionClash2 = *n;
                            ClashFound = true;
                        }
                    }
                } // while(queue.size() > 0)
            } // while(!Q.empty())


            if(ClashFound)
            {
                VertexSet Path1;
                Vertex* p1 = BipartitionClash1;
                while(true)
                {
                    Path1.insert(p1);
                    pair<Vertex*,Edge*>* p1pred = path[p1];
                    if(p1pred == NULL)
                        break;
                    p1 = p1pred->first;
                }

                Vertex* p2 = BipartitionClash2;
                while(true)
                {
                    OddCircle.push_back(p2);
                    if(Path1.contains(p2))
                    {
                        BipartitionClashRoot = p2;
                        break;
                    }

                    pair<Vertex*,Edge*>* p2pred = path[p2];
                    if(p2pred == NULL)
                        break;
                    p2 = p2pred->first;
                }

                p1 = BipartitionClash1;
                while(p1 != BipartitionClashRoot)
                {
                    OddCircle.push_front(p1);
                    pair<Vertex*,Edge*>* p1pred = path[p1];
                    if(p1pred == NULL)
                        break;
                    p1 = p1pred->first;
                }
            }

            for(map<Vertex*, pair<Vertex*,Edge*>* >::iterator i = path.begin(); i != path.end(); i++)
                delete i->second;
            return ClashFound;
        }

        void AlgorithmODDCIRCLE::AddOddCircle(Graph &G, std::string OddCircleName)
        {
            list<Vertex*> OddCircle;
            if(FindOddCircle(G, OddCircle))
            {
                VertexSet as_set;
                for(list<Vertex*>::iterator i = OddCircle.begin(); i != OddCircle.end(); i++)
                    as_set.insert(*i);
                G.AddVertexSet(as_set, OddCircleName);
            }
        }


        void AlgorithmODDCIRCLE::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string PathName = parameters[0];
            AddOddCircle(G, PathName);
        }

    }
}
