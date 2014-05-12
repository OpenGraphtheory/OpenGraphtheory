
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


        void AlgorithmMAXIMALMATCHING::FindMaximalMatching(Graph& G, set<Graph::EdgeIterator>& Matching, set<Graph::VertexIterator>& MatchedVertices)
        {
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                set<Graph::VertexIterator> Incident = e.CollectIncidentVertices(1,1,1);
                if(SetHelper::IntersectionEmpty(MatchedVertices, Incident))
                {
                    Matching.insert(e);
                    SetHelper::DestructiveUnion(MatchedVertices, Incident);
                }
            }

        }


        void AlgorithmMAXIMALMATCHING::FindMaximalMatching(Graph& G, set<Graph::EdgeIterator>& Matching)
        {
            set<Graph::VertexIterator> MatchedVertices;
            FindMaximalMatching(G, Matching, MatchedVertices);
        }


        void AlgorithmMAXIMALMATCHING::AddMaximalMatching(Graph &G, string MatchingName)
        {
            set<Graph::EdgeIterator> Matching;
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

