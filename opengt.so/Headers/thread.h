
#ifndef __THREAD_H
	#define __THREAD_H

	#ifdef __unix
		#ifndef __unix__
			#define __unix__
		#endif
	#endif
	#ifndef __unix__
        #ifdef __APPLE__ & __MACH__
            #define __unix__
        #endif
	#endif

	#ifdef __unix__
		#include <pthread.h>
	#elif __WIN32__ || _MSC_VER || _Windows || __NT__
		#include <windows.h>
		#include <process.h>
		#include <stdint.h>
	#else
		#error "No multithreading mode found for target operating system"
	#endif


	class Mutex
	{
		private:
			#ifdef __unix__
				pthread_mutex_t mutex;
			#elif __WIN32__ || _MSC_VER || _Windows || __NT__
				CRITICAL_SECTION mutex;
			#endif
		public:
			Mutex();
			~Mutex();

			void Lock();
			bool TryLock();
			void Unlock();
	};

	#ifdef __unix__
		void* ThreadWrapper(void*);
	#elif __WIN32__ || _MSC_VER || _Windows || __NT__
		void ThreadWrapper(void*);
	#endif

	class Thread
	{
		#ifdef __unix__
			friend void* ThreadWrapper(void*);
		#elif __WIN32__ || _MSC_VER || _Windows || __NT__
			friend void ThreadWrapper(void*);
		#endif

		public:
			enum State {Initializing, Running, Cleaning, Finished, Terminating, Terminated};
		protected:
			Mutex mutex;
			State state;
			#ifdef __unix__
				pthread_t thread;
			#elif __WIN32__ || _MSC_VER || _Windows || __NT__
				uintptr_t thread;
			#endif

			virtual void Init() {}
			virtual void RunThread() = 0;
			virtual void Cleanup() {}
		public:
			void Start();
			void Terminate();
			State GetState();

			void Lock();
			void Unlock();
			bool TryLock();
			virtual ~Thread();
	};

#endif
