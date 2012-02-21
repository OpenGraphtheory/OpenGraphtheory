
#include <list>
#include <string>
#include "../opengt.h"

namespace OpenGraphtheory
{
    namespace Generate
    {

        class Generator
        {
            public:
                string name;
                string description;
                int MinParamCount;
                int MaxParamCount;
            private:
                Graph (*GeneratorFunction)(list<int> parameter);

            public:
                static map<string, Generator> GetGenerators();
                Generator();
                Generator(string name, string description, int MinParamCount, int MaxParamCount, Graph GeneratorFunction(list<int> parameter));

                Graph Generate(list<int> parameter);
                static Graph Generate(list<int> parameter, string generator);
        };


        Graph GenerateCircle(list<int> parameter);
        Graph GenerateStar(list<int> parameter);
        Graph GeneratePath(list<int> parameter);
        Graph GenerateComplete(list<int> parameter);
        Graph GenerateGrid(list<int> parameter);
        Graph GenerateTree(list<int> parameter);
        Graph GenerateRandom(list<int> parameter);
    }
}

