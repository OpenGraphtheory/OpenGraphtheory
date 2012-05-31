#ifndef __OPENGRAPHTHEORY_TRANSFORM_SPRINGEMBEDDER3D_H
    #define __OPENGRAPHTHEORY_TRANSFORM_SPRINGEMBEDDER3D_H

    #include <list>
    #include "../opengt.h"
    #include "../visualize/vectornd.h"
    #include "../factory.h"
    #include "transformation.h"
    #include "springembedder.h"

    #include <iostream>
    #include <math.h>
    #include <stdlib.h> // for random numbers

    namespace OpenGraphtheory
    {
        namespace Transform
        {

            class TransformerSPRINGEMBEDDER3D : public OpenGraphtheory::Transform::TransformerSPRINGEMBEDDER
            {
                protected:
                    static FactoryRegistrator<Transformer> TransformerSpringEmbedder3dRegistrator;

                public:
                    TransformerSPRINGEMBEDDER3D();
                    void DoTransform(OpenGraphtheory::Graph& G, std::vector<float> parameters, IntermediateSteps::IntermediateStepHandler* intermediatestephandler = NULL);
            };

        }
    }

#endif
