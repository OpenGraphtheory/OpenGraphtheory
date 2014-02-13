
#include "../../../Headers/algorithms/maximummatching/maximummatching.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        FactoryRegistrator<Algorithm> AlgorithmMAXIMUMMATCHING::AlgorithmMaximumMatchingRegistrator(
            &Algorithm::AlgorithmFactory, "maximummatching", new DefaultInstantiator<Algorithm, AlgorithmMAXIMUMMATCHING>(
            "maximummatching", "Adds a maximum matching to the graph", "http://en.wikipedia.org/wiki/Matching_(graph_theory)"));


        bool AlgorithmMAXIMUMMATCHING::Augment(Graph& G, set<Graph::EdgeIterator>& Matching, set<Graph::VertexIterator>& MatchedVertices)
        {
            // Edmonds algorithm

            // Register for quick lookups

            map<Graph::VertexIterator, Graph::VertexIterator> MatchedVertex;
            map<Graph::VertexIterator, Graph::EdgeIterator> MatchedEdge;

            for(set<Graph::EdgeIterator>::iterator i = Matching.begin(); i != Matching.end(); i++)
            {
                Graph::EdgeIterator e = *i;
                Graph::VertexIterator from = e.From();
                Graph::VertexIterator to = e.To();

                MatchedVertex[from] = to;
                MatchedVertex[to] = from;
                MatchedEdge.insert(pair<Graph::VertexIterator, Graph::EdgeIterator>(from, e));
                MatchedEdge.insert(pair<Graph::VertexIterator, Graph::EdgeIterator>(to, e));
            }

            // initialize alternating forest
            set<Graph::VertexIterator> OuterVertices;
            queue<Graph::VertexIterator> OuterVertexQueue;
            map<Graph::VertexIterator, Graph::VertexIterator> ForestRoot;
            map<Graph::VertexIterator, Graph::VertexIterator> ForestPredecessor;
            map<Graph::VertexIterator, Graph::EdgeIterator> PredecessorEdge;

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                if(MatchedVertices.find(v) == MatchedVertices.end())
                {
                    OuterVertices.insert(v);
                    OuterVertexQueue.push(v);
                    ForestRoot[v] = v;
                    ForestPredecessor[v] = G.EndVertices();
                    PredecessorEdge.insert(pair<Graph::VertexIterator, Graph::EdgeIterator>(v, G.EndEdges()));
                }
            }

            // Construct alternating forest

            while(!OuterVertexQueue.empty())
            {
                Graph::VertexIterator v = OuterVertexQueue.front();
                OuterVertexQueue.pop();
cout << "pop\n"; cout.flush();

                set<Graph::VertexIterator> vNeighbors = v.CollectNeighbors(1,0,0,0,0,0,0,0,0);
                for(set<Graph::VertexIterator>::iterator n = vNeighbors.begin(); n != vNeighbors.end(); n++)
                {
                    // Case 1: n is not in the forest => n is matched (otherwise it would be a root itself
                    //         and be in the forest hence). Add n and its match to the forest.
                    if(ForestRoot.find(*n) == ForestRoot.end())
                    {
cout << "   case 1\n"; cout.flush();
                        ForestPredecessor[*n] = v;
                        PredecessorEdge.insert(pair<Graph::VertexIterator, Graph::EdgeIterator>(*n, v.GetEdge(*n, 1,0,0,0,0,0,0,0,0)));
                        ForestRoot[*n] = ForestRoot[v];

                        Graph::VertexIterator m = MatchedVertex[*n];

                        OuterVertices.insert(m);
                        OuterVertexQueue.push(m);
                        ForestPredecessor[m] = *n;
                        PredecessorEdge.insert(pair<Graph::VertexIterator, Graph::EdgeIterator>(m, MatchedEdge[*n]));
                        ForestRoot[m] = ForestRoot[v];

                        continue;
                    }

                    // Case 2: n is also an outer vertex and has a different root => augmenting path found
                    if(OuterVertices.find(*n) != OuterVertices.end()  &&  ForestRoot[*n] != ForestRoot[v])
                    {
cout << "   case 2\n"; cout.flush();
                        set<Graph::EdgeIterator> AugmentingPath;

                        for(Graph::VertexIterator i = v; i != ForestRoot[v]; i = ForestPredecessor[i])
                            AugmentingPath.insert(PredecessorEdge[i]);

                        AugmentingPath.insert( v.GetEdge(*n, 1,0,0,0,0,0,0,0,0) );

                        for(Graph::VertexIterator i = *n; i != ForestRoot[*n]; i = ForestPredecessor[i])
                            AugmentingPath.insert(PredecessorEdge[i]);

                        SetHelper::DestructiveSymmetricDifference(Matching, AugmentingPath);
                        MatchedVertices.insert(ForestRoot[v]);
                        MatchedVertices.insert(ForestRoot[*n]);

                        return true;
                    }

                    // Case 3: n is also an outer vertex and has the same root
                    //         => odd circle found, do blossom-step
                    if(OuterVertices.find(*n) != OuterVertices.end()  &&  ForestRoot[*n] == ForestRoot[v])
                    {
cout << "   case 3\n"; cout.flush();
                        // find least common ancestor of v and *n
                        Graph::VertexIterator LeastCommonPredecessor;
                        set<Graph::VertexIterator> vPredecessors;
                        for(Graph::VertexIterator i = v; i != G.EndVertices(); i = ForestPredecessor[i])
                            vPredecessors.insert(i);
                        for(Graph::VertexIterator i = *n; i != G.EndVertices(); i = ForestPredecessor[i])
                        {
                            if(vPredecessors.find(i) != vPredecessors.end())
                            {
                                LeastCommonPredecessor = i;
                                break;
                            }
                        }

                        // exchange edges on path from root to least common ancestor
                        // (size stays the same, but blossom step becomes applicable)
                        if(LeastCommonPredecessor != ForestRoot[v])
                        {
                            set<Graph::EdgeIterator> path;
                            for(Graph::VertexIterator i = LeastCommonPredecessor; i != G.EndVertices(); i = ForestPredecessor[i])
                                path.insert(PredecessorEdge[i]);
                            SetHelper::DestructiveSymmetricDifference(Matching, path);
                            MatchedVertices.insert(ForestRoot[v]);
                            MatchedVertices.erase(LeastCommonPredecessor);
                        }

                        // blossom step: contract the odd circle
                        set<Graph::VertexIterator> OddCircle;
                        OddCircle.insert(LeastCommonPredecessor);
                        for(Graph::VertexIterator i = v; i != LeastCommonPredecessor; i = ForestPredecessor[i])
                            OddCircle.insert(i);
                        for(Graph::VertexIterator i = *n; i != LeastCommonPredecessor; i = ForestPredecessor[i])
                            OddCircle.insert(i);
                        Graph G2 = G;
                        Graph::VertexIterator Fused = G2.Fuse(OddCircle); // contract

                        // Restart on G2
                        MatchedVertices.insert(Fused);
                        bool result = Augment(G2, Matching, MatchedVertices);
                        MatchedVertices.erase(Fused);
                        return result;
                    }

                } // loop over neighbors of OuterVertexQueue.front

            } // while(!OuterVertexQueue.empty())

            return false;
        }

        void AlgorithmMAXIMUMMATCHING::FindMaximumMatching(Graph& G, set<Graph::EdgeIterator>& Matching, set<Graph::VertexIterator>& MatchedVertices)
        {
            AlgorithmMAXIMALMATCHING::FindMaximalMatching(G, Matching, MatchedVertices);
            while(Augment(G,Matching,MatchedVertices))
            {
                // do nothing
            }
        }

