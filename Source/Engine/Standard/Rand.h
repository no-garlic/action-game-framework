
#ifndef __STD_RAND_H_
#define __STD_RAND_H_

#include <MMSystem.h>

class Random
{
public:
    Random(int seed = timeGetTime())
        : m_Seed(seed)
    {
    }

    float GetFloat()
    {
        MutateSeed();
        static const float tempRand = 1.0f;
        float result;
        *(int *)&result = (*(int *)&tempRand & 0xff800000) | (m_Seed & 0x007fffff);
        return result - (int)result;
    }

    int GetInt()
    {
        MutateSeed();
        return m_Seed;
    }

private:
    void MutateSeed()
    {
        m_Seed = (m_Seed * 196314165) + 907633515;
    }

    int m_Seed;

};

inline float FloatRandom(float start, float end)
{
    static Random random;
    return (random.GetFloat() * (end - start)) + start;
}

inline int IntRandom(int start, int end)
{
    static Random random;
    return (random.GetInt() % (end - start)) + start;
}

#endif // __STD_RAND_H_
