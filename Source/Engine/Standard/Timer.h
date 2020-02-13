
#ifndef __STD_TIMER_H_
#define __STD_TIMER_H_

#include "Type.h"
#include "Mem.h"

class Timer : private Unique
{
public:
    Timer();
    ~Timer();
    
    void Start();
    void Stop();
    void Reset();
    
    float GetTime(float *delta_time);
    bool IsRunning();

    struct Counter
    {
        signed long   high;
        unsigned long low;
};
private:
    
    Counter   m_start_counter;
    Counter   m_end_counter;  
    float     m_total_time;   
    bool      m_running;      

};

#endif // __STD_TIMER_H_

