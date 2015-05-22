
#include "../../Headers/generate/circle.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        FactoryRegistrator<Generator> GeneratorCIRCLE::GeneratorCircleRegistrator(
            &Generator::GeneratorFactory, "circle", new DefaultInstantiator<Generator, GeneratorCIRCLE> (
                "circle", "Circular graphs","http://en.wikipedia.org/wiki/Cycle_graph"));

        GeneratorCIRCLE::GeneratorCIRCLE()
        {
            MinParamCount = 1;
            MaxParamCount = 1;
        }


        Graph GeneratorCIRCLE::DoGenerate(list<int> parameter)
        {
            int n = parameter.front();
            Graph result;
            for(int i = 0; i < n; i++)
            {
                VertexIterator v = result.AddVertex();

                Coordinates coords;
                coords.push_back(-5 * cos(i * (2 * M_PI / n) + M_PI/2));
                coords.push_back( 5 * sin(i * (2 * M_PI / n) + M_PI/2));
                (*v)->SetCoordinates(coords);
            }

            VertexIterator v1 = result.BeginVertices();
            VertexIterator v2 = v1;
            v2++;
            for(; v2 != result.EndVertices(); v1++, v2++)
                result.AddEdge(v1, v2);
            result.AddEdge(v1, result.BeginVertices());

            return result;
        }

    }
}
