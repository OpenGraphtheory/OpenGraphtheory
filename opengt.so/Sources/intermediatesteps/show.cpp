
#include "../../Headers/intermediatesteps/show.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace IntermediateSteps
    {

        FactoryRegistrator<IntermediateStepHandler> IntermediateStepHandlerShow::IntermediateStepHandlerShowRegistrator(
            &IntermediateStepHandler::IntermediateStepHandlerFactory, "show", new DefaultInstantiator<IntermediateStepHandler, IntermediateStepHandlerShow>(
                "show", "shows the intermediate steps in an X11 window",""));


        IntermediateStepHandlerShow::IntermediateStepHandlerShow()
        {
            win = NULL;
        }

        IntermediateStepHandlerShow::~IntermediateStepHandlerShow()
        {
            if(win != NULL)
                delete win;
        }

        void IntermediateStepHandlerShow::Configure(vector<string> parameters)
        {
            win = new GraphWindow(800, 600, NULL, "http://www.Open-Graphtheory.org");
        }

        void IntermediateStepHandlerShow::Handle(Graph* G)
        {
            if(win != NULL)
                win->Display(G);
        }

    }
}
