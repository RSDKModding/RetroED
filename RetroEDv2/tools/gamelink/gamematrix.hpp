#ifndef GAMEMATRIX_HPP
#define GAMEMATRIX_HPP

namespace FunctionTable {
    void setIdentityMatrix(Matrix *matrix);
    void matrixMultiply(Matrix *dest, Matrix *matrixA, Matrix *matrixB);
    void matrixTranslateXYZ(Matrix *Matrix, int x, int y, int z, bool32 setIdentity);
    void matrixScaleXYZ(Matrix *matrix, int scaleX, int scaleY, int scaleZ);
    void matrixRotateX(Matrix *matrix, short angle);
    void matrixRotateY(Matrix *matrix, short angle);
    void matrixRotateZ(Matrix *matrix, short angle);
    void matrixRotateXYZ(Matrix *matrix, short rotationX, short rotationY, short rotationZ);
    void matrixInverse(Matrix *dest, Matrix *matrix);
    void matrixCopy(Matrix *matDst, Matrix *matSrc);
}

#endif