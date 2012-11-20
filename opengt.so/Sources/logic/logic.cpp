
#include "../../Headers/logic/logic.h"

namespace OpenGraphtheory
{
    namespace Logic
    {


        Factory<ModelChecker> ModelChecker::ModelCheckerFactory;

        void ModelChecker::ModelCheck(Graph &G, std::istream& formula)
        {
            Formula* phi = Parse(formula);
            if(phi != NULL)
            {
                phi->Interpret(G);
                delete phi;
            }
        }

        ModelChecker::~ModelChecker()
        {

        }


    } // namespace Logic
} // namespace OpenGraphtheory
