#include"console3D.h"
#include"matrix.h"
//#include<Windows.h>
#include<iostream>
#include<string>
#include<sstream>
#include<conio.h>
using namespace std;

float timer(int reset)
{
	static LARGE_INTEGER priorTime;
	LARGE_INTEGER framePreSec;
	QueryPerformanceFrequency(&framePreSec);
	float secPerFrame = 1.0 / framePreSec.QuadPart;
	if (reset)
	{
		QueryPerformanceCounter(&priorTime);
		return -1;
	}
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	float dt = (currentTime.QuadPart - priorTime.QuadPart) * secPerFrame;
	priorTime = currentTime;
	return dt;
}
swapChain::swapChain(short h, short w, short c_size,bool useBackBlank)
{
	backBlank = useBackBlank;
	front = CreateConsoleScreenBuffer(GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	back = CreateConsoleScreenBuffer(GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//SetConsoleTextAttribute(front, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	//SetConsoleTextAttribute(back, BACKGROUND_GREEN);
	height = h;
	width = w;
	data_buff = new CHAR_INFO[height * width];
	data = new CHAR_INFO * [height];
	depth_buff = new float[height*width];
	depth = new float* [height];
	for (int i = 0; i < height; i++)
	{
		data[i] = data_buff + width * i;
		depth[i] = depth_buff + width * i;
	}
	clearData();
	SMALL_RECT sr = { 0,0,width,height };
	CONSOLE_SCREEN_BUFFER_INFOEX buff;
	buff.bFullscreenSupported = false;
	buff.cbSize = sizeof(buff) * 1;
	buff.dwCursorPosition = COORD{ 0,0 };
	buff.dwMaximumWindowSize = COORD{ (short)width,(short)height };
	buff.dwSize = COORD{ (short)width,(short)height };
	buff.srWindow = sr;
	buff.ColorTable[0] = RGB(0, 0, 0);
	buff.ColorTable[1] = RGB(128, 0, 0);
	buff.ColorTable[2] = RGB(255, 0, 0);
	buff.ColorTable[3] = RGB(128, 128, 0);
	buff.ColorTable[4] = RGB(255, 255, 0);
	buff.ColorTable[5] = RGB(0, 128, 0);
	buff.ColorTable[6] = RGB(0, 255, 0);
	buff.ColorTable[7] = RGB(0, 128, 128);//
	buff.ColorTable[8] = RGB(0, 255, 255);
	buff.ColorTable[9] = RGB(0, 0, 128);
	buff.ColorTable[10] = RGB(0, 0, 255);
	buff.ColorTable[11] = RGB(128, 0, 128);
	buff.ColorTable[12] = RGB(255, 0, 255);
	buff.ColorTable[13] = RGB(85, 85, 85);
	buff.ColorTable[14] = RGB(170, 170, 170);//
	buff.ColorTable[15] = RGB(255, 255, 255);
	SetConsoleScreenBufferInfoEx(front, &buff);
	SetConsoleScreenBufferInfoEx(back, &buff);
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize.X = c_size;
	fontInfo.dwFontSize.Y = c_size;
	fontInfo.FontFamily = TMPF_VECTOR;
	fontInfo.FontWeight = 500;
	SetCurrentConsoleFontEx(front, FALSE, &fontInfo);
	SetCurrentConsoleFontEx(back, FALSE, &fontInfo);
}

swapChain::~swapChain()
{
	delete[] data_buff;
	delete[] data;
}

void swapChain::init()
{
	update();
	swap();
	update();
	swap();
}

void swapChain::swap()
{
	std::swap(front, back);
}

void swapChain::update()
{
	COORD pos = { 0,0 };
	DWORD bytes = 0;
	SMALL_RECT rect = { 0,0,width,height };
	WriteConsoleOutputA(back, data_buff, COORD{ width,height }, COORD{ 0,0 }, &rect);
}

void swapChain::show()
{
	SetConsoleActiveScreenBuffer(front);
}

void swapChain::clearData()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			data[i][j].Attributes = F_BLACK;
			data[i][j].Char.AsciiChar = ' ';
			depth[i][j] = -1;
		}
	}
}

