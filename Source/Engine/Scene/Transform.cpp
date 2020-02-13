
#include "Transform.h"
#include "ApplicationCentral.h"
#include "ObjectManager.h"

implement_typeinfo(Transform);

Transform::Transform()
:   ManagedObject()
{
    constructor(Transform);

    m_Scale = float2(1.0f, 1.0f);
    m_Position = float2(0.0f, 0.0f);
    m_Rotation = 0.0f;
    m_MatrixDirty = true;
}

Transform::~Transform()
{
    destructor(Transform);
}

void Transform::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);

    m_Position << serializer("Position");
    m_Rotation << serializer("Rotation");
    m_Scale    << serializer("Scale");

    m_MatrixDirty = true;
}

void Transform::Initialize()
{
    Parent::Initialize();

    GetLocalMatrix();
}

void Transform::Destroy()
{
    Parent::Destroy();
}

mat4 Transform::GetLocalMatrix() const
{
    if (m_MatrixDirty)
    {
        m_LocalMatrix = mat4();
        m_LocalMatrix = translate(m_LocalMatrix, vec3(m_Position, 0));
        m_LocalMatrix = scale(m_LocalMatrix, vec3(m_Scale, 1));
        m_LocalMatrix = rotate(m_LocalMatrix, radians(m_Rotation), vec3(0, 0, 1));
        m_MatrixDirty = false;
    }

    return m_LocalMatrix;
}

