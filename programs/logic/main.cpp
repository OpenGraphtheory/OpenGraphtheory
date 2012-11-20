
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "../../opengt.so/Headers/opengt.h"
#include "../../opengt.so/Headers/logic/logic.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Logic;

void usage(char* argv0);
int main(int argc, char** argv)
{
	try
	{
		Graph G;
        cin >> G;

        string logic = argv[1];
        string query = argv[2];
        stringstream querystream;
        querystream << query << "\n";

		ModelChecker* modelchecker = ModelChecker::ModelCheckerFactory.Produce(logic);
		if(modelchecker != NULL)
		{
		    try
		    {
                modelchecker-> ModelCheck(G, querystream);
                cout << G;
		    }
		    catch(...)
		    {
		        delete modelchecker;
		        throw;
		    }
		    delete modelchecker;
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
    map<string, ModelChecker*> ModelCheckers;//= ExportFilter::GetExportFilters();

	cerr << "usage: " << argv0 << " logic query\n"
	     << "     where logic can be\n";

    unsigned int maxlength = 0;
    for(map<string, ModelChecker*>::iterator i = ModelCheckers.begin(); i != ModelCheckers.end(); i++)
        if(i->first.length() > maxlength)
            maxlength = i->first.length();

    for(map<string, ModelChecker*>::iterator i = ModelCheckers.begin(); i != ModelCheckers.end(); i++)
    {
//        cerr << "          " << i->first << string(maxlength - i->first.length() + 2, ' ') << i->second->Description << "\n";
//        cerr << "          " << string(maxlength + 2, ' ') << "  " << i->second.URL << "\n";
    }
}
