#include "stdafx.h"
#include "Vector.h"
#include "HalfEdge.h"
#include "Parameters.h"

Vector::Vector() : m_x(0), m_y(0), m_z(0)
{
}

Vector::Vector(GLdouble x, GLdouble y, GLdouble z) : m_x(x), m_y(y), m_z(z)
{
}

Vector::Vector(const HEVert* vert)
{
	this->Vector::Vector(Parameters::fMagnification * (vert->m_vert));
}

/*Vector::Vector(HEEdge* edge) : m_x(edge->m_next->m_vert->m_x - edge->m_vert->m_x),
	m_y(edge->m_next->m_vert->m_y - edge->m_vert->m_y), m_z(edge->m_next->m_vert->m_z - edge->m_vert->m_z)
{
}*/

GLdouble Vector::Module(int power) const
{
	if (power % 2 == 0)
		return pow(m_x * m_x + m_y * m_y + m_z * m_z, power / 2);
	else
		return pow(m_x * m_x + m_y * m_y + m_z * m_z, (double)power / 2);
}

const Vector Vector::Normalize() const
{
	Vector temp = *this / (this->Module());
	return temp;
}

const GLdouble Vector::operator[](const int index) const
{
	switch (index)
	{
	case 0:
		return m_x;
	case 1:
		return m_y;
	case 2:
		return m_z;
	default :
		exit(0);
	}
}

GLdouble& Vector::operator[](const int index)
{
	switch (index)
	{
	case 0:
		return m_x;
	case 1:
		return m_y;
	case 2:
		return m_z;
	default :
		exit(0);
	}
}

const Vector Vector::operator+(const Vector& that) const
{
	return Vector(this->m_x + that.m_x, this->m_y + that.m_y, this->m_z + that.m_z);
}

const Vector Vector::operator+=(const Vector& that)
{
	return *this = Vector(this->m_x + that.m_x, this->m_y + that.m_y, this->m_z + that.m_z);
}

const Vector Vector::operator-() const
{
	return Vector(-this->m_x, -this->m_y, -this->m_z);
}

const Vector Vector::operator-(const Vector& that) const
{
	return Vector(this->m_x - that.m_x, this->m_y - that.m_y, this->m_z - that.m_z);
}

const Vector Vector::operator-=(const Vector& that)
{
	return *this = Vector(this->m_x - that.m_x, this->m_y - that.m_y, this->m_z - that.m_z);
}

const GLdouble Vector::operator*(const Vector& that) const
{
	return this->m_x * that.m_x + this->m_y * that.m_y + this->m_z * that.m_z;
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
	return Vector(lamda * a.m_x, lamda * a.m_y, lamda * a.m_z);
}

const Vector Vector::operator/(const GLdouble lamda) const
{
	return Vector(this->m_x / lamda, this->m_y / lamda, this->m_z / lamda);
}

const GLdouble WeightedInnerProduct(const Vector& I, const Vector& that, const Vector& weight)
{
	return I.m_x * that.m_x * weight.m_x + I.m_y * that.m_y * weight.m_y
		+ I.m_z * that.m_z * weight.m_z;
}

const Vector Transform(const GLdouble matrix[][4], const Vector& a)
{
	return Vector(matrix[0][0] * a[0] + matrix[0][1] * a[1] + matrix[0][2] * a[2] + matrix[0][3],
		matrix[1][0] * a[0] + matrix[1][1] * a[1] + matrix[1][2] * a[2] + matrix[1][3],
		matrix[2][0] * a[0] + matrix[2][1] * a[1] + matrix[2][2] * a[2] + matrix[2][3]);
}