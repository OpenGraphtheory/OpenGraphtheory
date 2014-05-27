
#include "../../Headers/generate/tree.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        FactoryRegistrator<Generator> GeneratorTREE::GeneratorTreeRegistrator(
            &Generator::GeneratorFactory, "tree", new DefaultInstantiator<Generator, GeneratorTREE>(
                "tree", "Trees","http://en.wikipedia.org/wiki/Tree_(graph_theory)"));

        GeneratorTREE::GeneratorTREE()
        {
            MinParamCount = -1;
            MaxParamCount = -1;
        }

        void GeneratorTREE::InternalGenerate(Graph* G, list<int>* parameter, list<int>::iterator* it, Vertex* v)
        {
            if(*it == parameter->end())
                return;

            for(int i = **it; i > 0; --i)
            {
                Vertex* u = *(G->AddVertex());
                G->AddEdge(v,u);

                if(*it != parameter->end())
                    (*it)++;
                InternalGenerate(G, parameter, it, u);
            }
        }

        Graph GeneratorTREE::DoGenerate(list<int> parameter)
        {
            Graph result;
            Vertex* root = *(result.AddVertex());
            list<int>::iterator i = parameter.begin();
            InternalGenerate(&result, &parameter, &i, root);
            return result;
        }

    }
}

