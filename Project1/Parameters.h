#pragma once

#include "stdafx.h"
#include "States.h"
#include "Point.h"

#define BLACK 0.0f, 0.0f, 0.0f
#define WHITE 1.0f, 1.0f, 1.0f

class Parameters
{
public:
	static const GLdouble PI;
	static const int MAX_LINE_LENGTH;
	static const int MAX_PREFIX_LENGTH;
	static const GLdouble zNear;
	static const GLdouble zFar;
	static GLfloat fMagnification;
	static bool bLine;
	static GLfloat& fIntersectRadius;
	static GLfloat fRevisedMaginification();
	static States status;
	static const GLfloat fNormalLineWidth;
	static const GLfloat fHighLightLineWidth;
	static const GLfloat fAxisLineWidth;
	static GLfloat fAxisLength;
	static vector<HEFace*>::size_type nMaxDrawNumber;
	static Point m_vMin, m_vMax;
	static GLfloat fMaxDrawSize;
};