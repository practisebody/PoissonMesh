#pragma once

#include "stdafx.h"
#include "Triple.h"
#include "HalfEdgeDeclaration.h"

class Vector : public _Triple
{
public:
	Vector();
	explicit Vector(const _Triple& triple);
	Vector(GLdouble x, GLdouble y, GLdouble z);
	Vector(const HEVert* vert);
	const Vector Normalize() const;
	virtual const Vector operator+(const Vector& that) const;
	virtual const Vector operator+=(const Vector& that);
	friend const Vector operator-(const Vector& I);
	virtual const Vector operator-(const Vector& that) const;
	virtual const Vector operator-=(const Vector& that);
	virtual const GLdouble operator*(const Vector& that) const;
	const Vector VectorProduct(const Vector& that) const;
	virtual const Vector OuterProduct(const Vector& that) const;
	friend const Vector operator*(const GLdouble lamda, const Vector& I);
	friend const Vector operator*=(Vector& I, const GLdouble& lamda);
	friend const Vector operator/(const Vector& I, const GLdouble lamda);
	friend const Vector operator/=(Vector& I, const GLdouble& lamda);
private:
	operator _Triple() ACCESSDENYVOID
};