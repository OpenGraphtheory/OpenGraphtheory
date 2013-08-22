
#include "../../../Headers/algorithms/maximummatching/maximummatching.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        bool AlgorithmMAXIMUMMATCHING::FindAugmentingPath(Graph& G, Graph::VertexIterator Source,
                                set<Graph::EdgeIterator>& Matching,
                                set<Graph::VertexIterator>& MatchedVertices,
                                vector<Graph::EdgeIterator>& AugmentingPath)
        {
            set<Graph::VertexIterator> Even;
            set<Graph::VertexIterator> Odd;
            set<Graph::VertexIterator> LastEven;
            set<Graph::VertexIterator> LastOdd;
            set<Graph::VertexIterator> NextEven;
            set<Graph::VertexIterator> NextOdd;

            map<Graph::VertexIterator, Graph::VertexIterator> OPredecessor;
            map<Graph::VertexIterator, Graph::VertexIterator> EPredecessor;
            map<Graph::VertexIterator, Graph::EdgeIterator> EdgeToOPredecessor;
            map<Graph::VertexIterator, Graph::EdgeIterator> EdgeToEPredecessor;

            LastEven.insert(Source);
            Even.insert(Source);
            Odd.insert(Source); // to prevent the algorithm from computing an odd cycle

            while(LastEven.size() > 0 || LastOdd.size() > 0)
            {

                for(set<Graph::VertexIterator>::iterator ieven = LastEven.begin(); ieven != LastEven.end(); ieven++)
                {
                    Graph::VertexIterator even = *ieven;
                    set<Graph::EdgeIterator> inc = even.CollectIncidentEdges(1,1,1);
                    for(set<Graph::EdgeIterator>::iterator ei = inc.begin(); ei != inc.end(); ei++)
                        if(Matching.find(*ei) == Matching.end())
                        {
                            Graph::EdgeIterator e = *ei;
                            set<Graph::VertexIterator> neighbors = e.CollectIncidentVertices(1,1,1);
                            for(set<Graph::VertexIterator>::iterator neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
                                if(*neighbor != even
                                   && Odd.find(*neighbor) == Odd.end())
                                {
                                    NextOdd.insert(*neighbor);
                                    Odd.insert(*neighbor);
                                    OPredecessor[*neighbor] = even;
                                    EdgeToOPredecessor.insert(pair<Graph::VertexIterator, Graph::EdgeIterator>(*neighbor, e));
                                }
                        }
                }

                for(set<Graph::VertexIterator>::iterator iodd = LastOdd.begin(); iodd != LastOdd.end(); iodd++)
                {
                    Graph::VertexIterator odd = *iodd;
                    set<Graph::EdgeIterator> inc = odd.CollectIncidentEdges(1,1,1);
                    for(set<Graph::EdgeIterator>::iterator ei = inc.begin(); ei != inc.end(); ei++)
                        if(Matching.find(*ei) != Matching.end())
                        {
                            Graph::EdgeIterator e = *ei;
                            set<Graph::VertexIterator> neighbors = e.CollectIncidentVertices(1,1,1);
                            for(set<Graph::VertexIterator>::iterator neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
                                if(*neighbor != odd
                                   && Even.find(*neighbor) == Even.end())
                                {
                                    NextEven.insert(*neighbor);
                                    Even.insert(*neighbor);
                                    EPredecessor[*neighbor] = odd;
                                    EdgeToEPredecessor.insert(pair<Graph::VertexIterator, Graph::EdgeIterator>(*neighbor, e));
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


            for(set<Graph::VertexIterator>::iterator vi = LastOdd.begin(); vi != LastOdd.end(); vi++)
            {
                if(// Even.find(*v) != Even.end() ||
                   MatchedVertices.find(*vi) != MatchedVertices.end())
                   continue;

                AugmentingPath.clear();
                Graph::VertexIterator v = *vi;
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
                                std::set<Graph::EdgeIterator>& Matching,
                                std::set<Graph::VertexIterator>& MatchedVertices,
                                std::vector<Graph::EdgeIterator>& AugmentingPath)
        {
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                if(MatchedVertices.find(v) == MatchedVertices.end())
                    if(FindAugmentingPath(G, v, Matching, MatchedVertices, AugmentingPath))
                        return true;
            return false;
        }

        void AlgorithmMAXIMUMMATCHING::FindMaximumMatchingBipartite(Graph& G, set<Graph::EdgeIterator>& Matching, set<Graph::VertexIterator>& MatchedVertices)
        {
            vector<Graph::EdgeIterator> AugmentingPath;
            AlgorithmMAXIMALMATCHING::FindMaximalMatching(G, Matching, MatchedVertices);

            while(true)
            {
                if(!FindAugmentingPath(G, Matching, MatchedVertices, AugmentingPath))
                    break;

                for(vector<Graph::EdgeIterator>::iterator i = AugmentingPath.begin();; i++)
                {
                    Matching.insert(*i);
                    Graph::EdgeIterator e = *i;
                    SetHelper::DestructiveUnion(MatchedVertices, e.CollectIncidentVertices(1,1,1));

                    i++;
                    if(i == AugmentingPath.end())
                        break;
                    Matching.erase(*i);
                }

                AugmentingPath.clear();
            }
        }

    }
}

