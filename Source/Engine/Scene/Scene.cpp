
#include "Scene.h"
#include "libRenderCore.h"
#include "libObjectModel.h"
#include "SpriteComponent.h"
#include "AnimationComponent.h"

implement_typeinfo(Scene);

Scene::Scene()
 : Parent()
{
    constructor(Scene);
}

Scene::~Scene()
{
    destructor(Scene);
}

void Scene::AddToRenderList(RenderList &renderList)
{
    Parent::AddToRenderList(renderList);

    if (Group *group = m_SpawnedObjects.GetObject())
    {
        group->AddToRenderList(renderList);
    }
}

Reference<GameObject> Scene::Spawn(const String &blueprintName, const String &name)
{
    Reference<GameObject> gameObjectRef;

    if (name == blueprintName)
        return gameObjectRef;

    if (!Application.GetContentDatabase<GameObject>()->DuplicateBlueprint(blueprintName, name))
        return gameObjectRef;

    gameObjectRef = GameObject::Spawn(name);
    if (GameObject *gameObject = gameObjectRef.GetObject())
    {
        Group *group = m_SpawnedObjects.GetObject();
        if (!group)
        {
            group = ObjectManager.Create<Group>("SpawnedObjects");
            m_SpawnedObjects.Assign(group);
        }

        group->m_Objects.push_back(gameObjectRef);
    }

    return gameObjectRef;
}

void Scene::Unspawn(const String &name)
{
    if (Group *group = m_SpawnedObjects.GetObject())
    {
        ObjectID objectId = GenerateObjectID<GameObject>(name);

        for (Reference<GameObject> &gameObjectRef : group->m_Objects)
        {
            if (gameObjectRef.GetObjectID() == objectId)
            {
                gameObjectRef.Release();
            }
        }
    }
}

void Scene::Update()
{
    Parent::Update();

    if (Group *group = m_SpawnedObjects.GetObject())
    {
        group->Update();
    }
}

void Scene::Destroy()
{
    Parent::Destroy();
    
    if (Group *group = m_SpawnedObjects.GetObject())
    {
        group->Destroy();
    }
    m_SpawnedObjects.Release();
}
