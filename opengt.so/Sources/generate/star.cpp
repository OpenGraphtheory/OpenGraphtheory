
#include "../../Headers/generate/star.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        FactoryRegistrator<Generator> GeneratorSTAR::GeneratorStarRegistrator(
            &Generator::GeneratorFactory, "star", new DefaultInstantiator<Generator, GeneratorSTAR>(
                "star", "Star graphs","http://en.wikipedia.org/wiki/Star_(graph_theory)"));

        GeneratorSTAR::GeneratorSTAR()
        {
            MinParamCount = 1;
            MaxParamCount = 1;
        }

        Graph GeneratorSTAR::DoGenerate(list<int> parameter)
        {
            int n = parameter.front();
            return Generate(n);
        }

        Graph GeneratorSTAR::Generate(int n)
        {
            Graph result;
            result.AddVertex(0, 0);
            for(int i = 0; i < n; i++)
                result.AddVertex(-100 * cos(i * (2 * M_PI / n) + M_PI/2), 100 * sin(i * (2 * M_PI / n) + M_PI/2));

            for(VertexIterator v = result.BeginVertices()+1; v != result.EndVertices(); v++)
                result.AddEdge(result.BeginVertices(), v);

            return result;
        }

    }
}

