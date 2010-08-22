
#include "../../../opengt/opengt.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;

int main(int argc, char** argv) {
	if(argc < 2) {
		cerr << "usage: " << argv[0] << " <int>\n";
		return 1;
	}
	
	int n = atoi(argv[1]);
	Graph G(n);
	int i = 0;
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++) {
		v.SetX(100*cos(i*(2*3.1415926)/n));
		v.SetY(100*sin(i*(2*3.1415926)/n));
		Graph::VertexIterator v1 = v;
		v1++;
		if(v1 == G.EndVertices())
			G.AddEdge(v, G.BeginVertices());
		else
			G.AddEdge(v,v1);
		i++;
	}

	cout << G; // stream it to the next program
	return 0;
}
