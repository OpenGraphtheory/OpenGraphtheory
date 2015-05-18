
#include "../../../Headers/algorithms/vertexdisjointpaths/vertexdisjointpaths.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Algorithms;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        MultiFactoryRegistrator<Algorithm> AlgorithmVERTEXDISJOINTPATHS::AlgorithmVertexDisjointPathsRegistrator(
            &Algorithm::AlgorithmFactory, "vertexdisjointpaths", new DefaultInstantiator<Algorithm, AlgorithmVERTEXDISJOINTPATHS>(
            "vertexdisjointpaths", "adds a maximum set of vertex-disjoint paths to the graph", ""));


        /*
          every vertex is (theoretically) split into two vertices, the head and the tail
        */
        bool AlgorithmVERTEXDISJOINTPATHS::FindAugmentingPathOrSeparator(Graph &G, VertexSet& Sources, VertexSet& Drains, EdgeSet& EFlow,
                                                                                   VertexSet& VFlow, vector<Edge*>& AugmentingPath,
                                                                                   VertexSet& Separator,
                                                                                   VertexFilter* vertexfilter, EdgeFilter* edgefilter)
        {
            map<Vertex*, Vertex*> PredecessorOfHead;
            map<Vertex*, Vertex*> PredecessorOfTail;
            map<Vertex*, Edge*> EdgeToPredecessorOfHead;
            map<Vertex*, Edge*> EdgeToPredecessorOfTail;
            VertexSet LastRoundTails, LastRoundHeads;
            VertexSet NextRoundTails, NextRoundHeads;
            for(VertexIterator Source = Sources.begin(); Source != Sources.end(); Source++)
            {
                if(vertexfilter != NULL && !vertexfilter->VertexAllowed(*Source))
                    continue;
                LastRoundHeads.insert(*Source);
                PredecessorOfHead[*Source] = NULL; // prevents algoritm from adding paths to tail of source (which doesnt exist logically)
                PredecessorOfTail[*Source] = NULL;
            }

            VertexIterator FoundDrain = Drains.end();
            while(FoundDrain==Drains.end() && !(LastRoundHeads.empty() && LastRoundTails.empty()))
            {
                // Process Heads
                for(VertexIterator v = LastRoundHeads.begin(); v != LastRoundHeads.end(); v++)
                {
                    // forward edges
                    EdgeSet posincident = (*v)->CollectIncidentEdges(0,1,0);
                    for(EdgeIterator e = posincident.begin(); e != posincident.end(); e++)
                    {
                        if(edgefilter != NULL && !edgefilter->EdgeAllowed(*e))
                            continue;

                        // the first condition is turned off, because the actual edges have infinite capacity
                        if( /* EFlow.find(*e) == EFlow.end() */
                        /* && */ PredecessorOfTail.find((*e)->To()) == PredecessorOfTail.end()
                        && (vertexfilter == NULL || vertexfilter->VertexAllowed((*e)->To())))
                        {
                            NextRoundTails.insert((*e)->To());
                            PredecessorOfTail[(*e)->To()] = *v;
                            EdgeToPredecessorOfTail[(*e)->To()] = *e;
                        }
                    }

                    // the internal backward edge
                    if(VFlow.find(*v) != VFlow.end()
                    && PredecessorOfTail.find(*v) == PredecessorOfTail.end())
                    {
                        NextRoundTails.insert(*v);
                        PredecessorOfTail[*v] = *v;
                    }
                }

                // Process Tails
                for(VertexIterator v = LastRoundTails.begin(); v != LastRoundTails.end(); v++)
                {
                    // the internal forward edge
                    if(VFlow.find(*v) == VFlow.end()
                    && PredecessorOfHead.find(*v) == PredecessorOfHead.end())
                    {
                        NextRoundHeads.insert(*v);
                        PredecessorOfHead[*v] = *v;
                    }

                    // backward edges
                    EdgeSet negincident = (*v)->CollectIncidentEdges(0,0,1);
                    for(EdgeIterator e = negincident.begin(); e != negincident.end(); e++)
                    {
                        if(edgefilter != NULL && !edgefilter->EdgeAllowed(*e))
                            continue;

                        if(EFlow.find(*e) != EFlow.end() // flow = 0 or 1, so this test is sufficient
                           && PredecessorOfHead.find((*e)->From()) == PredecessorOfHead.end()
                           && (vertexfilter == NULL || vertexfilter->VertexAllowed((*e)->From())))
                        {
                            NextRoundHeads.insert((*e)->From());
                            PredecessorOfHead[(*e)->From()] = *v;
                            EdgeToPredecessorOfHead[(*e)->From()] = *e;
                        }
                    }
                }

                LastRoundHeads = NextRoundHeads;
                LastRoundTails = NextRoundTails;
                NextRoundHeads.clear();
                NextRoundTails.clear();

                for(VertexIterator Drain = Drains.begin(); FoundDrain == Drains.end() && Drain != Drains.end(); Drain++)
                    if(LastRoundTails.find(*Drain) != LastRoundTails.end())
                        // forbidden vertices are not in LastRoundTails anyways, so vertexfilter need not be applied
                        FoundDrain = Drain;
            }


            if(FoundDrain == Drains.end())
            {
                // no augmenting path found => construct the cut
                Separator.clear();
                // the separator consists of the vertices, whose tails are reachable, but their heads are not
                for(map<Vertex*, Vertex*>::iterator i = PredecessorOfTail.begin(); i != PredecessorOfTail.end(); i++)
                    if(PredecessorOfHead.find( i->first ) == PredecessorOfHead.end())
                        Separator.insert(i->first);

                return false;
            }


            // construct the augmenting path from the collected data
            AugmentingPath.clear();
            Vertex* v = *FoundDrain;
            bool vIsTail= true; // Drain is a tail
            while(Sources.find(v) == Sources.end())
            {
                if(vIsTail)
                {
                    // otherwise, we only move across the internal edge of the vertex by setting vIsTail to false
                    if(PredecessorOfTail[v] != v)
                    {
                        AugmentingPath.insert(AugmentingPath.begin(), EdgeToPredecessorOfTail[v]);
                        v = PredecessorOfTail[v];
                    }
                    vIsTail = false;
                }
                else // vIsHead
                {
                    // otherwise, we only move across the internal edge of the vertex by setting vIsTail to true
                    if(PredecessorOfHead[v] != v)
                    {
                        AugmentingPath.insert(AugmentingPath.begin(), EdgeToPredecessorOfHead[v]);
                        v = PredecessorOfHead[v];
                    }
                    vIsTail = true;
                }
            }
            return true;
        }



        bool AlgorithmVERTEXDISJOINTPATHS::FindDisjointPathsAndSeparator(Graph& G, VertexSet& Sources, VertexSet& Drains,
                                                                         EdgeSet& DisjointPaths, VertexSet& Separator,
                                                                         VertexFilter* vertexfilter, EdgeFilter* edgefilter)
        {
            // if a source is also a drain, then it cannot be separated (and the flow is infinite)
            for(VertexIterator Source = Sources.begin(); Source != Sources.end(); Source++)
                if(Drains.find(*Source) != Drains.end())
                    return false;

            vector<Edge*> AugmentingPath;
            VertexSet VFlow;

            while(FindAugmentingPathOrSeparator(G, Sources, Drains, DisjointPaths, VFlow, AugmentingPath, Separator, vertexfilter, edgefilter))
            {
                // augment the flow
                Vertex* v = (*AugmentingPath.begin())->From();
                bool LastWasForwardArc = false; // false to prevent loop from inserting <Source> into VFlow
                for(vector<Edge*>::iterator i = AugmentingPath.begin(); i != AugmentingPath.end(); i++)
                {
                    if(v == (*i)->From()) // Forward-Arc
                    {
                        // two forward-arcs in a row means, the (internal edge of the) common vertex becomes saturated
                        if(LastWasForwardArc)
                          VFlow.insert(v);
                        DisjointPaths.insert(*i);
                        v = (*i)->To();
                        LastWasForwardArc = true;
                    }
                    else // Backward-Arc
                    {
                        // two backward-arcs in a row means, the (internal edge of the) common vertex becomes desaturated
                        if(!LastWasForwardArc)
                          VFlow.erase(v);
                        DisjointPaths.erase(*i);
                        v = (*i)->From();
                        LastWasForwardArc = false;
                    }
                }
            }

            return true;
        }



        bool AlgorithmVERTEXDISJOINTPATHS::FindDisjointPaths(Graph& G, VertexSet& Sources, VertexSet& Drains, EdgeSet& DisjointPaths,
                                                                       VertexFilter* vertexfilter, EdgeFilter* edgefilter)
        {
            VertexSet Separator;
            return FindDisjointPathsAndSeparator(G, Sources, Drains, DisjointPaths, Separator, vertexfilter, edgefilter);
        }

        bool AlgorithmVERTEXDISJOINTPATHS::FindDisjointPaths(Graph& G, Vertex* Source, Vertex* Drain, EdgeSet& DisjointPaths,
                                                                       VertexFilter* vertexfilter, EdgeFilter* edgefilter)
        {
            VertexSet Sources;
            Sources.insert(Source);
            VertexSet Drains;
            Drains.insert(Drain);
            return FindDisjointPaths(G, Sources, Drains, DisjointPaths, vertexfilter, edgefilter);
        }


        void AlgorithmVERTEXDISJOINTPATHS::AddVertexDisjointPaths(Graph &G, VertexSet& Sources, VertexSet& Drains, string DisjointPathsName)
        {
            EdgeSet DisjointPaths;
            if(FindDisjointPaths(G, Sources, Drains, DisjointPaths))
            {
                G.AddEdgeSet(DisjointPaths, DisjointPathsName);
            }
        }

        void AlgorithmVERTEXDISJOINTPATHS::AddVertexDisjointPaths(Graph &G, Vertex* Source, Vertex* Drain, string DisjointPathsName)
        {
            VertexSet Sources;
            Sources.insert(Source);
            VertexSet Drains;
            Drains.insert(Drain);
            AddVertexDisjointPaths(G,Sources,Drains,DisjointPathsName);
        }



        void AlgorithmVERTEXDISJOINTPATHS::Run(Graph& G, std::vector<std::string> parameters)
        {
            if(parameters.size() < 3)
                throw "vertex disjoint paths algorithm needs 3 parameters (source, drain, result name)";

            string SourceName = parameters[0];
            string DrainName = parameters[1];
            string MaximumFlowName = parameters[2];

            VertexIterator Source = G.EndVertices();
            VertexIterator Drain = G.EndVertices();
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                if(SourceName == (*v)->GetLabel())
                    Source = v;
                if(DrainName == (*v)->GetLabel())
                    Drain = v;
            }

            if(Source == G.EndVertices())
                throw "vertex disjoint paths algorithm: source not found";
            if(Drain == G.EndVertices())
                throw "vertex disjoint paths algorithm: drain not found";

            AddVertexDisjointPaths(G, *Source, *Drain, MaximumFlowName);
        }

    }
}
