#include "stdafx.h"
#include "Vector.h"
#include "HalfEdge.h"
#include "Parameters.h"

Vector::Vector() : _Triple(0.0, 0.0, 0.0)
{
}

Vector::Vector(const _Triple& triple) : _Triple(triple)
{
}

Vector::Vector(GLdouble x, GLdouble y, GLdouble z) : _Triple(x, y, z)
{
}

Vector::Vector(const HEVert* vert) : _Triple(Parameters::fMagnification * (vert->m_vert))
{
}

const Vector Vector::Normalize() const
{
	Vector temp = *this / (Module());
	return temp;
}

const Vector Vector::operator+(const Vector& that) const
{
	return (Vector)(_Triple::operator+(that));
}

const Vector Vector::operator+=(const Vector& that)
{
	return (Vector)(_Triple::operator+=(that));
}

const Vector operator-(const Vector& I)
{
	return (Vector)(-(_Triple)I);
}

const Vector Vector::operator-(const Vector& that) const
{
	return (Vector)(_Triple::operator-(that));
}

const Vector Vector::operator-=(const Vector& that)
{
	return (Vector)(_Triple::operator-=(that));
}

const GLdouble Vector::operator*(const Vector& that) const
{
	return _Triple::operator*(that);
}

const Vector Vector::VectorProduct(const Vector& that) const
{
	return Vector(this->m_x * that.m_x, this->m_y * that.m_y, this->m_z * that.m_z);
}

const Vector Vector::OuterProduct(const Vector& that) const
{
	return Vector(this->m_y * that.m_z - that.m_y * this->m_z, this->m_z * that.m_x - that.m_z * this->m_x,
		this->m_x * that.m_y - that.m_x * this->m_y);
}

const Vector operator*(const GLdouble lamda, const Vector& a)
{
	return (Vector)(lamda * (_Triple)a);
}

const Vector operator*=(Vector& I, const GLdouble& lamda)
{
	return (Vector)((_Triple)I *= lamda);
}

const Vector operator/(const Vector& I, const GLdouble lamda)
{
	return (Vector)((_Triple)I / lamda);
}

const Vector operator/=(Vector& I, const GLdouble& lamda)
{
	return (Vector)((_Triple)I /= lamda);
}