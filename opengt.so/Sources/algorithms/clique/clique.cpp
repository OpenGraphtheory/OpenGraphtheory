
#include "../../../Headers/algorithms/clique/clique.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        FactoryRegistrator<Algorithm> AlgorithmCLIQUE::AlgorithmCliqueRegistrator(
            &Algorithm::AlgorithmFactory, "clique", new DefaultInstantiator<Algorithm, AlgorithmCLIQUE>(
            "clique", "Adds a clique to the graph", "http://en.wikipedia.org/wiki/Clique_(graph_theory)"));


        bool AlgorithmCLIQUE::TestClique(set<Graph::VertexIterator>& Clique, set<Graph::VertexIterator>& Candidates, unsigned int k, bool CliqueOrIndependentSet)
        {
            if(k == 0)
                return true;
            if(k > Candidates.size())
                return false;

            for(set<Graph::VertexIterator>::iterator v = Candidates.begin(); v != Candidates.end(); v++)
            {
                set<Graph::VertexIterator> NextCandidates;
                set<Graph::VertexIterator>::iterator n = v;
                for(n++; n != Candidates.end(); n++)
                    if(v->UnderlyingAdjacent(*n) == CliqueOrIndependentSet)
                        NextCandidates.insert(*n);

                // Remove Candidates that dont have enough Neighbors in the Candidates-Set
                set<Graph::VertexIterator> RemovableCandidates;
                bool CandidateRemoved;
                do
                {
                    CandidateRemoved = false;
                    for(set<Graph::VertexIterator>::iterator i = NextCandidates.begin(); i != NextCandidates.end(); i++)
                    {
                        if(RemovableCandidates.find(*i) != RemovableCandidates.end())
                            continue;

                        set<Graph::VertexIterator> iNeighborCandidates;
                        for(set<Graph::VertexIterator>::iterator j = NextCandidates.begin(); j != NextCandidates.end(); j++)
                            if((i->UnderlyingAdjacent(*j) == CliqueOrIndependentSet) && (RemovableCandidates.find(*j) == RemovableCandidates.end()))
                                iNeighborCandidates.insert(*j);
                        iNeighborCandidates.erase(*i);

                        if(iNeighborCandidates.size() < k-2)
                        {
                            RemovableCandidates.insert(*i);
                            CandidateRemoved = true;
                        }
                    }

                } while(CandidateRemoved);

                for(set<Graph::VertexIterator>::iterator i = RemovableCandidates.begin(); i != RemovableCandidates.end(); i++)
                    NextCandidates.erase(*i);


                Clique.insert(*v);
                if(TestClique(Clique, NextCandidates, k-1, CliqueOrIndependentSet))
                    return true;
                Clique.erase(*v);

            }

            return false;
        }


        bool AlgorithmCLIQUE::FindClique(Graph& G, set<Graph::VertexIterator>& Clique, unsigned int k)
        {
            set<Graph::VertexIterator> V;
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                V.insert(v);

            return TestClique(Clique, V, k, true);
        }


        void AlgorithmCLIQUE::AddClique(Graph &G, string CliqueName)
        {
            set<Graph::VertexIterator> LastClique;
            set<Graph::VertexIterator> CurrentClique;

            if(G.NumberOfVertices() <= 0)
                return;
            CurrentClique.insert(G.BeginVertices());

            for(unsigned int k = 2; ; k++)
            {
                if(!FindClique(G, CurrentClique, k))
                    break;

                LastClique = CurrentClique;
                CurrentClique.clear();
            }

            G.AddVertexSet(LastClique, CliqueName);
        }

        void AlgorithmCLIQUE::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string CliqueName =  parameters[0];
            AddClique(G, CliqueName);
        }

    }
}

