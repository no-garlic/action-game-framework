
#include "Timer.h"
#include "Log.h"

namespace
{
    enum COUNTERTYPE
    {
        UNCHECKED,
        NORMAL,
        HIGHPERFORMANCE
};
    
    COUNTERTYPE    g_countertype = UNCHECKED;
    Timer::Counter g_counterfrequency;       
    float          g_counterperiod;          
};

Timer::Counter GetCounterFrequency() throw()
{
    if (g_countertype == UNCHECKED)
    {
        LARGE_INTEGER freq;

        if (QueryPerformanceFrequency(&freq) != 0)
            g_countertype = HIGHPERFORMANCE;
        else
            g_countertype = NORMAL;

        g_counterfrequency.low = freq.LowPart;
        g_counterfrequency.high = freq.HighPart;

        if (g_counterfrequency.low == 0 && g_counterfrequency.high == 0)
        {
            g_counterfrequency.low = 1000;
            g_counterperiod = 0.001f;
        }
        else
        {
            g_counterperiod = 1.0f / static_cast<float>(freq.QuadPart);
        }
    }

    return g_counterfrequency;
}

Timer::Counter GetCounter() throw()
{
    Timer::Counter retvalue;

    if (g_countertype == UNCHECKED)
        GetCounterFrequency();

    if (g_countertype == HIGHPERFORMANCE)
    {
        LARGE_INTEGER freq;
        QueryPerformanceCounter(&freq);

        retvalue.low = freq.LowPart;
        retvalue.high = freq.HighPart;
    }
    else
    {
        retvalue.low = timeGetTime();
        retvalue.high = 0;
    }

    return retvalue;
}

float CalculateTimeDifference(Timer::Counter start, Timer::Counter finish) throw()
{
    LARGE_INTEGER listart, lifinish, lidiff;

    listart.LowPart = start.low;
    listart.HighPart = start.high;
    lifinish.LowPart = finish.low;
    lifinish.HighPart = finish.high;
    lidiff.QuadPart = lifinish.QuadPart - listart.QuadPart;

    return static_cast<float>(lidiff.QuadPart) * g_counterperiod;
}

Timer::Timer() : m_running(false)
{
    constructor(Timer);

    Reset();
}

Timer::~Timer()
{
    destructor(Timer);
}

void Timer::Start() throw()
{
    if (!m_running)
    {
        m_running = true;
        m_start_counter = GetCounter();
    }
}

void Timer::Stop() throw()
{
    if (m_running)
    {
        m_end_counter = GetCounter();
        m_running = false;
        m_total_time += CalculateTimeDifference(m_start_counter, m_end_counter);
    }
}

void Timer::Reset()
{
    m_total_time = 0;;
    m_running = false;
}

float Timer::GetTime(float *delta_time)
{
    float diff = 0.0;

    if (m_running)
    {
        m_end_counter = GetCounter();
        diff = CalculateTimeDifference(m_start_counter, m_end_counter);
        m_total_time += diff;
        m_start_counter = GetCounter();
    }

    if (delta_time)
        (*delta_time) = diff;

    return m_total_time;
}

bool Timer::IsRunning()
{
    return m_running;
}

