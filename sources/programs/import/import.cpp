
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "../../opengt.so/opengt.h"
#include "../../opengt.so/import/import.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Import;

void usage(char* argv0);
int main(int argc, char** argv)
{
	try
	{
		Graph G;


		/// select output destination
		ostream *os;
		bool mustdeleteOS = false;
		if(argc > 3) {
			os = new ofstream(argv[3]);
			mustdeleteOS = true;
		} else {
			os = &cout;
		}


		/// import
		string format = (argc > 1) ? argv[1] : "--help";
        map<string, ImportFilter> Filters = ImportFilter::GetImportFilters();
		if(Filters.find(format) != Filters.end())
		{
            if(argc > 2)
                G = Filters[format].Import(argv[2]);
            else
                G = Filters[format].Import(cin);

		    *os << G;
		}
		else
			usage(argv[0]);


		if(mustdeleteOS)
			delete os;
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
    map<string, ImportFilter> Filters = ImportFilter::GetImportFilters();

	cerr << "usage: " << argv0 << " format [sourcefile] [destinationfile]\n"
	     << "     where format can be\n";

    unsigned int maxlength = 0;
    for(map<string, ImportFilter>::iterator i = Filters.begin(); i != Filters.end(); i++)
        if(i->first.length() > maxlength)
            maxlength = i->first.length();

    for(map<string, ImportFilter>::iterator i = Filters.begin(); i != Filters.end(); i++)
    {
        cerr << "          " << i->first << string(maxlength - i->first.length() + 2, ' ') << i->second.description << "\n";
        cerr << "          " << string(maxlength + 2, ' ') << "  " << i->second.URL << "\n";
    }
}
