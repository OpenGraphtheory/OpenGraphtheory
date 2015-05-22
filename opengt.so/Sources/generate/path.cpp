
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
            if(n <= 0)
                return result;

            for(int i = 0; i < n; i++)
            {
                VertexIterator v = result.AddVertex();

                Coordinates coords;
                coords.push_back(0);
                coords.push_back(i);
                (*v)->SetCoordinates(coords);
            }

            VertexIterator v = result.BeginVertices();
            VertexIterator v1 = v;
            v1++;
            for(; v1 != result.EndVertices(); v++,v1++)
                result.AddEdge(v, v1);

            return result;
        }

    }
}

