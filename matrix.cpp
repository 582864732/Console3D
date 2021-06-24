#include"matrix.h"

void projection(int _near, int _far, float fov, float aspect, matrix4& matrix)
{
	float cot_fov = 1.0 / tan(fov);
	matrix4 m = zero(4);
	//m[0][0] = cot_fov / (2 * aspect);
	//m[1][1] = cot_fov / 2;
	//m[2][2] = ((float)_near + (float)_far) / ((float)_near - (float)_far);
	//m[2][3] = -(2.0f * _near * _far) / ((float)_near - (float)_far);
	//m[3][2] = 1;
	m[0][0] = _near;
	m[1][1] = _near;
	m[2][2] = _near + _far;
	m[2][3] = -_near * _far;
	m[3][2] = 1;
	matrix = product(m, matrix);
}

int cross2D(POS p1, POS p2)
{
	int result = p1.X* p2.Y - p2.X * p1.Y;
	if (result > 0) return LEFT;
	else if (result == 0) return COLLINEAR;
	else return RIGHT;
}
void rotateX(float phi, matrix4& matrix)
{
	float c_phi = cos(phi);
	float s_phi = sin(phi);
	matrix4 m = identity(4);
	m[1][1] = m[2][2] = c_phi;
	m[1][2] = -s_phi;
	m[2][1] = s_phi;
	matrix = product(m,matrix);
}

matrix4 inverse(matrix4 matrix)
{
	Matrix4f m;
	int size = matrix.size();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			m(i, j) = matrix[i][j];
		}
	}
	Matrix4f m1 = m.inverse();
	matrix4 new_matrix = zero(4);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			new_matrix[i][j] = m1(i, j);
		}
	}
	return new_matrix;
}

void rotateY(float phi, matrix4& matrix)
{
	float c_phi = cos(phi);
	float s_phi = sin(phi);
	matrix4 m = identity(4);
	m[0][0] = m[2][2] = c_phi;
	m[2][0] = -s_phi;
	m[0][2] = s_phi;
	matrix = product(m,matrix);
}

void rotateZ(float phi, matrix4& matrix)
{
	float c_phi = cos(phi);
	float s_phi = sin(phi);
	matrix4 m = identity(4);
	m[0][0] = m[1][1] = c_phi;
	m[0][1] = -s_phi;
	m[1][0] = s_phi;
	matrix = product(m,matrix);
}

void translate3D(int dx, int dy, int dz, matrix4& matrix)
{
	matrix4 m = identity(4);
	m[0][3] = dx;
	m[1][3] = dy;
	m[2][3] = dz;
	matrix = product(m,matrix);
}
matrix4 product(matrix4 m1, matrix4 m2)
{
	matrix4 m = zero(4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			m[i][j] = m1[i][0] * m2[0][j] + m1[i][1] * m2[1][j] + m1[i][2] * m2[2][j] + m1[i][3] * m2[3][j];
	}
	return m;
}

matrix4 zero(int n)
{
	matrix4 m;
	m.resize(n);
	for (int i = 0; i < n; i++)
		m[i].resize(n);
	return m;
}
void scale3D(float sX, float sY, float sZ, matrix4& matrix)
{
	matrix4 m = identity(4);
	m[0][0] = sX;
	m[1][1] = sY;
	m[2][2] = sZ;
	matrix = product(m,matrix);
}
POS operator-(POS p1, POS p2)
{
	return POS{ p1.X - p2.X,p1.Y - p2.Y };
}
void transform(matrix4 m, vector3& v)
{
	vector3 v1 = v;
	v1.X = m[0][0] * v.X + m[0][1] * v.Y + m[0][2] * v.Z + m[0][3] * v.W;
	v1.Y = m[1][0] * v.X + m[1][1] * v.Y + m[1][2] * v.Z + m[1][3] * v.W;
	v1.Z = m[2][0] * v.X + m[2][1] * v.Y + m[2][2] * v.Z + m[2][3] * v.W;
	v1.W = m[3][0] * v.X + m[3][1] * v.Y + m[3][2] * v.Z + m[3][3] * v.W;
	v = v1;
	if (v.W == 0) return;
	v.X /= v.W;
	v.Y /= v.W;
}

void transform(matrix4 m, vector3f& v)
{
	vector3f v1 = v;
	v1.X = m[0][0] * v.X + m[0][1] * v.Y + m[0][2] * v.Z + m[0][3] * v.W;
	v1.Y = m[1][0] * v.X + m[1][1] * v.Y + m[1][2] * v.Z + m[1][3] * v.W;
	v1.Z = m[2][0] * v.X + m[2][1] * v.Y + m[2][2] * v.Z + m[2][3] * v.W;
	v1.W = m[3][0] * v.X + m[3][1] * v.Y + m[3][2] * v.Z + m[3][3] * v.W;
	v = v1;
	if (v.W == 0) return;
	v.X /= v.W;
	v.Y /= v.W;
}

matrix4 identity(int n)
{
	matrix4 m;
	m.resize(n);
	for (int i = 0; i < n; i++)
	{
		m[i].resize(n);
		m[i][i] = 1;
	}
	return m;
}