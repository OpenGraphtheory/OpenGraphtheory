
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

            Vertex* center = *(result.AddVertex());
            Coordinates centercoords(2, 0.0f);
            center->SetCoordinates(centercoords);

            Vertex* prev = NULL;
            Vertex* first = NULL;
            for(int i = 0; i < n; i++)
            {
                Vertex* v = *(result.AddVertex());

                Coordinates coords(2);
                coords[0] = -5 * cos(i * (2 * M_PI / n) + M_PI/2);
                coords[1] =  5 * sin(i * (2 * M_PI / n) + M_PI/2);
                v->SetCoordinates(coords);

                if(prev != NULL)
                    result.AddEdge(prev, v);
                else
                    first = v;

                result.AddEdge(v, center);
                prev = v;
            }

            if(prev != NULL && first != NULL)
                result.AddEdge(prev, first);

            return result;
        }

    }
}
