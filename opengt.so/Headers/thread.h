
#ifndef __OPENGRAPHTHEORY_THREAD_H
    #define __OPENGRAPHTHEORY_THREAD_H

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
        protected:
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

    class ConditionVariable : public Mutex
    {
        protected:
            #ifdef __unix__
                pthread_cond_t cond;
            #elif __WIN32__ || _MSC_VER || _Windows || __NT__
                //CRITICAL_SECTION mutex;
            #endif
        public:
            ConditionVariable();
            ~ConditionVariable();

            bool Wait();
            void Signal();
    };


    class Thread;
    class ThreadContext
    {
        protected:
            void* parameter;
            ConditionVariable* threadFinishedSignal;
            Thread** threadFinishedRegister;
            Thread* threadObject;
        public:
            ThreadContext(Thread* threadObject, void* parameter, ConditionVariable* threadFinishedSignal, Thread** threadFinishedRegister);
            void Execute();
    };

    class Thread
    {
        #ifdef __unix__
            friend void* ThreadWrapper(void*);
        #elif __WIN32__ || _MSC_VER || _Windows || __NT__
            friend void ThreadWrapper(void*);
        #endif
        friend class ThreadContext;

        protected:
            Mutex mutex;
            bool Started;
            #ifdef __unix__
                pthread_t thread;
            #elif __WIN32__ || _MSC_VER || _Windows || __NT__
                uintptr_t thread;
            #endif

            virtual void RunThread(void* parameter) = 0;
            void Start(void* parameter, ConditionVariable* threadFinishedSignal = NULL, Thread** threadFinishedRegister = NULL);
        public:
            void Terminate();
            static void TestTermination();
            void Join();

            void Lock();
            void Unlock();
            bool TryLock();
            Thread();
            virtual ~Thread();
    };

#endif
