#pragma once

#include "stdafx.h"

#include "Parameters.h"
#include "Material.h"
#include "Vector.h"
#include "HalfEdgeDeclaration.h"

class Utility
{
public:
	static void DrawTranslateAxis(Point point);
	static void DrawScaleAxis(Point point);
	static void DrawRotateAxis(Point point);
	static void DrawCircle(GLdouble R, int n);
};