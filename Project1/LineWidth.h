#pragma once

#include "stdafx.h"

#include "Utility.h"
#include <stack>

class LineWidth
{
public:
	static void DrawHighLight();
	static void DrawNormal();
	static void DrawAxis();
	static void PushAndSetLineWidth(GLfloat width);
	static void PopLineWidth();
private:
	static stack<GLfloat> _stack;
};