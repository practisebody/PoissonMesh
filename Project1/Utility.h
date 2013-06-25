#pragma once

#include "stdafx.h"
#include "Direction.h"
#include "States.h"
#include "Parameters.h"
#include "Material.h"
#include "LineWidth.h"
#include "Vector.h"
#include "Point.h"
#include "HalfEdgeDeclaration.h"

class Utility
{
public:
	static void DrawBasicAxis(const Point& here);
	static void DrawCircle(GLdouble R, int n);
	static void DrawTranslateAxis(const Point& here);
	static void DrawScaleAxis(const Point& here);
	static void DrawRotateAxis(const Point& here);
	static void DrawCube(const Point& leftdown, const Vector& scale);

	static void Translated(const _Triple& triple);
	static void Scaled(const _Triple& triple);

	template<typename T>
	inline static void SetMin(T& a, const T& b);
	template<typename T>
	inline static void SetMax(T& a, const T& b);
	static const Point minPoint, maxPoint;
	static void SetMin(Point& a, const Point& b);
	static void SetMax(Point& a, const Point& b);

	static void Print(const char* string);
	static void InitHelp();
	static void PrintHelp(bool bSelect, bool bShift, bool bCtrl, bool bAlt);
private:
	static map<int, string> HelpInfo;
};

template<typename T>
void Utility::SetMin(T& a, const T& b)
{
	if (a > b)
		a = b;
}

template<typename T>
void Utility::SetMax(T& a, const T& b)
{
	if (a < b)
		a = b;
}