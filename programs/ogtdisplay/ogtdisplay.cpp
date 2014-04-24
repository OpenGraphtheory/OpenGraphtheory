#include <iostream>
#include <unistd.h>
#include "../../opengt.so/Headers/opengt.h"
#include "../../opengt.so/Headers/visualize/GraphWindow.h"
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
        float VertexRadius = -1;
        float EdgeWidth = -1;
        for(int i = 1; i < argc; i++)
        {
            if(string(argv[i]) == "--vertexcoloring" && argc > i+1)
                vertexcoloring = argv[i+1];
            if(string(argv[i]) == "--edgecoloring" && argc > i+1)
                edgecoloring = argv[i+1];
            if(string(argv[i]) == "--edgewidth" && argc > i+1)
            {
                stringstream s;
                s << argv[i+1];
                s >> EdgeWidth;
            }
            if(string(argv[i]) == "--vertexradius" && argc > i+1)
            {
                stringstream s;
                s << argv[i+1];
                s >> VertexRadius;
            }
        }


        GraphWindow Win(800, 600, &G, "http://www.Open-Graphtheory.org", vertexcoloring,
                        edgecoloring, EdgeWidth, VertexRadius);
        Win.WaitUntilClosed();


        cout << G;
	}
	catch(const char* s)
	{
		cerr << argv[0] << " ERROR: " << s << "\n";
		return 1;
	}

	return 0;
}
