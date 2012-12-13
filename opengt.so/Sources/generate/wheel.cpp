
#include <cmath>
#include "../../Headers/generate/wheel.h"
using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        FactoryRegistrator<Generator> GeneratorWHEEL::GeneratorWheelRegistrator(
            &Generator::GeneratorFactory, "wheel", new DefaultInstantiator<Generator, GeneratorWHEEL> (
                "wheel", "Wheel graphs","http://en.wikipedia.org/wiki/Wheel_graph"));

        GeneratorWHEEL::GeneratorWHEEL()
        {
            MinParamCount = 1;
            MaxParamCount = 1;
        }


        Graph GeneratorWHEEL::DoGenerate(list<int> parameter)
        {
            int n = parameter.front();
            Graph result;
            for(int i = 0; i < n; i++)
                result.AddVertex(-100 * cos(i * (2 * M_PI / n) + M_PI/2), 100 * sin(i * (2 * M_PI / n) + M_PI/2));
			Graph::VertexIterator center = result.AddVertex(0,0);
			
            Graph::VertexIterator v1 = result.BeginVertices();
            for(Graph::VertexIterator v2 = v1 + 1; v2 != result.EndVertices(); v1++, v2++)
			{
                result.AddEdge(v1, v2);
				result.AddEdge(v2, center);
			}
            result.AddEdge(v1, result.BeginVertices());
			result.AddEdge(center, result.BeginVertices());

            return result;
        }

    }
}