camera::camera(vector3f _pos, vector3f _direction, vector3f _top_direction)
{
	aspect = 1;
	fov = PI / 4;
	f = 1000;
	n = 100;
	pos = _pos;
	direction = _direction/normal(_direction);
	top_direction = _top_direction/normal(_top_direction);
	right_direction = cross3D(top_direction,direction);
	tan_h_fov = 0;
}
camera::~camera() {}
matrix4 camera::getViewMatrix()
{
	vector3f _right_direction = right_direction;
	vector3f _direction = direction;
	vector3f _top_direction = top_direction;
	
	Matrix4f m;
	m << right_direction.X, top_direction.X, direction.X, pos.X,
		right_direction.Y, top_direction.Y, direction.Y, pos.Y,
		right_direction.Z, top_direction.Z, direction.Z, pos.Z,
		0, 0, 0, 1;
	matrix4 matrix = zero(4);
	Matrix4f m_inv = m.inverse();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix[i][j] = m_inv(i, j);
		}
	}
	matrix[3][3] = 1;
	return matrix;
}

matrix4 camera::getDirectionMatrix()
{
	matrix4 m = identity(4);
	m[0][0] = right_direction.X;
	m[0][1] = right_direction.Y;
	m[0][2] = right_direction.Z;
	m[1][0] = top_direction.X;
	m[1][1] = top_direction.Y;
	m[1][2] = top_direction.Z;
	m[2][0] = direction.X;
	m[2][1] = direction.Y;
	m[2][2] = direction.Z;
	return m;
}

void camera::rotateRightDirection(float phi)
{
	//matrix4 m = getViewMatrix();
	matrix4 m1 = getDirectionMatrix();
	matrix4 m = inverse(m1);
	rotateX(phi, m);
	m = product(m1, m);
	//transform(m, right_direction);
	transform(m, direction);
	//unitization(direction);
	transform(m, top_direction);
	//unitization(top_direction);
}

void camera::rotateTopDirection(float phi)
{
	//matrix4 m = getViewMatrix();
	matrix4 m1 = getDirectionMatrix();
	matrix4 m = inverse(m1);
	rotateY(phi, m);
	m = product(m1, m);
	transform(m, direction);
	transform(m, right_direction);
}

void camera::rotateDirection(float phi)
{
	matrix4 m1 = getDirectionMatrix();
	matrix4 m = inverse(m1);
	rotateZ(phi, m);
	m = product(m1, m);
	transform(m, top_direction);
	transform(m, right_direction);
}



void drawTriangle(vector3f p1, vector3f p2, vector3f p3, swapChain& sp, char fillWith, WORD color)
{
	if (p1.W || p2.W || p3.W);
	else return;
	if (sp.backBlank && backBlanking(p1, p2, p3)==1) return;
	vector3f max_pos = p1;
	vector3f min_pos = p2;
	vector<vector3f> sort_pos = { p1,p2,p3 };
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2 - i; j++)
			if (sort_pos[j].X > sort_pos[j + 1].X)
				swap(sort_pos[j], sort_pos[j + 1]);
	}
	min_pos.X = sort_pos[0].X;
	max_pos.X = sort_pos[2].X;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2 - i; j++)
			if (sort_pos[j].Y > sort_pos[j + 1].Y)
				swap(sort_pos[j], sort_pos[j + 1]);
	}
	min_pos.Y = sort_pos[0].Y;
	max_pos.Y = sort_pos[2].Y;
	vector3f point = { 0,0 };
	matrix3x1 b = resize(3,1);
	matrix3x1 result(b);
	matrix3 m = zero(3);
	m[0][0] = p1.X; m[1][0] = p2.X; m[2][0] = p3.X;
	m[0][1] = p1.Y; m[1][1] = p2.Y; m[2][1] = p3.Y;
	m[0][2] = 1;    m[1][2] = 1;    m[2][2] = 1;
	b[0][0] = p1.Z; b[1][0] = p2.Z; b[2][0] = p3.Z;
	result = product(inverse(m), b);
	for (point.X = min_pos.X; point.X <= max_pos.X; point.X++)
	{
		for (point.Y = min_pos.Y; point.Y <= max_pos.Y; point.Y++)
		{
			if (point.X < 0 || point.X >= sp.width)
				continue;
			else if (point.Y < 0 || point.Y >= sp.height)
				continue;
			int d1 = cross2D(p2 - p1, point - p1);
			int d2 = cross2D(p3 - p2, point - p2);
			int d3 = cross2D(p1 - p3, point - p3);
			float z = result[0][0] * point.X + result[1][0] * point.Y + result[2][0];
			if (d1 <= 0 && d2 <= 0 && d3 <= 0)
			{
				//if (z < 0) continue;
				if (sp.depth[(int)point.Y][(int)point.X] == -1 || sp.depth[(int)point.Y][(int)point.X] > z)
				{
					sp.data[(int)point.Y][(int)point.X].Char.AsciiChar = fillWith;
					sp.data[(int)point.Y][(int)point.X].Attributes = color;
					sp.depth[(int)point.Y][(int)point.X] = z;
				}
			}
			else if (d1 >= 0 && d2 >= 0 && d3 >= 0)
			{
				//if (z < 0)continue;
				if (sp.depth[(int)point.Y][(int)point.X] == -1 || sp.depth[(int)point.Y][(int)point.X] > z)
				{
					sp.data[(int)point.Y][(int)point.X].Char.AsciiChar = fillWith;
					sp.data[(int)point.Y][(int)point.X].Attributes = color;
					sp.depth[(int)point.Y][(int)point.X] = z;
				}
			}
		}
	}
}

