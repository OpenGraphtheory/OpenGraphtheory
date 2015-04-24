#ifndef __OPENGRAPHTHEORY_TRANSFORM_ROTATE_H
    #define __OPENGRAPHTHEORY_TRANSFORM_ROTATE_H

    #include <vector>
    #include "../opengt.h"
    #include "../factory.h"
    #include "transform.h"

    namespace OpenGraphtheory
    {
        namespace Transform
        {

            class TransformerROTATION : public Transformer
            {
                protected:
                    static FactoryRegistrator<Transformer> TransformerRotationRegistrator;
                public:
                    TransformerROTATION();
                    void DoTransform(OpenGraphtheory::Graph& G, std::vector<float> parameters, IntermediateSteps::IntermediateStepHandler* intermediatestephandler);
            };

        }
    }

#endif
