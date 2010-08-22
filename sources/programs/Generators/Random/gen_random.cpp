
#include "../../../opengt/opengt.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <set>
#include <math.h>
using namespace std;

int main(int argc, char** argv) {
	if(argc < 3) {
		cerr << "usage: " << argv[0] << " n(int) m(int)\n";
		return 1;
	}
	
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	if(m > n*(n-1)) {
		cerr << "m cannot be larger than n*(n-1)/2\n";
		return 1;
	}
	Graph G(n);
	srand(time(NULL));

	vector<int> indices(n*(n-1)/2);
	for(int i = (n*(n-1)/2)-1; i >= 0; i--)
		indices[i] = i;

	for(int i = (n*(n-1)/2)-1; i > 0; i--) {
		int rnd_idx = rand() % (i+1);
		int temp = indices[rnd_idx];
		indices[rnd_idx] = indices[i];
		indices[i] = temp;
	}

	set<int> selected_indices;
	for(int i = m-1; i >= 0; i--)
		selected_indices.insert(indices[i]);

	int i = 0;
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++) {
		Graph::VertexIterator v1 = v;
		for(v1++; v1 != G.EndVertices(); v1++) {
			if(selected_indices.find(i) != selected_indices.end())
				G.AddEdge(v,v1);
			i++;
		}
	}

	cout << G; // stream it to the next program
	return 0;
}
