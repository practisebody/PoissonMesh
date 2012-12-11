#include "stdafx.h"
#include "Triple.h"

_Triple::_Triple(GLdouble x, GLdouble y, GLdouble z) : m_x(x), m_y(y), m_z(z)
{
}

GLdouble _Triple::Module(int power) const
{
	if (power % 2 == 0)
		return pow(m_x * m_x + m_y * m_y + m_z * m_z, power / 2);
	else
		return pow(m_x * m_x + m_y * m_y + m_z * m_z, (double)power / 2);
}

const GLdouble _Triple::operator[](const int index) const
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

GLdouble& _Triple::operator[](const int index)
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

const _Triple _Triple::operator+(const _Triple& that) const
{
	return _Triple(m_x + that.m_x, m_y + that.m_y, m_z + that.m_z);
}

const _Triple _Triple::operator+=(const _Triple& that)
{
	return *this = _Triple(m_x + that.m_x, m_y + that.m_y, m_z + that.m_z);
}

const _Triple operator-(const _Triple& I)
{
	return _Triple(- I.m_x, - I.m_y, - I.m_z);
}

const _Triple _Triple::operator-(const _Triple& that) const
{
	return _Triple(m_x - that.m_x, m_y - that.m_y, m_z - that.m_z);
}

const _Triple _Triple::operator-=(const _Triple& that)
{
	return *this = _Triple(m_x - that.m_x, m_y - that.m_y, m_z - that.m_z);
}

const GLdouble _Triple::operator*(const _Triple& that) const
{
	return m_x * that.m_x + m_y * that.m_y + m_z * that.m_z;
}

const _Triple _Triple::RespectiveProduct(const _Triple& that) const
{
	return _Triple(m_x * that.m_x, m_y * that.m_y, m_z * that.m_z);
}

const _Triple operator*(const GLdouble lamda, const _Triple& I)
{
	return _Triple(lamda * I.m_x, lamda * I.m_y, lamda * I.m_z);
}

const _Triple operator*=(_Triple& I, const GLdouble& lamda)
{
	return I = _Triple(lamda * I.m_x, lamda * I.m_y, lamda * I.m_z);
}

const _Triple operator/(const _Triple& I, const GLdouble lamda)
{
	return _Triple(I.m_x / lamda, I.m_y / lamda, I.m_z / lamda);
}

const _Triple operator/=(_Triple& I, const GLdouble& lamda)
{
	return I = _Triple(I.m_x / lamda, I.m_y / lamda, I.m_z / lamda);
}

const GLdouble WeightedInnerProduct(const _Triple& I, const _Triple& that, const _Triple& weight)
{
	return I.m_x * that.m_x * weight.m_x + I.m_y * that.m_y * weight.m_y
		+ I.m_z * that.m_z * weight.m_z;
}

const _Triple Transform(const GLdouble matrix[][4], const _Triple& I)
{
	return _Triple(matrix[0][0] * I[0] + matrix[0][1] * I[1] + matrix[0][2] * I[2] + matrix[0][3],
		matrix[1][0] * I[0] + matrix[1][1] * I[1] + matrix[1][2] * I[2] + matrix[1][3],
		matrix[2][0] * I[0] + matrix[2][1] * I[1] + matrix[2][2] * I[2] + matrix[2][3]);
}

_Triple* _Triple::operator->()
{
	return this;
}