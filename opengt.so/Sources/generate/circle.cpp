
#include <cmath>
#include "../../Headers/generate/circle.h"
using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        //FactoryRegistrator<Generator> GeneratorCIRCLE::GeneratorCircleRegistrator(
        //    &Generator::GeneratorFactory, "circle", new DefaultInstantiator<Generator, GeneratorCIRCLE> (
        //        "circle", "Circular graphs","http://en.wikipedia.org/wiki/Cycle_graph"));

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
                result.AddVertex(-100 * cos(i * (2 * M_PI / n) + M_PI/2), 100 * sin(i * (2 * M_PI / n) + M_PI/2));

            Graph::VertexIterator v1 = result.BeginVertices();
            for(Graph::VertexIterator v2 = v1 + 1; v2 != result.EndVertices(); v1++, v2++)
                result.AddEdge(v1, v2);
            result.AddEdge(v1, result.BeginVertices());

            return result;
        }

    }
}
