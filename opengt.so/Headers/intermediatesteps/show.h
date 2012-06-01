
#ifndef __OPENGRAPHTHEORY_INTERMEDIATE_SHOW_H
    #define __OPENGRAPHTHEORY_INTERMEDIATE_SHOW_H

    #include "intermediatesteps.h"
    #include "../visualize/GraphWindow.h"

    namespace OpenGraphtheory
    {
        namespace IntermediateSteps
        {

            class IntermediateStepHandlerShow : public IntermediateStepHandler
            {
                protected:
                    static FactoryRegistrator<IntermediateStepHandler> IntermediateStepHandlerShowRegistrator;
                    OpenGraphtheory::Visualization::GraphWindow* win;
                public:
                    IntermediateStepHandlerShow();
                    ~IntermediateStepHandlerShow();
                    void Configure(vector<string> parameters);
                    void Handle(Graph* G);
            };

        }
    }

#endif
