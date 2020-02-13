
#ifndef __OM_REFERENCE_H_
#define __OM_REFERENCE_H_

#include "libStandard.h"
#include "BaseObject.h"

template <typename T>
class Reference
{
public:
    inline Reference()
    {
        constructor(Reference);
        m_Object = nullptr;
    }

    inline explicit Reference(BaseObject *objectPtr)
    {
        constructor(Reference);
        m_Object = dyn_cast<T>(objectPtr);
        SafeRef();
    }

    inline explicit Reference(T *objectPtr)
    {
        constructor(Reference);
        m_Object = objectPtr;
        SafeRef();
    }

    inline Reference(const Reference<T> &other)
    {
        constructor(Reference);
        m_Object = other.m_Object;
        SafeRef();
    }

    inline Reference(Reference<T> && other)
    {
        constructor(Reference);
        m_Object = other.m_Object;
        SafeRef();
    }

    virtual inline ~Reference()
    {
        destructor(Reference);
        Release();
    }
    
    inline Reference &operator=(const Reference<T> &other)
    {
        Assign(other.m_Object);
        return *this;
    }

    inline bool operator==(const Reference<T> &other) const
    {
        return m_Object == other.m_Object;
    }

    inline bool operator!=(const Reference<T> &other) const
    {
        return m_Object != other.m_Object;
    }

    inline bool operator==(const T *otherObjectPtr) const
    {
        return m_Object == otherObjectPtr;
    }

    inline bool operator!=(const T *otherObjectPtr) const
    {
        return m_Object != otherObjectPtr;
    }

    inline T *GetObject() const
    {
        return m_Object;
    }

    inline ObjectID GetObjectID() const
    {
        if (m_Object)
            return m_Object->GetObjectID();

        return ID_InvalidObject;
    }

    inline bool HasValidObjectID() const
    {
        return (GetObjectID() != ID_InvalidObject);
    }

    inline bool IsValid() const
    {
        return (m_Object != nullptr);
    }

    inline bool IsNotValid() const
    {
        return (m_Object == nullptr);
    }

    inline void Assign(T *objectPtr)
    {
        Release();

        m_Object = objectPtr;
        SafeRef();
    }

    inline void Release()
    {
        SafeUnRef();
        m_Object = nullptr;
    }

private:
    inline int SafeUnRef()
    {
        if (m_Object)
        {
            int refCount = m_Object->GetRefCount();
            m_Object->UnRef();
            return refCount - 1;
        }

        return -1;
    }

    inline int SafeRef()
    {
        if (m_Object)
        {
            return m_Object->Ref();
        }

        return -1;
    }

    T *m_Object;
};

#endif // __OM_REFERENCE_H_
