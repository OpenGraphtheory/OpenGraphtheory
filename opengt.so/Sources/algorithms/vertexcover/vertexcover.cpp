
#include "../../../Headers/algorithms/vertexcover/vertexcover.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        FactoryRegistrator<Algorithm> AlgorithmVERTEXCOVER::AlgorithmVertexCoverRegistrator(
            &Algorithm::AlgorithmFactory, "vertexcover", new DefaultInstantiator<Algorithm, AlgorithmVERTEXCOVER>(
            "vertexcover", "Adds a vertex cover to the graph", "http://en.wikipedia.org/wiki/Vertex_cover"));


        bool AlgorithmVERTEXCOVER::TestVertexCover(Graph &G,
                                                   set<Graph::EdgeIterator>& Uncovered,
                                                   set<Graph::VertexIterator>& VertexCover,
                                                   unsigned int k)
        {
            if(Uncovered.size() <= 0)
                return true;
            if(k <= 0)
                return false;

            unsigned int MaxNewlyCoverable = 0;
            Graph::VertexIterator selected = G.EndVertices();

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                if(VertexCover.find(v) != VertexCover.end()) // v already in VertexCover
                    continue;
                unsigned int NewlyCoverable = SetHelper::IntersectionSize(v.CollectIncidentEdges(1,1,1), Uncovered);

                if(NewlyCoverable > MaxNewlyCoverable)
                {
                    selected = v;
                    MaxNewlyCoverable = NewlyCoverable;
                }

                // vertex of degree 1 => select its neighbor
                // vertex of degree > k => select it
                if(NewlyCoverable == 1 || NewlyCoverable > k)
                {
                    selected = v;
                    MaxNewlyCoverable = NewlyCoverable;
                    break;
                }
            }

            if(MaxNewlyCoverable > 1 && MaxNewlyCoverable <= k)
            {
                if(k*MaxNewlyCoverable < Uncovered.size())
                    return false;
            }

            set<set<Graph::VertexIterator> > candidate_sets;

            set<Graph::VertexIterator> enforced_neighbors = SetHelper::SetMinus(selected.CollectNeighbors(1,1,1,1,1,1,1,1,1), VertexCover);
            if(MaxNewlyCoverable == 1)
            {
                candidate_sets.insert(enforced_neighbors);
            }
            else
            {
                set<Graph::VertexIterator> selected_set;
                selected_set.insert(selected);
                if(MaxNewlyCoverable <= k)
                    candidate_sets.insert(enforced_neighbors);
                candidate_sets.insert(selected_set);
            }

            // RECURSIVE CALL
            set<Graph::EdgeIterator> NextUncovered;
            for(set<set<Graph::VertexIterator> >::iterator selection = candidate_sets.begin(); selection != candidate_sets.end(); selection++)
            {
                VertexCover = SetHelper::Union(VertexCover, *selection);
                NextUncovered = Uncovered;
                for(set<Graph::VertexIterator>::iterator v = selection->begin(); v != selection->end(); v++)
                {
                    Graph::VertexIterator f = *v;
                    SetHelper::DestructiveSetMinus(NextUncovered, f.CollectIncidentEdges(1,1,1));
                }


                if(TestVertexCover(G, NextUncovered, VertexCover, k - selection->size() ))
                    return true;

                VertexCover = SetHelper::SetMinus(VertexCover, *selection);
            }

            return false;
        }




        bool AlgorithmVERTEXCOVER::FindVertexCover(Graph& G, set<Graph::VertexIterator>& VertexCover, unsigned int k)
        {
            set<Graph::EdgeIterator> Uncovered;
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                Uncovered.insert(e);

            return TestVertexCover(G, Uncovered, VertexCover, k);
        }

        void AlgorithmVERTEXCOVER::FindMinimumVertexCover(Graph& G, set<Graph::VertexIterator>& VertexCover)
        {
            if(G.NumberOfEdges() <= 0)
                return;

            set<Graph::EdgeIterator> MaximumMatching;
            AlgorithmMAXIMUMMATCHING::FindMaximumMatching(G, MaximumMatching, "");

            for(int k = MaximumMatching.size(); k<=G.NumberOfVertices(); k++)
            {
                cerr << "k = " << k << endl;
                if(FindVertexCover(G, VertexCover, k))
                    break;
                VertexCover.clear();
            }
        }

        void AlgorithmVERTEXCOVER::AddVertexCover(Graph &G, string VertexCoverName)
        {
            set<Graph::VertexIterator> VertexCover;
            FindMinimumVertexCover(G, VertexCover);
            G.AddVertexSet(VertexCover, VertexCoverName);
        }

        void AlgorithmVERTEXCOVER::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string VertexCoverName =  parameters[0];
            AddVertexCover(G, VertexCoverName);
        }

    }
}
