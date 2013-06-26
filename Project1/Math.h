#pragma once

#include "stdafx.h"
#include "Vector.h"
#include "Point.h"
#include "Direction.h"
#include "HalfEdgeDeclaration.h"
#include "matrix.h"
#include "matrix_inl.h"

class Math
{
public:
	static void GetNormalizedDis(const Point& center, const vector<HEFace*>& faces, vector<GLdouble>& distances, vector<Point>& centers);
	static void GetRotationMatrix(const Direction& dir, const GLdouble& angle, GLdouble Rotation[4][4]);
	static void Math::CalcGradient(const vector<HEFace*>& faces);
	static void InitPoisson(const vector<HEFace*>& faces, vector<HEVert*>& rawVerts);
	static void CalcPoisson(const vector<HEFace*>& faces, vector<HEVert*>& rawVerts, const set<HEVert*>& fixedVerts);
public:
	static matrix::DOK* Poisson;
};