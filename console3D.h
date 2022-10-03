#ifndef CONSOLE3D_H_
#define CONSOLE3D_H_
#include"consoleHead.h"
using std::vector;

float timer(int reset = 0);//返回值以秒为单位

class swapChain
{
private:
	HANDLE front;
	HANDLE back;
public:
	CHAR_INFO* data_buff;
	CHAR_INFO** data;//用于储存每个像素的颜色和字符
	float* depth_buff;
	float** depth;//储存各个像素的深度值
	bool backBlank;//背面消隐，对于顺时针排列的三角形不予渲染
	short height;
	short width;
	swapChain(short h, short w, short c_size = 1,bool useBackBlank = false);
	~swapChain();
	void init();//初始化
	void clearData();//清空data中的数据
	//下面三个用来绘制，实际上可以写成一个函数
	void swap();
	void update();
	void show();
};
class camera
{
private:
	float tan_h_fov;
public:
	enum {
		DIRECTION,
		TOP_DIRECTION,
		RIGHT_DIRECTION
	};
	int n;
	int f;
	float fov;
	float aspect;
	vector3f pos;
	vector3f direction;
	vector3f top_direction;
	vector3f right_direction;
	//用一个坐标和两个正交向量可表示一个摄像机的位置与方向信息，至于另外一个方向的向量可通过叉乘得到
	camera(vector3f _pos = { 0,0,0,1 }, vector3f _direction = { 0,0,1,0 }, vector3f _top_direction = { 0,1,0,0 });
	void rotateDirection(float phi);
	void rotateTopDirection(float phi);
	void rotateRightDirection(float phi);
	void setCamperaAttribute(int _near, int _far, float _fov, float _aspect);
	//设置摄像头的任意两个方向，可以为摄像机定位
	void setFrontTop(vector3f _direction,vector3f _top);
	void setTopRight(vector3f _top, vector3f _right);
	void setRightFront(vector3f _right, vector3f _direction);
	//得到视角转换矩阵，把所有物体转换到摄像机坐标系
	matrix4 getViewMatrix();
	matrix4 getDirectionMatrix();
	~camera();
};
//绘制三角形
void drawTriangle(vector3f p1, vector3f p2, vector3f p3, swapChain& sp, char fillWith = '*', WORD color = F_WHITE);
//绘制线
void drawLine(POS p1, POS p2, swapChain& sp, char fillWith = '*', WORD color = F_WHITE);

//背部消隐
template<class T>
bool backBlanking(T p1, T p2, T p3)
{
	T p;
	p.X = p1.X / 3 + p2.X / 3 + p3.X / 3;
	p.Y = p1.Y / 3 + p2.Y / 3 + p3.Y / 3;
	int d1 = cross2D(p2 - p1, p - p1);
	int d2 = cross2D(p3 - p2, p - p2);
	int d3 = cross2D(p1 - p3, p - p3);
	if (d1>=0 && d2 >=0 && d3 >=0)
		return true;
	return false;
}
#endif