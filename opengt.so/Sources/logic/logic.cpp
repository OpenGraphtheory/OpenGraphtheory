
#include "../../Headers/logic/logic.h"
using namespace std;

namespace OpenGraphtheory
{
    namespace Logic
    {


        Factory<ModelChecker> ModelChecker::ModelCheckerFactory;

        void ModelChecker::ModelCheck(Graph& G, istream& formula, list<string> parameters)
        {
            Formula* phi = Parse(formula);
            if(phi != NULL)
            {
                phi->Interpret(G, parameters);
                delete phi;
            }
        }

        void ModelChecker::ModelCheck(Graph& G, string formula, list<string> parameters)
        {
            stringstream s;
            s << formula;
            ModelCheck(G, s, parameters);
        }

        ModelChecker::~ModelChecker()
        {

        }

        Formula::~Formula()
        {

        }

    } // namespace Logic
} // namespace OpenGraphtheory
