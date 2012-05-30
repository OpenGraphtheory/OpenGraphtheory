
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "../../opengt.so/Headers/opengt.h"
#include "../../opengt.so/Headers/export/export.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Export;

void usage(char* argv0);
int main(int argc, char** argv)
{
	try
	{
		Graph G;


		/// select input source
		if(!isatty(fileno(stdin)))
			cin >> G;
		else
			if(argc > 2)
				G.LoadFromFile(argv[2]);
			else
				throw "no input source specified";


		/// select output destination
		ostream *os;
		bool mustdeleteOS = false;
		if(argc > 3) {
			os = new ofstream(argv[3]);
			mustdeleteOS = true;
		} else {
			os = &cout;
		}


		/// export
		string format = (argc > 1) ? argv[1] : "--help";
		ExportFilter* exportfilter = ExportFilter::ExportFilterFactory.Produce(format);
		if(exportfilter != NULL)
		{
		    try
		    {
                exportfilter->Export(G, *os);
		    }
		    catch(...)
		    {
		        delete exportfilter;
		        throw;
		    }
		    delete exportfilter;
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
    map<string, ExportFilter*> Filters ;//= ExportFilter::GetExportFilters();

	cerr << "usage: " << argv0 << " format [sourcefile] [destinationfile]\n"
	     << "     where format can be\n";

    unsigned int maxlength = 0;
    for(map<string, ExportFilter*>::iterator i = Filters.begin(); i != Filters.end(); i++)
        if(i->first.length() > maxlength)
            maxlength = i->first.length();

    for(map<string, ExportFilter*>::iterator i = Filters.begin(); i != Filters.end(); i++)
    {
//        cerr << "          " << i->first << string(maxlength - i->first.length() + 2, ' ') << i->second->Description << "\n";
//        cerr << "          " << string(maxlength + 2, ' ') << "  " << i->second.URL << "\n";
    }
}
