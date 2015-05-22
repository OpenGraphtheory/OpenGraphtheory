
#include "../../../Headers/algorithms/minimumcut/minimumcut.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmMINIMUMCUT::AlgorithmMinimumCutRegistrator(
            &Algorithm::AlgorithmFactory, "minimumcut", new DefaultInstantiator<Algorithm, AlgorithmMINIMUMCUT>(
            "minimumcut", "Adds a minimum cut to the graph", "http://en.wikipedia.org/wiki/Minimum_cut"));


        void AlgorithmMINIMUMCUT::FindMinimumCut(Graph& G, Vertex* Source, Vertex* Drain,
                                                 EdgeWeighting& Capacities, EdgeSet& MinimumCut)
        {
            EdgeWeighting MaximumFlow;
            AlgorithmMAXIMUMFLOW::FindMaximumFlow(G, Source, Drain, Capacities, MaximumFlow);

            VertexSet SourceComponent;
            SourceComponent.insert(Source);
            VertexSet LastRound;
            LastRound.insert(Source);
            VertexSet NextRound;

            while(!LastRound.empty())
            {
                for(VertexIterator v = LastRound.begin(); v != LastRound.end(); v++)
                {
                    EdgeSet posincident = (*v)->CollectIncidentEdges(0,1,0);
                    for(EdgeIterator e = posincident.begin(); e != posincident.end(); e++)
                    {
                        if(Capacities[*e] > MaximumFlow[*e]
                           && SourceComponent.find((*e)->To()) == SourceComponent.end())
                        {
                            NextRound.insert((*e)->To());
                            SourceComponent.insert((*e)->To());
                        }
                    }

                    EdgeSet negincident = (*v)->CollectIncidentEdges(0,0,1);
                    for(EdgeIterator e = negincident.begin(); e != negincident.end(); e++)
                    {
                        if(MaximumFlow[*e] > 0
                           && SourceComponent.find((*e)->From()) == SourceComponent.end())
                        {
                            NextRound.insert((*e)->From());
                            SourceComponent.insert((*e)->From());
                        }
                    }

                    /*
                    set<EdgeIterator> undirincident = v.CollectIncidentEdges(1,0,0);
                    for(set<EdgeIterator>::iterator ei = negincident.begin(); ei != negincident.end(); ei++)
                    {
                        EdgeIterator e = *ei;
                        if(Flow[e] > 0
                           && Predecessor.find(e.From()) == Predecessor.end())
                        {
                            NextRound.insert(e.From());
                            Predecessor[e.From()] = v;
                            EdgeToPredecessor[e.From()] = e;
                        }
                    }
                    */
                }

                LastRound = NextRound;
                NextRound.clear();
            }

            for(EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                // if exactly one incident vertex of e is augmenting-path-reachable from "Source"
                if(  (SourceComponent.find((*e)->From()) == SourceComponent.end())
                   !=(SourceComponent.find((*e)->To()) == SourceComponent.end()))
                MinimumCut.insert(*e);
            }
        }


        void AlgorithmMINIMUMCUT::FindMinimumCut(Graph& G, EdgeWeighting& Capacities, EdgeSet& MinimumCut)
        {
            bool Undirected = G.IsUndirected();
            float MinimumCutSize = 0;
            for(EdgeWeighting::iterator i = Capacities.begin(); i != Capacities.end(); i++)
                MinimumCutSize += i->second >= 0 ? i->second : 0;

            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                VertexIterator w = (Undirected ? v : G.BeginVertices());
                if(Undirected)
                    w++;

                for(; w != G.EndVertices(); w++)
                {
                    if(v==w)
                        continue;

                    EdgeSet vwCut;
                    FindMinimumCut(G, *v, *w, Capacities, vwCut);
                    float vwCutSize = 0;
                    for(EdgeIterator i = vwCut.begin(); i != vwCut.end(); i++)
                        vwCutSize += Capacities[*i];

                    if(vwCutSize < MinimumCutSize)
                    {
                        MinimumCutSize = vwCutSize;
                        MinimumCut = vwCut;
                    }
                }
            }
        }

        void AlgorithmMINIMUMCUT::FindMinimumCut(Graph& G, EdgeSet& MinimumCut)
        {
            EdgeWeighting Capacities;
            for(EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                Capacities[*e] = 1;
            FindMinimumCut(G, Capacities, MinimumCut);
        }





        void AlgorithmMINIMUMCUT::AddMinimumCut(Graph &G, Vertex* Source, Vertex* Drain,
                                  EdgeWeighting& Capacities, std::string MinimumCutName)
        {
            EdgeSet MinimumCut;
            FindMinimumCut(G, Source, Drain, Capacities, MinimumCut);
            G.AddEdgeSet(MinimumCut, MinimumCutName);
        }

        void AlgorithmMINIMUMCUT::AddMinimumCut(Graph &G, EdgeWeighting& Capacities, std::string MinimumCutName)
        {
            EdgeSet MinimumCut;
            FindMinimumCut(G, Capacities, MinimumCut);
            G.AddEdgeSet(MinimumCut, MinimumCutName);
        }


        void AlgorithmMINIMUMCUT::AddMinimumCut(Graph &G, std::string MinimumCutName)
        {
            EdgeSet MinimumCut;
            FindMinimumCut(G, MinimumCut);
            G.AddEdgeSet(MinimumCut, MinimumCutName);
        }




        void AlgorithmMINIMUMCUT::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() < 3)
                throw "minimum cut algorithm needs 3 parameters (source, drain, result name [, capacities name])";

            string SourceName = parameters[0];
            string DrainName = parameters[1];
            string MinimumCutName = parameters[2];
            string CapacitiesName = parameters.size() > 3 ? parameters[3] : "";

            VertexIterator Source = G.EndVertices();
            VertexIterator Drain = G.EndVertices();
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                if(SourceName == (*v)->GetLabel())
                    Source = v;
                if(DrainName == (*v)->GetLabel())
                    Drain = v;
            }

            EdgeWeighting Capacities;
            for(EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                if(CapacitiesName != "")
                {
                    Attribute* attr = (*e)->GetAttribute(CapacitiesName);
                    FloatAttribute* fattr = dynamic_cast<FloatAttribute*>(attr);
                    if(fattr != NULL)
                        Capacities[*e] = fattr->Value;
                    else
                        Capacities[*e] = 0;
                }
                else
                {
                    Capacities[*e] = (*e)->GetWeight();
                }
            }

            if(Source != G.EndVertices() && Drain != G.EndVertices())
                AddMinimumCut(G, *Source, *Drain, Capacities, MinimumCutName);
        }

    }
}

