
#include "../../../Headers/algorithms/clique/clique.h"
#include<set>

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        FactoryRegistrator<Algorithm> AlgorithmCLIQUE::AlgorithmCliqueRegistrator(
            &Algorithm::AlgorithmFactory, "clique", new DefaultInstantiator<Algorithm, AlgorithmCLIQUE>(
            "clique", "Adds a clique to the graph", "http://en.wikipedia.org/wiki/Clique_(graph_theory)"));


        bool AlgorithmCLIQUE::TestClique(set<Graph::VertexIterator>& Clique, set<Graph::VertexIterator>& Candidates, unsigned int k)
        {
            if(k == 0)
                return true;
            if(k > Candidates.size())
                return false;

            for(set<Graph::VertexIterator>::iterator v = Candidates.begin(); v != Candidates.end(); v++)
            {
                set<Graph::VertexIterator> NextCandidates;
                set<Graph::VertexIterator>::iterator n = v;
                for(n++; n != Candidates.end(); n++)
                    if(v->UnderlyingAdjacent(*n))
                        NextCandidates.insert(*n);

                Clique.insert(*v);
                if(TestClique(Clique, NextCandidates, k-1))
                    return true;
                Clique.erase(*v);

            }

            return false;
        }


        bool AlgorithmCLIQUE::FindClique(Graph& G, set<Graph::VertexIterator>& Clique, unsigned int k)
        {
            set<Graph::VertexIterator> Excluded;
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {

                if(Excluded.find(v) != Excluded.end())
                    continue;

                set<Graph::VertexIterator> Candidates = v.UnderlyingNeighborhood();
                // remove excluded vertices and v from candidates set
                for(set<Graph::VertexIterator>::iterator i = Excluded.begin(); i != Excluded.end(); i++)
                    Candidates.erase(*i);
                Candidates.erase(v);

                Clique.insert(v);
                if(TestClique(Clique, Candidates, k-1))
                    return true;
                Clique.clear();

                Candidates.clear();
                Excluded.insert(v);
            }

            return false;
        }


        void AlgorithmCLIQUE::AddClique(Graph &G, string CliqueName)
        {
            set<Graph::VertexIterator> LastClique;
            set<Graph::VertexIterator> CurrentClique;

            if(G.NumberOfVertices() <= 0)
                return;
            CurrentClique.insert(G.BeginVertices());

            for(unsigned int k = 2; ; k++)
            {
                if(!FindClique(G, CurrentClique, k))
                    break;

                LastClique = CurrentClique;
                CurrentClique.clear();
            }

            for(set<Graph::VertexIterator>::iterator i = LastClique.begin(); i != LastClique.end(); i++)
            {
                Graph::VertexIterator j = *i;
                j.Attributes().Add(CliqueName, "int");
                IntAttribute* attr = dynamic_cast<IntAttribute*>(j.Attributes().GetAttribute(CliqueName));
                if(attr != NULL)
                    attr->Value = 1;
            }
        }

        void AlgorithmCLIQUE::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string CliqueName =  parameters[0];
            AddClique(G, CliqueName);
        }

    }
}

