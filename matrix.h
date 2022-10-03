#ifndef MATRIX_H_
#define MATRIX_H_
#include"consoleHead.h"
;//IDN
//零矩阵和单位矩阵生成
matrix4 zero(int n);
matrix4 identity(int n);
//矩阵乘法
matrix4 product(matrix4 m1, matrix4 m2);
//旋转，缩放和平移矩阵矩阵
void rotateX(float phi, matrix4& matrix);
void rotateY(float phi, matrix4& matrix);
void rotateZ(float phi, matrix4& matrix);
void translate3D(int dx, int dy, int dz, matrix4& matrix);
void scale3D(float sX, float sY, float sZ, matrix4& matrix);
//转换矩阵
void transform(matrix4 m, vector3& v);
void transform(matrix4 m, vector3f& v);
void projection(camera* cam,matrix4 &matrix);
float normal(vector3 v);
float normal(vector3f v);
//矩阵求逆
matrix4 inverse(matrix4 matrix);
//向量加减法和数乘
vector3f operator-(vector3f v1, vector3f v2);
vector3 operator-(vector3 v1, vector3 v2);
POS operator-(POS p1, POS p2);
vector3f operator-(vector3f v);
vector3 operator-(vector3 v);
vector3 operator/(vector3 v, float norm);
vector3f operator/(vector3f v, float norm);
matrix4 transpose(matrix4 m);
matrix_ resize(int n, int m);
//3维向量叉乘
template<class T>
T cross3D(T v1, T v2)
{
	T v;
	v.X = v1.Y * v2.Z - v1.Z * v2.Y;
	v.Y = v1.Z * v2.X - v1.X * v2.Z;
	v.Z = v1.X * v2.Y - v1.Y * v2.X;
	v.W = 0;
	return v;
}
//点乘
template<class T>
float dot(T v1, T v2)
{
	return v1.X * v2.X + v1.Y * v2.Y;
}
//单位化向量
template<class T>
void unitization(T& v)
{
	v =  v / normal(v);
}
template<class T>
//2维向量叉乘，只需要返回正负值
int cross2D(T p1, T p2)
{
	int result = p1.X * p2.Y - p2.X * p1.Y;
	if (result > 0) return LEFT;
	else if (result == 0) return COLLINEAR;
	else return RIGHT;
}


#endif
