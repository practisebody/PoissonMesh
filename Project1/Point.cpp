#include "stdafx.h"
#include "Point.h"
#include "HalfEdge.h"
#include "Parameters.h"

Point::Point() : _Triple(0.0, 0.0, 0.0)
{
}

Point::Point(const _Triple& triple) : _Triple(triple)
{
}

Point::Point(GLdouble x, GLdouble y, GLdouble z) : _Triple(x, y, z)
{
}

Point::Point(const HEVert* vert) : _Triple(Parameters::fMagnification * (vert->m_vert))
{
}

const Point Point::operator+(const Vector& that) const
{
	return (Point)(_Triple::operator+(that));
}

const Point Point::operator+=(const Vector& that)
{
	return (Point)(_Triple::operator+=(that));
}

const Point operator-(const Point& I)
{
	return (Point)(-(_Triple)I);
}

const Vector Point::operator-(const Point& that) const
{
	return (Vector)(_Triple::operator-(that));
}

const Point Point::operator-(const Vector& I) const
{
	return *this + (- I);
}

const Point Point::operator-=(const Vector& I)
{
	return *this = (Point)_Triple::operator-=(I);
}

const Point operator*(const GLdouble lamda, const Point& a)
{
	return (Point)(lamda * (_Triple)a);
}

const Point operator*=(Point& I, const GLdouble& lamda)
{
	return (Point)((_Triple)I *= lamda);
}

const Point operator/(const Point& I, const GLdouble lamda)
{
	return (Point)((_Triple)I / lamda);
}

const Point operator/=(Point& I, const GLdouble& lamda)
{
	return (Point)((_Triple)I /= lamda);
}

const Point Transform(const GLdouble matrix[][4], const Point& I)
{
	return (Point)Transform(matrix, (_Triple)I);
}

const Point MidPoint(const Point& A, const Point& B)
{
	return Point((A.m_x + B.m_x) / 2, (A.m_y + B.m_y) / 2, (A.m_z + B.m_z) / 2);
}

const Point MidPoint(const Point* A, const Point* B)
{
	return Point((A->m_x + B->m_x) / 2, (A->m_y + B->m_y) / 2, (A->m_z + B->m_z) / 2);
}