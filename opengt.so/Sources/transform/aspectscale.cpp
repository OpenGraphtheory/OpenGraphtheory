
#include "../../Headers/transform/aspectscale.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::IntermediateSteps;

namespace OpenGraphtheory
{
    namespace Transform
    {

        FactoryRegistrator<Transformer> TransformerASPECTSCALE::TransformerAspectScaleRegistrator(
            &(Transformer::TransformerFactory), "aspectscale", new DefaultInstantiator<Transformer, TransformerASPECTSCALE>(
                "aspectscale", "fits the vertices into the given space (width height [depth ...]) preserving aspect ratio",
                "http://en.wikipedia.org/wiki/Scaling_(geometry)"));

        TransformerASPECTSCALE::TransformerASPECTSCALE()
        {
            MinParamCount = 1;
            MaxParamCount = -1;
        }

        void TransformerASPECTSCALE::DoTransform(Graph& G, vector<float> parameters, IntermediateStepHandler* intermediatestephandler)
        {
            unsigned int dimensions = 0;
            vector<float> dimension_min;
            vector<float> dimension_max;
            Coordinates coordinates;

            // find minimum and maximum values on all dimensions
            for(VertexIterator v1 = G.BeginVertices(); v1 != G.EndVertices(); v1++)
            {
                coordinates = (*v1)->GetCoordinates(0);
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

            // select minimum of scales
            float scale = parameters[0] / (dimension_max[0] - dimension_min[0]);
            for(unsigned int i = 0; i < dimensions; ++i)
                if(dimension_max[i] > dimension_min[i])
                    if(parameters[i] / (dimension_max[i] - dimension_min[i]) < scale)
                        scale = parameters[i] / (dimension_max[i] - dimension_min[i]);

            // scale
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                Coordinates coordinates = (*v)->GetCoordinates(0);
                for(unsigned int i = 0; i < coordinates.size(); ++i)
                    coordinates[i] = (coordinates[i] - dimension_min[i]) * scale;
                (*v)->SetCoordinates(coordinates);
            }
        }

    }
}
