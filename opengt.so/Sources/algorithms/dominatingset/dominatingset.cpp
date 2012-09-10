
#include "../../../Headers/algorithms/dominatingset/dominatingset.h"
#include<set>

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        FactoryRegistrator<Algorithm> AlgorithmDOMINATINGSET::AlgorithmDominatingSetRegistrator(
            &Algorithm::AlgorithmFactory, "dominatingset", new DefaultInstantiator<Algorithm, AlgorithmDOMINATINGSET>(
            "dominatingset", "Adds a dominating set to the graph", "http://en.wikipedia.org/wiki/Dominating_set"));


        bool AlgorithmDOMINATINGSET::TestDominatingSet(map<Graph::VertexIterator, int>& Dominators, set<Graph::VertexIterator>& Undominated,
                                                       set<Graph::VertexIterator>& Excluded, set<Graph::VertexIterator>& DominatingSet,
                                                       unsigned int k, bool VerticesCanDominateThemselves)
        {
            if(Undominated.size() <= 0)
                return true;
            if(k <= 0)
                return false;


            Graph::VertexIterator v = *(Undominated.begin());
            set<Graph::VertexIterator> NextExcluded;
/*
            for(set<Graph::VertexIterator>::iterator i = Undominated.begin(); i != Undominated.end(); i++)
            {

            }
*/
            // at this point v must be a VertexIterator with the vertex that has been chosen to be dominated next.

            set<Graph::VertexIterator> Candidates;
            for(Graph::EdgeIterator e = v.BeginIncidentEdges(); e != v.EndIncidentEdges(); e++)
            {
                for(Graph::VertexIterator n = e.BeginIncidentVertices(); n != e.EndIncidentVertices(); n++)
                    Candidates.insert(n);
                for(Graph::VertexIterator n = e.BeginNegativeIncidentVertices(); n != e.EndNegativeIncidentVertices(); n++)
                    Candidates.insert(n);
            }

            for(Graph::EdgeIterator e = v.BeginNegativeIncidentEdges(); e != v.EndNegativeIncidentEdges(); e++)
            {
                for(Graph::VertexIterator n = e.BeginIncidentVertices(); n != e.EndIncidentVertices(); n++)
                    Candidates.insert(n);
                for(Graph::VertexIterator n = e.BeginNegativeIncidentVertices(); n != e.EndNegativeIncidentVertices(); n++)
                    Candidates.insert(n);
            }

            if(VerticesCanDominateThemselves)
                Candidates.insert(v);
            for(set<Graph::VertexIterator>::iterator excluded = Excluded.begin(); excluded != Excluded.end(); excluded++)
                Candidates.erase(*excluded);


            for(set<Graph::VertexIterator>::iterator candidateiterator = Candidates.begin(); candidateiterator != Candidates.end(); candidateiterator++)
            {
                Graph::VertexIterator candidate = *candidateiterator;

                for(Graph::EdgeIterator e = candidate.BeginIncidentEdges(); e != candidate.EndIncidentEdges(); e++)
                {
                    for(Graph::VertexIterator n = e.BeginIncidentVertices(); n != e.EndIncidentVertices(); n++)
                        if(++Dominators[n] == 1) Undominated.erase(n);
                    for(Graph::VertexIterator n = e.BeginPositiveIncidentVertices(); n != e.EndPositiveIncidentVertices(); n++)
                        if(++Dominators[n] == 1) Undominated.erase(n);
                }
                for(Graph::EdgeIterator e = candidate.BeginPositiveIncidentEdges(); e != candidate.EndPositiveIncidentEdges(); e++)
                {
                    for(Graph::VertexIterator n = e.BeginIncidentVertices(); n != e.EndIncidentVertices(); n++)
                        if(++Dominators[n] == 1) Undominated.erase(n);
                    for(Graph::VertexIterator n = e.BeginPositiveIncidentVertices(); n != e.EndPositiveIncidentVertices(); n++)
                        if(++Dominators[n] == 1) Undominated.erase(n);
                }


                // RECURSIVE CALL
                DominatingSet.insert(candidate);
                if(TestDominatingSet(Dominators, Undominated, Excluded, DominatingSet, k-1))
                    return true;
                DominatingSet.erase(candidate);
                if(Excluded.find(candidate) == Excluded.end())
                {
                    Excluded.insert(candidate);
                    NextExcluded.insert(candidate);
                }

                for(Graph::EdgeIterator e = candidate.BeginIncidentEdges(); e != candidate.EndIncidentEdges(); e++)
                {
                    for(Graph::VertexIterator n = e.BeginIncidentVertices(); n != e.EndIncidentVertices(); n++)
                        if(--Dominators[n] == 0) Undominated.insert(n);
                    for(Graph::VertexIterator n = e.BeginPositiveIncidentVertices(); n != e.EndPositiveIncidentVertices(); n++)
                        if(--Dominators[n] == 0) Undominated.insert(n);
                }
                for(Graph::EdgeIterator e = candidate.BeginPositiveIncidentEdges(); e != candidate.EndPositiveIncidentEdges(); e++)
                {
                    for(Graph::VertexIterator n = e.BeginIncidentVertices(); n != e.EndIncidentVertices(); n++)
                        if(--Dominators[n] == 0) Undominated.insert(n);
                    for(Graph::VertexIterator n = e.BeginPositiveIncidentVertices(); n != e.EndPositiveIncidentVertices(); n++)
                        if(--Dominators[n] == 0) Undominated.insert(n);
                }
            }

            for(set<Graph::VertexIterator>::iterator nextexcluded = NextExcluded.begin(); nextexcluded != NextExcluded.end(); nextexcluded++)
                Excluded.erase(*nextexcluded);
            return false;
        }


        bool AlgorithmDOMINATINGSET::FindDominatingSet(Graph& G, set<Graph::VertexIterator>& DominatingSet, unsigned int k, bool VerticesCanDominateThemselves)
        {
            map<Graph::VertexIterator, int> Dominators;
            set<Graph::VertexIterator> Undominated;
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                Dominators[v] = 0;
                Undominated.insert(v);
            }

            set<Graph::VertexIterator> Excluded;

            return TestDominatingSet(Dominators, Undominated, Excluded, DominatingSet, k, VerticesCanDominateThemselves);
        }


        void AlgorithmDOMINATINGSET::AddDominatingSet(Graph &G, string DominatingSetName, bool VerticesCanDominateThemselves)
        {
            set<Graph::VertexIterator> DominatingSet;

            if(G.NumberOfVertices() <= 0)
                return;

            for(unsigned int k = 1; ; k++)
            {
                if(FindDominatingSet(G, DominatingSet, k))
                    break;
                DominatingSet.clear();
            }

            for(set<Graph::VertexIterator>::iterator i = DominatingSet.begin(); i != DominatingSet.end(); i++)
            {
                Graph::VertexIterator j = *i;
                j.Attributes().Add(DominatingSetName, "int");
                IntAttribute* attr = dynamic_cast<IntAttribute*>(j.Attributes().GetAttribute(DominatingSetName));
                if(attr != NULL)
                    attr->Value = 1;
            }
        }

        void AlgorithmDOMINATINGSET::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string DominatingSetName =  parameters[0];
            AddDominatingSet(G, DominatingSetName);
        }

    }
}

