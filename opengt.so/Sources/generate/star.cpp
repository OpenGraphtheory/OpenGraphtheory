
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
            VertexIterator center = result.AddVertex();
            Coordinates centercoords(2, 0.0f);
            (*center)->SetCoordinates(centercoords);

            for(int i = 0; i < n; i++)
            {
                VertexIterator v = result.AddVertex();

                Coordinates coords(2);
                coords.push_back(-5 * cos(i * (2 * M_PI / n) + M_PI/2));
                coords.push_back( 5 * sin(i * (2 * M_PI / n) + M_PI/2));
                (*v)->SetCoordinates(coords);
            }

            for(VertexIterator v = result.BeginVertices()+1; v != result.EndVertices(); v++)
                result.AddEdge(result.BeginVertices(), v);

            return result;
        }

    }
}

