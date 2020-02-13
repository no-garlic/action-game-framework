#pragma once
#ifndef __OM_COMPONENT_H_
#define __OM_COMPONENT_H_

#include "libStandard.h"
#include "BaseObject.h"

class GameObject; 

class Component : public BaseObject
{
public:
    declare_typeinfo(BaseObject);

    Component();
    virtual ~Component();
    
    GameObject *GetParent() { return m_Parent; }
    const GameObject *GetParent() const { return m_Parent; }

    virtual void Initialize();
    virtual void Destroy();
        
    virtual void Update();

private:
    friend GameObject;

    GameObject *m_Parent;
    
};

#endif // __OM_COMPONENT_H_

