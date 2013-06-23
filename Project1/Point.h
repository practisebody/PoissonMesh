#pragma once

#include "stdafx.h"
#include "Triple.h"
#include "Vector.h"
#include "HalfEdgeDeclaration.h"

class Point : public _Triple
{
public:
	Point();
	explicit Point(const _Triple& triple);
	Point(GLdouble x, GLdouble y, GLdouble z);
	explicit Point(const HEVert* vert);
	virtual const Point operator+(const Vector& that) const;
	virtual const Point operator+=(const Vector& that);
	friend const Point operator-(const Point& I);
	virtual const Vector operator-(const Point& that) const;
	const Point operator-(const Vector& I) const;
	const Point operator-=(const Vector& I);
	friend const Point operator*(const GLdouble lamda, const Point& I);
	friend const Point operator*=(Point& I, const GLdouble& lamda);
	friend const Point operator/(const Point& I, const GLdouble lamda);
	friend const Point operator/=(Point& I, const GLdouble& lamda);
	friend const Point Transform(const GLdouble matrix[][4], const Point& I);
	friend const Point MidPoint(const Point& A, const Point& B);
	friend const Point MidPoint(const Point* A, const Point* B);
private:
	operator _Triple() ACCESSDENYVOID
	const Vector Normalize() const ACCESSDENYVECTOR
	virtual const GLdouble operator*(const Point& that) ACCESSDENYDOUBLE
	virtual const Point operator-=(const Point& that) ACCESSDENYPOINT
	virtual const Point OuterProduct(const Point& that) const ACCESSDENYPOINT
};