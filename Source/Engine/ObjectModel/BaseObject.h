
#ifndef __OM_BASE_OBJECT_H_
#define __OM_BASE_OBJECT_H_

#include "libStandard.h"
#include "libConfig.h"
#include "TypeInfo.h"

class BaseObject;
class PropertyBase;
class _ObjectManager;

ObjectID GenerateObjectID(const TypeInfo *typeInfo, const String &objectName);
ObjectID GenerateObjectID(const String &objectType, const String &objectName);
ObjectID GenerateObjectID(const BaseObject *baseObject);

template <class T>
ObjectID GenerateObjectID(const String &objectName)
{
    return GenerateObjectID(&(T::s_TypeInfo), objectName);
}

struct Serializer
{
public:
    inline explicit Serializer(const PropertySection *properties)
    :   Properties(properties),
        ChildSections(properties->GetChildSections()),
        IsError(false)
    {
    }

    inline PropertyValue operator()(const String &key) const
    {
        PropertyValue propertyValue;
        if (!Properties->GetValue(key, propertyValue))
        {
            IsError = true;
        }
        return propertyValue;
    }

    const PropertySection *Properties;
    const PropertySectionMap &ChildSections;
    mutable bool IsError;

};

class BaseObject
{
    declare_typeinfo(BaseObject);

public:
    BaseObject();
    virtual ~BaseObject();
    
    const String &GetName() const
    {
        return m_Name;
    }
    
    ObjectID GetObjectID() const
    {
        if (m_ObjectId == ID_InvalidObject)
        {
            m_ObjectId = GenerateObjectID(this);
        }

        return m_ObjectId;
    }

    virtual void Serialize(const Serializer &serializer)
    {
    }

    virtual void Initialize()
    {
    }

protected:
    friend class _ObjectManager;

    void SetName(const String &name)
    {
        m_Name = name;
        m_ObjectId = ID_InvalidObject;
    }
            
    String m_Name;
    mutable ObjectID m_ObjectId;

};

#endif // __OM_BASE_OBJECT_H_
