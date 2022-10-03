#ifndef CONSOLEHEAD_H_
#define CONSOLEHEAD_H_
#include<Windows.h>
#include<vector>
#include<math.h>
#define PI 3.141592

#include "Eigen/Eigen"
using namespace Eigen;

typedef std::vector<std::vector<float>>  matrix4;
typedef std::vector<std::vector<float>>  matrix2;
typedef std::vector<std::vector<float>>  matrix3;
typedef std::vector<std::vector<float>>  matrix_;
typedef std::vector<std::vector<float>>  matrix2x1;
typedef std::vector<std::vector<float>>  matrix3x1;
class camera;

struct POS
{
	int X;
	int Y;
};

struct vector3
{
	int X;
	int Y;
	int Z;
	int W;
};

struct vector3f
{
	float X;
	float Y;
	float Z;
	short W;
};
enum
{
	LEFT = -1,
	COLLINEAR = 0,
	RIGHT = 1
};
//颜色
#define F_BLACK 0X0000
#define F_DARK_RED 0X0001
#define F_RED 0X0002
#define F_DARK_YELLOW 0X0003
#define F_YELLOW 0X0004
#define F_DARK_GREEN 0X0005
#define F_GREEN 0X0006
#define F_DARK_CYAN 0X0007
#define F_CYAN 0X0008
#define F_DARK_BLUE 0X0009
#define F_BLUE 0X000A
#define F_DARK_PURPLE 0X000B
#define F_PURPLE 0X000C
#define F_DARK_GREY 0X000D
#define F_GREY 0X000E
#define F_WHITE 0X000F
#define B_BLACK 0x0000
#define B_DARK_RED 0X0010
#define B_RED 0X0020
#define B_DARK_YELLOW 0X0030
#define B_YELLOW 0X0040
#define B_DARK_GREEN 0X0050
#define B_GREEN 0X0060
#define B_DARK_CYAN 0X0070
#define B_CYAN 0X0080
#define B_DARK_BLUE 0X0090
#define B_BLUE 0X00A0
#define B_DARK_PURPLE 0X00B0
#define B_PURPLE 0X00C0
#define B_DARK_GREY 0X00D0
#define B_GREY 0X00E0
#define B_WHITE 0X00F0

#endif // !CONSOLEHEAD_H_#pragma once
