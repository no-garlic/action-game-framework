
#include "Matrix.h"
#include "Plane.h"
#include "Quaternion.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

Matrix MatrixIdentity(Matrix::MakeNewIdentity());

Matrix::Matrix() 
    : D3DXMATRIX() 
{
}

Matrix::Matrix(const float *f) 
    : D3DXMATRIX(f) 
{
}

Matrix::Matrix(float _11, float _12, float _13, float _14,
               float _21, float _22, float _23, float _24,
               float _31, float _32, float _33, float _34,
               float _41, float _42, float _43, float _44)
    : D3DXMATRIX(_11, _12, _13, _14,
                 _21, _22, _23, _24,
                 _31, _32, _33, _34,
                 _41, _42, _43, _44) 
{
}

Matrix Matrix::MakeNewIdentity()
{
    Matrix m;
    m.Identity();
    return m;
}

Matrix Matrix::MakeNewRotationAxis(const Vector3 &axis, float angle)
{
    Matrix m;
    m.RotationAxis(axis, angle);
    return m;
}

Matrix Matrix::MakeNewRotationQuaternion(const Quat &q)
{
    Matrix m;
    m.RotationQuaternion(q);
    return m;
}

Matrix Matrix::MakeNewRotationYawPitchRoll(float yaw, float pitch, float roll)
{
    Matrix m;
    m.RotationYawPitchRoll(yaw, pitch, roll);
    return m;
}

Matrix Matrix::MakeNewTranslation(float x, float y, float z)
{
    Matrix m;
    m.Translation(x, y, z);
    return m;
}

Matrix Matrix::MakeNewTranslation(const Vector3 &trans)
{
    Matrix m;
    m.Translation(trans);
    return m;
}

Matrix Matrix::MakeNewScaling(float x, float y, float z)
{
    Matrix m;
    m.Scaling(x, y, z);
    return m;
}

Matrix Matrix::MakeNewPosRotScale(const Vector3 &pos, const Vector3 &rot, const Vector3 &scale)
{
    Matrix m;
    m = MakeNewScaling(scale.x, scale.y, scale.z);
    m.Multiply(m, Matrix::MakeNewRotationYawPitchRoll(D3DXToRadian(rot.x), D3DXToRadian(rot.y), D3DXToRadian(rot.z)));
    m.SetTAxis(pos);
    return m;
}

Matrix &Matrix::GetIdentityRef()
{
    static Matrix m = Matrix::MakeNewIdentity();
    return m;
}

Matrix &Matrix::AffineTransformation(float scaling, const Vector3 &rotation_center,
    const Quat &rotation, const Vector3 &translation)
{
    D3DXMatrixAffineTransformation(this, scaling,
        &rotation_center, &rotation, &translation);
    return *this;
}

Matrix &Matrix::Identity()
{
    D3DXMatrixIdentity(this);
    return *this;
}

Matrix &Matrix::Inverse(float determinant, const Matrix &m)
{
    D3DXMatrixInverse(this, &determinant, &m);
    return *this;
}

Matrix &Matrix::Inverse(const Matrix &m)
{
    D3DXMatrixInverse(this, 0, &m);
    return *this;
}

bool Matrix::IsIdentity()
{
    return D3DXMatrixIsIdentity(this) == TRUE;
}

Matrix &Matrix::LookAtLH(const Vector3 &eye, const Vector3 &at, const Vector3 &up)
{
    D3DXMatrixLookAtLH(this, &eye, &at, &up);
    return *this;
}

Matrix &Matrix::LookAtRH(const Vector3 &eye, const Vector3 &at, const Vector3 &up)
{
    D3DXMatrixLookAtRH(this, &eye, &at, &up);
    return *this;
}

Matrix &Matrix::Multiply(const Matrix &m1, const Matrix &m2)
{
    D3DXMatrixMultiply(this, &m1, &m2);
    return *this;
}

Matrix &Matrix::PreMultiply(const Matrix &m)
{
    D3DXMatrixMultiply(this, this, &m);
    return *this;
}

Matrix &Matrix::PostMultiply(const Matrix &m)
{
    D3DXMatrixMultiply(this, &m, this);
    return *this;
}

Matrix &Matrix::OrthoLH(float width, float height, float z_near, float z_far)
{
    D3DXMatrixOrthoLH(this, width, height, z_near, z_far);
    return *this;
}

Matrix &Matrix::OrthoRH(float width, float height, float z_near, float z_far)
{
    D3DXMatrixOrthoRH(this, width, height, z_near, z_far);
    return *this;
}

Matrix &Matrix::OrthoOffCenterLH(float left, float right, float bottom,
    float top, float z_near, float z_far)
{
    D3DXMatrixOrthoOffCenterLH(this, left, right, bottom, top, z_near, z_far);
    return *this;
}

