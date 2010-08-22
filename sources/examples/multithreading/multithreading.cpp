
#include<iostream>
#include<vector>


/*

put your algorithms here

*/


// adapt this class to your signatures
class algorithm_info {
	public:
		int (*algorithm)(int parameter1, short parameter2); // function pointer
		int parameter1;
		short parameter2;

		algorithm_info(int (*algo)(int, short), int param1, short param2 ) {
			algorithm = algo;
			parameter1 = param1;
			parameter2 = param2;
		}
};

class thread_info {
	public:
		string name;
		pthread thread;
		int status;
};

// the signature is mandatory due to pthreads
void* algorithm_starter(void* asvoid) {
	algorithm_info *algoinfo = (algorithm_info*)(asvoid);
	// adapt this call to your signatures
	int result = algoinfo->algorithm(algoinfo->parameter1, algoinfo->parameter2);
}



int main(int argc, char** argv) {
	algorithm.push_back(pair<string,algorithm_info>("myalgo", new algorithm_info(functionpointer, parameter1, parameter2)));


}










