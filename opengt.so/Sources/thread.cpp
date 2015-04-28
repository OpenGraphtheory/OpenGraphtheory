
#include "../Headers/thread.h"

// ============================================================= Mutex

Mutex::Mutex()
{
    #ifdef __unix__
        pthread_mutex_init(&mutex, NULL);
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
        InitializeCriticalSection(&mutex);
    #endif
}

Mutex::~Mutex()
{
    #ifdef __unix__
        pthread_mutex_destroy(&mutex);
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
        DeleteCriticalSection(&mutex);
    #endif
}

void Mutex::Lock()
{
    #ifdef __unix__
        pthread_mutex_lock(&mutex);
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
        EnterCriticalSection(&mutex);
    #endif
}

void Mutex::Unlock()
{
    #ifdef __unix__
        pthread_mutex_unlock(&mutex);
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
        LeaveCriticalSection(&mutex);
    #endif
}

bool Mutex::TryLock()
{
    #ifdef __unix__
        return pthread_mutex_trylock(&mutex);
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
        return TryEnterCriticalSection(&mutex) == TRUE;
    #endif
}


// ============================================================= ConditionVariable


ConditionVariable::ConditionVariable()
    : Mutex()
{
    #ifdef __unix__
        pthread_cond_init(&cond, NULL);
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
    #endif
}

ConditionVariable::~ConditionVariable()
{
    #ifdef __unix__
        pthread_cond_destroy(&cond);
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
    #endif
}

bool ConditionVariable::Wait()
{
    #ifdef __unix__
        return pthread_cond_wait(&cond,&mutex) == 0;
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
    #endif
}

void ConditionVariable::Signal()
{
    Lock();
    #ifdef __unix__
        pthread_cond_signal(&cond);
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
    #endif
    Unlock();
}


// ============================================================= Thread


ThreadContext::ThreadContext(Thread* threadObject, void* parameter, ConditionVariable* threadFinishedSignal)
{
    this->threadObject = threadObject;
    this->parameter = parameter;
    this->threadFinishedSignal = threadFinishedSignal;
}


void ThreadContext::Execute()
{
    try
    {
        this->threadObject->RunThread(parameter);
    }
    catch(...)
    {
        if(threadFinishedSignal != NULL)
            threadFinishedSignal->Signal();
        throw;
    }

    if(threadFinishedSignal != NULL)
        threadFinishedSignal->Signal();

}


// ============================================================= Thread



#ifdef __unix__
    void* ThreadWrapper(void* t)
#elif __WIN32__ || _MSC_VER || _Windows || __NT__
    __cdecl void ThreadWrapper(void* t)
#endif
{
    ThreadContext* threadcontext = (ThreadContext*)(t);
    try
    {
        threadcontext->Execute();
    }
    catch(...)
    {
        delete threadcontext;
        throw;
    }

    delete threadcontext; // instantiated in Thread::Start
    #ifdef __unix__
        return NULL;
        // pthread_exit(NULL);
    #endif
}

void Thread::Start(void* parameter, ConditionVariable* threadFinishedSignal)
{
    ThreadContext* context = new ThreadContext(this, parameter, threadFinishedSignal); // deleted in ThreadWrapper
    #ifdef __unix__
        pthread_create(&thread, NULL, &ThreadWrapper, context);
        //pthread_detach(thread);
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
        thread = _beginthread(ThreadWrapper, 0, context);
    #endif
}

void Thread::Terminate()
{
    #ifdef __unix__
        pthread_cancel(thread);
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
        TerminateThread((void*)(thread),0);
    #endif
}

void Thread::TestTermination()
{
    #ifdef __unix__
        pthread_testcancel();
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
    #endif
}

Thread::~Thread()
{
    Terminate();
}

void Thread::Lock()
{
    mutex.Lock();
}

void Thread::Unlock()
{
    mutex.Unlock();
}

bool Thread::TryLock()
{
    return mutex.TryLock();
}

void Thread::Join()
{
    #ifdef __unix__
        pthread_join(thread, NULL);
    #elif __WIN32__ || _MSC_VER || _Windows || __NT__
    #endif
}