/*

            // contract an odd circle
            list<Graph::VertexIterator> OddCircle;
            if(!AlgorithmODDCIRCLE::FindOddCircle(G, OddCircle))
            {
                FindMaximumMatchingBipartite(G, Matching, MatchedVertices);
                return;
            }
            set<Graph::VertexIterator> OddCircleAsSet;
            for(list<Graph::VertexIterator>::iterator i = OddCircle.begin(); i != OddCircle.end(); i++)
                OddCircleAsSet.insert(*i);
            Graph G2 = G;
            Graph::VertexIterator Fused = G2.Fuse(OddCircleAsSet);

            // compute matching on graph obtained by contracting the odd circle
            set<Graph::EdgeIterator> G2Matching;
            set<Graph::VertexIterator> G2MatchedVertices;
            FindMaximumMatching(G2, G2Matching, G2MatchedVertices);
            for(set<Graph::EdgeIterator>::iterator e = G2Matching.begin(); e != G2Matching.end(); e++)
                Matching.insert(G.GetEdge(e->GetID()));
            for(set<Graph::VertexIterator>::iterator v = G2MatchedVertices.begin(); v != G2MatchedVertices.end(); v++)
                if((*v) != Fused)
                    MatchedVertices.insert(G.GetVertex(v->GetID()));

            // find the vertex that is incident to the edge, which is matched to the "Fused" vertex in G2
            if(G2MatchedVertices.find(Fused) != G2MatchedVertices.end())
            {
                Graph::VertexIterator FusedOriginalInG = G.EndVertices();
                for(set<Graph::EdgeIterator>::iterator G2e = G2Matching.begin(); G2e != G2Matching.end(); G2e++)
                    if(G2e->Incident(Fused))
                    {
                        Graph::EdgeIterator Ge = G.GetEdge(G2e->GetID());
                        for(list<Graph::VertexIterator>::iterator i = OddCircle.begin(); i != OddCircle.end(); i++)
                            if(Ge.Incident(*i))
                            {
                                FusedOriginalInG = *i;
                                break;
                            }
                        break;
                    }

                // rotate the OddCircle list so that the previously selected vertex is at the beginning
                MatchedVertices.insert(FusedOriginalInG);
                while(*(OddCircle.begin()) != FusedOriginalInG)
                {
                    Graph::VertexIterator temp = *(OddCircle.begin());
                    OddCircle.pop_front();
                    OddCircle.push_back(temp);
                }

            }

            // include every other edge that is not incident to the previously selected vertex (blossom-step of edmonds algorithm)
            list<Graph::VertexIterator>::iterator v = OddCircle.begin();
            v++;
            while(v != OddCircle.end())
            {
                list<Graph::VertexIterator>::iterator v1 = v;
                v1++;
                Matching.insert(v->GetEdge(*v1,1,1,1,1,1,1,1,1,1));
                MatchedVertices.insert(*v);
                MatchedVertices.insert(*v1);
                v++;
                v++;
            }

            */


        void AlgorithmMAXIMUMMATCHING::FindMaximumMatching(Graph& G, set<Graph::EdgeIterator>& Matching)
        {
            set<Graph::VertexIterator> MatchedVertices;
            FindMaximumMatching(G, Matching, MatchedVertices);
        }

        void AlgorithmMAXIMUMMATCHING::AddMaximumMatching(Graph &G, string MatchingName)
        {
            set<Graph::EdgeIterator> Matching;
            FindMaximumMatching(G, Matching);
            G.AddEdgeSet(Matching, MatchingName);
        }


        void AlgorithmMAXIMUMMATCHING::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string MatchingName = parameters[0];
            AddMaximumMatching(G, MatchingName);
        }

    }
}

