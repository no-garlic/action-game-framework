
#include "Quaternion.h"
#include "Matrix.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

Quat::Quat() : D3DXQUATERNION() {}
Quat::Quat(const float *f) : D3DXQUATERNION(f) {}
Quat::Quat(float x, float y, float z, float w) : D3DXQUATERNION(x, y, z, w) {}

Quat &Quat::BaryCentric(const Quat &q1, const Quat &q2, const Quat &q3, float f, float g)
{
    D3DXQuaternionBaryCentric(this, &q1, &q2, &q3, f, g);
    return *this;
}

Quat &Quat::Conjugate()
{
    D3DXQuaternionConjugate(this, this);
    return *this;
}

float Quat::Dot()
{
    return D3DXQuaternionDot(this, this);
}

Quat &Quat::Exp()
{
    D3DXQuaternionExp(this, this);
    return *this;
}

Quat &Quat::Identity()
{
    D3DXQuaternionIdentity(this);
    return *this;
}

Quat &Quat::Inverse()
{
    D3DXQuaternionInverse(this, this);
    return *this;
}

bool Quat::IsIdentity()
{
    return D3DXQuaternionIsIdentity(this) == TRUE;
}

float Quat::Length()
{
    return D3DXQuaternionLength(this);
}

float Quat::LengthSq()
{
    return D3DXQuaternionLengthSq(this);
}

Quat &Quat::Ln()
{
    D3DXQuaternionLn(this, this);
    return *this;
}

Quat &Quat::Multiply(const Quat &q)
{
    D3DXQuaternionMultiply(this, this, &q);
    return *this;
}

Quat &Quat::Normalize()
{
    D3DXQuaternionNormalize(this, this);
    return *this;
}

Quat &Quat::RotationAxis(const Vector3 &v, float angle)
{
    D3DXQuaternionRotationAxis(this, &v, angle);
    return *this;
}

Quat &Quat::RotationMatrix(const Matrix &m)
{
    D3DXQuaternionRotationMatrix(this, &m);
    return *this;
}

Quat &Quat::RotationYawPitchRoll(float yaw, float pitch, float roll)
{
    D3DXQuaternionRotationYawPitchRoll(this, yaw, pitch, roll);
    return *this;
}

Quat &Quat::Slerp(const Quat &q1, const Quat &q2, float t)
{
    D3DXQuaternionSlerp(this, &q1, &q2, t);
    return *this;
}

Quat &Quat::Slerp(const Quat &q, float t)
{
    D3DXQuaternionSlerp(this, this, &q, t);
    return *this;
}

Quat &Quat::ToAxisAngle(Vector3 *axis, float *angle)
{
    D3DXQuaternionToAxisAngle(this, axis, angle);
    return *this;
}
