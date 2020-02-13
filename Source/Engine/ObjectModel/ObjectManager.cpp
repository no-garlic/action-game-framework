
#include "ObjectManager.h"

_ObjectManager::_ObjectManager()
{
    constructor(ObjectManager);
}

_ObjectManager::~_ObjectManager()
{
    destructor(ObjectManager);

    for (BlueprintPair pair : m_Blueprints)
    {
        delete pair.second;
    }
    m_Blueprints.clear();
}

BaseObject *_ObjectManager::Create(const PropertySection *properties)
{
    if (properties == nullptr)
    {
        return nullptr;
    }

    BlueprintIterator i = m_Blueprints.find(properties->GetType());
    if (i == m_Blueprints.end())
    {
        return nullptr;
    }

    BaseObject *obj = i->second->Create();
    obj->SetName(properties->GetName());

    ObjectID objectID = obj->GetObjectID();
    ASSERT(objectID != ID_InvalidObject, "Object does not have a valid ObjectID");
    ASSERT(m_Objects.find(objectID) == m_Objects.end(), "Cannot have multiple objects with the same ObjectID");
    m_Objects[objectID] = obj;

    Serializer serializer(properties);
    obj->Serialize(serializer);
    obj->Initialize();

    return obj;
}

BaseObject *_ObjectManager::Create(const String &objectType, const String &objectName)
{
    String lObjectType = ToLower(objectType);
    BlueprintIterator i = m_Blueprints.find(lObjectType);
    if (i == m_Blueprints.end())
    {
        return nullptr;
    }

    BaseObject *obj = i->second->Create();
    obj->SetName(objectName);

    ObjectID objectID = obj->GetObjectID();
    ASSERT(objectID != ID_InvalidObject, "Object does not have a valid ObjectID");
    ASSERT(m_Objects.find(objectID) == m_Objects.end(), "Cannot have multiple objects with the same ObjectID");
    m_Objects[objectID] = obj;

    obj->Initialize();
    return obj;
}

BaseObject *_ObjectManager::Find(ObjectID objectID) const
{
    auto iter = m_Objects.find(objectID);
    if (iter == m_Objects.end())
    {
        return nullptr;
    }

    return iter->second;
}

BaseObject *_ObjectManager::Find(const String &objectType, const String &objectName) const
{
    ObjectID objectID = GenerateObjectID(objectType, objectName);
    return Find(objectID);
}

void _ObjectManager::OnObjectDestroy(BaseObject *baseObject)
{
    ObjectID objectID = baseObject->GetObjectID();
    size_t result = m_Objects.erase(objectID);
    ASSERT(result == 1, "Failed to remove the object from the ObjectManager");
}
