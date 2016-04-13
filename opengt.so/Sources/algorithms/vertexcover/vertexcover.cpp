
#include "../../../Headers/algorithms/vertexcover/vertexcover.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        MultiFactoryRegistrator<Algorithm> AlgorithmVERTEXCOVER::AlgorithmVertexCoverRegistrator(
            &Algorithm::AlgorithmFactory, "vertexcover", new DefaultInstantiator<Algorithm, AlgorithmVERTEXCOVER>(
            "vertexcover", "Adds a vertex cover to the graph", "http://en.wikipedia.org/wiki/Vertex_cover"));


        bool AlgorithmVERTEXCOVER::TestVertexCover(Graph &G,
                                                   EdgeSet& Uncovered,
                                                   VertexSet& VertexCover,
                                                   int k)
        {
            if(this->TestTermination())
                return false;


            if(Uncovered.size() <= 0 && k >= 0)
                return true;
            if(k <= 0)
                return false;

            int MaxNewlyCoverable = 0;
            Vertex* selected = *G.EndVertices();

            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                if(VertexCover.find(*v) != VertexCover.end()) // v already in VertexCover
                    continue;
                int NewlyCoverable = SetHelper::IntersectionSize((*v)->CollectIncidentEdges(1,1,1), Uncovered);

                if(NewlyCoverable > MaxNewlyCoverable)
                {
                    selected = *v;
                    MaxNewlyCoverable = NewlyCoverable;
                }

                // vertex of degree 1 => select its neighbor
                // vertex of degree > k => select it
                if(NewlyCoverable == 1 || NewlyCoverable > k)
                {
                    selected = *v;
                    MaxNewlyCoverable = NewlyCoverable;
                    break;
                }
            }

            if(MaxNewlyCoverable > 1 && MaxNewlyCoverable <= k)
            {
                if(k*MaxNewlyCoverable < (int)Uncovered.size())
                    return false;
            }

            vector<VertexSet*> candidate_sets;
            VertexSet selected_set; // a set that will contain only the selected vertex

            VertexSet enforced_neighbors = SetHelper::SetMinus( selected->CollectNeighbors(1,1,1,1,1,1,1,1,1), VertexCover);
            enforced_neighbors.erase(selected);
            if(MaxNewlyCoverable == 1) // end-vertex => select its neighbor
            {
                candidate_sets.push_back(&enforced_neighbors);
            }
            else
            {
                if(MaxNewlyCoverable <= k) // degree > k => cannot choose neighbors
                    candidate_sets.push_back(&enforced_neighbors);

                selected_set.insert(selected);
                candidate_sets.push_back(&selected_set);
            }

            // RECURSIVE CALL
            EdgeSet NextUncovered;
            for(vector<VertexSet*>::iterator selection = candidate_sets.begin(); selection != candidate_sets.end(); selection++)
            {
                SetHelper::DestructiveUnion(VertexCover, **selection); // *selection is disjoint from VertexCover, so this is a safe step

                NextUncovered = Uncovered;

                for(VertexIterator v = (*selection)->begin(); v != (*selection)->end(); v++)
                    SetHelper::DestructiveSetMinus(NextUncovered, (*v)->CollectIncidentEdges(1,1,1));
                if(TestVertexCover(G, NextUncovered, VertexCover, k - (*selection)->size() ))
                    return true;

                SetHelper::DestructiveSetMinus(VertexCover, **selection);
            }

            return false;
        }




        bool AlgorithmVERTEXCOVER::FindVertexCover(Graph& G, VertexSet& VertexCover, unsigned int k)
        {
            EdgeSet Uncovered;
            for(EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                Uncovered.insert(*e);

            return TestVertexCover(G, Uncovered, VertexCover, k);
        }

        void AlgorithmVERTEXCOVER::FindMinimumVertexCover(Graph& G, VertexSet& VertexCover)
        {
            if(G.NumberOfEdges() <= 0)
                return;

            EdgeSet MaximumMatching;
            AlgorithmMAXIMUMMATCHING::FindMaximumMatching(G, MaximumMatching);

            for(int k = MaximumMatching.size(); k<=G.NumberOfVertices(); k++)
            {
                if(this->TestTermination())
                    return;

                if(FindVertexCover(G, VertexCover, k))
                    break;
                VertexCover.clear();
            }
        }

        void AlgorithmVERTEXCOVER::AddVertexCover(Graph &G, string VertexCoverName)
        {
            VertexSet VertexCover;
            FindMinimumVertexCover(G, VertexCover);
            if(this->TestTermination())
                return;
            G.AddVertexSet(VertexCover, VertexCoverName);
        }

        void AlgorithmVERTEXCOVER::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string VertexCoverName = parameters[0];
            AddVertexCover(G, VertexCoverName);
        }

    }
}
