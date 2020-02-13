
#include "PropertyValue.h"
#include "BaseObject.h"

PropertyValue::PropertyValue()
{
    constructor(PropertyValue);
}

PropertyValue::PropertyValue(const PropertyValue &other)
{
    constructor(PropertyValue);
    *this = other;
}

PropertyValue::~PropertyValue()
{
    destructor(PropertyValue);
}

PropertyValue::operator float2() const
{
    float2 result;

    String value = Value;
    EraseChars(value, " ()[];");
    StringArray tokens = Tokenize(value, ',');

    if (tokens.size() >= 2)
    {
        result.x = LexicalCast<float, String>(tokens[0]);
        result.y = LexicalCast<float, String>(tokens[1]);
    }

    return result;
}

PropertyValue::operator float3() const
{
    float3 result;

    String value = Value;
    EraseChars(value, " ()[];");
    StringArray tokens = Tokenize(value, ',');

    if (tokens.size() >= 3)
    {
        result.x = LexicalCast<float, String>(tokens[0]);
        result.y = LexicalCast<float, String>(tokens[1]);
        result.z = LexicalCast<float, String>(tokens[2]);
    }

    return result;
}

PropertyValue::operator float4() const
{
    float4 result;

    String value = Value;
    EraseChars(value, " ()[];");
    StringArray tokens = Tokenize(value, ',');

    if (tokens.size() >= 4)
    {
        result.x = LexicalCast<float, String>(tokens[0]);
        result.y = LexicalCast<float, String>(tokens[1]);
        result.z = LexicalCast<float, String>(tokens[2]);
        result.w = LexicalCast<float, String>(tokens[3]);
    }

    return result;
}

PropertyValue::operator int2() const
{
    int2 result;

    String value = Value;
    EraseChars(value, " ()[];");
    StringArray tokens = Tokenize(value, ',');

    if (tokens.size() >= 2)
    {
        result.x = LexicalCast<int, String>(tokens[0]);
        result.y = LexicalCast<int, String>(tokens[1]);
    }

    return result;
}

PropertyValue::operator int3() const
{
    int3 result;

    String value = Value;
    EraseChars(value, " ()[];");
    StringArray tokens = Tokenize(value, ',');

    if (tokens.size() >= 3)
    {
        result.x = LexicalCast<int, String>(tokens[0]);
        result.y = LexicalCast<int, String>(tokens[1]);
        result.z = LexicalCast<int, String>(tokens[2]);
    }

    return result;
}

PropertyValue::operator int4() const
{
    int4 result;

    String value = Value;
    EraseChars(value, " ()[];");
    StringArray tokens = Tokenize(value, ',');

    if (tokens.size() >= 4)
    {
        result.x = LexicalCast<int, String>(tokens[0]);
        result.y = LexicalCast<int, String>(tokens[1]);
        result.z = LexicalCast<int, String>(tokens[2]);
        result.w = LexicalCast<int, String>(tokens[3]);
    }

    return result;
}

PropertyValue::operator bool2() const
{
    bool2 result;

    String value = Value;
    EraseChars(value, " ()[];");
    StringArray tokens = Tokenize(value, ',');

    if (tokens.size() >= 2)
    {
        result.x = LexicalCast<bool, String>(tokens[0]);
        result.y = LexicalCast<bool, String>(tokens[1]);
    }

    return result;
}

PropertyValue::operator bool3() const
{
    bool3 result;

    String value = Value;
    EraseChars(value, " ()[];");
    StringArray tokens = Tokenize(value, ',');

    if (tokens.size() >= 3)
    {
        result.x = LexicalCast<bool, String>(tokens[0]);
        result.y = LexicalCast<bool, String>(tokens[1]);
        result.z = LexicalCast<bool, String>(tokens[2]);
    }

    return result;
}

PropertyValue::operator bool4() const
{
    bool4 result;

    String value = Value;
    EraseChars(value, " ()[];");
    StringArray tokens = Tokenize(value, ',');

    if (tokens.size() >= 4)
    {
        result.x = LexicalCast<bool, String>(tokens[0]);
        result.y = LexicalCast<bool, String>(tokens[1]);
        result.z = LexicalCast<bool, String>(tokens[2]);
        result.w = LexicalCast<bool, String>(tokens[3]);
    }

    return result;
}

const String &PropertyValue::AsString() const
{
    return Value;
}

bool PropertyValue::AsObject(String &objectType, String &objectName) const
{
    StringArray tokens = Tokenize(Value, '(');

    if (tokens.size() == 2)
    {
        objectType = tokens[0];
        objectName = tokens[1];

        EraseChars(objectName, " ()[];");
        EraseChars(objectType, " ()[];");

        return true;
    }

    return false;
}

ObjectID PropertyValue::AsObjectID() const
{
    String objectType, objectName;
    if (AsObject(objectType, objectName))
    {
        return GenerateObjectID(objectType, objectName);
    }
    return ID_InvalidObject;
}

void PropertyValue::SetAsObject(String &objectType, String &objectName)
{
    Value = PrintF("%s(%s);", objectType.c_str(), objectName.c_str());
}

bool PropertyValue::operator==(BaseObject *object) const
{
    String thisType, thisName;
    if (AsObject(thisType, thisName) && object)
    {
        String otherName = object->GetName();
        String otherType = object->GetTypeInfo()->m_pName;

        if (otherName == thisName && otherType == thisType)
            return true;
    }
    return false;
}