Matrix &Matrix::OrthoOffCenterRH(float left, float right, float bottom,
    float top, float z_near, float z_far)
{
    D3DXMatrixOrthoOffCenterRH(this, left, right, top, bottom, z_near, z_far);
    return *this;
}

Matrix &Matrix::PerspectiveFovLH(float fov, float aspect, float z_near, float z_far)
{
    D3DXMatrixPerspectiveFovLH(this, fov, aspect, z_near, z_far);
    return *this;
}

Matrix &Matrix::PerspectiveFovRH(float fov, float aspect, float z_near, float z_far)
{
    D3DXMatrixPerspectiveFovRH(this, fov, aspect, z_near, z_far);
    return *this;
}

Matrix &Matrix::PerspectiveLH(float width, float height, float z_near, float z_far)
{
    D3DXMatrixPerspectiveLH(this, width, height, z_near, z_far);
    return *this;
}

Matrix &Matrix::PerspectiveRH(float width, float height, float z_near, float z_far)
{
    D3DXMatrixPerspectiveRH(this, width, height, z_near, z_far);
    return *this;
}

Matrix &Matrix::PerspectiveOffCenterLH(float left, float right, float bottom,
    float top, float z_near, float z_far)
{
    D3DXMatrixPerspectiveOffCenterLH(this, left, right, bottom, top, z_near, z_far);
    return *this;
}

Matrix &Matrix::PerspectiveOffCenterRH(float left, float right, float bottom,
    float top, float z_near, float z_far)
{
    D3DXMatrixPerspectiveOffCenterRH(this, left, right, bottom, top, z_near, z_far);
    return *this;
}

Matrix &Matrix::Reflect(const Plane &plane)
{
    D3DXMatrixReflect(this, &plane);
    return *this;
}

Matrix &Matrix::Shadow(const Vector4 &light, const Plane &plane)
{
    D3DXMatrixShadow(this, &light, &plane);
    return *this;
}

Matrix &Matrix::RotationAxis(const Vector3 &axis, float angle)
{
    D3DXMatrixRotationAxis(this, &axis, angle);
    return *this;
}

Matrix &Matrix::RotationQuaternion(const Quat &q)
{
    D3DXMatrixRotationQuaternion(this, &q);
    return *this;
}

Matrix &Matrix::RotationYawPitchRoll(float yaw, float pitch, float roll)
{
    D3DXMatrixRotationYawPitchRoll(this, yaw, pitch, roll);
    return *this;
}

Matrix &Matrix::RotationX(float angle)
{
    D3DXMatrixRotationX(this, angle);
    return *this;
}

Matrix &Matrix::RotationY(float angle)
{
    D3DXMatrixRotationY(this, angle);
    return *this;
}

Matrix &Matrix::RotationZ(float angle)
{
    D3DXMatrixRotationZ(this, angle);
    return *this;
}

Matrix &Matrix::Scaling(float x, float y, float z)
{
    D3DXMatrixScaling(this, x, y, z);
    return *this;
}

Matrix &Matrix::Transformation(const Vector3 &scaling_center, const Quat &scaling_rotation, const Vector3 &scaling,
    const Vector3 &rotation_center, const Quat &rotation, const Vector3 &translation)
{
    D3DXMatrixTransformation(this, &scaling_center, &scaling_rotation, &scaling,
        &rotation_center, &rotation, &translation);
    return *this;
}

Matrix &Matrix::Translation(float x, float y, float z)
{
    D3DXMatrixTranslation(this, x, y, z);
    return *this;
}

Matrix &Matrix::Translation(const Vector3 &trans)
{
    D3DXMatrixTranslation(this, trans[0], trans[1], trans[2]);
    return *this;
}

Matrix &Matrix::Transpose(const Matrix &m)
{
    D3DXMatrixTranspose(this, &m);
    return *this;
}

bool Matrix::IsUnitScale() const
{
    float xAxisSquared = (_11 * _11) + (_21 * _21) + (_31 * _31);
    if (xAxisSquared < 0.99f || xAxisSquared > 1.01f)
        return false;

    float yAxisSquared = (_12 * _12) + (_22 * _22) + (_32 * _32);
    if (yAxisSquared < 0.99f || yAxisSquared > 1.01f)
        return false;

    float zAxisSquared = (_13 * _13) + (_23 * _23) + (_33 * _33);
    if (zAxisSquared < 0.99f || zAxisSquared > 1.01f)
        return false;

    return true;
}

String Matrix::ToString(int row) const
{
    Vector4 *pVec = (Vector4 *) &(m[row][0]);
    return pVec->ToString();
}

bool Matrix::FromString(const String &str, int row)
{
    Vector4 *pVec = (Vector4 *) &(m[row][0]);
    return pVec->FromString(str);
}

