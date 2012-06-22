#ifndef __OPENGRAPHTHEORY_TRANSFORM_SCALEFACTOR_H
    #define __OPENGRAPHTHEORY_TRANSFORM_SCALEFACTOR_H

    #include <vector>
    #include "../opengt.h"
    #include "../factory.h"
    #include "transformation.h"

    namespace OpenGraphtheory
    {
        namespace Transform
        {

            class TransformerSCALEFACTOR : public Transformer
            {
                protected:
                    static FactoryRegistrator<Transformer> TransformerScaleFactorRegistrator;
                public:
                    TransformerSCALEFACTOR();
                    void DoTransform(OpenGraphtheory::Graph& G, std::vector<float> parameters, IntermediateSteps::IntermediateStepHandler* intermediatestephandler=NULL);
            };

        }
    }

#endif
