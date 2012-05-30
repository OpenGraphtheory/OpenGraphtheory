
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "../../opengt.so/Headers/opengt.h"
#include "../../opengt.so/Headers/import/import.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Import;

void usage(char* argv0);
int main(int argc, char** argv)
{
	try
	{
		Graph G;

		/// import
		if(argc > 1)
		{
            G = ImportFilter::Import(cin, argv[1]);
		    cout << G;
		}
		else
			usage(argv[0]);
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
	cerr << "usage: " << argv0 << " format [sourcefile] [destinationfile]\n"
	     << "     where format can be\n";
}
