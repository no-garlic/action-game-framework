
#ifndef __MATH_QUAT_H_
#define __MATH_QUAT_H_

#include "Type.h"

class Matrix;
class Vector2;
class Vector3;
class Vector4;

class Quat : public D3DXQUATERNION
{
public:
    Quat();
    Quat(const float *f);
    Quat(float x, float y, float z, float w);

    // Makes this quaternion from bary centric coordinates
    Quat &BaryCentric(const Quat &q1, const Quat &q2, const Quat &q3, float f, float g);

    // Sets this quaternion to the conjugate of it's self
    Quat &Conjugate();

    // Get the dot product of this quaternion
    float Dot();

    // Calculates the exponential
    Quat &Exp();

    // Sets the quaternion to 0,0,0,1
    Quat &Identity();
    bool IsIdentity();

    // Inverses the quaternion
    Quat &Inverse();

    // Gets the length of the quaternion
    float Length();
    float LengthSq();

    // Calculates the natural log
    Quat &Ln();

    // Multiply this quaternion with another
    Quat &Multiply(const Quat &q);

    // Normalizes this quaternion
    Quat &Normalize();

    // Sets the quaternion to the rotation
    Quat &RotationAxis(const Vector3 &v, float angle);
    Quat &RotationMatrix(const Matrix &m);
    Quat &RotationYawPitchRoll(float yaw, float pitch, float roll);

    // Gets the rotation from the quaternion
    Quat &ToAxisAngle(Vector3 *axis, float *angle);

    // Spheically interpolate between the two given quaternions
    Quat &Slerp(const Quat &q1, const Quat &q2, float t);
    Quat &Slerp(const Quat &q, float t);

};

#endif //__MATH_QUAT_H_

