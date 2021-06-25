#include"console3D.h"
#include"matrix.h"
//#include<Windows.h>
#include<iostream>
#include<string>
#include<sstream>
#include<conio.h>
using namespace std;
CONSOLE_FONT_INFOEX a;
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);


void setPos(int X, int Y)
{
	COORD pos;
	pos.X = X;
	pos.Y = Y;
	SetConsoleCursorPosition(hOutput, pos);
}

float timer(int reset = 0)
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
	for (int i = 0; i < height; i++)
	{
		data[i] = data_buff + width * i;
	}
	clearData();
	SMALL_RECT sr = { 0,0,width,height };
	//CONSOLE_SCREEN_BUFFER_INFOEX buff;
	//SetConsoleScreenBufferSize(front, COORD{ 600,600 });
	//SetConsoleScreenBufferSize(back, COORD{ 600,600 });
	//SetConsoleWindowInfo(front, TRUE, &sr);
	//SetConsoleWindowInfo(back, TRUE, &sr);
	//GetConsoleScreenBufferInfoEx(front, &buff);
	//buff.dwSize = COORD{ (short)width,(short)height };
	//buff.dwMaximumWindowSize = COORD{ (short)width,(short)height };
	CONSOLE_SCREEN_BUFFER_INFOEX buff;
	//GetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
	buff.bFullscreenSupported = false;
	buff.cbSize = sizeof(buff) * 1;
	buff.dwCursorPosition = COORD{ 0,0 };
	buff.dwMaximumWindowSize = COORD{ (short)width,(short)height };
	buff.dwSize = COORD{ (short)width,(short)height };
	buff.srWindow = sr;
	//buff.wAttributes = F_R;
	//buff.wPopupAttributes = F_R;
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
		}
	}
}

camera::camera(vector3f _pos, vector3f _direction, vector3f _top_direction, vector3f _right_direction)
{
	//direction_phi = 0;
	//top_phi = 0;
	//right_phi = 0;
	pos = _pos;
	direction = _direction;
	top_direction = _top_direction;
	right_direction = _right_direction;
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
	return matrix;
}

void camera::rotateRightDirection(float phi)
{
	matrix4 m = identity(4);
	matrix4 m1 = m;
	//translate3D(-pos.X, -pos.Y, -pos.Z, m);
	m[0][0] = right_direction.X;
	m[1][0] = right_direction.Y;
	m[2][0] = right_direction.Z;
	m = inverse(m);
	rotateX(phi, m);
	m = product(m1, m);
	//translate3D(pos.X, pos.Y, pos.Z, m);
	transform(m, right_direction);
	transform(m, direction);
	transform(m, top_direction);
}

void camera::rotateTopDirection(float phi)
{
	matrix4 m = identity(4);
	matrix4 m1 = m;
	//translate3D(-pos.X, -pos.Y, -pos.Z, m);
	m[0][1] = top_direction.X;
	m[1][1] = top_direction.Y;
	m[2][1] = top_direction.Z;
	m = inverse(m);
	rotateY(phi, m);
	m = product(m1, m);
	//translate3D(pos.X, pos.Y, pos.Z, m);
	transform(m, top_direction);
	transform(m, direction);
	transform(m, right_direction);
}

void camera::rotateDirection(float phi)
{
	matrix4 m = identity(4);
	matrix4 m1 = m;
	//translate3D(-pos.X, -pos.Y, -pos.Z, m);
	m[0][2] = direction.X;
	m[1][2] = direction.Y;
	m[2][2] = direction.Z;
	m = inverse(m);
	rotateZ(phi, m);
	m = product(m1, m);
	//translate3D(pos.X, pos.Y, pos.Z, m);
	transform(m, top_direction);
	transform(m, direction);
	transform(m, right_direction);
}

//void camera::rotateDirection(vector3f& _top_direction, vector3f& _direction, vector3f& _right_direction)
//{
//	matrix4 m = identity(4);
//	translate3D(-pos.X, -pos.Y, -pos.Z, m);
//	m[0][2] = _direction.X;
//	m[1][2] = _direction.Y;
//	m[2][2] = _direction.Z;
//	m = inverse(m);
//	rotateZ(direction_phi, m);
//	translate3D(pos.X, pos.Y, pos.Z, m);
//	transform(m,_top_direction);
//	transform(m,_direction);
//	transform(m, _right_direction);
//}
//
//void camera::rotateTopDirection(vector3f& _top_direction, vector3f& _direction, vector3f& _right_direction)
//{
//	matrix4 m = identity(4);
//	translate3D(-pos.X, -pos.Y, -pos.Z, m);
//	m[0][1] = _top_direction.X;
//	m[1][1] = _top_direction.Y;
//	m[2][1] = _top_direction.Z;
//	m = inverse(m);
//	rotateY(top_phi, m);
//	translate3D(pos.X, pos.Y, pos.Z, m);
//	transform(m, _direction);
//	transform(m, _right_direction);
//}
//
//void camera::rotateRightDirection(vector3f& _top_direction, vector3f& _direction, vector3f& _right_direction)
//{
//	matrix4 m = identity(4);
//	translate3D(-pos.X, -pos.Y, -pos.Z, m);
//	m[0][0] = _right_direction.X;
//	m[1][0] = _right_direction.Y;
//	m[2][0] = _right_direction.Z;
//	m = inverse(m);
//	rotateX(right_phi, m);
//	translate3D(pos.X, pos.Y, pos.Z, m);
//	transform(m, _direction);
//	transform(m, _top_direction);
//}

