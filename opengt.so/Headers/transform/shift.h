#ifndef __OPENGRAPHTHEORY_TRANSFORM_SHIFT_H
    #define __OPENGRAPHTHEORY_TRANSFORM_SHIFT_H

    #include <vector>
    #include "../opengt.h"
    #include "../factory.h"
    #include "transformation.h"

    namespace OpenGraphtheory
    {
        namespace Transform
        {

            class TransformerSHIFT : public Transformer
            {
                protected:
                    static FactoryRegistrator<Transformer> TransformerShiftRegistrator;
                public:
                    TransformerSHIFT();
                    void DoTransform(OpenGraphtheory::Graph& G, std::vector<float> parameters, IntermediateSteps::IntermediateStepHandler* intermediatestephandler = NULL);
            };

        }
    }

#endif
