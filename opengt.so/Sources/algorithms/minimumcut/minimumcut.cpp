
#include "../../../Headers/algorithms/minimumcut/minimumcut.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        FactoryRegistrator<Algorithm> AlgorithmMINIMUMCUT::AlgorithmMinimumCutRegistrator(
            &Algorithm::AlgorithmFactory, "minimumcut", new DefaultInstantiator<Algorithm, AlgorithmMINIMUMCUT>(
            "minimumcut", "Adds a minimum cut to the graph", "http://en.wikipedia.org/wiki/Minimum_cut"));


        void AlgorithmMINIMUMCUT::FindMinimumCut(Graph& G, Graph::VertexIterator Source, Graph::VertexIterator Drain,
                                   std::map<Graph::EdgeIterator, float>& Capacities, std::set<Graph::EdgeIterator>& MinimumCut)
        {
            map<Graph::EdgeIterator, float> MaximumFlow;
            AlgorithmMAXIMUMFLOW::FindMaximumFlow(G, Source, Drain, Capacities, MaximumFlow);

            set<Graph::VertexIterator> SourceComponent;
            SourceComponent.insert(Source);
            set<Graph::VertexIterator> LastRound;
            LastRound.insert(Source);
            set<Graph::VertexIterator> NextRound;

            while(!LastRound.empty())
            {
                for(set<Graph::VertexIterator>::iterator vi = LastRound.begin(); vi != LastRound.end(); vi++)
                {
                    Graph::VertexIterator v = *vi;
                    set<Graph::EdgeIterator> posincident = v.CollectIncidentEdges(0,1,0);
                    for(set<Graph::EdgeIterator>::iterator ei = posincident.begin(); ei != posincident.end(); ei++)
                    {
                        Graph::EdgeIterator e = *ei;
                        if(Capacities[e] > MaximumFlow[e]
                           && SourceComponent.find(e.To()) == SourceComponent.end())
                        {
                            NextRound.insert(e.To());
                            SourceComponent.insert(e.To());
                        }
                    }

                    set<Graph::EdgeIterator> negincident = v.CollectIncidentEdges(0,0,1);
                    for(set<Graph::EdgeIterator>::iterator ei = negincident.begin(); ei != negincident.end(); ei++)
                    {
                        Graph::EdgeIterator e = *ei;
                        if(MaximumFlow[e] > 0
                           && SourceComponent.find(e.From()) == SourceComponent.end())
                        {
                            NextRound.insert(e.From());
                            SourceComponent.insert(e.From());
                        }
                    }

                    /*
                    set<Graph::EdgeIterator> undirincident = v.CollectIncidentEdges(1,0,0);
                    for(set<Graph::EdgeIterator>::iterator ei = negincident.begin(); ei != negincident.end(); ei++)
                    {
                        Graph::EdgeIterator e = *ei;
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

            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                // if exactly one incident vertex of e is augmenting-path-reachable from "Source"
                if(  (SourceComponent.find(e.From())==SourceComponent.end())
                   !=(SourceComponent.find(e.To())==SourceComponent.end()))
                MinimumCut.insert(e);
            }
        }


        void AlgorithmMINIMUMCUT::FindMinimumCut(Graph& G, std::map<Graph::EdgeIterator, float>& Capacities, std::set<Graph::EdgeIterator>& MinimumCut)
        {
            bool Undirected = G.IsUndirected();
            float MinimumCutSize = 0;
            for(map<Graph::EdgeIterator,float>::iterator i = Capacities.begin(); i != Capacities.end(); i++)
                MinimumCutSize += i->second >= 0 ? i->second : 0;

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                for(Graph::VertexIterator w = (Undirected ? v + 1 : G.BeginVertices()); w != G.EndVertices(); w++)
                {
                    if(v==w)
                        continue;

                    set<Graph::EdgeIterator> vwCut;
                    FindMinimumCut(G, v, w, Capacities, vwCut);
                    float vwCutSize = 0;
                    for(set<Graph::EdgeIterator>::iterator i = vwCut.begin(); i != vwCut.end(); i++)
                        vwCutSize += Capacities[*i];

                    if(vwCutSize < MinimumCutSize)
                    {
                        MinimumCutSize = vwCutSize;
                        MinimumCut = vwCut;
                    }
                }
        }

        void AlgorithmMINIMUMCUT::FindMinimumCut(Graph& G, std::set<Graph::EdgeIterator>& MinimumCut)
        {
            map<Graph::EdgeIterator, float> Capacities;
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                Capacities[e] = 1;
            FindMinimumCut(G, Capacities, MinimumCut);
        }





        void AlgorithmMINIMUMCUT::AddMinimumCut(Graph &G, Graph::VertexIterator Source, Graph::VertexIterator Drain,
                                  std::map<Graph::EdgeIterator, float>& Capacities, std::string MinimumCutName)
        {
            set<Graph::EdgeIterator> MinimumCut;
            FindMinimumCut(G, Source, Drain, Capacities, MinimumCut);
            G.AddEdgeSet(MinimumCut, MinimumCutName);
        }

        void AlgorithmMINIMUMCUT::AddMinimumCut(Graph &G, std::map<Graph::EdgeIterator, float>& Capacities, std::string MinimumCutName)
        {
            set<Graph::EdgeIterator> MinimumCut;
            FindMinimumCut(G, Capacities, MinimumCut);
            G.AddEdgeSet(MinimumCut, MinimumCutName);
        }


        void AlgorithmMINIMUMCUT::AddMinimumCut(Graph &G, std::string MinimumCutName)
        {
            set<Graph::EdgeIterator> MinimumCut;
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

            Graph::VertexIterator Source = G.EndVertices();
            Graph::VertexIterator Drain = G.EndVertices();
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                if(SourceName == v.GetLabel())
                    Source = v;
                if(DrainName == v.GetLabel())
                    Drain = v;
            }

            map<Graph::EdgeIterator, float> Capacities;
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                if(CapacitiesName != "")
                {
                    Attribute* attr = e.Attributes().GetAttribute(CapacitiesName);
                    FloatAttribute* fattr = dynamic_cast<FloatAttribute*>(attr);
                    if(fattr != NULL)
                        Capacities[e] = fattr->Value;
                    else
                        Capacities[e] = 0;
                }
                else
                {
                    Capacities[e] = e.GetWeight();
                }
            }

            if(Source != G.EndVertices() && Drain != G.EndVertices())
                AddMinimumCut(G, Source, Drain, Capacities, MinimumCutName);
        }

    }
}

