
#include <sstream>
#include <list>
#include "../../Headers/generate/generate.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        Factory<Generator> Generator::GeneratorFactory;

        Generator::~Generator()
        {

        }

        Generator::Generator()
        {
            MinParamCount = -1;
            MaxParamCount = -1;
        }

        Graph Generator::Generate(list<int> parameter)
        {
            if(MinParamCount > 0 && (((int)parameter.size()) < MinParamCount))
            {
                stringstream exception;
                exception << "generator requires at least " << MinParamCount << " parameters";
                throw exception.str().c_str();
            }
            if(MaxParamCount >= 0 && (((int)parameter.size()) > MaxParamCount))
            {
                stringstream exception;
                exception << "generator allows at most " << MaxParamCount << " parameters";
                throw exception.str().c_str();
            }
            return DoGenerate(parameter);
        }

        Graph Generator::Generate(string generator, list<int> parameter)
        {
            Generator* gen = GeneratorFactory.Produce(generator);
            if(gen == NULL)
                throw "unknown generator";
            Graph result = gen->Generate(parameter);
            delete gen;
            return result;
        }

    }
}

