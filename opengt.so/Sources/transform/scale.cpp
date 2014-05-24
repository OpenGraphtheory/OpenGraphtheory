
#include "../../Headers/transform/scale.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::IntermediateSteps;

namespace OpenGraphtheory
{
    namespace Transform
    {


        FactoryRegistrator<Transformer> TransformerSCALE::TransformerScaleRegistrator(
            &Transformer::TransformerFactory, "scale", new DefaultInstantiator<Transformer, TransformerSCALE>(
                "scale", "to fit the graph into the rectangle given by parameters (width height [depth ...])", "http://en.wikipedia.org/wiki/Scaling_(geometry)"));

        TransformerSCALE::TransformerSCALE()
        {
            MinParamCount = 1;
            MaxParamCount = -1;
        }

        void TransformerSCALE::DoTransform(Graph& G, vector<float> parameters, IntermediateStepHandler* intermediatestephandler)
        {
            unsigned int dimensions = 0;
            vector<float> dimension_min;
            vector<float> dimension_max;
            Coordinates coordinates;

            // find minimum and maximum values on all dimensions
            for(VertexIterator v1 = G.BeginVertices(); v1 != G.EndVertices(); v1++)
            {
                coordinates = (*v1)->GetCoordinates();
                for(unsigned int j = 0; j < min(dimensions, (unsigned int)(coordinates.size())); ++j)
                {
                    if(coordinates[j] < dimension_min[j])
                        dimension_min[j] = coordinates[j];
                    if(coordinates[j] > dimension_max[j])
                        dimension_max[j] = coordinates[j];
                }

                // add more dimensions, if neccessary
                if(coordinates.size() > dimensions)
                {
                    for(unsigned int j = dimensions; j < coordinates.size(); j++)
                    {
                        dimension_min.push_back(coordinates[j]);
                        dimension_max.push_back(coordinates[j]);
                    }
                    dimensions = coordinates.size();
                }
            }

            // set scales for all dimensions
            vector<float> scale(dimensions);
            for(unsigned int i = 0; i < dimensions; ++i)
                if(dimension_max[i] > dimension_min[i])
                    scale[i] = parameters[i] / (dimension_max[i] - dimension_min[i]);
                else
                    scale[i] = 0.0f;

            // scale
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                Coordinates coordinates = (*v)->GetCoordinates();
                for(unsigned int i = 0; i < coordinates.size(); ++i)
                    coordinates[i] = (coordinates[i] - dimension_min[i]) * scale[i];
                (*v)->SetCoordinates(coordinates);
            }
        }


    }
}
