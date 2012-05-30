
#include "../Headers/thread.h"

Mutex::Mutex() {
	#ifdef __unix__
		pthread_mutex_init(&mutex, NULL);
	#elif __WIN32__ || _MSC_VER || _Windows || __NT__
		InitializeCriticalSection(&mutex);
	#endif
}
Mutex::~Mutex() {
	#ifdef __unix__
		pthread_mutex_destroy(&mutex);
	#elif __WIN32__ || _MSC_VER || _Windows || __NT__
		DeleteCriticalSection(&mutex);
	#endif
}
void Mutex::Lock() {
	#ifdef __unix__
		pthread_mutex_lock(&mutex);
	#elif __WIN32__ || _MSC_VER || _Windows || __NT__
		EnterCriticalSection(&mutex);
	#endif
}
void Mutex::Unlock() {
	#ifdef __unix__
		pthread_mutex_unlock(&mutex);
	#elif __WIN32__ || _MSC_VER || _Windows || __NT__
		LeaveCriticalSection(&mutex);
	#endif
}
bool Mutex::TryLock() {
	#ifdef __unix__
		return pthread_mutex_trylock(&mutex);
	#elif __WIN32__ || _MSC_VER || _Windows || __NT__
		return TryEnterCriticalSection(&mutex) == TRUE;
	#endif
}


#ifdef __unix__
	void* ThreadWrapper(void* t) {
#elif __WIN32__ || _MSC_VER || _Windows || __NT__
	__cdecl void ThreadWrapper(void* t) {
#endif
	Thread* thread = (Thread*)(t);
	thread->Lock();
	thread->state = Thread::Initializing;
	thread->Init();
	thread->state = Thread::Running;
	thread->Unlock();
	thread->RunThread();
	thread->Lock();
	thread->state = Thread::Cleaning;
	thread->Cleanup();
	thread->state = Thread::Finished;
	thread->Unlock();
	#ifdef __unix__
		return NULL;
	#endif
}

void Thread::Start() {
	#ifdef __unix__
		pthread_create(&thread, NULL, &ThreadWrapper, this);
		pthread_detach(thread);
	#elif __WIN32__ || _MSC_VER || _Windows || __NT__
		thread = _beginthread(ThreadWrapper, 0, this);
	#endif
}

void Thread::Terminate() {
	Lock();
	if(state == Running) {
		state = Terminating;
		#ifdef __unix__
			pthread_cancel(thread);
		#elif __WIN32__ || _MSC_VER || _Windows || __NT__
			TerminateThread((void*)(thread),0);
		#endif
		state = Cleaning;
		Cleanup();
		state = Terminated;
	}
	Unlock();
}

Thread::State Thread::GetState() {
	return state;
}

Thread::~Thread() {
	Terminate();
}

void Thread::Lock() {
	mutex.Lock();
}

void Thread::Unlock() {
	mutex.Unlock();
}

bool Thread::TryLock() {
	return mutex.TryLock();
}
