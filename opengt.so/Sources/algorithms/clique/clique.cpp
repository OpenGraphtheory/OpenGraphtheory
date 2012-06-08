
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

        bool AlgorithmCLIQUE::Clique(Graph& G, set<Graph::VertexIterator>& Clique, int k)
        {
            return false;
        }


        void AlgorithmCLIQUE::AddClique(Graph &G, string CliqueName)
        {
            set<Graph::VertexIterator> LastClique;
            set<Graph::VertexIterator> CurrentClique;

            for(unsigned int k = 1; ; k++)
            {
                if(!Clique(G, CurrentClique, k))
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

