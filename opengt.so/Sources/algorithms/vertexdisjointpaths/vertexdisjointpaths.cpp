
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
        bool AlgorithmVERTEXDISJOINTPATHS::FindVertexFlowAugmentingPath(Graph &G, Vertex* Source, Vertex* Drain, EdgeSet& EFlow,
                                                                              VertexSet& VFlow, vector<Edge*>& AugmentingPath)
        {
            map<Vertex*, Vertex*> PredecessorOfHead;
            map<Vertex*, Vertex*> PredecessorOfTail;
            map<Vertex*, Edge*> EdgeToPredecessorOfHead;
            map<Vertex*, Edge*> EdgeToPredecessorOfTail;
            VertexSet LastRoundTails, LastRoundHeads;
            VertexSet NextRoundTails, NextRoundHeads;
            LastRoundHeads.insert(Source);

            while(LastRoundTails.find(Drain) == LastRoundTails.end() && !(LastRoundHeads.empty() && LastRoundTails.empty()))
            {
                // Heads
                for(VertexIterator v = LastRoundHeads.begin(); v != LastRoundHeads.end(); v++)
                {
                    // forward edges
                    EdgeSet posincident = (*v)->CollectIncidentEdges(0,1,0);
                    for(EdgeIterator e = posincident.begin(); e != posincident.end(); e++)
                    {
                        if(EFlow.find(*e) == EFlow.end()
                        && PredecessorOfTail.find((*e)->To()) == PredecessorOfTail.end())
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

                // Tails
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
                        if(EFlow.find(*e) != EFlow.end()
                           && PredecessorOfHead.find((*e)->From()) == PredecessorOfHead.end())
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
            }

            if(LastRoundTails.find(Drain) == LastRoundTails.end()) // no augmenting path found
                return false;

            // construct the result from the collected data
            AugmentingPath.clear();
            Vertex* v = Drain;
            bool vIsTail= true; // Drain is a tail
            while(v != Source)
            {
                if(vIsTail)
                {
                    // otherwise, we move across the internal edge of the vertex
                    if(PredecessorOfTail[v] != v)
                    {
                        AugmentingPath.insert(AugmentingPath.begin(), EdgeToPredecessorOfTail[v]);
                        v = PredecessorOfTail[v];
                    }
                    vIsTail = false;
                }
                else // vIsHead
                {
                    // otherwise, we move across the internal edge of the vertex
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



        bool AlgorithmVERTEXDISJOINTPATHS::FindVertexDisjointPaths(Graph& G, Vertex* Source, Vertex* Drain, EdgeSet& EFlow)
        {
            vector<Edge*> AugmentingPath;
            VertexSet VFlow;

            while(FindVertexFlowAugmentingPath(G, Source, Drain, EFlow, VFlow, AugmentingPath))
            {
                // augment the flow
                Vertex* v = Source;
                bool LastWasForwardArc = false; // false to prevent loop from inserting <Source> into VFlow
                for(vector<Edge*>::iterator i = AugmentingPath.begin(); i != AugmentingPath.end(); i++)
                {
                    if(v == (*i)->From()) // Forward-Arc
                    {
                        // two forward-arcs in a row means, the (internal edge of the) common vertex becomes saturated
                        if(LastWasForwardArc)
                          VFlow.insert(v);
                        EFlow.insert(*i);
                        v = (*i)->To();
                        LastWasForwardArc = true;
                    }
                    else // Backward-Arc
                    {
                        // two backward-arcs in a row means, the (internal edge of the) common vertex becomes desaturated
                        if(!LastWasForwardArc)
                          VFlow.erase(v);
                        EFlow.erase(*i);
                        v = (*i)->From();
                        LastWasForwardArc = false;
                    }
                }
            }

            return true;
        }

        void AlgorithmVERTEXDISJOINTPATHS::AddVertexDisjointPaths(Graph &G, Vertex* Source, Vertex* Drain, string FlowName)
        {
            EdgeSet EFlow;
            if(FindVertexDisjointPaths(G, Source, Drain, EFlow))
            {
                G.AddEdgeSet(EFlow, FlowName);
            }
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
