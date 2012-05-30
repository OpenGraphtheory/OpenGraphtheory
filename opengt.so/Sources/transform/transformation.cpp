
#include "../../Headers/transform/transformation.h"
using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Transform
    {

        Factory<Transformer> Transformer::TransformerFactory;

        Transformer::~Transformer()
        {

        }

        void Transformer::Transform(Graph& G, vector<float> parameter)
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

            DoTransform(G, parameter);
        }


        void Transformer::Transform(Graph& G, vector<float> parameter, string transformer)
        {
            Transformer* trans = Transformer::TransformerFactory.Produce(transformer);
            if(trans == NULL)
                throw "unknown transformer";
            trans->Transform(G, parameter);
            delete trans;
       }

    }
}
