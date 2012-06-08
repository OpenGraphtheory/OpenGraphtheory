
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "../../Headers/intermediatesteps/stdout.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Export;

namespace OpenGraphtheory
{
    namespace IntermediateSteps
    {

        FactoryRegistrator<IntermediateStepHandler> IntermediateStepHandlerStdout::IntermediateStepHandlerStdoutRegistrator(
            &IntermediateStepHandler::IntermediateStepHandlerFactory, "stdout", new DefaultInstantiator<IntermediateStepHandler, IntermediateStepHandlerStdout>(
                "stdout", "prints the graphs to stdout (using an export-filter, if present)",""));


        IntermediateStepHandlerStdout::IntermediateStepHandlerStdout()
        {
            exporter = NULL;
        }

        IntermediateStepHandlerStdout::~IntermediateStepHandlerStdout()
        {
            if(exporter != NULL)
                delete exporter;
        }

        void IntermediateStepHandlerStdout::Configure(vector<string> parameters)
        {
            if(parameters.size() > 0)
                exporter = ExportFilter::ExportFilterFactory.Produce(parameters[0]);
        }

        void IntermediateStepHandlerStdout::Handle(Graph* G)
        {
            if(exporter != NULL)
                exporter->Export(*G, cout, "", "");
            else
                cout << *G;
        }

    }
}
