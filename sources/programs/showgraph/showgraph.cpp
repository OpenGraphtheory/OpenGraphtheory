#include <iostream>
#include "../../opengt.so/opengt.h"
#include "../../opengt.so/visualize/GraphWindow.h"
using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

int main(int argc, char** argv)
{
	try
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

        string vertexcoloring = "";
        string edgecoloring = "";
        for(int i = 1; i < argc; i++)
        {
            if(string(argv[i]) == "--vertexcoloring" && argc > i+1)
                vertexcoloring = argv[i+1];
            if(string(argv[i]) == "--edgecoloring" && argc > i+1)
                edgecoloring = argv[i+1];
        }

        /// do your thing
        GraphWindow Win(800, 600, &G, "http://www.Open-Graphtheory.org", vertexcoloring, edgecoloring);
        Win.WaitUntilClosed();


		/// select output destination
		if(argc > 2)
			G.SaveToFile(argv[2]);
		else
			cout << G;
	}
	catch(const char* s)
	{
		cerr << argv[0] << " ERROR: " << s << "\n";
		return 1;
	}

	return 0;
}
