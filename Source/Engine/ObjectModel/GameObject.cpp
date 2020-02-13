
#include "GameObject.h"
#include "ApplicationCentral.h"
#include "ObjectManager.h"
#include "VisualComponent.h"

implement_typeinfo(GameObject);

GameObject::GameObject()
:   Parent()
{
    constructor(GameObject);
}

GameObject::~GameObject()
{
    destructor(GameObject);
}

void GameObject::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);

    const PropertySectionMap &componentSections = serializer.ChildSections;
    for (auto pair : componentSections)
    {
        const PropertySection *componentSection = pair.second;
        if (BaseObject *object = ObjectManager.Create(componentSection))
        {
            if (Component *component = dyn_cast<Component>(object))
            {
                component->m_Parent = this;
                m_Components.push_back(component);
            }
            else
            {
                delete object;
            }
        }
    }
}

void GameObject::Initialize()
{
    Parent::Initialize();
}

void GameObject::Destroy()
{
    for (Component *component : m_Components)
    {
        ObjectManager.OnObjectDestroy(component);
        delete component;
    }
    m_Components.clear();

    Parent::Destroy();
}

void GameObject::Update()
{
    for (Component *component : m_Components)
    {
        component->Update();
    }
}

const VisualComponent *GameObject::GetVisualComponent() const
{
    if (const VisualComponent *component = GetComponentByType<VisualComponent>())
    {
        return component;
    }
    return nullptr;
}

VisualComponent *GameObject::GetVisualComponent()
{
    if (VisualComponent *component = GetComponentByType<VisualComponent>())
    {
        return component;
    }
    return nullptr;
}

Reference<GameObject> GameObject::Spawn(const String &name)
{
    Reference<GameObject> spawnedObject;

    if (ContentDatabase<GameObject> *contentDatabase = Application.GetContentDatabase<GameObject>())
    {
        spawnedObject = contentDatabase->GetOrCreateObject(name);
    }

    return spawnedObject;
}
