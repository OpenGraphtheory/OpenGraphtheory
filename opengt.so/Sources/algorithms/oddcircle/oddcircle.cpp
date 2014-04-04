
#include "../../../Headers/algorithms/oddcircle/oddcircle.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        FactoryRegistrator<Algorithm> AlgorithmODDCIRCLE::AlgorithmOddCircleRegistrator(
            &Algorithm::AlgorithmFactory, "oddcircle", new DefaultInstantiator<Algorithm, AlgorithmODDCIRCLE>(
            "oddcircle", "Adds an odd circle to the graph", "http://en.wikipedia.org/wiki/Bipartite_graph#Characterization"));

        // need to store the edges between a vertex and its predecessor

        bool AlgorithmODDCIRCLE::FindOddCircle(Graph &G, list<Graph::VertexIterator>& OddCircle)
        {
            set<Graph::VertexIterator> Q;
            map<Graph::VertexIterator, pair<Graph::VertexIterator,Graph::EdgeIterator>* > path;
            map<Graph::VertexIterator, int> coloring;
            list<Graph::VertexIterator> queue;

            bool ClashFound = false;
            Graph::VertexIterator BipartitionClash1, BipartitionClash2, BipartitionClashRoot;


            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                Q.insert(v);
                coloring[v] = 0;
            }


            while(!Q.empty() && !ClashFound)
            {
                queue.push_back(*(Q.begin()));
                coloring[*(Q.begin())] = 1;
                path[*(Q.begin())] = NULL;
                Q.erase(Q.begin());

                while(queue.size() > 0 && !ClashFound)
                {
                    Graph::VertexIterator front = *(queue.begin());
                    queue.pop_front();
                    int neighbor_color = (coloring[front] == 1 ? 2 : 1);

                    set<Graph::VertexIterator> neighbors = front.CollectNeighbors(1,1,1,1,1,1,1,1,1);
                    neighbors.erase(front); // just in case
                    for(set<Graph::VertexIterator>::iterator n = neighbors.begin(); n != neighbors.end() && !ClashFound; n++)
                    {
                        if(coloring[*n] == 0)
                        {
                            coloring[*n] = neighbor_color;
                            path[*n] = new pair<Graph::VertexIterator,Graph::EdgeIterator>(front, G.EndEdges());
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
                set<Graph::VertexIterator> Path1;
                Graph::VertexIterator p1 = BipartitionClash1;
                while(true)
                {
                    Path1.insert(p1);
                    pair<Graph::VertexIterator,Graph::EdgeIterator>* p1pred = path[p1];
                    if(p1pred == NULL)
                        break;
                    p1 = p1pred->first;
                }

                Graph::VertexIterator p2 = BipartitionClash2;
                while(true)
                {
                    OddCircle.push_back(p2);
                    if(Path1.find(p2) != Path1.end())
                    {
                        BipartitionClashRoot = p2;
                        break;
                    }

                    pair<Graph::VertexIterator,Graph::EdgeIterator>* p2pred = path[p2];
                    if(p2pred == NULL)
                        break;
                    p2 = p2pred->first;
                }

                p1 = BipartitionClash1;
                while(p1 != BipartitionClashRoot)
                {
                    OddCircle.push_front(p1);
                    pair<Graph::VertexIterator,Graph::EdgeIterator>* p1pred = path[p1];
                    if(p1pred == NULL)
                        break;
                    p1 = p1pred->first;
                }
            }

            return ClashFound;
        }

        void AlgorithmODDCIRCLE::AddOddCircle(Graph &G, std::string OddCircleName)
        {
            list<Graph::VertexIterator> OddCircle;
            if(FindOddCircle(G, OddCircle))
            {
                set<Graph::VertexIterator> as_set;
                for(list<Graph::VertexIterator>::iterator i = OddCircle.begin(); i != OddCircle.end(); i++)
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
