
#ifndef __STD_CALLBACK_H_
#define __STD_CALLBACK_H_

#include "Type.h"

template <typename R, typename A>
class Callback
{
public:
    typedef R result_type;
    typedef A argument_type;

public:
    virtual ~Callback() {}
    
    virtual R operator()(A arg) const = 0;    
    virtual bool operator==(const Callback<R, A> &rhs) const = 0;

    virtual Callback<R, A> *Clone() const = 0;
    virtual int GetTypeID() const = 0;

};

template <typename R, class O, typename A>
class MemberCallback : public Callback<R, A>
{
public:
    MemberCallback(O *pobj, R(O::*pm)(A)) : m_pobj(pobj), m_pm(pm) { };

    virtual R operator()(A arg) const
    {
        return (m_pobj->*m_pm)(arg);
    }

    virtual bool operator==(const Callback<R, A> &rhs) const
    {
        if (this->GetTypeID() != rhs.GetTypeID())
            return false;

        const MemberCallback<R, O, A> *prhs = static_cast<const MemberCallback<R, O, A> *>(&rhs);
        if ((m_pm == prhs->m_pm) && (m_pobj == prhs->m_pobj))
            return true;

        return false;
    }

    virtual MemberCallback<R, O, A> *Clone() const
    {
        return new MemberCallback<R, O, A>(m_pobj, m_pm);
    }

    virtual int GetTypeID() const { return 1; }

private:
    typedef R(O::*MemFunType) (A);

    mutable O *m_pobj;
    MemFunType m_pm;

};

template <typename R, typename A>
class StaticCallback : public Callback<R, A>
{
public:
    StaticCallback(R(*pf)(A)) : m_pf(pf) { };

    virtual R operator()(A arg) const
    {
        return (m_pf)(arg);
    }
    
    virtual bool operator==(const Callback<R, A> &rhs) const
    {
        if (this->GetTypeID() != rhs.GetTypeID())
            return false;

        const StaticCallback<R, A> *prhs = static_cast<const StaticCallback<R, A> *>(&rhs);
        if (m_pf == prhs->m_pf)
            return true;

        return false;
    }

    virtual StaticCallback<R, A> *Clone() const
    {
        return new StaticCallback<R, A>(m_pf);
    }
    
    virtual int GetTypeID() const { return 2; }

public:
    typedef R(*FunType) (A);

    FunType m_pf;

};

template <typename R, class O, typename A>
MemberCallback<R, O, A> MakeCallback(R(O::*pm)(A), O *pobj)
{
    return MemberCallback<R, O, A>(pobj, pm);
}

template <typename R, typename A>
StaticCallback<R, A> MakeCallback(R(*pf)(A))
{
    return StaticCallback<R, A>(pf);
}

#endif // __STD_CALLBACK_H_
