
#ifndef __SCN_SCENE_H_
#define __SCN_SCENE_H_

#include "libStandard.h"
#include "libObjectModel.h"
#include "Group.h"
#include "RenderList.h"

class Scene : public Group
{
    declare_typeinfo(Group);
    
public:
    Scene();
    virtual ~Scene();

    virtual void Destroy();

    virtual void Update();

    void AddToRenderList(RenderList &renderList);

    Reference<GameObject> Spawn(const String &blueprintName, const String &name);
    void Unspawn(const String &name);

protected:
    Reference<Group> m_SpawnedObjects;

};

#endif // _SCE_SCENE_H_
