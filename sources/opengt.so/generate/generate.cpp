
#include <sstream>
#include <map>
#include <list>
#include "../opengt.h"
#include "generate.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        Generator::Generator()
        {

        }

        Generator::Generator(string name, string description, int MinParameterCount, int MaxParameterCount, Graph GeneratorFunction(list<int> parameter))
        {
            this->name = name;
            this->description = description;
            this->MinParamCount = MinParameterCount;
            this->MaxParamCount = MaxParameterCount;
            this->GeneratorFunction = GeneratorFunction;
        }



        Graph Generator::Generate(list<int> parameter)
        {
            if(MinParamCount > 0 && (((int)parameter.size()) < MinParamCount))
            {
                stringstream exception;
                exception << "generator \"" << name << "\" requires at least " << MinParamCount << " parameters";
                throw exception.str().c_str();
            }
            if(MaxParamCount >= 0 && (((int)parameter.size()) > MaxParamCount))
            {
                stringstream exception;
                exception << "generator \"" << name << "\" allows at most " << MaxParamCount << " parameters";
                throw exception.str().c_str();
            }
            return this->GeneratorFunction(parameter);
        }



        map<string, Generator> Generator::GetGenerators()
        {
            map<string, Generator> result;

            result["circle"] = Generator("circle",
                                     "To generate a circle-graph",
                                     1, 1,
                                     GenerateCircle);
            result["path"] = Generator("path",
                                     "To generate a path-graph",
                                     1, 1,
                                     GeneratePath);
            result["star"] = Generator("star",
                                     "To generate a star-graph",
                                     1, 1,
                                     GenerateStar);
            result["complete"] = Generator("complete",
                                     "To generate a complete graph",
                                     1, -1,
                                     GenerateComplete);
            result["grid"] = Generator("grid",
                                     "To generate a grid-graph",
                                     2, 2,
                                     GenerateGrid);
            result["tree"] = Generator("tree",
                                     "To generate a tree",
                                     0, -1,
                                     GenerateTree);
            result["random"] = Generator("random",
                                     "To generate a random graph",
                                     2, 2,
                                     GenerateRandom);
            return result;
        }


        Graph Generator::Generate(list<int> parameter, string generator)
        {
            map<string, Generator> Generators = GetGenerators();
            if(Generators.find(generator) == Generators.end())
                throw "unknown generator";
            return Generators[generator].Generate(parameter);
        }
    }
}

