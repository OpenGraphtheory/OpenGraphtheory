
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
	    if(argc < 3)
	    {
	        usage(argv[0]);
            return 1;
	    }

        string logic = argv[1];
        string query = argv[2];
        list<string> parameters;
        for(int i = 3; i < argc; i++)
            parameters.push_back(argv[i]);

		Graph G;
        cin >> G;

		ModelChecker* modelchecker = ModelChecker::ModelCheckerFactory.Produce(logic);
		if(modelchecker != NULL)
		{
		    try
		    {
                modelchecker-> ModelCheck(G, query, parameters);
                cout << G;
                delete modelchecker;
		    }
		    catch(...)
		    {
		        delete modelchecker;
		        throw;
		    }
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



class FactoryMaxNameLength : public OpenGraphtheory::FactoryEnumerator
{
    unsigned int length;
    public:
        FactoryMaxNameLength();
        void Enumerate(string name, string description, string url);
        int GetLength();
};
FactoryMaxNameLength::FactoryMaxNameLength()
{
    length = 0;
}
void FactoryMaxNameLength::Enumerate(string name, string description, string url)
{
    if(name.size() > length)
        length = name.size();
}
int FactoryMaxNameLength::GetLength()
{
    return length;
}



class FactoryLister : public OpenGraphtheory::FactoryEnumerator
{
    int length;
    ostream* os;
    public:
        FactoryLister(int Col1Width, ostream& os);
        void Enumerate(string name, string description, string url);
};
FactoryLister::FactoryLister(int Col1Width, ostream &os)
{
    length = Col1Width;
    this->os = &os;
}
void FactoryLister::Enumerate(string name, string description, string url)
{
    *os << "      " << name;
    for(int i = name.length(); i < length; i++)
        *os << " ";
    *os << "  " << description << "\n      ";
    for(int i = 0; i < length; i++)
        *os << " ";
    *os << "  " << url << "\n";
}



void usage(char* argv0)
{
	cerr << "usage: " << argv0 << " logic query\n"
	     << "   where logic can be\n";

    FactoryMaxNameLength* l = new FactoryMaxNameLength();
    Logic::ModelChecker::ModelCheckerFactory.Enumerate(l);
    FactoryLister* w = new FactoryLister(l->GetLength(), cerr);
    Logic::ModelChecker::ModelCheckerFactory.Enumerate(w);
    delete l;
    delete w;
}
