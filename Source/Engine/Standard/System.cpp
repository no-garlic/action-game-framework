
#include "System.h"
#include "Log.h"

#include <exception>
#include <mmsystem.h>

inline void ThrowOSException(const String &message)
{
    LOG_ERROR << message << ENDLOG;
    std::exception e(message.c_str());
    throw e;
}

namespace OS
{
    ThreadHandle CreateThread(unsigned long stacksize, ThreadFunc startaddress, void *pargument)
    {
        DWORD threadid;
        ThreadHandle threadhandle = ::CreateThread(
            0,
            stacksize,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(startaddress),
            pargument,
            0,
            &threadid);

        if (!threadhandle)
            ThrowOSException("Create thread failed");

        return reinterpret_cast<ThreadHandle>(threadhandle);
    }

    void TerminateThread(ThreadHandle thread, unsigned long milliseconds)
    {
        if (milliseconds > 0)
        {
            // If we are successful don't bother trying to terminate the thread
            if (::WaitForSingleObject(reinterpret_cast<Handle>(thread), milliseconds) == WAIT_OBJECT_0)
            {
                CloseThread(thread);
                return;
            }
        }

        // We have waited long enough - terminate the thread
        if (::TerminateThread(reinterpret_cast<Handle>(thread), 0) == 0)
            ThrowOSException("TerminateThread failed");

        // Close the thread too
        CloseThread(thread);
    }

    void ExitThread(unsigned long exit_value)
    {
        ::ExitThread(exit_value);
    }

    int GetThreadPriority(ThreadHandle thread)
    {
        int priority = ::GetThreadPriority(reinterpret_cast<Handle>(thread));

        if (priority == THREAD_PRIORITY_ERROR_RETURN)
            ThrowOSException("GetThreadPriority failed");

        return priority;
    }

    void SetThreadPriority(ThreadHandle thread, int priority)
    {
        if (::SetThreadPriority((reinterpret_cast<Handle>(thread)), priority) == FALSE)
            ThrowOSException("SetThreadPriorityFailed");
    }

    void Sleep(unsigned long milliseconds) throw()
    {
        ::Sleep(milliseconds);
    }

    unsigned long Join(ThreadHandle thread)
    {
        // Wait for the thread to terminate
        if (::WaitForSingleObject(thread, INFINITE) != 0)
            ThrowOSException("Error waiting for thread to finish!");

        DWORD exit_code;
        if (::GetExitCodeThread(reinterpret_cast<Handle>(thread), &exit_code) == 0)
            ThrowOSException("Join failed");

        CloseThread(thread);
        unsigned long exit_value = static_cast<unsigned long>(exit_code);
        return exit_value;
    }

    ThreadHandle GetCurrentThreadHandle()
    {
        Handle thr = GetCurrentThread();
        return reinterpret_cast<ThreadHandle>(thr);
    }

    void CloseThread(ThreadHandle thread)
    {
        if (CloseHandle(reinterpret_cast<void*>(thread)) == 0)
            ThrowOSException("CloseThread failed");
    }

    OS::Handle CreateTimer(bool manualreset)
    {
        Handle htimer;
        if ((htimer = ::CreateWaitableTimer(0, manualreset ? TRUE : FALSE, 0)) == 0)
            ThrowOSException("CreateTimer failed");

        return reinterpret_cast<OS::Handle>(htimer);
    }

    void CloseTimer(OS::Handle timer)
    {
        if (::CloseHandle(reinterpret_cast<Handle>(timer)) == 0)
            ThrowOSException("CloseTimer failed");
    }

    void SetTimer(OS::Handle timer, TimeInfo duetime, bool relative, TimeInfo period)
    {
        // Remember - this is in terms of 100 ns intervals
        LARGE_INTEGER liduetime;
        liduetime.QuadPart = (duetime.seconds * 10000000) + (duetime.nano_seconds / 100);

        // Make this negative if we are looking for relative timing
        if (relative)
            liduetime.QuadPart *= -1;

        unsigned long period_ms = (period.seconds * 1000) + (period.nano_seconds / 1000000);

        if (::SetWaitableTimer(reinterpret_cast<Handle>(timer), &liduetime, period_ms, 0, 0, TRUE) == 0)
            ThrowOSException("SetTimer failed");
    }

    bool CancelTimer(OS::Handle timer) throw()
    {
        return ::CancelWaitableTimer(reinterpret_cast<Handle>(timer)) != 0;
    }

    OS::Handle CreateEvent(bool manualreset, bool initialstate, const char *pname)
    {
        Handle hevent;
        if ((hevent = ::CreateEventA(0, manualreset ? TRUE : FALSE, initialstate ? TRUE : FALSE, pname)) == 0)
            ThrowOSException("CreateEvent failed");

        return reinterpret_cast<OS::Handle>(hevent);
    }

