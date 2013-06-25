#pragma once

#include "stdafx.h"
#include "Vector.h"
#include "Point.h"
#include "Direction.h"
#include "HalfEdgeDeclaration.h"

class Math
{
public:
	static void GetNormalizedDis(const Point& center, const vector<HEFace*>& faces, vector<GLdouble>& distances, vector<Point>& centers);
	static void GetRotationMatrix(const Direction& dir, const GLdouble& angle, GLdouble Rotation[4][4]);
	static void InitPoisson(vector<HEFace*>& faces);
	static void CalcPoisson(vector<HEFace*>& faces);
};