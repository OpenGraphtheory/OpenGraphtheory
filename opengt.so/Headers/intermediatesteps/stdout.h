
#ifndef __OPENGRAPHTHEORY_INTERMEDIATESTEPS_STDOUT_H
    #define __OPENGRAPHTHEORY_INTERMEDIATESTEPS_STDOUT_H

    #include <iostream>
    #include <sstream>
    #include <map>
    #include<vector>
    #include<string>

    #include "intermediatesteps.h"
    #include "../export/export.h"

    namespace OpenGraphtheory
    {
        namespace IntermediateSteps
        {

            class IntermediateStepHandlerStdout : public IntermediateStepHandler
            {
                protected:
                    static FactoryRegistrator<IntermediateStepHandler> IntermediateStepHandlerStdoutRegistrator;
                    OpenGraphtheory::Export::ExportFilter* exporter;

                public:
                    IntermediateStepHandlerStdout();
                    ~IntermediateStepHandlerStdout();
                    void Configure(std::vector<std::string> parameters);
                    void Handle(Graph* G);
            };

        }
    }

#endif
