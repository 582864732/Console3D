#ifndef MATRIX_H_
#define MATRIX_H_
#include"consoleHead.h"

int cross2D(POS p1, POS p2);

matrix4 zero(int n);
matrix4 identity(int n);
matrix4 product(matrix4 m1, matrix4 m2);
void rotateX(float phi, matrix4& matrix);
void rotateY(float phi, matrix4& matrix);
void rotateZ(float phi, matrix4& matrix);
void translate3D(int dx, int dy, int dz, matrix4& matrix);
void scale3D(float sX, float sY, float sZ, matrix4& matrix);
void transform(matrix4 m, vector3& v);
void transform(matrix4 m, vector3f& v);
void projection(int near,int far,float fov,float aspect,matrix4 &matrix);
matrix4 inverse(matrix4 matrix);
POS operator-(POS p1, POS p2);
#endif
