
#ifndef __OPENGRAPHTHEORY_INTERMEDIATE_WRITEFILE_H
    #define __OPENGRAPHTHEORY_INTERMEDIATE_WRITEFILE_H

    #include<vector>
    #include<string>
    #include "intermediatesteps.h"
    #include "../export/export.h"

    namespace OpenGraphtheory
    {
        namespace IntermediateSteps
        {

            class IntermediateStepHandlerWriteFile : public IntermediateStepHandler
            {
                protected:
                    static FactoryRegistrator<IntermediateStepHandler> IntermediateStepHandlerWriteFileRegistrator;

                    OpenGraphtheory::Export::ExportFilter* exporter;

                    int index;
                    int digits;
                    string prefix;
                    string suffix;

                public:
                    IntermediateStepHandlerWriteFile();
                    ~IntermediateStepHandlerWriteFile();
                    void Configure(vector<string> parameters);
                    void Handle(Graph* G);
            };

        }
    }

#endif
