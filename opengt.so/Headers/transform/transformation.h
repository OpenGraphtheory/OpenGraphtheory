
#ifndef __OPENGRAPHTHEORY_TRANSFORM_TRANSFORMATION_H
    #define __OPENGRAPHTHEORY_TRANSFORM_TRANSFORMATION_H

    #include <list>
    #include "../opengt.h"
    #include "../factory.h"
    #include "../intermediatesteps/intermediatesteps.h"
    using namespace std;

    namespace OpenGraphtheory
    {
        namespace Transform
        {

            class Transformer
            {
                public:
                    int MinParamCount;
                    int MaxParamCount;

                protected:
                    virtual void DoTransform(Graph& G, vector<float> parameter, IntermediateSteps::IntermediateStepHandler* intermediatestephandler) = 0;

                public:
                    static Factory<Transformer> TransformerFactory;

                    virtual ~Transformer();
                    void Transform(Graph& G, vector<float> parameter, IntermediateSteps::IntermediateStepHandler* intermediatestephandler);
                    static void Transform(Graph& G, vector<float> parameter, string transformer, IntermediateSteps::IntermediateStepHandler* intermediatestephandler);
            };

        }
    }

#endif
