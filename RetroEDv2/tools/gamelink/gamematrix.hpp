#pragma once

namespace FunctionTable
{
void SetIdentityMatrix(Matrix *matrix);
void MatrixMultiply(Matrix *dest, Matrix *matrixA, Matrix *matrixB);
void MatrixTranslateXYZ(Matrix *Matrix, int x, int y, int z, bool32 setIdentity);
void MatrixScaleXYZ(Matrix *matrix, int scaleX, int scaleY, int scaleZ);
void MatrixRotateX(Matrix *matrix, short angle);
void MatrixRotateY(Matrix *matrix, short angle);
void MatrixRotateZ(Matrix *matrix, short angle);
void MatrixRotateXYZ(Matrix *matrix, short rotationX, short rotationY, short rotationZ);
void MatrixInverse(Matrix *dest, Matrix *matrix);
void MatrixCopy(Matrix *matDst, Matrix *matSrc);
} // namespace FunctionTable
