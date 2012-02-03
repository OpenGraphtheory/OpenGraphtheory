
#include <iostream>
#include <list>
#include <string>
#include <stdlib.h>
#include "../../opengt.so/opengt.h"
#include "../../opengt.so/generate/generate.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Generate;

int main(int argc, char** argv)
{
    try
    {

        list<int> parameters;
        for(int i = 2; i < argc; i++)
            parameters.push_back(atoi(argv[i]));

        Graph result = Generator::Generate(parameters, string(argv[1]));
        cout << result;

    }
    catch(const char* error)
    {
        cerr << "ERROR: " << error << endl;
        return 1;
    }

    return 0;
}
