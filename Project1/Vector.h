#pragma once

#include "stdafx.h"
#include "HalfEdgeDeclaration.h"

typedef class Vector
{
public:
	Vector();
	Vector(GLdouble x, GLdouble y, GLdouble z);
	Vector(const HEVert* vert);
	//Vector(HEEdge* edge);
	GLdouble Module(int power = 1) const;
	const Vector Normalize() const;
	const GLdouble operator[](const int index) const;
	GLdouble& operator[](const int index);
	const Vector operator+(const Vector& that) const;
	const Vector operator+=(const Vector& that);
	const Vector operator-() const;
	const Vector operator-(const Vector& that) const;
	const Vector operator-=(const Vector& that);
	const GLdouble operator*(const Vector& that) const;
	const Vector VectorProduct(const Vector& that) const;
	const Vector OuterProduct(const Vector& that) const;
	friend const Vector operator*(const GLdouble lamda, const Vector& a);
	const Vector operator/(const GLdouble lamda) const;
	friend const GLdouble WeightedInnerProduct(const Vector& I, const Vector& that, const Vector& weight);
	friend const Vector Transform(const GLdouble matrix[][4], const Vector& a);
public:
	GLdouble m_x, m_y, m_z;
} Point;