
#include <iostream>
#include <fstream>
#include <string>
#include "../../Headers/intermediatesteps/intermediatesteps.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace IntermediateSteps
    {

        Factory<IntermediateStepHandler> IntermediateStepHandler::IntermediateStepHandlerFactory;

        IntermediateStepHandler::IntermediateStepHandler()
        {

        }

        IntermediateStepHandler::~IntermediateStepHandler()
        {

        }

        void IntermediateStepHandler::BeginHandling()
        {

        }

        void IntermediateStepHandler::EndHandling()
        {

        }

        void IntermediateStepHandler::Configure(vector<string> parameters)
        {

        }

    }
}


