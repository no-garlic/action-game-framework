
#ifndef __STD_MEM_H_
#define __STD_MEM_H_

#include "Type.h"

#ifdef _DEBUG
#include <crtdbg.h>

#ifdef ASSERT
#undef ASSERT
#endif // ASSERT

#define NULL_ASSERT(x) ASSERT(x, "##x is NULL")
#define DEBUG_BREAK ASSERT(NULL, "Debug Break")
#define CHECK(check, statement) { if (!(check)) { DEBUG_BREAK; statement; } }

#ifdef NO_ASSERT
#define ASSERT(expression, msg)
#elif defined ASSERT_DIALOG
#define ASSERT(expression, msg) if (!(expression)) { Assert::FailDialog(#expression, msg, __FILE__, __LINE__); }
#else // !NO_ASSERT && !ASSERT_DIALOG
#define ASSERT(expression, msg) if (!(expression)) { LOG_ERROR << msg << ENDLOG; _CrtDbgBreak(); }
#endif // !NO_ASSERT && !ASSERT_DIALOG

#else // !_DEBUG
#ifdef ASSERT
#undef ASSERT
#endif // !ASSERT
#define ASSERT(expression, msg)
#define NULL_ASSERT(x)
#define CHECK(check, statement) { if (!(check)) { statement; } }
#define DEBUG_BREAK
#endif // !_DEBUG

#ifdef _DEBUG

#ifdef ASSERT_DIALOG
#ifdef Assert
#undef Assert

#endif // Assert

namespace Assert
{
    inline void FailDialog(const char *expression, const char *message, const char *file, int line)
    {
        char buffer[1024];
        sprintf(buffer, "%s:%d\n%s\n%s\nWould You Like To Break?", file, line, message, expression);
        if (MessageBox(0, buffer, "Assertion Failed", MB_ICONERROR | MB_YESNO | MB_TOPMOST) == IDYES)
            _CrtDbgBreak();
    }
};

#endif // ASSERT_DIALOG
#endif // _DEBUG

template <class T>
class StridePointer
{
public:
    StridePointer() : m_pdata(0), m_stride(0) {}
    StridePointer(ubyte stride, ubyte offset, void *pdata) : m_pdata((ubyte *)pdata + offset), m_stride(stride) {}

    operator T () { return *((T*)(m_pdata)); }
    T &operator * () { return *((T*)(m_pdata)); }
    T *operator ->() { return  ((T*)(m_pdata)); }
    T &operator [](uint idx) { return *((T*)(m_pdata + (idx * m_stride))); }
    const T &operator [](uint idx) const { return *((T*)(m_pdata + (idx * m_stride))); }
    T &operator ++() { m_pdata += m_stride; return *((T*)(m_pdata)); }
    T &operator --() { m_pdata -= m_stride; return *((T*)(m_pdata)); }
    operator bool() { return (m_pdata != 0); }

private:
    uchar *m_pdata;
    ubyte m_stride;

};

#define SafeDelete(x) { delete x; x = NULL; }

template <typename T>
struct StackDelete
{
    StackDelete(T *obj) : obj(obj) {}
    ~StackDelete() { delete obj; }
    T *obj;
};

template <typename T>
struct StackDelete_NewObject
{
    T *m_Object;
    StackDelete_NewObject() { m_Object = new T; }
    ~StackDelete_NewObject() { delete m_Object; }
    T* operator->() { return m_Object; }    
};

template <class T>
struct AutoArray
{
public:
    explicit AutoArray(int count) { pdata = (T *) new ubyte[sizeof(T) * count]; }
    ~AutoArray() { delete[] pdata; }
    T *get() { return pdata; }
    operator T*() { return pdata; }

private:
    T *pdata;

};

template <class T>
struct AutoZero : public T
{
    AutoZero()
    {
        memset(this, 0, sizeof(T));
        dwSize = sizeof(T);
    }
};

class Unique
{
protected:
    Unique() {}
    virtual ~Unique() {}

private:
    Unique(const Unique &);
    const Unique &operator=(const Unique &);

};

class NoInstance
{
public:
    virtual ~NoInstance() {}

private:
    NoInstance() {}

};

#endif // __STD_MEM_H_
