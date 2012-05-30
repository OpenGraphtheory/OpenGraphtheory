#ifndef __OPENGRAPHTHEORY_TRANSFORM_ASPECTSCALE_H
    #define __OPENGRAPHTHEORY_TRANSFORM_ASPECTSCALE_H

    #include <vector>
    #include "../opengt.h"
    #include "../factory.h"
    #include "transformation.h"

    namespace OpenGraphtheory
    {
        namespace Transform
        {

            class TransformerASPECTSCALE : public Transformer
            {
                protected:
                    static FactoryRegistrator<Transformer> TransformerAspectScaleRegistrator;
                public:
                    TransformerASPECTSCALE();
                    void DoTransform(OpenGraphtheory::Graph& G, std::vector<float> parameters);
            };

        }
    }

#endif
