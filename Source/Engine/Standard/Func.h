
#ifndef __STD_FUNC_H_
#define __STD_FUNC_H_

#include "Type.h"

template <class T>
struct Identity : public std::unary_function<T, T>
{
    // The call operator that returns its input
    const T &operator()(const T &object) const { return object; }
};

template <class _Pair>
struct FirstSelector : public std::unary_function<_Pair, typename _Pair::first_type>
{
    // Call operator returns the first element of the pair provided
    const typename _Pair::first_type &operator()(const _Pair &object) const { return object.first; }
};

template <class _Pair>
struct SecondSelector : public std::unary_function<_Pair, typename _Pair::second_type>
{
    // Call operator returns the first element of the pair provided
    const typename _Pair::second_type &operator()(const _Pair &object) const { return object.second; }
};

template <class T, typename R, R T::*Member>
struct MemberSelector : public std::unary_function<T, R>
{
    // Call operator returns the nominated element of the object provided
    const R &operator()(const T &object) const { return object.*Member; }
};

template <class Result>
struct NullaryFunction
{
    typedef Result result_type;
};

template <class Result>
class PointerToNullaryFunction : public NullaryFunction<Result>
{
public:
    PointerToNullaryFunction() {}
    explicit PointerToNullaryFunction(Result(*ptr)()) : m_ptr(ptr) {}

    Result operator()() const { return m_ptr(); }

protected:
    Result(*m_ptr)();

};

template <class Result>
inline PointerToNullaryFunction<Result> ptr_fun(Result(*ptr)())
{
    return PointerToNullaryFunction<Result>(ptr);
}

#endif // __STD_FUNC_H_
