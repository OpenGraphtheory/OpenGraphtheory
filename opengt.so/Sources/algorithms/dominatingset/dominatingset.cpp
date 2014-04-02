
#include "../../../Headers/algorithms/dominatingset/dominatingset.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        FactoryRegistrator<Algorithm> AlgorithmDOMINATINGSET::AlgorithmDominatingSetRegistrator(
            &Algorithm::AlgorithmFactory, "dominatingset", new DefaultInstantiator<Algorithm, AlgorithmDOMINATINGSET>(
            "dominatingset", "Adds a dominating set to the graph", "http://en.wikipedia.org/wiki/Dominating_set"));


        bool AlgorithmDOMINATINGSET::TestDominatingSet(Graph &G,
                                                       map<Graph::VertexIterator, int>& Dominators, set<Graph::VertexIterator>& Undominated,
                                                       set<Graph::VertexIterator>& Excluded, set<Graph::VertexIterator>& DominatingSet,
                                                       unsigned int k, bool VerticesCanDominateThemselves)
        {
            if(Undominated.size() <= 0)
                return true;
            if(k <= 0)
                return false;

            set<Graph::VertexIterator> LargeEnough;
            map<Graph::VertexIterator, set<Graph::VertexIterator> > Dominatable;


            // Test if the sum of the k largest vertices have < |Undominated| neighbors (if so, the instance is not solvable)
            int* NewDominationPotential = new int[G.NumberOfVertices()+1];
            for(int i = G.NumberOfVertices(); i >= 0; --i)
                NewDominationPotential[i] = 0;
            for(Graph::VertexIterator i = G.BeginVertices(); i != G.EndVertices(); i++)
            {
                if(Excluded.find(i) != Excluded.end())
                    continue;
                if(DominatingSet.find(i) != DominatingSet.end())
                    continue;

                // which vertices can be NEWLY dominated by vertex i?
                set<Graph::VertexIterator> Dominated = i.CollectNeighbors(1,1,0,1,1,0,0,0,0);
                if(VerticesCanDominateThemselves)
                    Dominated.insert(i);
                set<Graph::VertexIterator> removable; // these are already dominated
                for(set<Graph::VertexIterator>::iterator j = Dominated.begin(); j != Dominated.end(); j++)
                    if(Dominators[*j] > 0)
                        removable.insert(*j);
                for(set<Graph::VertexIterator>::iterator j = removable.begin(); j != removable.end(); j++)
                    Dominated.erase(*j);



                for(map<Graph::VertexIterator, set<Graph::VertexIterator> >::iterator sets = Dominatable.begin(); sets != Dominatable.end(); sets++)
                {
                    bool subset = true;
                    for(set<Graph::VertexIterator>::iterator j = Dominated.begin(); subset && j != Dominated.end(); j++)
                        if(sets->second.find(*j) == sets->second.end())
                            subset = false;

                    if(subset)
                    {
                        Excluded.insert(i);
                        bool temp = TestDominatingSet(G, Dominators, Undominated, Excluded, DominatingSet, k);
                        Excluded.erase(i);
                        delete[] NewDominationPotential;
                        return temp;
                    }

                    bool superset = true;

                    for(set<Graph::VertexIterator>::iterator j = sets->second.begin(); superset && j != sets->second.end(); j++)
                        if(Dominated.find(*j) == Dominated.end())
                            superset = false;

                    if(superset)
                    {
                        Excluded.insert(sets->first);
                        bool temp = TestDominatingSet(G, Dominators, Undominated, Excluded, DominatingSet, k);
                        Excluded.erase(sets->first);
                        delete[] NewDominationPotential;
                        return temp;
                    }
                }

                NewDominationPotential[Dominated.size()]++;
                if(Dominated.size() * k >= Undominated.size())
                    LargeEnough.insert(i);
                Dominatable[i] = Dominated;

            }
            unsigned int MaxNewDomination = 0;
            int k_left = k;
            for(int d = G.NumberOfVertices(); d >= 0; --d)
            {
                if(NewDominationPotential[d] >= k_left)
                {
                    MaxNewDomination += k_left * d;
                    break;
                }
                else
                {
                    MaxNewDomination += NewDominationPotential[d] * d;
                    k_left -= NewDominationPotential[d];
                    if(MaxNewDomination > Undominated.size())
                        break;
                }
            }
            delete[] NewDominationPotential;
            if(MaxNewDomination < Undominated.size())
                return false;


            Graph::VertexIterator SelectedToBeDominated;
            unsigned int MinCandidateCount = G.NumberOfVertices() + 2;
            for(set<Graph::VertexIterator>::iterator vi = Undominated.begin(); vi != Undominated.end(); vi++)
            {
                Graph::VertexIterator vCandidate = *vi;
                set<Graph::VertexIterator> CandidateDominators = vCandidate.CollectNeighbors(1,0,1,0,0,0,1,0,1);
                if(VerticesCanDominateThemselves)
                    CandidateDominators.insert(vCandidate);
                for(set<Graph::VertexIterator>::iterator excluded = Excluded.begin(); excluded != Excluded.end(); excluded++)
                    CandidateDominators.erase(*excluded);
                if(CandidateDominators.size() < MinCandidateCount)
                {
                    SelectedToBeDominated = vCandidate;
                    MinCandidateCount = CandidateDominators.size();
                }
            }

            set<Graph::VertexIterator> NextExcluded;


            set<Graph::VertexIterator> Candidates = SelectedToBeDominated.CollectNeighbors(1,0,1,0,0,0,1,0,1);
            if(VerticesCanDominateThemselves)
                Candidates.insert(SelectedToBeDominated);
            for(set<Graph::VertexIterator>::iterator excluded = Excluded.begin(); excluded != Excluded.end(); excluded++)
                Candidates.erase(*excluded);


            if(LargeEnough.size() < Candidates.size())
                Candidates = LargeEnough;


            for(set<Graph::VertexIterator>::iterator candidateiterator = Candidates.begin(); candidateiterator != Candidates.end(); candidateiterator++)
            {
                Graph::VertexIterator candidate = *candidateiterator;
                set<Graph::VertexIterator> Dominated = candidate.CollectNeighbors(1,1,0,1,1,0,0,0,0);
                if(VerticesCanDominateThemselves)
                    Dominated.insert(candidate);
                for(set<Graph::VertexIterator>::iterator n = Dominated.begin(); n != Dominated.end(); n++)
                    if(++Dominators[*n] == 1) Undominated.erase(*n);

                // RECURSIVE CALL
                DominatingSet.insert(candidate);
                if(TestDominatingSet(G, Dominators, Undominated, Excluded, DominatingSet, k-1))
                    return true;
                DominatingSet.erase(candidate);
                if(Excluded.find(candidate) == Excluded.end())
                {
                    Excluded.insert(candidate);
                    NextExcluded.insert(candidate);
                }

                for(set<Graph::VertexIterator>::iterator n = Dominated.begin(); n != Dominated.end(); n++)
                    if(--Dominators[*n] == 0) Undominated.insert(*n);
            }

            for(set<Graph::VertexIterator>::iterator nextexcluded = NextExcluded.begin(); nextexcluded != NextExcluded.end(); nextexcluded++)
                Excluded.erase(*nextexcluded);
            return false;
        }


        bool AlgorithmDOMINATINGSET::FindDominatingSet(Graph& G, set<Graph::VertexIterator>& DominatingSet, unsigned int k, bool VerticesCanDominateThemselves)
        {
            map<Graph::VertexIterator, int> Dominators;
            set<Graph::VertexIterator> Undominated;
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                Dominators[v] = 0;
                Undominated.insert(v);
            }

            set<Graph::VertexIterator> Excluded;

            return TestDominatingSet(G, Dominators, Undominated, Excluded, DominatingSet, k, VerticesCanDominateThemselves);
        }


        void AlgorithmDOMINATINGSET::AddDominatingSet(Graph &G, string DominatingSetName, bool VerticesCanDominateThemselves)
        {
            set<Graph::VertexIterator> DominatingSet;

            if(G.NumberOfVertices() <= 0)
                return;

            for(int k = 1; k<=G.NumberOfVertices(); k++)
            {
                if(FindDominatingSet(G, DominatingSet, k))
                    break;
                DominatingSet.clear();
            }

            G.AddVertexSet(DominatingSet, DominatingSetName);
        }

        void AlgorithmDOMINATINGSET::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string DominatingSetName =  parameters[0];
            AddDominatingSet(G, DominatingSetName);
        }

    }
}

