
#include "../../../Headers/algorithms/maximumflow/maximumflow.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmMAXIMUMFLOW::AlgorithmMaximumFlowRegistrator(
            &Algorithm::AlgorithmFactory, "maximumflow", new DefaultInstantiator<Algorithm, AlgorithmMAXIMUMFLOW>(
            "maximumflow", "Adds a maximum flow to the graph", "http://en.wikipedia.org/wiki/Maximum_flow_problem"));



        bool AlgorithmMAXIMUMFLOW::FindAugmentingPath(Graph &G, Vertex* Source, Vertex* Drain, EdgeWeighting& Capacities,
                                                      EdgeWeighting& Flow, vector<Edge*>& AugmentingPath)
        {
            map<Vertex*, Vertex*> Predecessor;
            map<Vertex*, Edge*> EdgeToPredecessor;
            VertexSet LastRound;
            VertexSet NextRound;
            LastRound.insert(Source);
            while(LastRound.find(Drain) == LastRound.end() && !LastRound.empty())
            {
                for(VertexIterator v = LastRound.begin(); v != LastRound.end(); v++)
                {
                    // forward edges
                    EdgeSet posincident = (*v)->CollectIncidentEdges(0,1,0);
                    for(EdgeIterator e = posincident.begin(); e != posincident.end(); e++)
                    {
                        if(Capacities[*e] > Flow[*e]
                        && Predecessor.find((*e)->To()) == Predecessor.end())
                        {
                            NextRound.insert((*e)->To());
                            Predecessor[(*e)->To()] = *v;
                            EdgeToPredecessor[(*e)->To()] = *e;
                        }
                    }

                    // backward edges
                    EdgeSet negincident = (*v)->CollectIncidentEdges(0,0,1);
                    for(EdgeIterator e = negincident.begin(); e != negincident.end(); e++)
                    {
                        if(Flow[*e] > 0
                           && Predecessor.find((*e)->From()) == Predecessor.end())
                        {
                            NextRound.insert((*e)->From());
                            Predecessor[(*e)->From()] = *v;
                            EdgeToPredecessor[(*e)->From()] = *e;
                        }
                    }
                }

                LastRound = NextRound;
                NextRound.clear();
            }

            if(LastRound.find(Drain) == LastRound.end()) // no augmenting path found
                return false;

            AugmentingPath.clear();
            for(Vertex* v = Drain; v != Source; v = Predecessor[v])
                AugmentingPath.insert(AugmentingPath.begin(), EdgeToPredecessor[v]);
            return true;
        }


        void AlgorithmMAXIMUMFLOW::FindMaximumFlow(Graph &G, Vertex* Source, Vertex* Drain,
                                                   EdgeWeighting& Capacities, EdgeWeighting& Flow)
        {
            vector<Edge*> AugmentingPath;

            while(FindAugmentingPath(G, Source, Drain, Capacities, Flow, AugmentingPath))
            {
                float Augmentation = 0;
                for(vector<Edge*>::iterator i = AugmentingPath.begin(); i != AugmentingPath.end(); i++)
                    Augmentation += Capacities[*i] + Flow[*i];

                // Find out how much the flow can be augmented
                Vertex* v = Source;
                for(vector<Edge*>::iterator i = AugmentingPath.begin(); i != AugmentingPath.end(); i++)
                {
                    if(v == (*i)->From()) // Forward-Arc
                    {
                        if(Capacities[*i] - Flow[*i] < Augmentation)
                            Augmentation = Capacities[*i] - Flow[*i];
                        v = (*i)->To();
                    }
                    else // Backward-Arc
                    {
                        if(Flow[*i] < Augmentation)
                            Augmentation = Flow[*i];
                        v = (*i)->From();
                    }
                }

                // augment the flow
                v = Source;
                for(vector<Edge*>::iterator i = AugmentingPath.begin(); i != AugmentingPath.end(); i++)
                {
                    if(v == (*i)->From()) // Forward-Arc
                    {
                        Flow[*i] += Augmentation;
                        v = (*i)->To();
                    }
                    else // Backward-Arc
                    {
                        Flow[*i] -= Augmentation;
                        v = (*i)->From();
                    }
                }
            }
        }


        void AlgorithmMAXIMUMFLOW::AddMaximumFlow(Graph &G, Vertex* Source, Vertex* Drain,
                                                  EdgeWeighting& Capacities, string FlowName)
        {
            EdgeWeighting Flow;
            for(EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                Flow[*e] = 0;
            FindMaximumFlow(G, Source, Drain, Capacities, Flow);
            G.AddEdgeWeighting(Flow, FlowName);
        }


        void AlgorithmMAXIMUMFLOW::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() < 3)
                throw "maximum flow algorithm needs 3 parameters (source, drain, result name [, capacities name])";

            string SourceName = parameters[0];
            string DrainName = parameters[1];
            string MaximumFlowName = parameters[2];
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
                AddMaximumFlow(G, *Source, *Drain, Capacities, MaximumFlowName);
        }

    }
}

