#pragma once

#include "stdafx.h"
#include "Object.h"

#include "HEIteratorsDeclaration.h"

struct HEVert : public HEObject
{
	typedef _HEVert_Iterator_HEVert VertIterator;
	inline const VertIterator beginVert();
	inline const VertIterator endVert();

	typedef _HEVert_Iterator_HEEdge EdgeIterator;
	inline const EdgeIterator beginEdge();
	inline const EdgeIterator endEdge();

	Point m_vert;
    HEEdge* m_edge;  // one of the half-edges leaving the vertex
	HEVert(GLfloat _x, GLfloat _y, GLfloat _z);
	virtual void DrawSelected();
	virtual double Intersect(const Point& poing, const Vector& dir);
	virtual void operator+=(Vector vector);
	virtual void ToVerts(vector<HEVert*>& vector);
	virtual void Delete(set<HEObject*>& deletedObjects);
};

#include "HEVertIterators.h"

inline const HEVert::VertIterator HEVert::beginVert()
{
	return VertIterator(m_edge->m_next->m_vert, m_edge);
}

inline const HEVert::VertIterator HEVert::endVert()
{
	return VertIterator(m_edge->m_next->m_vert, m_edge);
}

inline const HEVert::EdgeIterator HEVert::beginEdge()
{
	return EdgeIterator(m_edge, this);
}

inline const HEVert::EdgeIterator HEVert::endEdge()
{
	return EdgeIterator(m_edge, this);
}
