#pragma once

#include "stdafx.h"
#include "Object.h"

#include "HEIteratorsDeclaration.h"

struct HEFace : public HEObject
{
	typedef _HEFace_Iterator_HEVert VertIterator;
	inline const VertIterator beginVert();
	inline const VertIterator endVert();

	typedef _HEFace_Iterator_HEEdge EdgeIterator;
	inline const EdgeIterator beginEdge();
	inline const EdgeIterator endEdge();

    HEEdge* m_edge;  // one of the half-edges
	Material* m_mtl;
	virtual void Draw();
	virtual void DrawSelected();
	virtual double Intersect(const Point& D, const Vector& E);
	virtual void operator+=(Vector vector);
	virtual void ToVerts(vector<HEVert*>& vector);
	virtual void Delete();
	bool InPlane(const Point& point);
};

#include "HEFaceIterators.h"

inline const HEFace::VertIterator HEFace::beginVert()
{
	return VertIterator(m_edge->m_vert, m_edge);
}

inline const HEFace::VertIterator HEFace::endVert()
{
	return VertIterator(m_edge->m_vert, m_edge);
}

inline const HEFace::EdgeIterator HEFace::beginEdge()
{
	return EdgeIterator(m_edge, this);
}

inline const HEFace::EdgeIterator HEFace::endEdge()
{
	return EdgeIterator(m_edge, this);
}