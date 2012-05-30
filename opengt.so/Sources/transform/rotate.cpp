
#include "../../Headers/transform/rotate.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Transform
    {

        //FactoryRegistrator<Transformer> TransformerROTATION::TransformerRotationRegistrator(
        //    &Transformer::TransformerFactory, "rotate", new DefaultInstantiator<Transformer, TransformerROTATION>(
        //        "rotate", "for rotating the graph (parameter 2 and 3 to set the center of the rotation, if present)", "http://en.wikipedia.org/wiki/Rotation_(mathematics)"));

        TransformerROTATION::TransformerROTATION()
        {
            MinParamCount = 1;
            MaxParamCount = 3;
        }


        //    x' = x*cos a + y*sin a,
        //    y' = − x*sin a + y*cos a,
        // additional parameters define the center of the rotation
        void TransformerROTATION::DoTransform(Graph& G, vector<float> parameters)
        {
            //float angle=parameters[0];
            //float xshift = 0;
            //float yshift = 0;

            if(parameters.size() == 3)
            {
                //xshift = parameters[1];
                //yshift = parameters[2];
                //TransformLinear(G, 1, 0, 0, 1, -xshift, -yshift);
            }

            //TransformLinear(G, cos(angle), sin(angle), -sin(angle), cos(angle), xshift, yshift);
        }

    }
}
