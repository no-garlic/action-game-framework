
#include "ObjectManager.h"

implement_typeinfo(BaseObject);

ObjectID GenerateObjectID(const TypeInfo *typeInfo, const String &objectName)
{
    String objectType = typeInfo->m_pName;
    return GenerateObjectID(objectType, objectName);
}

ObjectID GenerateObjectID(const String &objectType, const String &objectName)
{
    if (objectName.length() == 0 || objectType.length() == 0)
        return ID_InvalidObject;

    String lName = ToLower(objectName);
    String typeName = PrintF("Type:%s Name:%s", objectType.c_str(), lName.c_str());    
    ObjectID objectId = (ObjectID) GetCRC(typeName);
        
    return objectId;
}

ObjectID GenerateObjectID(const BaseObject *baseObject)
{
    const TypeInfo *typeInfo = baseObject->GetTypeInfo();
    const String &objectName = baseObject->GetName();

    return GenerateObjectID(typeInfo, objectName);
}

BaseObject::BaseObject()
 :  m_ObjectId(ID_InvalidObject)
{
    constructor(BaseObject);
}

BaseObject::~BaseObject()
{
    destructor(BaseObject);
}
