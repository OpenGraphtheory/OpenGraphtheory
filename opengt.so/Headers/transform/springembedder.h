#ifndef __OPENGRAPHTHEORY_TRANSFORM_SPRINGEMBEDDER_H
    #define __OPENGRAPHTHEORY_TRANSFORM_SPRINGEMBEDDER_H

    #include <list>
    #include "../opengt.h"
    #include "../visualize/vectornd.h"
    #include "../factory.h"
    #include "transform.h"

    #include <iostream>
    #include <math.h>
    #include <stdlib.h> // for random numbers

    namespace OpenGraphtheory
    {
        namespace Transform
        {

            class TransformerSPRINGEMBEDDER : public Transformer
            {
                protected:
                    OpenGraphtheory::Visualization::VectorND hooke(OpenGraphtheory::Visualization::VectorND u, OpenGraphtheory::Visualization::VectorND v);
                    OpenGraphtheory::Visualization::VectorND coulomb(OpenGraphtheory::Visualization::VectorND u, OpenGraphtheory::Visualization::VectorND v);

                    static FactoryRegistrator<Transformer> TransformerSpringEmbedderRegistrator;


                    std::vector<float> dimension_limits;
                    float c_repel;  // force with which vertices push each other off
                    float c_spring;  // force with which adjacent vertices attract each other
                                                // must not be 0 (or division by zero error will happen)
                    float friction;

                    float unstressed_spring_length; // if distance < this, then no more force between them
                    float delta;  // scaling factor to make the movement more smooth
                    float movement_threshold;    // stop if no vertex moves more than this far

                    int iteration;
                    int nextincrease;

                    void SpringEmbed(Graph& G, std::vector<float> dimension_limits,
                                     IntermediateSteps::IntermediateStepHandler* intermediatestephandler = NULL);


                public:
                    TransformerSPRINGEMBEDDER();
                    void DoTransform(OpenGraphtheory::Graph& G, std::vector<float> parameters, IntermediateSteps::IntermediateStepHandler* intermediatestephandler = NULL);
            };

        }
    }

#endif
