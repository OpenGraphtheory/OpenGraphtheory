
#include "../../../Headers/algorithms/maximumflow/maximumflow.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        FactoryRegistrator<Algorithm> AlgorithmMAXIMUMFLOW::AlgorithmMaximumFlowRegistrator(
            &Algorithm::AlgorithmFactory, "maximumflow", new DefaultInstantiator<Algorithm, AlgorithmMAXIMUMFLOW>(
            "maximumflow", "Adds a maximum flow to the graph", "http://en.wikipedia.org/wiki/Maximum_flow_problem"));



        bool AlgorithmMAXIMUMFLOW::FindAugmentingPath(Graph &G, Graph::VertexIterator Source, Graph::VertexIterator Drain,
                                                   map<Graph::EdgeIterator, float>& Capacities,
                                                   map<Graph::EdgeIterator, float>& Flow,
                                                   vector<Graph::EdgeIterator>& AugmentingPath)
        {
            map<Graph::VertexIterator, Graph::VertexIterator> Predecessor;
            map<Graph::VertexIterator, Graph::EdgeIterator> EdgeToPredecessor;
            set<Graph::VertexIterator> LastRound;
            set<Graph::VertexIterator> NextRound;
            LastRound.insert(Source);
            while(LastRound.find(Drain) == LastRound.end() && !LastRound.empty())
            {
                for(set<Graph::VertexIterator>::iterator vi = LastRound.begin(); vi != LastRound.end(); vi++)
                {
                    Graph::VertexIterator v = *vi;
                    set<Graph::EdgeIterator> posincident = v.CollectIncidentEdges(0,1,0);
                    for(set<Graph::EdgeIterator>::iterator ei = posincident.begin(); ei != posincident.end(); ei++)
                    {
                        Graph::EdgeIterator e = *ei;
                        if(Capacities[e] > Flow[e]
                           && Predecessor.find(e.To()) == Predecessor.end())
                        {
                            NextRound.insert(e.To());
                            Predecessor[e.To()] = v;
                            EdgeToPredecessor[e.To()] = e;
                        }
                    }

                    set<Graph::EdgeIterator> negincident = v.CollectIncidentEdges(0,0,1);
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
                }

                LastRound = NextRound;
                NextRound.clear();
            }

            if(LastRound.find(Drain) == LastRound.end()) // no augmenting path found
                return false;

            AugmentingPath.clear();
            for(Graph::VertexIterator v = Drain; v != Source; v = Predecessor[v])
                AugmentingPath.insert(AugmentingPath.begin(), EdgeToPredecessor[v]);
            return true;
        }


        void AlgorithmMAXIMUMFLOW::FindMaximumFlow(Graph &G, Graph::VertexIterator Source, Graph::VertexIterator Drain,
                                                   map<Graph::EdgeIterator, float>& Capacities,
                                                   map<Graph::EdgeIterator, float>& Flow)
        {
            vector<Graph::EdgeIterator> AugmentingPath;

            while(FindAugmentingPath(G, Source, Drain, Capacities, Flow, AugmentingPath))
            {
                float Augmentation = 0;
                for(vector<Graph::EdgeIterator>::iterator i = AugmentingPath.begin(); i != AugmentingPath.end(); i++)
                    Augmentation += Capacities[*i] + Flow[*i];

                // Find out how much the flow can be augmented
                Graph::VertexIterator v = Source;
                for(vector<Graph::EdgeIterator>::iterator i = AugmentingPath.begin(); i != AugmentingPath.end(); i++)
                {
                    if(v == i->From()) // Forward-Arc
                    {
                        if(Capacities[*i] - Flow[*i] < Augmentation)
                            Augmentation = Capacities[*i] - Flow[*i];
                        v = i->To();
                    }
                    else // Backward-Arc
                    {
                        if(Flow[*i] < Augmentation)
                            Augmentation = Flow[*i];
                        v = i->From();
                    }
                }

                // augment the flow
                v = Source;
                for(vector<Graph::EdgeIterator>::iterator i = AugmentingPath.begin(); i != AugmentingPath.end(); i++)
                {
                    if(v == i->From()) // Forward-Arc
                    {
                        Flow[*i] += Augmentation;
                        v = i->To();
                    }
                    else // Backward-Arc
                    {
                        Flow[*i] -= Augmentation;
                        v = i->From();
                    }
                }
            }
        }


        void AlgorithmMAXIMUMFLOW::AddMaximumFlow(Graph &G, Graph::VertexIterator Source, Graph::VertexIterator Drain,
                                                   map<Graph::EdgeIterator, float>& Capacities, string FlowName)
        {
            map<Graph::EdgeIterator, float> Flow;
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                Flow[e] = 0;
            FindMaximumFlow(G, Source, Drain, Capacities, Flow);
            G.AddEdgeWeight(Flow, FlowName);
        }


        void AlgorithmMAXIMUMFLOW::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() < 3)
                throw "maximum flow algorithm needs 3 parameters (source, drain, result name [, capacities name])";

            string SourceName = parameters[0];
            string DrainName = parameters[1];
            string MaximumFlowName = parameters[2];
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
                AddMaximumFlow(G, Source, Drain, Capacities, MaximumFlowName);
        }

    }
}

