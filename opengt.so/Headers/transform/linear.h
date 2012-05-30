#ifndef __OPENGRAPHTHEORY_TRANSFORM_LINEAR_H
    #define __OPENGRAPHTHEORY_TRANSFORM_LINEAR_H

    #include <vector>
    #include "../opengt.h"
    #include "../factory.h"
    #include "transformation.h"

    namespace OpenGraphtheory
    {
        namespace Transform
        {

            class TransformerLINEAR : public Transformer
            {
                protected:
                    static FactoryRegistrator<Transformer> TransformerLinearRegistrator;

                    void TransformLinear(Graph& G, vector<vector<float> > A, vector<float> B);
                    void TransformLinear(Graph& G, float A11, float A12, float A21, float A22, float B1, float B2);

                public:
                    TransformerLINEAR();
                    void DoTransform(OpenGraphtheory::Graph& G, std::vector<float> parameters);
            };

        }
    }

#endif
