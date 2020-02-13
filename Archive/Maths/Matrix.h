
#ifndef __MATH_MATRIX_H
#define __MATH_MATRIX_H

#include "Type.h"
#include "Str.h"
#include "Mathematics.h"
#include "Mem.h"
#include "Log.h"
#include "Vector3.h"

class Matrix;
class Plane;
class Quat;
class Vector2;
class Vector4;

extern Matrix MatrixIdentity;

class Matrix : public D3DXMATRIX
{
public:
    Matrix();
    Matrix(const float *f);
    Matrix(float _11, float _12, float _13, float _14,
           float _21, float _22, float _23, float _24,
           float _31, float _32, float _33, float _34,
           float _41, float _42, float _43, float _44);

    static Matrix MakeNewIdentity();
    static Matrix MakeNewRotationAxis(const Vector3 &axis, float angle);
    static Matrix MakeNewRotationQuaternion(const Quat &q);
    static Matrix MakeNewRotationYawPitchRoll(float yaw, float pitch, float roll);
    static Matrix MakeNewTranslation(float x, float y, float z);
    static Matrix MakeNewTranslation(const Vector3 &trans);
    static Matrix MakeNewScaling(float x, float y, float z);
    static Matrix MakeNewPosRotScale(const Vector3 &pos, const Vector3 &rot, const Vector3 &scale);

    static Matrix &GetIdentityRef();
    bool    IsIdentity();
    Matrix &Identity();

    Vector3 GetTAxis() const { return Vector3(_41, _42, _43); }
    Vector3 GetXAxis() const { return Vector3(_11, _21, _31); }
    Vector3 GetYAxis() const { return Vector3(_12, _22, _32); }
    Vector3 GetZAxis() const { return Vector3(_13, _23, _33); }
    void GetTAxis(Vector3 &v) const { v.Set(_41, _42, _43); }
    void GetXAxis(Vector3 &v) const { v.Set(_11, _21, _31); }
    void GetYAxis(Vector3 &v) const { v.Set(_12, _22, _32); }
    void GetZAxis(Vector3 &v) const { v.Set(_13, _23, _33); }

    void SetTAxis(const Vector3 &v) { _41 = v.x; _42 = v.y; _43 = v.z; }
    void SetXAxis(const Vector3 &v) { _11 = v.x; _21 = v.y; _31 = v.z; }
    void SetYAxis(const Vector3 &v) { _12 = v.x; _22 = v.y; _32 = v.z; }
    void SetZAxis(const Vector3 &v) { _13 = v.x; _23 = v.y; _33 = v.z; }

    Vector3 GetScale() const { return Vector3(GetXAxis().Length(), GetYAxis().Length(), GetZAxis().Length()); }
    void GetScale(Vector3 &v) const { v.Set(GetXAxis().Length(), GetYAxis().Length(), GetZAxis().Length()); }
    bool IsUnitScale() const;
    inline float GetScaleMax() const;

    Matrix &Inverse(float determinant, const Matrix &m);
    Matrix &Inverse(const Matrix &m);
    Matrix &Inverse() { return Inverse(*this); }

    Matrix &Transpose(const Matrix &m);
    Matrix &Transpose() { return Transpose(*this); }

    Matrix &LookAtLH(const Vector3 &eye, const Vector3 &at, const Vector3 &up);
    Matrix &LookAtRH(const Vector3 &eye, const Vector3 &at, const Vector3 &up);

    Matrix &Multiply(const Matrix &m1, const Matrix &m2);
    Matrix &Multiply(const Matrix &m2) { return Multiply(*this, m2); }
    Matrix &PreMultiply(const Matrix &m);
    Matrix &PostMultiply(const Matrix &m);

    Matrix &OrthoLH(float width, float height, float z_near, float z_far);
    Matrix &OrthoRH(float width, float height, float z_near, float z_far);
    Matrix &OrthoOffCenterLH(float left, float right, float bottom, float top, float z_near, float z_far);
    Matrix &OrthoOffCenterRH(float left, float right, float bottom, float top, float z_near, float z_far);

    Matrix &PerspectiveFovLH(float fov, float aspect, float z_near, float z_far);
    Matrix &PerspectiveFovRH(float fov, float aspect, float z_near, float z_far);
    Matrix &PerspectiveLH(float width, float height, float z_near, float z_far);
    Matrix &PerspectiveRH(float width, float height, float z_near, float z_far);
    Matrix &PerspectiveOffCenterLH(float left, float right, float bottom, float top, float z_near, float z_far);
    Matrix &PerspectiveOffCenterRH(float left, float right, float bottom, float top, float z_near, float z_far);

    Matrix &Reflect(const Plane &plane);
    Matrix &Shadow(const Vector4 &light, const Plane &plane);

    Matrix &Translation(float x, float y, float z);
    Matrix &Translation(const Vector3 &trans);

    Matrix &RotationAxis(const Vector3 &axis, float angle);
    Matrix &RotationQuaternion(const Quat &q);
    Matrix &RotationYawPitchRoll(float yaw, float pitch, float roll);
    Matrix &RotationX(float angle);
    Matrix &RotationY(float angle);
    Matrix &RotationZ(float angle);

    Matrix &Scaling(float x, float y, float z);

    Matrix &Transformation(const Vector3 &scaling_center, const Quat &scaling_rotation, const Vector3 &scaling,
        const Vector3 &rotation_center, const Quat &rotation, const Vector3 &translation);
    Matrix &AffineTransformation(float scaling, const Vector3 &rotation_center,
        const Quat &rotation, const Vector3 &translation);

    String ToString(int row) const;
    bool FromString(const String &str, int row);

};

inline float Matrix::GetScaleMax() const
{
    float x = GetXAxis().Length();
    float y = GetYAxis().Length();
    float z = GetZAxis().Length();
    float maxScale = Max<float>(x, Max<float>(y, z));
    return maxScale;
}

#endif // __MATH_MATRIX_H

