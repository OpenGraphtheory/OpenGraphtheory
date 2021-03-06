
#include "../../Headers/transform/shift.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::IntermediateSteps;

namespace OpenGraphtheory
{
    namespace Transform
    {


        FactoryRegistrator<Transformer> TransformerSHIFT::TransformerShiftRegistrator(
            &Transformer::TransformerFactory, "shift", new DefaultInstantiator<Transformer, TransformerSHIFT>(
                "shift", "adding constant values to the coordinates of the vertices", "http://en.wikipedia.org/wiki/Shift_operator"));

        TransformerSHIFT::TransformerSHIFT()
        {
            MinParamCount = 1;
            MaxParamCount = -1;
        }

        void TransformerSHIFT::DoTransform(Graph& G, vector<float> parameters, IntermediateStepHandler* intermediatestephandler)
        {
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                Coordinates coordinates = (*v)->GetCoordinates(0);

                unsigned int ubound = min(coordinates.size(), parameters.size());
                for(unsigned int i = 0; i < ubound; ++i)
                    coordinates[i] += parameters[i];
                for(unsigned int i = coordinates.size(); i < parameters.size(); ++i)
                    coordinates.push_back(parameters[i]);

                (*v)->SetCoordinates(coordinates);
            }
        }


    }
}
