
#include "../../../Headers/algorithms/maximummatching/maximummatching.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
/*
        bool AlgorithmMAXIMUMMATCHING::FindAugmentingPath(Graph& G, VertexIterator Source,
                                set<EdgeIterator>& Matching,
                                set<VertexIterator>& MatchedVertices,
                                vector<EdgeIterator>& AugmentingPath)
        {
            set<VertexIterator> Even;
            set<VertexIterator> Odd;
            set<VertexIterator> LastEven;
            set<VertexIterator> LastOdd;
            set<VertexIterator> NextEven;
            set<VertexIterator> NextOdd;

            map<VertexIterator, VertexIterator> OPredecessor;
            map<VertexIterator, VertexIterator> EPredecessor;
            map<VertexIterator, EdgeIterator> EdgeToOPredecessor;
            map<VertexIterator, EdgeIterator> EdgeToEPredecessor;

            LastEven.insert(Source);
            Even.insert(Source);
            Odd.insert(Source); // to prevent the algorithm from computing an odd cycle

            while(LastEven.size() > 0 || LastOdd.size() > 0)
            {

                for(set<VertexIterator>::iterator ieven = LastEven.begin(); ieven != LastEven.end(); ieven++)
                {
                    VertexIterator even = *ieven;
                    set<EdgeIterator> inc = even.CollectIncidentEdges(1,1,1);
                    for(set<EdgeIterator>::iterator ei = inc.begin(); ei != inc.end(); ei++)
                        if(Matching.find(*ei) == Matching.end())
                        {
                            EdgeIterator e = *ei;
                            set<VertexIterator> neighbors = e.CollectIncidentVertices(1,1,1);
                            for(set<VertexIterator>::iterator neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
                                if(*neighbor != even
                                   && Odd.find(*neighbor) == Odd.end())
                                {
                                    NextOdd.insert(*neighbor);
                                    Odd.insert(*neighbor);
                                    OPredecessor[*neighbor] = even;
                                    EdgeToOPredecessor.insert(pair<VertexIterator, EdgeIterator>(*neighbor, e));
                                }
                        }
                }

                for(set<VertexIterator>::iterator iodd = LastOdd.begin(); iodd != LastOdd.end(); iodd++)
                {
                    VertexIterator odd = *iodd;
                    set<EdgeIterator> inc = odd.CollectIncidentEdges(1,1,1);
                    for(set<EdgeIterator>::iterator ei = inc.begin(); ei != inc.end(); ei++)
                        if(Matching.find(*ei) != Matching.end())
                        {
                            EdgeIterator e = *ei;
                            set<VertexIterator> neighbors = e.CollectIncidentVertices(1,1,1);
                            for(set<VertexIterator>::iterator neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
                                if(*neighbor != odd
                                   && Even.find(*neighbor) == Even.end())
                                {
                                    NextEven.insert(*neighbor);
                                    Even.insert(*neighbor);
                                    EPredecessor[*neighbor] = odd;
                                    EdgeToEPredecessor.insert(pair<VertexIterator, EdgeIterator>(*neighbor, e));
                                }
                        }
                }

                LastEven = NextEven;
                LastOdd = NextOdd;
                NextEven.clear();
                NextOdd.clear();

                if(!SetHelper::IsSubset(LastOdd, MatchedVertices)) // LastOdd contains an augmenting-path endpoint
                    break;
            }


            for(set<VertexIterator>::iterator vi = LastOdd.begin(); vi != LastOdd.end(); vi++)
            {
                if(// Even.find(*v) != Even.end() ||
                   MatchedVertices.find(*vi) != MatchedVertices.end())
                   continue;

                AugmentingPath.clear();
                VertexIterator v = *vi;
                while(true)
                {
                    AugmentingPath.insert(AugmentingPath.begin(), EdgeToOPredecessor[v]);
                    v = OPredecessor[v];

                    if(v == Source)
                        break;

                    AugmentingPath.insert(AugmentingPath.begin(), EdgeToEPredecessor[v]);
                    v = EPredecessor[v];
                }

                return true;
            }

            return false;
        }

        bool AlgorithmMAXIMUMMATCHING::FindAugmentingPath(Graph& G,
                                std::set<EdgeIterator>& Matching,
                                std::set<VertexIterator>& MatchedVertices,
                                std::vector<EdgeIterator>& AugmentingPath)
        {
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                if(MatchedVertices.find(v) == MatchedVertices.end())
                    if(FindAugmentingPath(G, v, Matching, MatchedVertices, AugmentingPath))
                        return true;
            return false;
        }

        void AlgorithmMAXIMUMMATCHING::FindMaximumMatchingBipartite(Graph& G, set<EdgeIterator>& Matching, set<VertexIterator>& MatchedVertices)
        {
            vector<EdgeIterator> AugmentingPath;
            AlgorithmMAXIMALMATCHING::FindMaximalMatching(G, Matching, MatchedVertices);

            while(true)
            {
                if(!FindAugmentingPath(G, Matching, MatchedVertices, AugmentingPath))
                    break;

                for(vector<EdgeIterator>::iterator i = AugmentingPath.begin();; i++)
                {
                    Matching.insert(*i);
                    EdgeIterator e = *i;
                    SetHelper::DestructiveUnion(MatchedVertices, e.CollectIncidentVertices(1,1,1));

                    i++;
                    if(i == AugmentingPath.end())
                        break;
                    Matching.erase(*i);
                }

                AugmentingPath.clear();
            }
        }
    */

    } // namespace Algorithms
} // namespace OpenGraphtheory

