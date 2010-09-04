#include <iostream>
#include <cstdlib>

#ifdef __unix__
	#define Sleep(x) usleep(100000*x)
#elif __WIN32__ || _MSC_VER || _Windows || __NT__
	#include <windows.h>
#endif

#include "../../opengt/thread.h"

using namespace std;



class CharPrintThread : public Thread {
	private:
		char c;
		int t;
	protected:
		void RunThread() {
			while(1) {
				cout << c;
				cout.flush();
				Sleep(t);
			}
		}
	public:
		CharPrintThread(char C, int T) {
			c=C;
			t=T;
		}
};



int main(int argc, char** argv) {
	CharPrintThread a('a', 2);
	CharPrintThread b('b', 1);

	
	cout << "Press Return to start thread a\n"; cin.ignore(1);
	a.Start();
	cout << "Press Return to start thread b\n"; cin.ignore(1);
	b.Start();
	
	cout << "Press Return to stop thread a\n"; cin.ignore(1);
	a.Terminate();
	cout << "Press Return to stop thread b\n"; cin.ignore(1);
	b.Terminate();
	
	
	cout << "Press Return to quit\n"; cin.ignore(1);
	return 0;
}










