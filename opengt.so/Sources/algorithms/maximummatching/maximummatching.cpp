
#include "../../../Headers/algorithms/maximummatching/maximummatching.h"

using namespace std;
using namespace OpenGraphtheory;


namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmMAXIMUMMATCHING::AlgorithmMaximumMatchingRegistrator(
            &Algorithm::AlgorithmFactory, "maximummatching", new DefaultInstantiator<Algorithm, AlgorithmMAXIMUMMATCHING>(
            "maximummatching", "Adds a maximum matching to the graph", "http://en.wikipedia.org/wiki/Matching_(graph_theory)"));


        bool AlgorithmMAXIMUMMATCHING::Augment(Graph& G, EdgeSet& Matching, VertexPartitioning& ContractionEquivalenceClass)
        {
            // Edmonds algorithm

            // Register for quick lookups
            map<int, int> MatchedVertex;
            map<int, Edge*> MatchingEdge;

            map<int, map<int, Edge*> > EdgesBetweenEquivalenceClasses;
            for(EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                Vertex* from = (*e)->From();
                Vertex* to = (*e)->To();
                int iFrom = ContractionEquivalenceClass[from];
                int iTo = ContractionEquivalenceClass[to];

                if(iFrom != iTo)
                {
                    EdgesBetweenEquivalenceClasses[iFrom].insert(pair<int, Edge*>(iTo, *e));
                    EdgesBetweenEquivalenceClasses[iTo].insert(pair<int, Edge*>(iFrom, *e));
                }
            }

            for(EdgeIterator e = Matching.begin(); e != Matching.end(); e++)
            {
                Vertex* from = (*e)->From();
                Vertex* to = (*e)->To();
                int iFrom = ContractionEquivalenceClass[from];
                int iTo = ContractionEquivalenceClass[to];

                if(iFrom != iTo)
                {
                    MatchedVertex[iFrom] = iTo;
                    MatchedVertex[iTo] = iFrom;
                    MatchingEdge.insert(pair<int, Edge*>(iFrom, *e));
                    MatchingEdge.insert(pair<int, Edge*>(iTo, *e));
                    EdgesBetweenEquivalenceClasses[iFrom][iTo] = *e;
                    EdgesBetweenEquivalenceClasses[iTo][iFrom] = *e;
                }
            }

            // initialize alternating forest
            set<int> OuterVertices;
            queue<int> OuterVertexQueue;
            map<int, int> ForestRoot;
            map<int, int> ForestPredecessor;
            map<int, Edge*> PredecessorEdge;

            for(map<int, map<int, Edge*> >::iterator i = EdgesBetweenEquivalenceClasses.begin(); i != EdgesBetweenEquivalenceClasses.end(); i++)
            {
                if(MatchedVertex.find(i->first) == MatchedVertex.end())
                {
                    OuterVertices.insert(i->first);
                    OuterVertexQueue.push(i->first);
                    ForestRoot[i->first] = i->first;
                }
            }

            // Construct alternating forest
            while(!OuterVertexQueue.empty())
            {
                int v = OuterVertexQueue.front();
                OuterVertexQueue.pop();

                set<int> vNeighbors;
                for(map<int, Edge*>::iterator vN = EdgesBetweenEquivalenceClasses[v].begin(); vN != EdgesBetweenEquivalenceClasses[v].end(); vN++)
                    vNeighbors.insert(vN->first);

                // iterate over neighbors n of v
                for(set<int>::iterator n = vNeighbors.begin(); n != vNeighbors.end(); n++)
                {
                    // Case 1: n is not in the forest => n is matched (otherwise it would be a root itself
                    //         and hence be in the forest). Add n and its match m to the forest.
                    if(ForestRoot.find(*n) == ForestRoot.end())
                    {
                        ForestPredecessor[*n] = v;
                        PredecessorEdge.insert(pair<int, Edge*>(*n, EdgesBetweenEquivalenceClasses[*n][v]));
                        ForestRoot[*n] = ForestRoot[v];

                        int m = MatchedVertex[*n];
                        OuterVertices.insert(m);
                        OuterVertexQueue.push(m);
                        ForestPredecessor[m] = *n;
                        ForestRoot[m] = ForestRoot[v];
                        PredecessorEdge.insert(pair<int, Edge*>(m, MatchingEdge[*n]));

                        continue;
                    }

                    // Case 2: n is also an outer vertex and has a different root => augmenting path found
                    if(OuterVertices.find(*n) != OuterVertices.end()  &&  ForestRoot[*n] != ForestRoot[v])
                    {
                        EdgeSet AugmentingPathEdges;
                        for(int i = v; i != ForestRoot[v]; i = ForestPredecessor[i])
                            AugmentingPathEdges.insert(PredecessorEdge[i]);
                        for(int i = *n; i != ForestRoot[*n]; i = ForestPredecessor[i])
                            AugmentingPathEdges.insert(PredecessorEdge[i]);
                        AugmentingPathEdges.insert(EdgesBetweenEquivalenceClasses[v][*n]);

                        SetHelper::DestructiveSymmetricDifference(Matching,AugmentingPathEdges);
                        return true;
                    }

                    // Case 3: n is also an outer vertex and has the same root
                    //         => odd circle found, do blossom-step
                    if(OuterVertices.find(*n) != OuterVertices.end()  &&  ForestRoot[*n] == ForestRoot[v])
                    {
                        // find least common predecessor of v and *n
                        int LeastCommonPredecessor = -1;
                        set<int> vPredecessors;
                        for(int i = v; i != ForestRoot[v]; i = ForestPredecessor[i])
                            vPredecessors.insert(i);
                        vPredecessors.insert(ForestRoot[v]);
                        for(int i = *n; /* i != ForestRoot[*n] */ ; i = ForestPredecessor[i])
                        {
                            if(vPredecessors.find(i) != vPredecessors.end())
                            {
                                LeastCommonPredecessor = i;
                                break;
                            }
                        }

                        // exchange edges on path from root to least common predecessor
                        // (size of matching stays the same, but blossom step becomes applicable)
                        EdgeSet NextMatching;
                        if(LeastCommonPredecessor != ForestRoot[v])
                        {
                            EdgeSet path;
                            for(int i = LeastCommonPredecessor; i != ForestRoot[v]; i = ForestPredecessor[i])
                                path.insert(PredecessorEdge[i]);

                            NextMatching = SetHelper::SymmetricDifference(Matching,path);
                        }
                        else
                        {
                            NextMatching = Matching;
                        }

                        // blossom step 1: determine the odd circle
                        list<int> OddCircle;
                        set<int> OddCircleVertices;
                        EdgeSet OddCircleEdges;
                        OddCircleVertices.insert(LeastCommonPredecessor);
                        for(int i = v; i != LeastCommonPredecessor; i = ForestPredecessor[i])
                        {
                            OddCircle.push_front(i);
                            OddCircleVertices.insert(i);
                            OddCircleEdges.insert(PredecessorEdge[i]);
                        }
                        OddCircle.push_front(LeastCommonPredecessor);
                        for(int i = *n; i != LeastCommonPredecessor; i = ForestPredecessor[i])
                        {
                            OddCircle.push_back(i);
                            OddCircleVertices.insert(i);
                            OddCircleEdges.insert(PredecessorEdge[i]);
                        }
                        OddCircleEdges.insert(EdgesBetweenEquivalenceClasses[v][*n]);

                        // blossom step 2: "contract" the odd circle (here, this is done by assigning the
                        // same number to all of them in ContractionEquivalenceClass)
                        VertexPartitioning NextContractionEquivalenceClasses;
                        for(VertexIterator vi = G.BeginVertices(); vi != G.EndVertices(); vi++)
                        {
                            if(OddCircleVertices.find(ContractionEquivalenceClass[*vi]) == OddCircleVertices.end())
                                NextContractionEquivalenceClasses[*vi] = ContractionEquivalenceClass[*vi];
                            else
                                NextContractionEquivalenceClasses[*vi] = LeastCommonPredecessor;
                        }
                        SetHelper::DestructiveSetMinus(NextMatching, OddCircleEdges);

                        // Recursion on contracted graph
                        bool Augmented = false;
                        while(Augment(G, NextMatching, NextContractionEquivalenceClasses))
                            Augmented = true;

                        if(Augmented)
                        {
                            Matching = NextMatching;

                            // find the vertex of the circle, that is incident with an edge
                            // of NextMatching (if any)
                            int CircleVertex = -1;
                            for(EdgeIterator m = Matching.begin(); m != Matching.end() && CircleVertex < 0; m++)
                            {
                                int iFrom = ContractionEquivalenceClass[(*m)->From()];
                                int iTo = ContractionEquivalenceClass[(*m)->To()];
                                if(OddCircleVertices.find(iFrom) != OddCircleVertices.end())
                                    CircleVertex = iFrom;
                                else if(OddCircleVertices.find(iTo) != OddCircleVertices.end())
                                    CircleVertex = iTo;
                            }

                            // rotate odd circle until CircleVertex is in front (if no such vertex
                            // exists, then it doesn't matter, which vertex is in front)
                            if(CircleVertex >= 0)
                                while(OddCircle.front() != CircleVertex)
                                {
                                    OddCircle.push_back(OddCircle.front());
                                    OddCircle.pop_front();
                                }
                            OddCircle.pop_front();

                            // add the edges on the odd circle to the Matching constructed from NextMatching
                            list<int>::iterator o = OddCircle.begin();
                            list<int>::iterator o1 = o;
                            o++;
                            while(true)
                            {
                                Matching.insert(
                                    EdgesBetweenEquivalenceClasses[*o][*o1]
                                );
                                o++;
                                if(o == OddCircle.end())
                                    break;
                                o1=o;
                                o++;
                            }

                            return true;
                        }
                        return false;
                    }

                } // loop over neighbors of OuterVertexQueue.front

            } // while(!OuterVertexQueue.empty())

            return false;
        }

        void AlgorithmMAXIMUMMATCHING::FindMaximumMatching(Graph& G, EdgeSet& Matching)
        {
            VertexSet MatchedVertices;
            AlgorithmMAXIMALMATCHING::FindMaximalMatching(G, Matching, MatchedVertices);

            VertexPartitioning ContractionEquivalenceClass;
            int i = 1;
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                ContractionEquivalenceClass[*v] = i++;

            while(Augment(G, Matching, ContractionEquivalenceClass))
            {
                // nothing to do here
            }
        }

        void AlgorithmMAXIMUMMATCHING::AddMaximumMatching(Graph &G, string MatchingName)
        {
            EdgeSet Matching;
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

