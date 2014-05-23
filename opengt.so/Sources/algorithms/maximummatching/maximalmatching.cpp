
#include "../../../Headers/algorithms/maximummatching/maximalmatching.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmMAXIMALMATCHING::AlgorithmMaximalMatchingRegistrator(
            &Algorithm::AlgorithmFactory, "maximalmatching", new DefaultInstantiator<Algorithm, AlgorithmMAXIMALMATCHING>(
            "maximalmatching", "Adds a maximal matching (not necessarily a maximUM matching) to the graph", "http://en.wikipedia.org/wiki/Matching_(graph_theory)"));


        void AlgorithmMAXIMALMATCHING::FindMaximalMatching(Graph& G, EdgeSet& Matching, VertexSet& MatchedVertices)
        {
            for(EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                VertexSet Incident = (*e)->CollectIncidentVertices(1,1,1);
                if(MatchedVertices.intersection(Incident).empty())
                {
                    Matching.insert(*e);
                    MatchedVertices += Incident;
                }
            }

        }


        void AlgorithmMAXIMALMATCHING::FindMaximalMatching(Graph& G, EdgeSet& Matching)
        {
            VertexSet MatchedVertices;
            FindMaximalMatching(G, Matching, MatchedVertices);
        }


        void AlgorithmMAXIMALMATCHING::AddMaximalMatching(Graph &G, string MatchingName)
        {
            EdgeSet Matching;
            FindMaximalMatching(G, Matching);
            G.AddEdgeSet(Matching, MatchingName);
        }


        void AlgorithmMAXIMALMATCHING::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string MatchingName = parameters[0];
            AddMaximalMatching(G, MatchingName);
        }

    }
}

