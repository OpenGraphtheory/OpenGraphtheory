
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

        void AlgorithmMAXIMUMMATCHING::FindMaximumMatching(Graph& G, set<Graph::EdgeIterator>& Matching, set<Graph::VertexIterator>& MatchedVertices)
        {
            // Edmonds algorithm

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
        }


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

