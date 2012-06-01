
#include <iostream>
#include <stdlib.h>
#include <list>
#include "../../opengt.so/Headers/opengt.h"
#include "../../opengt.so/Headers/algorithms/algorithm.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Algorithms;

void usage(char* argv0);
int main(int argc, char** argv)
{
	try
	{
		Graph G;
        cin >> G;

		/// do your thing
		vector<string> parameters;
		int argvi = 2;
		for(; argvi < argc; argvi++)
            parameters.push_back(string(argv[argvi]));


        Algorithm* algo = Algorithm::AlgorithmFactory.Produce(argv[1]);
        algo->Run(G, parameters);
        delete algo;

        cout << G;
	}
	catch(const char* s)
	{
		cerr << argv[0] << " ERROR: " << s << "\n";
		usage(argv[0]);
		return 1;
	}

	return 0;
}

void usage(char* argv0)
{
	cerr << "usage: " << argv0 << " transformation <parameters>\n"
	     << "     where transformation can be\n";
}

