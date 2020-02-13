
#include "Group.h"
#include "libRenderCore.h"
#include "libObjectModel.h"
#include "libComponents.h"

implement_typeinfo(Group);

Group::Group()
 :  Parent()
{
    constructor(Group);
}

Group::~Group()
{
    destructor(Group);
}

void Group::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);

    SerializeGameObjects(serializer);
    SerializeGroups(serializer);
}

void Group::SerializeGameObjects(const Serializer &serializer)
{
    const PropertySection *section = serializer.Properties->GetChildSectionByType<GameObject>(nullptr);
    while (section)
    {
        BaseObject *baseObject = ObjectManager.Create(section);
        if (baseObject)
        {
            if (GameObject *gameObject = dyn_cast<GameObject>(baseObject))
            {
                Reference<GameObject> gameObjectRef(gameObject);
                m_Objects.push_back(gameObjectRef);
            }
        }
        
        section = serializer.Properties->GetChildSectionByType<GameObject>(section);
    }
}

void Group::SerializeGroups(const Serializer &serializer)
{
    const PropertySection *section = serializer.Properties->GetChildSectionByType<Group>(nullptr);
    while (section)
    {
        BaseObject *baseObject = ObjectManager.Create(section);
        if (baseObject)
        {
            if (Group *group = dyn_cast<Group>(baseObject))
            {
                Reference<Group> groupRef(group);
                m_Groups.push_back(groupRef);
            }
        }
        
        section = serializer.Properties->GetChildSectionByType<Group>(section);
    }
}

void Group::Initialize()
{
    Parent::Initialize();
}

void Group::Destroy()
{
    m_Objects.clear();
    m_Groups.clear();

    Parent::Destroy();
}

void Group::Update()
{
    for (Reference<GameObject> &object : m_Objects)
    {
        if (GameObject *gameObject = object.GetObject())
        {
            gameObject->Update();
        }
    }
    
    for (Reference<Group> &groupRef : m_Groups)
    {
        if (Group *group = groupRef.GetObject())
        {
            group->Update();
        }
    }
}

void Group::AddToRenderList(RenderList &renderList)
{
    for (Reference<GameObject> &object : m_Objects)
    {
        if (GameObject *gameObject = object.GetObject())
        {
            if (VisualComponent *visualComponent = gameObject->GetVisualComponent())
            {
                renderList.PushMatrix(gameObject->GetLocalMatrix());
                visualComponent->AddToRenderList(renderList);
                renderList.PopMatrix();
            }
        }
    }
    
    for (Reference<Group> &groupRef : m_Groups)
    {
        if (Group *group = groupRef.GetObject())
        {
            group->AddToRenderList(renderList);
        }
    }
}

