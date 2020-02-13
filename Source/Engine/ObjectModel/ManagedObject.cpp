
#include "ManagedObject.h"
#include "ObjectManager.h"
#include "libRenderCore.h"

implement_typeinfo(ManagedObject);

ManagedObject::ManagedObject()
:   Parent(),
    m_ReferenceCount(0)
{
    constructor(ManagedObject);
}

ManagedObject::ManagedObject(const ManagedObject &other)
 :  Parent(other),
    m_ReferenceCount(0)
{
    constructor(ManagedObject);
}
    
ManagedObject::~ManagedObject()
{
    destructor(ManagedObject);
}

void ManagedObject::UnRef() const
{
    m_ReferenceCount--;
    if (m_ReferenceCount <= 0)
    {
        ManagedObject *thisObject = const_cast<ManagedObject *>(this);
        ObjectManager.OnObjectDestroy(thisObject);
        thisObject->Destroy();
        delete this;
    }
}