scene::scene(swapChain* _sp)
{
	sp = _sp;
}

scene::~scene(){}

bool backBlanking(POS p1, POS p2, POS p3)
{
	POS p;
	p.X = p1.X / 3 + p2.X / 3 + p3.X / 3;
	p.Y = p1.Y / 3 + p2.Y / 3 + p3.Y / 3;
	int d1 = cross2D(p2 - p1, p - p1);
	int d2 = cross2D(p3 - p2, p - p2);
	int d3 = cross2D(p1 - p3, p - p3);
	if (d1 == RIGHT && d2 == RIGHT && d3 == RIGHT)
		return true;
	return false;
}

void drawTriangle(vector3 p1, vector3 p2, vector3 p3, swapChain& sp, char fillWith, WORD color)
{
	drawTriangle(POS{ p1.X,p1.Y }, POS{ p2.X,p2.Y }, POS{ p3.X,p3.Y }, sp, fillWith, color);
}

void drawTriangle(POS p1, POS p2, POS p3, swapChain& sp, char fillWith , WORD color )
{
	if (sp.backBlank && backBlanking(p1, p2, p3)) return;
	POS max_pos = p1;
	POS min_pos = p2;
	vector<POS> sort_pos = { p1,p2,p3 };
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
	POS point = {0,0};
	int c = 0;
	int c1 = 0;
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
			if (d1 <= 0 && d2 <= 0 && d3 <= 0)
			{
				sp.data[point.Y][point.X].Char.AsciiChar = fillWith;
				sp.data[point.Y][point.X].Attributes = color;
			}
			else if (d1 >= 0 && d2 >= 0 && d3 >= 0)
			{
				sp.data[point.Y][point.Y].Char.AsciiChar = fillWith;
				sp.data[point.Y][point.X].Attributes = color;
			}
			else
				c++;
			c1++;
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

void update(swapChain& sp)
{
	sp.update();
	sp.swap();
	sp.show();
}

int main()
{
	short z = 200;
	camera cam(vector3f{ 125,125,0,1 });
	swapChain win(250, 250, 3,true);
	win.init();
	timer(1);
	vector<vector3> v = { {50,50,z,1},{50,200,z,1},{200,200,z,1 },{ 200,50,z + 100,1} };
	vector<vector3> v_sq = {
		{0,0,300,1},{0,100,300,1},{100,100,300,1},{100,0,300,1},
		{0,0,400,1},{0,100,400,1},{100,100,400,1},{100,0,400,1} };
	double phi = 0;
	double beta = 0;
	double alpha = 0;
	double omiga = 3.1415926 / 6.0;
	int m = 0, n = 0;
	char c = 0;
	float rotate = 0.1;
	float speed = 0.1;
	while (true)
	{
		matrix4 viewMatrix = cam.getViewMatrix();
		//vector<vector3> v1(v);
		vector<vector3> v1(v_sq);
		matrix4 m = identity(4);
		translate3D(-50, -50, -350, m);
		//translate3D(-125, -125, -z, m);
		rotateY(phi, m);
		rotateX(beta, m);
		rotateZ(alpha, m);
		translate3D(125, 125, z, m);
		//translate3D(50, 50, 350, m);
		m = product(viewMatrix, m);
		projection(100, 1000,3.14159/4.0, 1, m);
		for (int i = 0; i < v1.size(); i++)
			transform(m, v1[i]);
		for (int i = 0; i < v1.size(); i++)
		{
			v1[i].X += win.width/2;
			v1[i].Y += win.height/2;
		}
		
		//drawTriangle(POS{ (int)v1[0].X, (int)v1[0].Y }, POS{ (int)v1[2].X, (int)v1[2].Y }, POS{ (int)v1[3].X, (int)v1[3].Y }, win, '*', F_RED | B_RED);
		//drawTriangle(POS{ (int)v1[0].X, (int)v1[0].Y }, POS{ (int)v1[3].X, (int)v1[3].Y }, POS{ (int)v1[1].X, (int)v1[1].Y }, win, '*', F_BLUE | B_BLUE);
		//drawTriangle(POS{ (int)v1[3].X, (int)v1[3].Y }, POS{ (int)v1[2].X, (int)v1[2].Y }, POS{ (int)v1[1].X, (int)v1[1].Y }, win, '*', F_YELLOW | B_YELLOW);
		//drawTriangle(POS{ (int)v1[0].X, (int)v1[0].Y }, POS{ (int)v1[1].X, (int)v1[1].Y }, POS{ (int)v1[2].X, (int)v1[2].Y }, win, '*', F_WHITE | B_WHITE);
		//zheng front
		drawTriangle(POS{ (int)v1[2].X, (int)v1[2].Y }, POS{ (int)v1[1].X, (int)v1[1].Y }, POS{ (int)v1[0].X, (int)v1[0].Y }, win, '*', F_WHITE | B_WHITE);
		drawTriangle(POS{ (int)v1[3].X, (int)v1[3].Y }, POS{ (int)v1[2].X, (int)v1[2].Y }, POS{ (int)v1[0].X, (int)v1[0].Y }, win, '*', F_WHITE | B_WHITE);
		//right
		drawTriangle(POS{ (int)v1[6].X, (int)v1[6].Y }, POS{ (int)v1[2].X, (int)v1[2].Y }, POS{ (int)v1[3].X, (int)v1[3].Y }, win, '*', F_RED | B_RED);
		drawTriangle(POS{ (int)v1[7].X, (int)v1[7].Y }, POS{ (int)v1[6].X, (int)v1[6].Y }, POS{ (int)v1[3].X, (int)v1[3].Y }, win, '*', F_RED | B_RED);
		//back
		drawTriangle(POS{ (int)v1[5].X, (int)v1[5].Y }, POS{ (int)v1[6].X, (int)v1[6].Y }, POS{ (int)v1[7].X, (int)v1[7].Y }, win, '*', F_BLUE | B_BLUE);
		drawTriangle(POS{ (int)v1[4].X, (int)v1[4].Y }, POS{ (int)v1[5].X, (int)v1[5].Y }, POS{ (int)v1[7].X, (int)v1[7].Y }, win, '*', F_BLUE | B_BLUE);
		//left
		drawTriangle(POS{ (int)v1[1].X, (int)v1[1].Y }, POS{ (int)v1[5].X, (int)v1[5].Y }, POS{ (int)v1[4].X, (int)v1[4].Y }, win, '*', F_YELLOW | B_YELLOW);
		drawTriangle(POS{ (int)v1[0].X, (int)v1[0].Y }, POS{ (int)v1[1].X, (int)v1[1].Y }, POS{ (int)v1[4].X, (int)v1[4].Y }, win, '*', F_YELLOW | B_YELLOW);
		//top
		drawTriangle(POS{ (int)v1[3].X, (int)v1[3].Y }, POS{ (int)v1[0].X, (int)v1[0].Y }, POS{ (int)v1[4].X, (int)v1[4].Y }, win, '*', F_CYAN | B_CYAN);
		drawTriangle(POS{ (int)v1[7].X, (int)v1[7].Y }, POS{ (int)v1[3].X, (int)v1[3].Y }, POS{ (int)v1[4].X, (int)v1[4].Y }, win, '*', F_CYAN | B_CYAN);
		//bottom
		drawTriangle(POS{ (int)v1[6].X, (int)v1[6].Y }, POS{ (int)v1[5].X, (int)v1[5].Y }, POS{ (int)v1[1].X, (int)v1[1].Y }, win, '*', F_PURPLE | B_PURPLE);
		drawTriangle(POS{ (int)v1[2].X, (int)v1[2].Y }, POS{ (int)v1[6].X, (int)v1[6].Y }, POS{ (int)v1[1].X, (int)v1[1].Y }, win, '*', F_PURPLE | B_PURPLE);


		win.update();
		win.swap();
		win.show();
		win.clearData();
		float t = timer();
		//phi += t * omiga;
		if (phi >= 3.1415926 * 2)phi = 0;
		if (_kbhit())
		{
			//getchar();
			c = _getch();
			switch (c)
			{
			case 'a':phi += speed; break;
			case 'd':phi -= speed; break;
			case 'w':beta += speed; break;
			case 's':beta -= speed; break;
			case 'r':alpha += speed; break;
			case 'f':alpha -= speed; break;
			case 'j':cam.rotateTopDirection(rotate); break;
			case'l':cam.rotateTopDirection(-rotate); break;
			case'i':cam.rotateRightDirection(rotate); break;
			case 'k':cam.rotateRightDirection(-rotate); break;
			case 'u':cam.rotateDirection(rotate); break;
			case 'o':cam.rotateDirection(-rotate); break;
			}
		}
		//wstring str = to_wstring(cam.pos.X)+L" "+to_wstring(cam.pos.Y)+L" "+ to_wstring(cam.pos.Z);
		wstring str = to_wstring(1.0 / t) + L"fps ";
		SetConsoleTitle(str.c_str());
	}
	return 0;
}