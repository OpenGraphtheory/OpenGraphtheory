
#include "../../Headers/generate/grid.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        FactoryRegistrator<Generator> GeneratorGRID::GeneratorGridRegistrator(
            &Generator::GeneratorFactory, "grid", new DefaultInstantiator<Generator, GeneratorGRID>(
                "grid", "Grid graphs","http://en.wikipedia.org/wiki/Lattice_graph"));

        GeneratorGRID::GeneratorGRID()
        {
            MinParamCount = 2;
            MaxParamCount = 2;
        }

        Graph GeneratorGRID::DoGenerate(list<int> parameter)
        {
            int w = parameter.front(); parameter.pop_front();
            int h = parameter.front();
            return Generate(w,h);
        }

        Graph GeneratorGRID::Generate(int w, int h)
	    {
            Graph result;

            list<Vertex*> CurrentColumn;
            list<Vertex*> LastColumn;
            list<Vertex*>::iterator LastColumnIterator;
            for(int x = 0; x < w; x++)
            {
                Vertex* LastVertex;
                for(int y = 0; y < h; y++)
                {
                    Vertex* CurrentVertex = *(result.AddVertex());

                    Coordinates coords;
                    coords.push_back(x);
                    coords.push_back(y);
                    CurrentVertex->SetCoordinates(coords);

                    if(y > 0)
                        result.AddEdge(LastVertex, CurrentVertex);

                    if(x > 0)
                    {
                        if(y == 0)
                            LastColumnIterator = LastColumn.begin();
                        else
                            LastColumnIterator++;

                        result.AddEdge(*LastColumnIterator, CurrentVertex);
                    }

                    CurrentColumn.push_back(CurrentVertex);
                    LastVertex = CurrentVertex;
                }

                LastColumn = CurrentColumn;
                CurrentColumn.clear();
            }

            return result;
        }


    }
}

