
#include "../Headers/thread.h"



// ======================================================================= Mutex



Mutex::Mutex()
{
    #ifdef __unix__
        pthread_mutex_init(&mutex, NULL);
    #elif __windows__
        InitializeCriticalSection(&mutex);
    #endif
}

Mutex::~Mutex()
{
    #ifdef __unix__
        pthread_mutex_destroy(&mutex);
    #elif __windows__
        DeleteCriticalSection(&mutex);
    #endif
}

void Mutex::Lock()
{
    #ifdef __unix__
        pthread_mutex_lock(&mutex);
    #elif __windows__
        EnterCriticalSection(&mutex);
    #endif
}

void Mutex::Unlock()
{
    #ifdef __unix__
        pthread_mutex_unlock(&mutex);
    #elif __windows__
        LeaveCriticalSection(&mutex);
    #endif
}

bool Mutex::TryLock()
{
    #ifdef __unix__
        return pthread_mutex_trylock(&mutex);
    #elif __windows__
        return TryEnterCriticalSection(&mutex) == TRUE;
    #endif
}



// =========================================================== ConditionVariable



ConditionVariable::ConditionVariable()
    : Mutex()
{
    #ifdef __unix__
        pthread_cond_init(&cond, NULL);
    #elif __windows__
        InitializeConditionVariable(&cond);
    #endif
}

ConditionVariable::~ConditionVariable()
{
    #ifdef __unix__
        pthread_cond_destroy(&cond);
    #elif __windows__
        /*
        On Windows, Condition Variables don't need to be destroyed explicitly, see
        http://stackoverflow.com/questions/28975958/why-does-windows-have-no-deleteconditionvariable-function-to-go-together-with
        */
    #endif
}

bool ConditionVariable::Wait()
{
    #ifdef __unix__
        return pthread_cond_wait(&cond,&mutex) == 0;
    #elif __windows__
        return SleepConditionVariableCS(&cond, &mutex, INFINITE) != 0;
    #endif
}

void ConditionVariable::SignalOne()
{
    Lock();
    #ifdef __unix__
        pthread_cond_signal(&cond);
    #elif __windows__
        WakeConditionVariable(&cond);
    #endif
    Unlock();
}

void ConditionVariable::SignalAll()
{
    Lock();
    #ifdef __unix__
        pthread_cond_broadcast(&cond);
    #elif __windows__
        WakeAllConditionVariable(&cond);
    #endif
    Unlock();
}



// ============================================================= Thread



ThreadContext::ThreadContext(Thread* threadObject, void* parameter, ConditionVariable* threadFinishedSignal, Thread** threadFinishedRegister)
{
    this->threadObject = threadObject;
    this->parameter = parameter;
    this->threadFinishedSignal = threadFinishedSignal;
    this->threadFinishedRegister = threadFinishedRegister;
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
            threadFinishedSignal->SignalOne();
        throw;
    }

    if(threadFinishedRegister != NULL)
        *threadFinishedRegister = threadObject;
    if(threadFinishedSignal != NULL)
        threadFinishedSignal->SignalOne();

}


// ============================================================= Thread



#ifdef __unix__
    void* ThreadWrapper(void* t)
#elif __windows__
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

void Thread::Start(void* parameter, ConditionVariable* threadFinishedSignal, Thread** FinishedThreadRegister)
{
    if(!Started)
    {
        ThreadContext* context = new ThreadContext(this, parameter, threadFinishedSignal, FinishedThreadRegister); // deleted in ThreadWrapper
        #ifdef __unix__
            pthread_create(&thread, NULL, &ThreadWrapper, context);
            //pthread_detach(thread);
        #elif __windows__
            thread = _beginthread(ThreadWrapper, 0, context);
        #endif
        Started = true;
    }
}

void Thread::Terminate()
{
    if(Started)
    {
        #ifdef __unix__
            pthread_cancel(thread);
        #elif __windows__
            TerminateThread((void*)(thread),0);
        #endif
    }
    Started = false;
}

void Thread::TestTermination()
{
    #ifdef __unix__
        pthread_testcancel();
    #elif __windows__
    #endif
}

Thread::Thread()
{
    Started = false;
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
    if(Started)
    {
        #ifdef __unix__
            pthread_join(thread, NULL);
        #elif __windows__
            WaitForSingleObject(thread, INFINITE);
        #endif
    }
    Started = false;
}
