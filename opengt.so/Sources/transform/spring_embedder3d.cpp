
#include "../../Headers/transform/springembedder3d.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;
using namespace OpenGraphtheory::IntermediateSteps;

namespace OpenGraphtheory
{
    namespace Transform
    {

        FactoryRegistrator<Transformer> TransformerSPRINGEMBEDDER3D::TransformerSpringEmbedder3dRegistrator(
            &Transformer::TransformerFactory, "springembed3d", new DefaultInstantiator<Transformer, TransformerSPRINGEMBEDDER3D>(
                "springembed3d", "assigns coordinates to vertices, using the spring-embedding technique in a 3 dimensional space",
                "http://i11www.iti.uni-karlsruhe.de/_media/teaching/sommer2004/networkdrawing/spring.pdf"));


        TransformerSPRINGEMBEDDER3D::TransformerSPRINGEMBEDDER3D() : TransformerSPRINGEMBEDDER()
        {
        }


        void TransformerSPRINGEMBEDDER3D::DoTransform(Graph& G, vector<float> parameters, IntermediateStepHandler* intermediatestephandler)
        {
            int width = 800;
            int height = 600;
            int depth = 600;

            if(parameters.size() > 1)
            {
                width = (int)(parameters[0]+0.5);
                height = (int)(parameters[1]+0.5);
            }
            if(parameters.size() > 2)
                depth = (int)(parameters[2]+0.5);

            vector<float> dimension_limits;
            dimension_limits.push_back(width);
            dimension_limits.push_back(height);
            dimension_limits.push_back(depth);
            SpringEmbed(G, dimension_limits, intermediatestephandler);
        }

    }
}
