
#include "../../Headers/logic/logic.h"

namespace OpenGraphtheory
{
    namespace Logic
    {


        Factory<ModelChecker> ModelChecker::ModelCheckerFactory;

        void ModelChecker::ModelCheck(Graph& G, std::istream& formula, list<std::string> parameters)
        {
            Formula* phi = Parse(formula);
            if(phi != NULL)
            {
                phi->Interpret(G, parameters);
                delete phi;
            }
        }

        void ModelChecker::ModelCheck(Graph& G, std::string formula, list<std::string> parameters)
        {
            stringstream s;
            s << formula;
            ModelCheck(G, s, parameters);
        }

        ModelChecker::~ModelChecker()
        {

        }


    } // namespace Logic
} // namespace OpenGraphtheory
