
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

        void AlgorithmMAXIMUMMATCHING::FindMaximumMatching(Graph& G, set<Graph::EdgeIterator>& Matching)
        {
            AlgorithmMAXIMALMATCHING::FindMaximalMatching(G, Matching); // for now

            /*
            list<Graph::VertexIterator> OddCircle;
            if(!AlgorithmODDCIRCLE::FindOddCircle(G, OddCircle))
            {
                FindMaximumMatchingBipartite(G, Matching);
                return;
            }

            set<Graph::VertexIterator> OddCircleAsSet;
            for(list<Graph::VertexIterator>::iterator i = OddCircle.begin(); i != OddCircle.end(); i++)
                OddCircleAsSet.insert(*i);

            //Graph G2 = G.Fuse(OddCircleAsSet);
            //set<Graph::EdgeIterator> G2Matching;
            //FindMaximumMatching(G2, G2Matching);
            */

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

