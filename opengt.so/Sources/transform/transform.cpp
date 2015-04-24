
#include "../../Headers/transform/transform.h"
using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::IntermediateSteps;

namespace OpenGraphtheory
{
    namespace Transform
    {

        Factory<Transformer> Transformer::TransformerFactory;

        Transformer::~Transformer()
        {

        }

        void Transformer::Transform(Graph& G, vector<float> parameter, IntermediateStepHandler* intermediatestephandler)
        {
            if(MinParamCount > 0 && (((int)parameter.size()) < MinParamCount))
            {
                stringstream exception;
                exception << "transformer requires at least " << MinParamCount << " parameters";
                throw exception.str().c_str();
            }

            if(MaxParamCount >= 0 && (((int)parameter.size()) > MaxParamCount))
            {
                stringstream exception;
                exception << "transformer allows at most " << MaxParamCount << " parameters";
                throw exception.str().c_str();
            }

            DoTransform(G, parameter, intermediatestephandler);
        }


        void Transformer::Transform(Graph& G, vector<float> parameter, string transformer, IntermediateSteps::IntermediateStepHandler* intermediatestephandler)
        {
            Transformer* trans = Transformer::TransformerFactory.Produce(transformer);
            if(trans == NULL)
                throw "unknown transformer";
            try
            {
                trans->Transform(G, parameter, intermediatestephandler);
                delete trans;
            }
            catch(...)
            {
                delete trans;
                throw;
            }
       }

    }
}
