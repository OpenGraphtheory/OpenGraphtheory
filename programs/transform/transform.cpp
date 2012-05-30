
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
#include "../../opengt.so/Headers/transform/transformation.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Transform;

void usage(char* argv0);
int main(int argc, char** argv)
{
	try
	{
		Graph G;
        cin >> G;

		/// do your thing
		vector<float> parameters;
		for(int i = 2; i < argc; i++)
            parameters.push_back(atof(argv[i]));

        Transformer::Transform(G, parameters, argv[1]);

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



