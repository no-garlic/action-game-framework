
#ifndef __MATH_MATRIXSTACK_H_
#define __MATH_MATRIXSTACK_H_

#include "Type.h"

class Matrix;
class Vector3;

class MatrixStack
{
public:
    MatrixStack();
    ~MatrixStack();

    // Get the matrix on the top of the stack
    Matrix &GetTop();

    // Check if the matrix stack is valid
    bool IsValid();

    // Load a matrix onto the stack
    bool LoadIdentity();
    bool LoadMatrix(const Matrix &m);

    // Multiply the given matrix onto the stack
    bool MultMatrix(const Matrix &m);
    bool MultMatrixLocal(const Matrix &m);

    void Pop();
    void Push();

    // Rotation matrix
    bool RotateAxis(const Vector3 &v, float angle);
    bool RotateAxisLocal(const Vector3 &v, float angle);
    bool RotateYawPitchRoll(float yaw, float pitch, float roll);
    bool RotateYawPitchRollLocal(float yaw, float pitch, float roll);

    // Scale matrix
    bool Scale(float x, float y, float z);
    bool ScaleLocal(float x, float y, float z);

    // Translation matrix
    bool Translate(float x, float y, float z);
    bool TranslateLocal(float x, float y, float z);

private:
    ID3DXMatrixStack *m_pmatrix_stack;

};

#endif //__MATH_MATRIXSTACK_H_