    OS::Handle OpenEvent(const char *pname)
    {
        Handle hevent;
        if ((hevent = ::OpenEvent(EVENT_ALL_ACCESS, TRUE, pname)) == 0)
            ThrowOSException("OpenEvent failed");

        return reinterpret_cast<OS::Handle>(hevent);
    }

    void CloseEvent(OS::Handle event)
    {
        if (::CloseHandle(reinterpret_cast<Handle>(event)) == 0)
            ThrowOSException("CloseEvent failed");
    }

    void SetEvent(OS::Handle event)
    {
        if (::SetEvent(reinterpret_cast<Handle>(event)) == 0)
            ThrowOSException("SetEvent failed");
    }

    void ResetEvent(OS::Handle event)
    {
        if (::ResetEvent(reinterpret_cast<Handle>(event)) == 0)
            ThrowOSException("ResetEvent failed");
    }

    void PulseEvent(OS::Handle event)
    {
        if (::PulseEvent(reinterpret_cast<Handle>(event)) == 0)
            ThrowOSException("PulseEvent failed");
    }

    OS::Handle CreateMutex(const char *pname)
    {
        Handle hmutex;
        if ((hmutex = ::CreateMutexA(0, FALSE, pname)) == 0)
            ThrowOSException("CreateMutex failed");
        return reinterpret_cast<OS::Handle>(hmutex);
    }

    OS::Handle OpenMutex(const char *pname)
    {
        Handle hmutex;
        if ((hmutex = ::OpenMutex(MUTEX_ALL_ACCESS, TRUE, pname)) == 0)
            ThrowOSException("OpenMutex failed");

        return reinterpret_cast<OS::Handle>(hmutex);
    }

    void CloseMutex(OS::Handle mutex)
    {
        if (::CloseHandle(reinterpret_cast<Handle>(mutex)) == 0)
            ThrowOSException("CloseMutex failed");
    }

    bool ReleaseMutex(OS::Handle mutex)
    {
        if (::ReleaseMutex(reinterpret_cast<Handle>(mutex)) == 0)
        {
            if (GetLastError() == ERROR_NOT_OWNER)
                return false;
            else
                ThrowOSException("ReleaseMutex failed");
        }
        return true;
    }

    int WaitForSingleObject(OS::Handle object, unsigned long timeout)
    {
        int retvalue = ::WaitForSingleObject(reinterpret_cast<Handle>(object), timeout);

        if (retvalue == WAIT_FAILED)
            ThrowOSException("WaitForSingleObject failed");

        return retvalue;
    }

    int WaitForMultipleObjects(int nobjects, OS::Handle* pobjects, unsigned long timeout)
    {
        if (nobjects > MAXIMUM_WAIT_OBJECTS)
        {
            const int     RELEASE_MS = 1;
            unsigned long time = 0;

            while (true)
            {
                for (int i = 0; i < nobjects; ++i)
                {
                    if (WaitForSingleObject(pobjects[i], 0) != WAIT_TIMEOUT)
                        return i;
                }

                unsigned long sleep_time = RELEASE_MS;
                if ((timeout != INFINITE) && ((timeout - time) < RELEASE_MS))
                    sleep_time = static_cast<unsigned long>(timeout - time);
                Sleep(sleep_time);

                time += sleep_time;

                if ((timeout != INFINITE) && (time >= timeout))
                    break;
            }

            return WAIT_TIMEOUT;
        }

        int retvalue = ::WaitForMultipleObjects(nobjects, reinterpret_cast<CONST Handle *>(pobjects), FALSE,
            timeout == INFINITE ? INFINITE : timeout);

        if (retvalue == WAIT_FAILED)
            ThrowOSException("WaitForMultipleObject failed");

        return (retvalue == WAIT_TIMEOUT) ? WAIT_TIMEOUT : retvalue - WAIT_OBJECT_0;
    }

    int WaitForMultipleObjects(Array <OS::Handle> &objects, unsigned long timeout)
    {
        return WaitForMultipleObjects(static_cast<int>(objects.size()), &objects[0], timeout);
    }

    namespace
    {
        enum OSCOUNTERTYPE
        {
            UNCHECKED,
            NORMAL,
            HIGHPERFORMANCE
        };

        OSCOUNTERTYPE       g_countertype = UNCHECKED;
        OS::CounterInfo     g_counterfrequency;
        float               g_counterperiod;
    };

