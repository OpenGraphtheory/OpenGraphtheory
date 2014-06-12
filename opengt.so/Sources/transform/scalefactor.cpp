
#include "../../Headers/transform/scalefactor.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::IntermediateSteps;

namespace OpenGraphtheory
{
    namespace Transform
    {

        FactoryRegistrator<Transformer> TransformerSCALEFACTOR::TransformerScaleFactorRegistrator(
            &Transformer::TransformerFactory, "scalefactor", new DefaultInstantiator<Transformer, TransformerSCALEFACTOR>(
                "scalefactor", "to scale the graph by a constant factor", "http://en.wikipedia.org/wiki/Scaling_(geometry)"));

        TransformerSCALEFACTOR::TransformerSCALEFACTOR()
        {
            MinParamCount = 1;
            MaxParamCount = -1;
        }

        void TransformerSCALEFACTOR::DoTransform(Graph& G, vector<float> parameters, IntermediateStepHandler* intermediatestephandler)
        {
            Coordinates coordinates;

            // scale
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                coordinates = (*v)->GetCoordinates(0);
                for(unsigned int i = 0; i < coordinates.size() && i < parameters.size(); ++i)
                    coordinates[i] *= parameters[i];
                (*v)->SetCoordinates(coordinates);
            }
        }


    }
}
