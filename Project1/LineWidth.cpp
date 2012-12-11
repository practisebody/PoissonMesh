#include "stdafx.h"
#include "LineWidth.h"

stack<GLfloat> LineWidth::_stack;

void LineWidth::DrawHighLight()
{
	PushAndSetLineWidth(Parameters::fHighLightLineWidth);
}

void LineWidth::DrawNormal()
{
	PushAndSetLineWidth(Parameters::fNormalLineWidth);
}

void LineWidth::DrawAxis()
{
	PushAndSetLineWidth(Parameters::fAxisLineWidth);
}

void LineWidth::PushAndSetLineWidth(GLfloat width)
{
	_stack.push(width);
	glLineWidth(width);
}

void LineWidth::PopLineWidth()
{
	MYASSERT(_stack.size() > 0);
	glLineWidth(_stack.top());
	_stack.pop();
}