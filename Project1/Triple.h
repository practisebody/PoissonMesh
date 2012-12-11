#pragma once

#include "stdafx.h"
#include "HalfEdgeDeclaration.h"

class _Triple
{
public:
	_Triple(GLdouble x, GLdouble y, GLdouble z);
	GLdouble Module(int power = 1) const;
	const GLdouble operator[](const int index) const;
	GLdouble& operator[](const int index);
	virtual const _Triple operator+(const _Triple& that) const;
	virtual const _Triple operator+=(const _Triple& that);
	friend const _Triple operator-(const _Triple& I);
	virtual const _Triple operator-(const _Triple& that) const;
	virtual const _Triple operator-=(const _Triple& that);
	virtual const GLdouble operator*(const _Triple& that) const;
	virtual const _Triple RespectiveProduct(const _Triple& that) const;
	friend const _Triple operator*(const GLdouble lamda, const _Triple& I);
	friend const _Triple operator*=(_Triple& I, const GLdouble& lamda);
	friend const _Triple operator/(const _Triple& I, const GLdouble lamda);
	friend const _Triple operator/=(_Triple& I, const GLdouble& lamda);
	friend const GLdouble WeightedInnerProduct(const _Triple& I, const _Triple& that, const _Triple& weight);
	friend const _Triple Transform(const GLdouble matrix[][4], const _Triple& I);
protected:
	GLdouble m_x, m_y, m_z;
private:
	_Triple* operator->();
};