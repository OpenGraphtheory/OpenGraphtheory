
#ifndef __OPENGRAPHTHEORY_INTERMEDIATESTEPS_SHOW_H
    #define __OPENGRAPHTHEORY_INTERMEDIATESTEPS_SHOW_H

    #include <iostream>
    #include <sstream>
    #include <string>
    #include <map>

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
                    static void StopClose(bool& AbortClose);
                public:
                    IntermediateStepHandlerShow();
                    ~IntermediateStepHandlerShow();
                    void Configure(std::vector<std::string> parameters);
                    void Handle(Graph* G);
            };

        }
    }

#endif
