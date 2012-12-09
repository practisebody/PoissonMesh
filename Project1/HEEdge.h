#pragma once

#include "stdafx.h"
#include "Object.h"

#include "HEIteratorsDeclaration.h"

struct HEEdge : public HEObject
{
	typedef _HEEdge_Iterator_HEVert VertIterator;
	inline const VertIterator beginVert();
	inline const VertIterator endVert();

	typedef _HEEdge_Iterator_HEEdge EdgeIterator;
	inline const EdgeIterator beginEdge();
	inline const EdgeIterator endEdge();

    HEVert* m_vert;		// vertex at the leaving
    HEEdge* m_pair;		// oppositely oriented adjacent half-edge 
    HEFace* m_face;		// face the half-edge borders
    HEEdge* m_next;		// next half-edge around the face
	Point* m_text;		// the texture point of above vertex
	Vector* m_norm;		// normal vector of above vertex
	HEEdge();
	virtual void DrawSelected();
	virtual double Intersect(const Point& point, const Vector& dir);
	virtual void ToVerts(vector<HEVert*>& vector);
	HEEdge* prev();
	HEEdge* left();
	virtual void Delete(set<HEObject*>& deletedObjects);
	void Delete(bool first, set<HEObject*>& deletedObjects);
};

#include "HEEdgeIterators.h"

inline const HEEdge::VertIterator HEEdge::beginVert()
{
	return VertIterator(m_vert, this);
}

inline const HEEdge::VertIterator HEEdge::endVert()
{
	return VertIterator(m_next->m_next->m_vert, this);
}

inline const HEEdge::EdgeIterator HEEdge::beginEdge()
{
	return EdgeIterator(this, m_face);
}

inline const HEEdge::EdgeIterator HEEdge::endEdge()
{
	return EdgeIterator(this, m_face);
}