    CounterInfo GetCounter() throw()
    {
        CounterInfo retvalue;

        if (g_countertype == UNCHECKED)
            GetCounterFrequency();

        if (g_countertype == HIGHPERFORMANCE)
        {
            LARGE_INTEGER liperformancecount;

            QueryPerformanceCounter(&liperformancecount);

            retvalue.low = liperformancecount.LowPart;
            retvalue.high = liperformancecount.HighPart;
        }
        else
        {
            retvalue.low = timeGetTime();
            retvalue.high = 0;
        }

        return retvalue;
    }

    CounterInfo GetCounterFrequency() throw()
    {
        if (g_countertype == UNCHECKED)
        {
            LARGE_INTEGER liperformancefrequency;

            if (QueryPerformanceFrequency(&liperformancefrequency) != 0)
                g_countertype = HIGHPERFORMANCE;
            else
                g_countertype = NORMAL;

            g_counterfrequency.low = liperformancefrequency.LowPart;
            g_counterfrequency.high = liperformancefrequency.HighPart;

            if (g_counterfrequency.low == 0 && g_counterfrequency.high == 0)
            {
                g_counterfrequency.low = 1000;
                g_counterperiod = 0.001f;
            }
            else
            {
                g_counterperiod = 1.0f / static_cast<float>(liperformancefrequency.QuadPart);
            }
        }

        return g_counterfrequency;
    }

    float CalculateTimeDifference(CounterInfo start, CounterInfo finish) throw()
    {
        LARGE_INTEGER listart, lifinish;
        listart.LowPart = start.low;
        listart.HighPart = start.high;
        lifinish.LowPart = finish.low;
        lifinish.HighPart = finish.high;

        LARGE_INTEGER lidiff;
        lidiff.QuadPart = lifinish.QuadPart - listart.QuadPart;

        return static_cast<float>(lidiff.QuadPart) * g_counterperiod;
    }

    Thread::Thread(OS::ThreadFunc func, void *parg)
    {
        constructor(Thread);

        DWORD threadid;
        m_threadhandle = ::CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(func), parg, 0, &threadid);

        if (m_threadhandle == NULL)
            ThrowOSException("Failed to create the thread");
    }

    Thread::~Thread()
    {
        destructor(Thread);

        if (::TerminateThread(m_threadhandle, 0) == 0)
            ThrowOSException("Failed to terminate the thread");

        if (CloseHandle(m_threadhandle) == 0)
            ThrowOSException("Failed to close the thread");
    }

    ThreadReturnType Thread::Join()
    {
        if (WaitForSingleObject(m_threadhandle, INFINITE) != 0)
            ThrowOSException("Error joining thread!");

        DWORD exit_code;
        if (GetExitCodeThread(m_threadhandle, &exit_code) == 0)
            ThrowOSException("Error joining thread!");

        if (CloseHandle(m_threadhandle) == 0)
            ThrowOSException("Failed to close the thread");

        ThreadReturnType exit_value = static_cast<ThreadReturnType>(exit_code);
        return exit_value;
    }

    Event::Event()
    {
        constructor(Event);

        m_event = CreateEventA(NULL, FALSE, FALSE, 0);

        if (m_event == NULL)
            ThrowOSException("Failed to create the event");
    }

    Event::~Event()
    {
        destructor(Event);

        if (!::SetEvent(m_event))
            ThrowOSException("Failed to set the event");

        if (!CloseHandle(m_event))
            ThrowOSException("Failed to close the event handle");
    }

    void Event::Set()
    {
        if (!::SetEvent(m_event))
            ThrowOSException("Failed to set the event");
    }

    void Event::Wait()
    {
        if (WaitForSingleObject(m_event, INFINITE) == WAIT_FAILED)
            ThrowOSException("Failed to wait for the event");
    }

    bool Event::Wait(ulong milliseconds)
    {
        if (WaitForSingleObject(m_event, milliseconds) == WAIT_FAILED)
            return false;

        return true;
    }

    Mutex::Mutex()
    {
        constructor(Mutex);

        m_mutex = CreateMutexA(NULL, FALSE, 0);

        if (!m_mutex)
            ThrowOSException("Failed to create the mutex");
    }

    Mutex::~Mutex()
    {
        destructor(Mutex);

        if (!CloseHandle(m_mutex))
            ThrowOSException("Failed to close the mutex handle");
    }

    void Mutex::Acquire()
    {
        if (WaitForSingleObject(m_mutex, INFINITE) == WAIT_FAILED)
            ThrowOSException("Failed to acquire the mutex");
    }

    void Mutex::Release()
    {
        if (!ReleaseMutex(m_mutex))
            ThrowOSException("Failed to release the mutex");
    }

};

