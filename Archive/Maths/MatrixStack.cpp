
#include "MatrixStack.h"
#include "Vector3.h"
#include "Matrix.h"

MatrixStack::MatrixStack()
{
    if (FAILED(D3DXCreateMatrixStack(0, &m_pmatrix_stack)))
        m_pmatrix_stack = 0;
}

MatrixStack::~MatrixStack()
{
    if (m_pmatrix_stack)
    {
        m_pmatrix_stack->Release();
        m_pmatrix_stack = 0;
    }
}

bool MatrixStack::IsValid()
{
    return (m_pmatrix_stack != 0);
}

Matrix &MatrixStack::GetTop()
{
    return *(reinterpret_cast<Matrix *>(m_pmatrix_stack->GetTop()));
}

bool MatrixStack::LoadIdentity()
{
    return m_pmatrix_stack->LoadIdentity() == TRUE;
}

bool MatrixStack::LoadMatrix(const Matrix &m)
{
    return m_pmatrix_stack->LoadMatrix(&m) == TRUE;
}

bool MatrixStack::MultMatrix(const Matrix &m)
{
    return m_pmatrix_stack->MultMatrix(&m) == TRUE;
}

bool MatrixStack::MultMatrixLocal(const Matrix &m)
{
    return m_pmatrix_stack->MultMatrixLocal(&m) == TRUE;
}

void MatrixStack::Pop()
{
    m_pmatrix_stack->Pop();
}

void MatrixStack::Push()
{
    m_pmatrix_stack->Push();
}

bool MatrixStack::RotateAxis(const Vector3 &v, float angle)
{
    return m_pmatrix_stack->RotateAxis(&v, angle) == TRUE;
}

bool MatrixStack::RotateAxisLocal(const Vector3 &v, float angle)
{
    return m_pmatrix_stack->RotateAxisLocal(&v, angle) == TRUE;
}

bool MatrixStack::RotateYawPitchRoll(float yaw, float pitch, float roll)
{
    return m_pmatrix_stack->RotateYawPitchRoll(yaw, pitch, roll) == TRUE;
}

bool MatrixStack::RotateYawPitchRollLocal(float yaw, float pitch, float roll)
{
    return m_pmatrix_stack->RotateYawPitchRollLocal(yaw, pitch, roll) == TRUE;
}

bool MatrixStack::Scale(float x, float y, float z)
{
    return m_pmatrix_stack->Scale(x, y, z) == TRUE;
}

bool MatrixStack::ScaleLocal(float x, float y, float z)
{
    return m_pmatrix_stack->ScaleLocal(x, y, z) == TRUE;
}

bool MatrixStack::Translate(float x, float y, float z)
{
    return m_pmatrix_stack->Translate(x, y, z) == TRUE;
}

bool MatrixStack::TranslateLocal(float x, float y, float z)
{
    return m_pmatrix_stack->TranslateLocal(x, y, z) == TRUE;
}