void drawLine(POS p1, POS p2, swapChain& sp, char fillWith , WORD color)
{
	float dy = ((float)p2.Y - (float)p1.Y);
	float dx = (((float)p2.X - (float)p1.X));
	if (dx == 0)
	{
		if (p1.X >= sp.width || p1.X < 0) return;
		if (dy > 0)
		{
			for (int i = 0; i < dy; i++)
			{
				if (p1.X + i >= sp.height) return;
				{
					sp.data[p1.X + i][p1.X].Char.AsciiChar = fillWith;
					sp.data[p1.X + i][p1.X].Attributes = color;
				}
			}
		}
		else
		{
			for (int i = 0; i > dy; i--)
			{
				if (p1.X + i < 0) return;
				{
					sp.data[p1.X + i][p1.X].Char.AsciiChar = fillWith;
					sp.data[p1.X + i][p1.X].Attributes = color;
				}
			}
		}
		return;
	}
	float k = dy / dx;
	if (dy < 0)
	{
		if (dx < 0)
		{
			swap(p1, p2);
			dx = -dx;
			dy = -dy;
		}
	}
	if (k * k < 1)
	{
		int di;
		if (dx > 0)
			di = 1;
		else
		{
			di = -1;
			dx = -dx;
			k = -k;
		}
		int X;
		float Y;
		X = p1.X;
		Y = p1.X;
		for (int i = 0; i < dx; i++)
		{
			sp.data[(int)Y][X].Char.AsciiChar = fillWith;
			sp.data[(int)Y][X].Attributes = color;
			X += di;
			Y += k;
		}
	}
	else
	{
		k = 1 / k;
		int di;
		if (dy > 0)
			di = 1;
		else
		{
			di = -1;
			dy = -dy;
			k = -k;
		}
		float X = p1.X;
		int Y = p1.X;
		for (int i = 0; i < dy; i++)
		{
			sp.data[Y][(int)X].Char.AsciiChar = fillWith;
			sp.data[Y][(int)X].Attributes = color;
			Y += di;
			X += k;
		}
	}
}

void camera::setCamperaAttribute(int _near, int _far, float _fov, float _aspect)
{
	n = _near;
	f = _far;
	fov = _fov;
	aspect = _aspect;
	tan_h_fov = tan(fov / 2);
}

void camera::setFrontTop(vector3f _direction, vector3f _top)
{
	unitization(_direction);
	unitization(_top);
	direction = _direction;
	top_direction = _top;
	right_direction = cross3D(top_direction, direction);
}

void camera::setTopRight(vector3f _top, vector3f _right)
{
	unitization(_right);
	unitization(_top);
	right_direction = _right;
	top_direction = _top;
	direction = cross3D(right_direction, top_direction);
}

void camera::setRightFront(vector3f _right, vector3f _direction)
{
	unitization(_right);
	unitization(_direction);
	right_direction = _right;
	direction = _direction;
	top_direction = cross3D(direction,right_direction);
}

enum mode
{
	ROTATE=0,
	NONE
};

void drawCube(vector<vector3f> v1,matrix_ viewMatrix,camera* showing_cam,swapChain& win,vector3 pos,float phi)
{
	matrix_ m = identity(4);
	translate3D(-25, -25, -25, m);
	translate3D(pos.X, pos.Y, pos.Z, m);
	rotateY(2*phi, m);
	rotateX(phi, m);
	m = product(viewMatrix, m);
	projection(showing_cam, m);
	for (int i = 0; i < v1.size(); i++)
	{
		transform(m, v1[i]);
		if (v1[i].X > win.width / 2 || v1[i].X<-win.width / 2 || v1[i].Y>win.height / 2 ||
			v1[i].Y<-win.height / 2 || v1[i].Z>showing_cam->f || v1[i].Z < showing_cam->n)
		{
			v1[i].W = 0;
		}
	}
	for (int i = 0; i < v1.size(); i++)
	{
		v1[i].X += win.width / 2;
		v1[i].Y += win.height / 2;
	}
	//randering cube
	//front
	drawTriangle(v1[2], v1[1], v1[0], win, '*', F_BLACK | B_WHITE);
	drawTriangle(v1[3], v1[2], v1[0], win, '*', F_BLACK | B_WHITE);
	//right
	drawTriangle(v1[6], v1[2], v1[3], win, '*', F_RED | B_RED);
	drawTriangle(v1[7], v1[6], v1[3], win, '*', F_RED | B_RED);
	//back
	drawTriangle(v1[5], v1[6], v1[7], win, '*', F_BLUE | B_BLUE);
	drawTriangle(v1[4], v1[5], v1[7], win, '*', F_BLUE | B_BLUE);
	//left
	drawTriangle(v1[1], v1[5], v1[4], win, '*', F_YELLOW | B_YELLOW);
	drawTriangle(v1[0], v1[1], v1[4], win, '*', F_YELLOW | B_YELLOW);
	//top
	drawTriangle(v1[3], v1[0], v1[4], win, '*', F_CYAN | B_CYAN);
	drawTriangle(v1[7], v1[3], v1[4], win, '*', F_CYAN | B_CYAN);
	//bottom
	drawTriangle(v1[6], v1[5], v1[1], win, '*', F_PURPLE | B_PURPLE);
	drawTriangle(v1[2], v1[6], v1[1], win, '*', F_PURPLE | B_PURPLE);
}


