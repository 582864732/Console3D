#ifndef CONSOLE3D_H_
#define CONSOLE3D_H_
#include"consoleHead.h"
using std::vector;

class swapChain
{
private:
	HANDLE front;
	HANDLE back;
public:
	CHAR_INFO* data_buff;
	CHAR_INFO** data;
	bool backBlank;//背面消隐，对于顺时针排列的三角形不予渲染
	short height;
	short width;
	swapChain(short h, short w, short c_size = 1,bool useBackBlank = false);
	~swapChain();
	void init();
	void clearData();
	void swap();
	void update();
	void show();
};


class camera
{
private:
	//enum
	//{
	//	DIRECTION,
	//	TOP,
	//	RIGHT
	//};
	//std::vector<int> rotate_pri;
	//float direction_phi;
	//float top_phi;
	//float right_phi;
	//void rotateDirection(vector3f &_top_direction, vector3f &_direction, vector3f &_right_direction);
	//void rotateTopDirection(vector3f& _top_direction, vector3f& _direction, vector3f& _right_direction);
	//void rotateRightDirection(vector3f& _top_direction, vector3f& _direction, vector3f& _right_direction);
public:
	vector3f pos;
	vector3f direction;
	vector3f top_direction;
	vector3f right_direction;
	camera(vector3f _pos = { 0,0,0,1 }, vector3f _direction = { 0,0,1,0 }, vector3f _top_direction = { 0,1,0,0 }, vector3f _right_direction = { 1,0,0,0 });
	void rotateDirection(float phi);
	void rotateTopDirection(float phi);
	void rotateRightDirection(float phi);
	matrix4 getViewMatrix();
	~camera();
};

class scene
{
public:
	swapChain* sp;
	scene(swapChain* _sp);
	~scene();

	void updateSceneToSwapChain(camera item);
};

void update(swapChain& sp);
bool backBlanking(POS p1, POS p2, POS p3);
void drawTriangle(POS p1, POS p2, POS p3, swapChain& sp, char fillWith = '*', WORD color = F_WHITE);
void drawTriangle(vector3 p1, vector3 p2, vector3 p3, swapChain& sp, char fillWith = '*', WORD color = F_WHITE);
void drawLine(POS p1, POS p2, swapChain& sp, char fillWith = '*', WORD color = F_WHITE)
#endif