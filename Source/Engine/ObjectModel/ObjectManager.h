
#ifndef __OM_OBJECT_MANAGER_H_
#define __OM_OBJECT_MANAGER_H_

#include "libStandard.h"
#include "libConfig.h"

#include "BaseObject.h"

class BaseCreator
{
public:
    virtual BaseObject *Create() = 0;
};

template<class T>
class ObjectCreator : public BaseCreator
{
public:
    virtual BaseObject *Create()
    {
        T *obj = new T();
        return obj;
    }
};

class _ObjectManager
{
public:
    _ObjectManager();
    ~_ObjectManager();

    template <class T>
    void Register()
    {
        String lName = T::s_TypeInfo.m_pName;
        ToLower(lName);
        ObjectCreator<T> *blueprint = new ObjectCreator<T>();
        m_Blueprints[lName] = blueprint;
    }

    BaseObject *Create(const PropertySection *properties);
    BaseObject *Create(const String &objectType, const String &objectName);

    template <class T>
    T *Create(const PropertySection *properties)
    {
        BaseObject *obj = Create(properties);
        return dyn_cast<T>(obj);
    }

    template <class T>
    T *Create(const String &objectName)
    {
        BaseObject *obj = Create(T::s_TypeInfo.m_pName, objectName);
        return dyn_cast<T>(obj);
    }

    BaseObject *Find(ObjectID objectID) const;
    BaseObject *Find(const String &objectType, const String &objectName) const;

    template <class T>
    T *Find(const String &objectName) const
    {
        ObjectID objectID = GenerateObjectID<T>(objectName);
        BaseObject *obj = Find(objectID);
        return dyn_cast<T>(obj);
    }

    template <class T>
    T *Find(ObjectID objectID) const
    {
        BaseObject *obj = Find(objectID);
        return dyn_cast<T>(obj);
    }

    void OnObjectDestroy(BaseObject *baseObject);

private:
    typedef Map<String, BaseCreator *>   BlueprintMap;
    typedef std::pair<String, BaseCreator *>  BlueprintPair;
    typedef BlueprintMap::iterator       BlueprintIterator;
    typedef BlueprintMap::const_iterator BlueprintConstIterator;
    
    typedef Map<ObjectID, BaseObject *>  ObjectMap;
    typedef std::pair<ObjectID, BaseObject *> ObjectPair;
    typedef ObjectMap::iterator          ObjectIterator;
    typedef ObjectMap::const_iterator    ObjectConstIterator;

    BlueprintMap m_Blueprints;
    ObjectMap m_Objects;

};

#endif // __OM_OBJECT_MANAGER_H_
