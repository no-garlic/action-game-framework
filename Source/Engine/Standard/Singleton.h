
#ifndef __STD_SINGLETON_H_
#define __STD_SINGLETON_H_

#include "Type.h"

template <class T>
struct CreateUsingNew
{
    static T    *Create() { return new T; }
    static void Destroy(T *p) { delete p; }
};

template <class T>
struct CreateUsingMalloc
{
    static T *Create()
    {
        void *p = std::malloc(sizeof(T));
        if (!p)
            return 0;

        return new(p) T;
    }
    
    static void Destroy(T *p)
    {
        p->~T();
        std::free(p);
    }
};

template <class T>
struct CreateStatic
{
    union MaxAlign
    {
        char            m_t[sizeof(T)];
        short int       m_short_int;
        int             m_int;
        long int        m_long_int;
        float           m_float;
        double          m_double;
        long double     m_long_double;
        struct          Test;
        int             Test::* m_pmember;
        int            (Test::*m_pmember_func)(int);
    };

    static T *Create()
    {
        static MaxAlign s_static_memory;
        return new(&s_static_memory) T;
    }
    
    static void Destroy(T *p)
    {
        p->~T();
    }
};

template <typename T, class CreationPolicy>
class SingletonHolder
{
public:
    static T &Instance();
    static T *GetPtr();

    static void MakeInstance();
    static void DestroyInstance();

private:
    SingletonHolder();
    SingletonHolder<T, CreationPolicy> &operator=(const SingletonHolder<T, CreationPolicy> &other);
    explicit SingletonHolder(const SingletonHolder<T, CreationPolicy> &other);

    typedef T *PtrInstanceType;

    static PtrInstanceType  s_pinstance;
    static bool             s_destroyed;
};

template <class T, class C>
typename SingletonHolder<T, C>::PtrInstanceType SingletonHolder<T, C>::s_pinstance = 0;
template <class T, class C>
bool SingletonHolder<T, C>::s_destroyed = false;

template <class T, class CreationPolicy>
inline T &SingletonHolder<T, CreationPolicy>::Instance()
{
    if (!s_pinstance)
        MakeInstance();

    return *s_pinstance;
}

template <class T, class CreationPolicy>
inline T *SingletonHolder<T, CreationPolicy>::GetPtr()
{
    return s_pinstance;
}

template <class T, class CreationPolicy>
inline void SingletonHolder<T, CreationPolicy>::MakeInstance()
{
    if (!s_pinstance)
    {
        s_destroyed = false;
        s_pinstance = CreationPolicy::Create();

        atexit(&SingletonHolder<T, CreationPolicy>::DestroyInstance);
    }
}

template <class T, class CreationPolicy>
inline void SingletonHolder<T, CreationPolicy>::DestroyInstance()
{
    if (!s_destroyed)
    {
        CreationPolicy::Destroy(s_pinstance);
        s_pinstance = 0;
        s_destroyed = true;
    }
}

#endif // __STD_SINGLETON_H_
