
#ifndef __OM_GAME_OBJECT_H_
#define __OM_GAME_OBJECT_H_

#include "libStandard.h"
#include "libConfig.h"
#include "Reference.h"
#include "Transform.h"
#include "Component.h"

class GameObject;
class VisualComponent;

typedef Array<GameObject *> GameObjectArray;
typedef Array< Reference<GameObject> > GameObjectReferenceArray;

class GameObject : public Transform
{
    declare_typeinfo(Transform);

public:
    GameObject();
    virtual ~GameObject();

    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    virtual void Update();
    
    template <class T>
    inline T *GetComponentByType()
    {
        for (Component *c : m_Components)
        {
            if (T *t = dyn_cast<T>(c))
                return t;
        }
        return nullptr;
    }

    template <class T>
    inline const T *GetComponentByType() const
    {
        for (const Component *c : m_Components)
        {
            if (const T *t = dyn_cast<const T>(c))
                return t;
        }
        return nullptr;
    }

    VisualComponent *GetVisualComponent();
    const VisualComponent *GetVisualComponent() const;

    static Reference<GameObject> Spawn(const String &name);    

protected:
    Array<Component *> m_Components;

};

#endif // __OM_GAME_OBJECT_H_
