
#include "../../Headers/generate/path.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        FactoryRegistrator<Generator> GeneratorPATH::GeneratorPathRegistrator(
            &Generator::GeneratorFactory, "path", new DefaultInstantiator<Generator, GeneratorPATH>(
                "path", "Paths","http://en.wikipedia.org/wiki/Path_graph"));

        GeneratorPATH::GeneratorPATH()
        {
            MinParamCount = 1;
            MaxParamCount = 1;
        }

        Graph GeneratorPATH::DoGenerate(list<int> parameter)
        {
            int n = parameter.front();
            return Generate(n);
        }

        Graph GeneratorPATH::Generate(int n)
        {
            Graph result;
            for(int i = 0; i < n; i++)
            {
                VertexIterator v = result.AddVertex();

                Coordinates coords(2);
                coords.push_back(0);
                coords.push_back(i);
                (*v)->SetCoordinates(coords);
            }

            for(VertexIterator v = result.BeginVertices(); v + 1 != result.EndVertices(); v++)
                result.AddEdge(v, v+1);

            return result;
        }

    }
}

