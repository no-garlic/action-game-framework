
#include "Profile.h"

stdProfile  g_Profilers[PROFILE_MAX];
Timer       g_GlobalTimer;

uint   stdProfileRender::s_RenderCalls = 0;
uint   stdProfileRender::s_Verts = 0;
uint   stdProfileRender::s_Faces = 0;
uint   stdProfileRender::s_StateChanges = 0;
uint   stdProfileRender::s_RenderCallsAccum = 0;
uint   stdProfileRender::s_VertsAccum = 0;
uint   stdProfileRender::s_FacesAccum = 0;
uint   stdProfileRender::s_StateChangesAccum = 0;
float  stdProfileRender::s_timeStamp = 0.0f;
uint   stdProfileRender::s_Frames = 0;

stdProfile &stdProfile::GetProfiler(int id)
{
    return g_Profilers[id];
}

float stdProfile::GetProfileTime()
{
    if (!g_GlobalTimer.IsRunning())
        g_GlobalTimer.Start();

    return g_GlobalTimer.GetTime(NULL);
}

void stdProfile::ResetAllProfilers()
{
    stdProfileRender::ResetAll();

    for (uint i = 0; i < PROFILE_MAX; i++)
        g_Profilers[i].Reset();
}

void stdProfile::Reset()
{
    float sampleTime = m_Timer.GetTime(NULL);
    m_Accum += sampleTime;
    m_Samples++;
    m_Timer.Reset();

    float runningTime = stdProfile::GetProfileTime();
    float deltaTime = runningTime - m_timeStamp;

    if (deltaTime > PROFILE_SAMPLE_DURATION)
    {
        m_timeStamp = runningTime;
        m_Time = m_Accum / (float)m_Samples;
        m_Accum = sampleTime;
        m_Samples = 1;
    }
}

void stdProfileRender::ResetAll()
{
    s_Frames++;

    float runningTime = stdProfile::GetProfileTime();
    float deltaTime = runningTime - s_timeStamp;

    if (deltaTime > PROFILE_SAMPLE_DURATION)
    {
        s_RenderCalls = s_RenderCallsAccum / s_Frames;
        s_Verts = s_VertsAccum / s_Frames;
        s_Faces = s_FacesAccum / s_Frames;
        s_StateChanges = s_StateChangesAccum / s_Frames;

        s_RenderCallsAccum = 0;
        s_VertsAccum = 0;
        s_FacesAccum = 0;
        s_StateChangesAccum = 0;

        s_Frames = 0;
        s_timeStamp = runningTime;
    }
}