int main()
{
	short r = 100;
	camera cam(vector3f{ 0,0,-(float)r,1 });
	camera cam1(vector3f{ 100,100,-100,1 }, { -1,-1,1,0 }, {1,1,2,0});
	cam.setCamperaAttribute(17, 500, PI / 4, 1);
	cam1.setCamperaAttribute(50, 500, PI / 4, 1);
	swapChain win(250, 250, 3,true);
	win.init();
	timer(1);
	vector<vector3f> v_sq = {
		{0,0,0,1},{0,50,0,1},{50,50,0,1},{50,0,0,1},
		{0,0,50,1},{0,50,50,1},{50,50,50,1},{50,0,50,1} };
	double phi = 0;
	double omiga = PI / 6.0;
	char c = 0;
	float rotate = 0.01;
	float speed = 0.1;
	mode _mode = ROTATE;
	float dx = 1;
	int d = 1;
	int cam_show = 0;
	camera* showing_cam = nullptr;
	while (true)
	{
		if (cam_show == 0)
		{
			showing_cam = &cam;
			if (_mode == ROTATE)
			{
				cam.pos.X += dx;
				if (cam.pos.X == 81.5 && d == 1) d = -1;
				else if (cam.pos.X == -81.5 && d == -1) d = 1;
				if (d == 1)
				{
					cam.pos.Z = (-cam.pos.X + sqrt(2 * r * r - 3 * cam.pos.X * cam.pos.X)) / 2;
					cam.pos.Y = cam.pos.X + cam.pos.Z;
				}
				else
				{
					cam.pos.Z = (-cam.pos.X - sqrt(2 * r * r - 3 * cam.pos.X * cam.pos.X)) / 2;
					cam.pos.Y = cam.pos.X + cam.pos.Z;
				}
				if (cam.pos.X >= 81.5 || cam.pos.X <= -81.5) dx = -dx;
				if (cam.pos.X > 60 || cam.pos.X < -60) dx = dx > 0 ? 0.5 : -0.5;
				else dx = dx > 0 ? 1 : -1;
				vector3f _top = { -1,1,-1,0 };
				vector3f _direction = { -cam.pos.X,-cam.pos.Y,-cam.pos.Z,0 };
				cam.setFrontTop(_direction, _top);
			}
		}
		else if (cam_show == 1)
		{
			showing_cam = &cam1;
		}
		matrix4 viewMatrix = showing_cam->getViewMatrix();


		drawCube(v_sq, viewMatrix, showing_cam, win, { 50,0,0,0 },phi);
		drawCube(v_sq, viewMatrix, showing_cam, win, { 0,0,50,0 },phi);
		drawCube(v_sq, viewMatrix, showing_cam, win, { 0,0,-50,0 },phi);
		drawCube(v_sq, viewMatrix, showing_cam, win, { -50,0,0,0 },phi);


		win.update();
		win.swap();
		win.show();
		win.clearData();
		float t = timer();
		phi += t * omiga;
		if (phi >= 3.1415926 * 2)phi = 0;
		if (_kbhit())
		{
			c = _getch();
			switch (c)
			{
			case '1':_mode = _mode == ROTATE ? NONE : ROTATE; break;
			case '2':omiga = omiga == 0 ? PI / 6.0 : 0; break;
			case '3':cam_show = cam_show==0?1:0; break;
			}
		}
		wstring str = to_wstring(1.0 / t) + L"fps";
		SetConsoleTitle(str.c_str());
	}
	return 0;
}