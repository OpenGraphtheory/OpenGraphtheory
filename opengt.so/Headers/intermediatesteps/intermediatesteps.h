
#ifndef __OPENGRAPHTHEORY_INTERMEDIATESTEPS_INTERMEDIATESTEPS_H
    #define __OPENGRAPHTHEORY_INTERMEDIATESTEPS_INTERMEDIATESTEPS_H

    #include <string>
    #include <iostream>
    #include "../opengt.h"
    #include "../factory.h"

    namespace OpenGraphtheory
    {
        namespace IntermediateSteps
        {

            class IntermediateStepHandler
            {
                public:
                    static Factory<IntermediateStepHandler> IntermediateStepHandlerFactory;
                    IntermediateStepHandler();
                    virtual ~IntermediateStepHandler();

                    virtual void Configure(vector<string> parameters);
                    virtual void BeginHandling();
                    virtual void Handle(Graph* G) = 0;
                    virtual void EndHandling();
            };

        }
    }

#endif
