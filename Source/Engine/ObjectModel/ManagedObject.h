
#ifndef __OM_MANAGED_OBJECT_H_
#define __OM_MANAGED_OBJECT_H_

#include "BaseObject.h"

class ManagedObject : public BaseObject
{
public:
    declare_typeinfo(BaseObject);

    ManagedObject();
    ManagedObject(const ManagedObject &other);    
    virtual ~ManagedObject();

    int Ref() const
    {
        return ++m_ReferenceCount;
    }

    void UnRef() const;

    int GetRefCount() const
    {
        return m_ReferenceCount;
    }

protected:
    virtual void Destroy()
    {
    }

private:    
    void SetRefCount(uint ref) const
    {
        m_ReferenceCount = ref;
    }

    mutable int m_ReferenceCount;

};

#endif // __OM_MANAGED_OBJECT_H_
