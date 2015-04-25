
/*

example program 1
=================

#include <iostream>
#include "../../opengt.so/opengt.h"

using namespace std;
using namespace OpenGraphtheory;

int main(int argc, char** argv)
{
    Graph G;
    cin >> G;

    /// do your thing

    cout << G;
	return 0;
}

example program 2
=================


#include <iostream>
#include <fstream>
#include <stdio.h>
#include "../../opengt.so/opengt.h"

using namespace std;
using namespace OpenGraphtheory;

int main(int argc, char** argv)
{
    Graph G;

    /// select input source
    if(!isatty(fileno(stdin)))
        cin >> G;
    else
        if(argc > 1)
            G.LoadFromFile(argv[1]);
        else
            throw "no input source specified";


    /// do your thing



    /// select output destination
    if(argc > 2)
        G.SaveToFile(argv[2]);
    else
        cout << G;
	return 0;
}
*/


#include <iostream>
#include <stdlib.h>
#include <list>
#include "../../opengt.so/Headers/opengt.h"
#include "../../opengt.so/Headers/transform/transform.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Transform;
using namespace OpenGraphtheory::IntermediateSteps;

void usage(char* argv0);
int main(int argc, char** argv)
{
	try
	{
		Graph G;
        cin >> G;
        string sIntermediatestephandler = "";
        IntermediateStepHandler* intermediatestephandler = NULL;

		/// do your thing
		vector<float> parameters;
		int argvi = 2;
		for(; argvi < argc; argvi++)
		{
		    if(string(argv[argvi]) == "--intermediatesteps")
		    {
		        if(argc > argvi+1)
                    sIntermediatestephandler = argv[argvi+1];
                break;
		    }
            parameters.push_back(atof(argv[argvi]));
		}

		if(sIntermediatestephandler != "")
		{
            intermediatestephandler = IntermediateStepHandler::IntermediateStepHandlerFactory.Produce(sIntermediatestephandler);
            if(intermediatestephandler != NULL)
            {
                argvi += 2;
                vector<string> intermediatestephandlerparameters;
                for(; argvi < argc; argvi++)
                    intermediatestephandlerparameters.push_back(argv[argvi]);
                intermediatestephandler->Configure(intermediatestephandlerparameters);
                intermediatestephandler->BeginHandling();
            }
		}


        Transformer::Transform(G, parameters, argv[1], intermediatestephandler);
        cout << G;

        if(intermediatestephandler != NULL)
        {
            intermediatestephandler->EndHandling();
            delete(intermediatestephandler);
        }

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
	cerr << "usage: " << argv0 << " transformation <parameters>\n"
	     << "   where transformation can be\n";


    FactoryMaxNameLength* l = new FactoryMaxNameLength();
    Transform::Transformer::TransformerFactory.Enumerate(l);
    FactoryLister* w = new FactoryLister(l->GetLength(), cerr);
    Transform::Transformer::TransformerFactory.Enumerate(w);
    delete l;
    delete w;
}



