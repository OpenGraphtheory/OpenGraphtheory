
#include <iostream>
#include <math.h>
#include <stdlib.h> // for random numbers
#include "../../../opengt/opengt.h"
using namespace std;


float width       =  800;
float height      =  600;

float c_repel     =    1;    // force with which vertices push each other off
float c_spring    =    1;  // force with which adjacent vertices attract each other
                             // must not be 0 (or division by zero error will happen)
float unstressed_spring_length = 100; // if distance < this, then no more force between them
float delta       =    0.005;  // scaling factor to make the movement more smooth
float threshold   =    3;    // stop if this no vertex moves more than this far

int update_interval = 20;
long usleep_after_update = 41666; //83333;


// -----------------------------------------------------------------------------


pair<float,float> coulomb(float x1, float y1, float x2, float y2) {
	// coulomb's law:
	//                 u-v    c_repel
	// f_repel(u,v) = ----- * -------
	//                |u-v|   |u-v|²

	float distance = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
	if(distance*distance < 0.001)
		return pair<float,float>(0,0);
	distance /= unstressed_spring_length;
	float xe = (x2-x1)/distance;
	float ye = (y2-y1)/distance;
	float factor = c_repel / (distance*distance);
	return pair<float,float>(factor*xe, factor*ye);
}

pair<float,float> hooke(float x1, float y1, float x2, float y2) {
	// hooke's law:
	// attraction force of a spring is proportional to distance
	
	float distance = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
	float xe = (x1-x2)/distance;
	float ye = (y1-y2)/distance;
	float factor = c_spring * (distance-unstressed_spring_length);
	return pair<float,float>(factor*xe, factor*ye);
}

pair<float,float> vector_add(pair<float,float> a, pair<float,float> b) {
	return pair<float,float>(a.first+b.first, a.second+b.second);
}
pair<float,float> vector_subtract(pair<float,float> a, pair<float,float> b) {
	return pair<float,float>(a.first-b.first, a.second-b.second);
}

// -----------------------------------------------------------------------------


int main(int argc, char** argv) {

	Graph G;
	cin >> G;
	vector<pair<float,float> > tractions;

	// init
	srand ( time(NULL) );
	int i = 0;
	for(Graph::VertexIterator a = G.BeginVertices(); a != G.EndVertices(); a++) {
		a.SetX(rand());
		a.SetX(a.GetX() - floor(a.GetX()/width)*width);
		a.SetY(rand());
		a.SetY(a.GetY() - floor(a.GetY()/height)*height);
		// should make sure that no two vertices have the same position
		a.SetTag(i++);
		tractions.push_back(pair<float,float>(0,0));
	}
	cerr << G;

	float max_movement;
	i = 0;
	do {
		// compute movement
		max_movement = 0;
		for(Graph::VertexIterator a = G.BeginVertices(); a != G.EndVertices(); a++) {
			pair<float,float> traction(0,0);
		
			// compute forces on a by the other vertices
			for(Graph::VertexIterator b = G.BeginVertices(); b != G.EndVertices(); b++) {
				if(b==a)
					continue;
				// force on a by vertex b

				traction = vector_add(traction, coulomb(a.GetX(), a.GetY(), b.GetX(), b.GetY()));
				if(a->Adjacent(b))
					traction = vector_subtract(traction, hooke(a.GetX(), a.GetY(), b.GetX(), b.GetY()));
			}
			
			tractions[a.GetTag()] = traction;
			// for the loop-condition
			if(max_movement < traction.first*traction.first + traction.second*traction.second)
				max_movement = traction.first*traction.first + traction.second*traction.second;
		}
		
		// execute movement
		Graph::VertexIterator a = G.BeginVertices();
		for(; a != G.EndVertices(); a++) {
			a.SetX( a.GetX() + delta * tractions[a.GetTag()].first  );
			a.SetY( a.GetY() + delta * tractions[a.GetTag()].second );

			// loop condition uses requested displacement -> might create an infinite loop
			if(a.GetX() < 0)
				a.SetX(0);
			if(a.GetX() > width)
				a.SetX(width);
			if(a.GetY() < 0)
				a.SetY(0);
			if(a.GetY() > height)
				a.SetY(height);
			
			cerr << "X " << a.GetID() << " " << a.GetX() << "\n";
			cerr << "Y " << a.GetID() << " " << a.GetY() << "\n";
			if(--i <= 0) { // update
				cerr << "F\n";
				i = update_interval;
				usleep(usleep_after_update);
			}
		}
		
	} while(max_movement > threshold*threshold);

	cerr << "quit\n";
	cout << G;
	return 0;
}

