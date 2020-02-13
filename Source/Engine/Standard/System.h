
#ifndef __STD_SYSTEM_H_
#define __STD_SYSTEM_H_

#include "Type.h"

#ifdef CreateEvent
#undef CreateEvent
#endif

#ifdef CreateMutex
#undef CreateMutex
#endif

namespace OS
{
    typedef unsigned long ThreadReturnType;
    typedef ThreadReturnType(*ThreadFunc)(void *);

    typedef HANDLE Handle;
    typedef HANDLE ThreadHandle;

    struct TimeInfo
    {
        unsigned long seconds;
        unsigned long nano_seconds;
    };

    struct CounterInfo
    {
        signed long   high;
        unsigned long low;
    };

    ThreadHandle    CreateThread(unsigned long stacksize, ThreadFunc startaddress, void *pargument);
    void            TerminateThread(ThreadHandle thread, unsigned long milliseconds = 0);
    void            ExitThread(unsigned long exit_value);
    void            Sleep(unsigned long milliseconds = 0) throw();
    unsigned long   Join(ThreadHandle thread);
    ThreadHandle    GetCurrentThreadHandle();
    void            CloseThread(ThreadHandle thread);
    int             GetThreadPriority(ThreadHandle thread);
    void            SetThreadPriority(ThreadHandle thread, int priority);

    Handle          CreateTimer(bool manual_reset);
    void            CloseTimer(Handle timer);
    void            SetTimer(Handle timer, TimeInfo duetime, bool relative, TimeInfo period);
    bool            CancelTimer(Handle timer) throw();

    Handle          CreateEvent(bool manualreset, bool initialstate, const char *pname = 0);
    Handle          OpenEvent(const char *pname);
    void            CloseEvent(Handle event);
    void            SetEvent(Handle event);
    void            ResetEvent(Handle event);
    void            PulseEvent(Handle event);

    Handle          CreateMutex(const char *pname = 0);
    Handle          OpenMutex(const char *pname);
    void            CloseMutex(Handle mutex);
    bool            ReleaseMutex(Handle mutex);

    int             WaitForSingleObject(Handle object, unsigned long timeout);
    int             WaitForMultipleObjects(int nobjects, Handle* pobjects, unsigned long timeout);
    int             WaitForMultipleObjects(Array <Handle> &objects, unsigned long timeout);

    CounterInfo     GetCounter() throw();
    CounterInfo     GetCounterFrequency() throw();
    float           CalculateTimeDifference(CounterInfo start, CounterInfo finish) throw();

    class Thread
    {
    public:
        Thread(OS::ThreadFunc func, void *parg);
        ~Thread();

        ThreadReturnType Join();

    private:
        Thread(const Thread &rhs) {}

        Handle m_threadhandle;
    };

    class Event
    {
    public:
        Event();
        ~Event();

        void Set();
        void Wait();
        bool Wait(ulong milliseconds);

    private:
        Event(const Event &rhs) {}

        Handle m_event;
    };

    class Mutex
    {
    public:
        Mutex();
        virtual ~Mutex();

        void Acquire();
        void Release();

    private:
        Mutex(const Mutex &rhs) {}

        Handle m_mutex;
    };

    class MutexLock
    {
    public:
        MutexLock(Mutex &m) : m_mutex(m) { m.Acquire(); }
        virtual ~MutexLock() { m_mutex.Release(); }

    private:
        Mutex &m_mutex;
    };

};

#endif // __STD_SYSTEM_H_

