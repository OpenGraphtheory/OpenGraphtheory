
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
            {
                VertexIterator v = result.AddVertex();

                Coordinates coords(2);
                coords.push_back(-5 * cos(i * (2 * M_PI / n) + M_PI/2));
                coords.push_back( 5 * sin(i * (2 * M_PI / n) + M_PI/2));
                (*v)->SetCoordinates(coords);
            }

			VertexIterator center = result.AddVertex();
			Coordinates centercoords(2, 0.0f);
			(*center)->SetCoordinates(centercoords);

            VertexIterator v1 = result.BeginVertices();
            for(VertexIterator v2 = v1 + 1; v2 != result.EndVertices(); v1++, v2++)
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
