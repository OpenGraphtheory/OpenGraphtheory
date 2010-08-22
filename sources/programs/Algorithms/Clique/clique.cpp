
#include <iostream>
#include <vector>
#include <string>
#include <pthread.h>
#include "../../../opengt/opengt.h"



#include "algo_neighborhood.h"
typedef vector<Graph::VertexIterator> (*algorithm)(Graph* G, int k);



class thread_info {
	public:
		string name;
		vector<Graph::VertexIterator>* result;
		Graph *G;
		int wanted_clique_size;
		int* status;
		algorithm algo; // function pointer
};


// signature due to pthreads
void* algorithm_starter(void* pthread_info) {
	thread_info *as_thread_info = (thread_info*)(pthread_info);
	// call algorithm and store its result
	*as_thread_info->result = as_thread_info->algo(as_thread_info->G, as_thread_info -> wanted_clique_size);
	// signal "" for the main thread
	*as_thread_info->status = 1;
}


int main(int argc, char** argv) {
	
	// load graph
	Graph G;
	cin >> G;
	int wanted_clique_size = 5;
	
	
	// register algorithms
	vector< pair<string, algorithm > > algorithms;
	algorithms.push_back(pair<string, algorithm >("neighborhood", neighborhood));
	
	
	// create threads
	vector< pair<thread_info*, pthread_t > > threads;
	for(vector< pair<string, algorithm > >:: iterator algo = algorithms.begin(); algo != algorithms.end(); algo++) {
		thread_info *ti = new thread_info();
		ti -> name = algo -> first;
		ti -> result = new vector<Graph::VertexIterator>();
		ti -> G = new Graph(G);
		ti -> wanted_clique_size = wanted_clique_size;
		ti -> status = new int(0);
		ti -> algo = algo->second;
		
		pthread_t thread;
		pthread_create(&thread, NULL, algorithm_starter, ti);
		threads.push_back(pair<thread_info*, pthread_t >(ti, thread));
	}
	
	
	// wait for (at least one) finished thread
	bool we_have_a_wiener = false; // no, the name is a simpsons reference
	while(!we_have_a_wiener) {
		for( vector< pair<thread_info*, pthread_t > >:: iterator it = threads.begin(); it != threads.end(); it++) {
			if(*(it->first->status) == 1) {
				cout << "well hot dog, we have a wiener!: " << it->first->name << endl;
				
				we_have_a_wiener = true;
				break;
			}
		}
	}
	
	
	// close all threads
	for( vector< pair<thread_info*, pthread_t > >:: iterator it = threads.begin(); it != threads.end(); it++) {
		cout << "cancelling " << it->first->name << endl;
		pthread_detach(it->second);
		pthread_cancel(it->second);
		
		delete it->first->result;
		delete it->first->G;
		delete it->first->status;
		delete it->first;
	}
	
	
	return 0;
}














