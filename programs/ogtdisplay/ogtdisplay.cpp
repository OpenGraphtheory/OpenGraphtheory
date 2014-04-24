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
		Graph G1;
		Graph G2;
		Graph* pG1 = &G1;
		Graph* pG2 = &G2;

        /// select input source

        string vertexcoloring = "";
        string edgecoloring = "";
        float VertexRadius = -1;
        float EdgeWidth = -1;
        bool autoclose = true;

        for(int i = 1; i < argc; i++)
        {
            if(string(argv[i]) == "--noclose")
                autoclose = false;
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


        GraphWindow Win(800, 600, NULL, "http://www.Open-Graphtheory.org", vertexcoloring,
                        edgecoloring, EdgeWidth, VertexRadius);


        do
        {
            try
            {
                cin >> *pG2;
                Win.Display(pG2);
                cout << *pG2;

                Graph* temp = pG1;
                pG1 = pG2;
                pG2 = temp;
            }
            catch(...) { }
        } while(!cin.eof());

        if(!autoclose)
            Win.WaitUntilClosed();
	}
	catch(const char* s)
	{
		cerr << argv[0] << " ERROR: " << s << "\n";
		return 1;
	}

	return 0;
}
