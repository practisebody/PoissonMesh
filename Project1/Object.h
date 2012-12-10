#pragma once

#include "stdafx.h"
#include "Utility.h"

class Object
{
public:
	virtual void DrawSelected() = 0;
	virtual double Intersect(const Point&, const Vector&) = 0;
	//virtual void operator+=(Vector vector) = 0;
};

class _Iteratored_Object : public Object
{
public:
	//virtual void ForEach(LPVOID Param) {}
};

class HEObject : public _Iteratored_Object
{
public:
	virtual void ToVerts(vector<HEVert*>& set) = 0;
	virtual HEVert* InsertVertex(vector<HEFace*>& faces) = 0;
	virtual void Delete(set<HEObject*>& deletedObjects) = 0;
};