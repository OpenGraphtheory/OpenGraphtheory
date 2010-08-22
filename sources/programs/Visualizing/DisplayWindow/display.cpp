 
#include "DisplayWindow.h"
#include "../../../opengt/opengt.h"
#include <iostream>
#include <sstream>
#include <unistd.h> // for usleep
using namespace std;

int sleep_nanosec = 41666; // must be less than 1,000,000
int update_interval = 100; // read this many changes before updating the window

int main(int argc, char** argv) {
	DisplayWindow Win(800,600,"Open-Graphtheory.org Display Window");
	Graph G;
	cin >> G;
	
	string input;
	int iterations = 0;
	do {
		cin >> input;
		if(!cin.good())
			break;

		if(input == "F") {
			// draw graph
			Win.Clear();
			for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++) {
				Win.Circle(v.GetX(), v.GetY(), 4);
				for(Graph::VertexIterator w = v->BeginAdjacentVertices(); w != v->EndAdjacentVertices(); w++) {
					Win.Line(v.GetX(),v.GetY(),w.GetX(),w.GetY());
				}
			}
			Win.Flush();
			usleep(sleep_nanosec);
		} else if(input == "X") {
			int vint;
			cin >> vint;
			Graph::VertexIterator vit = G.GetVertexByXML_ID(vint);
			float temp;
			cin >> temp;
			if(vit != G.EndVertices()) {
				 vit.SetX(temp);
			}
		} else if(input == "Y") {
			int vint;
			cin >> vint;
			Graph::VertexIterator vit = G.GetVertexByXML_ID(vint);
			float temp;
			cin >> temp;
			if(vit != G.EndVertices()) {
				 vit.SetY(temp);
			}
		}
		
	} while(input != "quit");

	Win.WaitUntilClosed();
	return 0;
}
