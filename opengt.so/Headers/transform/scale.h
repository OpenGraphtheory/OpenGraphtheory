#ifndef __OPENGRAPHTHEORY_TRANSFORM_SCALE_H
    #define __OPENGRAPHTHEORY_TRANSFORM_SCALE_H

    #include <vector>
    #include "../opengt.h"
    #include "../factory.h"
    #include "transformation.h"

    namespace OpenGraphtheory
    {
        namespace Transform
        {

            class TransformerSCALE : public Transformer
            {
                protected:
                    static FactoryRegistrator<Transformer> TransformerScaleRegistrator;
                public:
                    TransformerSCALE();
                    void DoTransform(OpenGraphtheory::Graph& G, std::vector<float> parameters);
            };

        }
    }

#endif
