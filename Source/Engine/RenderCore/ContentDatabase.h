
#ifndef __RC_CONTENT_DATABASE_H_
#define __RC_CONTENT_DATABASE_H_

#include "libStandard.h"
#include "libConfig.h"
#include "libObjectModel.h"

class ContentDatabaseBase
{
public:
    ContentDatabaseBase();
    virtual ~ContentDatabaseBase();

    bool Load(const String &filename);
    void Clear();

    virtual const TypeInfo &GetContentType() const = 0;

protected:
    bool Init(const String &objectType, const String &databaseName);

    BaseObject *GetOrCreateObject(const String &objectType, const String &objectName);

    bool DuplicateBlueprint(const String &objectType, const String &name, const String &copyName);
    PropertySection *DuplicateBlueprint(PropertySection *source, const String &name);
    void RecurseCopySection(PropertySection *source, PropertySection *dest);

    String                           m_ObjectType;
    FileDatabase                    *m_FileDatabase;
    Map<String, PropertyFile>        m_PropertyFiles;
    Map<ObjectID, PropertySection *> m_Blueprints;
    PropertyFile                     m_DuplicatedPropertyFile;

};

template <class T>
class ContentDatabase : public ContentDatabaseBase
{
public:
    ContentDatabase() : ContentDatabaseBase() {}
    virtual ~ContentDatabase() {}

    virtual const TypeInfo &GetContentType() const
    {
        return T::s_TypeInfo;
    }

    bool Init(const String &databaseName)
    {
        String objectType = T::s_TypeInfo.m_pName;
        return ContentDatabaseBase::Init(objectType, databaseName);
    }

    Reference<T> GetOrCreateObject(const String &name)
    {
        String objectType = T::s_TypeInfo.m_pName;
        BaseObject *object = ContentDatabaseBase::GetOrCreateObject(objectType, name);
        return Reference<T>(object);
    }

    bool DuplicateBlueprint(const String &name, const String &copyName)
    {
        String objectType = T::s_TypeInfo.m_pName;
        return ContentDatabaseBase::DuplicateBlueprint(objectType, name, copyName);

    }

};

#endif // __RC_CONTENT_DATABASE_H_
