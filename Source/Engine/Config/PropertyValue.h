
#ifndef __CFG_PROPERTY_VALUE_H_
#define __CFG_PROPERTY_VALUE_H_

#include "libStandard.h"

class BaseObject;

#define config_value_native_property(Type,Name) \
    operator Type() const { return LexicalCast<Type, String>(Value); } \
    Type As##Name() const { return (Type) *this; } \
    void operator=(Type value) { Value = ToString(value); } \
    bool operator==(Type value) { return ToString(value) == Value; }

#define config_value_string_property(Type) \
    operator const Type &() const { return Value; } \
    const Type &As##Type() const; \
    void operator=(const Type &value) { Value = value; } \
    bool operator==(const Type &value) { return Value == value; }

#define config_value_struct_property(Type,Name) \
    operator Type() const; \
    Type As##Name() const { return (Type) *this; } \
    void operator=(const Type &value) { Value = ToString(value); } \
    bool operator==(const Type &value) { return value == (Type) (*this); }

class PropertyValue
{
public:
    PropertyValue();
    PropertyValue(const PropertyValue &other);
    ~PropertyValue();

    String Value;

    config_value_native_property(float,  Float);
    config_value_native_property(uint,   UInt);
    config_value_native_property(ulong,  ULong);
    config_value_native_property(int,    Int);
    config_value_native_property(bool,   Bool);
    config_value_struct_property(float2, Float2);
    config_value_struct_property(float3, Float3);
    config_value_struct_property(float4, Float4);
    config_value_struct_property(int2,   Int2);
    config_value_struct_property(int3,   Int3);
    config_value_struct_property(int4,   Int4);
    config_value_struct_property(bool2,  Bool2);
    config_value_struct_property(bool3,  Bool3);
    config_value_struct_property(bool4,  Bool4);
    config_value_string_property(String);

    bool AsObject(String &objectType, String &objectName) const;
    ObjectID AsObjectID() const;
    void SetAsObject(String &objectType, String &objectName);
    bool operator==(BaseObject *object) const;

};

template<typename T>
inline T &operator<<(T &out, const PropertyValue &in)
{
    if (in.Value.length() > 0)
    {
        out = in;
    }
    return out;
}

template<>
inline String &operator<<(String &out, const PropertyValue &in)
{
    out = in.Value;
    return out;
}

#endif // __CFG_PROPERTY_VALUE_H_
