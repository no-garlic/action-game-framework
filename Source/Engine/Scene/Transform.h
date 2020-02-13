
#ifndef __SCN_TRANSFORM_H_
#define __SCN_TRANSFORM_H_

#include "libStandard.h"
#include "ManagedObject.h"

class Transform : public ManagedObject
{
    declare_typeinfo(ManagedObject);

public:
    Transform();
    virtual ~Transform();

    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    mat4 GetLocalMatrix() const;

    inline const float2 &GetPosition() const { return m_Position; }
    inline void SetPosition(const float2 &position);

    inline const float2 &GetScale() const { return m_Scale; }
    inline void SetScale(const float2 &scale);

    inline float GetRotation() const { return m_Rotation; }
    inline void SetRotation(float rotation);

protected:
    float2 m_Position;
    float2 m_Scale;
    float  m_Rotation;

    mutable bool m_MatrixDirty;
    mutable mat4 m_LocalMatrix;

};

inline void Transform::SetPosition(const float2 &position)
{
    m_Position = position;
    m_MatrixDirty = true; 
}

inline void Transform::SetScale(const float2 &scale)
{
    m_Scale = scale;
    m_MatrixDirty = true; 
}

inline void Transform::SetRotation(float rotation)
{
    m_Rotation = rotation;
    m_MatrixDirty = true;
}

#endif // __SCN_TRANSFORM_H_
