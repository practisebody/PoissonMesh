#pragma once

#include "stdafx.h"
#include "HEFace.h"

struct HENullFace : public HEFace
{
	virtual void Draw();
	virtual void DrawSelected();
	virtual double Intersect(const Point& D, const Vector& E);
};