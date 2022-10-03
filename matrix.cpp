#include"matrix.h"
#include"console3D.h"
;
void projection(camera* cam, matrix4& matrix)
{
	//float cot_fov = 1.0 / tan(cam->fov/2);
	matrix4 m = zero(4);
	//m[0][0] = cot_fov / (cam->aspect);
	//m[1][1] = cot_fov;
	//m[2][2] = ((float)cam->n + (float)cam->f) / ((float)cam->n - (float)cam->f);
	//m[2][3] = -(2.0f * cam->n*cam->f) / ((float)cam->n - (float)cam->f);
	//m[3][2] = 1;
	m[0][0] = cam->n;
	m[1][1] = cam->n;
	m[2][2] = cam->n + cam->f;
	m[2][3] = -cam->n * cam->f;
	m[3][2] = 1;
	matrix = product(m, matrix);
}

matrix4 transpose(matrix4 m)
{
	int len = m.size();
	matrix4 m1 = zero(len);
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
			m1[i][j] = m[j][i];
	}
	return m1;
}

matrix_ resize(int n, int m)
{
	matrix_ matrix;
	matrix.resize(n);
	for (int i = 0; i < n; i++)
		matrix[i].resize(m);
	return matrix;
}

vector3f operator-(vector3f v)
{
	return vector3f{ -v.X,-v.Y,-v.Z,0 };
}

vector3 operator-(vector3 v)
{
	return vector3{ -v.X,-v.Y,-v.Z,0 };
}

vector3 operator-(vector3 v1, vector3 v2)
{
	vector3 v = { v1.X - v2.X,v1.Y - v2.Y,v1.Z - v2.Z,v1.W - v2.W };
	if (v.W == -1)
	{
		v.X = -v.X;
		v.Y = -v.Y;
		v.Z = -v.Z;
		v.W = 1;
	}
	return v;
}

vector3f operator-(vector3f v1, vector3f v2)
{
	vector3f v ={ v1.X - v2.X,v1.Y - v2.Y,v1.Z - v2.Z,v1.W - v2.W };
	if (v.W == -1)
	{
		v.X = -v.X;
		v.Y = -v.Y;
		v.Z = -v.Z;
		v.W = 1;
	}
	return v;
}

float normal(vector3 v)
{
	return sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
}
float normal(vector3f v)
{
	return sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
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
	Matrix<float,Dynamic,Dynamic> m;
	int size = matrix.size();
	m.resize(size,size);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			m(i, j) = matrix[i][j];
		}
	}
	Matrix<float, Dynamic, Dynamic> m1 = m.inverse();
	matrix4 new_matrix = zero(size);
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
	matrix4 m;
	m.resize(m1.size());
	int len = m2.size();
	int width = m2[0].size();
	int height = m1.size();
	for (int i = 0; i < height; i++)
	{
		m[i].resize(width);
		for (int j = 0; j < width; j++)
		{
			for (int n = 0; n < width; n++)
			{
				m[i][j] += m1[i][n] * m2[n][j];
			}
			//m[i][j] = m1[i][0] * m2[0][j] + m1[i][1] * m2[1][j] + m1[i][2] * m2[2][j] + m1[i][3] * m2[3][j];
		}
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
	v.Z /= v.W;
	v.W = 1;
}

vector3 operator/(vector3 v, float norm)
{
	v.X /= norm;
	v.Y /= norm;
	v.Z /= norm;
	return v;
}
vector3f operator/(vector3f v, float norm)
{
	v.X /= norm;
	v.Y /= norm;
	v.Z /= norm;
	return v;
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
	v.Z /= v.W;
	v.W = 1;
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