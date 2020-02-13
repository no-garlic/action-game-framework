
#ifndef __SCN_GROUP_H_
#define __SCN_GROUP_H_

#include "libStandard.h"
#include "libObjectModel.h"
#include "RenderList.h"

class Group;
typedef Array< Reference<Group> > GroupReferenceArray;

class Group : public ManagedObject
{
    declare_typeinfo(ManagedObject);
    
public:
    Group();
    virtual ~Group();

    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    virtual void Update();

    GameObjectReferenceArray GetGameObjects() { return m_Objects; }
    const GameObjectReferenceArray GetGameObjects() const { return m_Objects; }

    void AddToRenderList(RenderList &renderList);

protected:
    friend class Scene;

    void SerializeGameObjects(const Serializer &serializer);
    void SerializeGroups(const Serializer &serializer);

    GameObjectReferenceArray m_Objects;
    GroupReferenceArray m_Groups;

};

#endif // __SCN_GROUP_H_
