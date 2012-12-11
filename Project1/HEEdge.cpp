#include "stdafx.h"
#include "HEEdge.h"
#include "HEFace.h"

HEEdge::HEEdge() : m_pair(NULL)
{
}

void HEEdge::DrawSelected()
{
	LineWidth::DrawHighLight();
	DrawSelectedInner();
	LineWidth::PopLineWidth();
}

void HEEdge::DrawSelectedInner()
{
	// Draw Line
	glBegin(GL_LINES);
	VertIterator iter = beginVert();
	do
	{
		glVertex3d(iter->m_vert.m_x, iter->m_vert.m_y, iter->m_vert.m_z);
		glNormal3d(iter->m_edge->m_norm->m_x, iter->m_edge->m_norm->m_y, iter->m_edge->m_norm->m_z);
		++iter;
	}
	while (iter != endVert());
	glEnd();
	// Draw Points
	iter = beginVert();
	do
	{
		iter->DrawSelectedInner();
		++iter;
	}
	while (iter != endVert());
}

double HEEdge::Intersect(const Point& point1, const Vector& dir1)
{
	Point point2 = m_vert;
	Vector dir2 = (Point)m_next->m_vert - (Point)m_vert;
	// | a00 a01 | = | b0 |
	// | a10 a11 |   | b1 |
	double a[2][2] =
	{
		{ dir1 * dir1, -dir1 * dir2 },
		{ dir2 * dir1, -dir2 * dir2 },
	};
	double b[2] = 
	{
		(point2 - point1) * dir1,
		(point2 - point1) * dir2,
	};
	double D = a[0][0] * a[1][1] - a[0][1] * a[1][0];
	double X = (b[0] * a[1][1] - b[1] * a[0][1]) / D;
	double Y = (a[0][0] * b[1] - a[1][0] * b[0]) / D;
	if (((point1 + X * dir1) - (point2 + Y * dir2)).Module() < Parameters::fIntersectRadius)
		return X;
	else
		return -1.0;
}

void HEEdge::ToVerts(vector<HEVert*>& vector)
{
	vector.clear();
	VertIterator iter = beginVert();
	do
	{
		vector.push_back(&(*iter));
		++iter;
	}
	while (iter != endVert());
}

HEEdge* HEEdge::prev()
{
	EdgeIterator iter = this;
	while (iter->m_next != this)
		++iter;
	return iter.m_this;
}

HEEdge* HEEdge::left()
{
	return prev()->m_pair;
}

HEVert* HEEdge::InsertVertex(vector<HEFace*>& /*faces*/)
{
	HEVert* vRet = new HEVert((m_vert->m_vert + m_next->m_vert->m_vert) / 2);
	// new edges
	HEEdge* eLeave = new HEEdge();
	HEEdge* eArrive = new HEEdge();
	eLeave->m_vert = vRet;
	eLeave->m_pair = eArrive;
	eLeave->m_face = m_face;
	eLeave->m_next = m_next;
	eLeave->m_text = new Point((*m_text + *(m_next->m_text)) / 2);
	eLeave->m_norm = new Vector((*m_norm + *(m_next->m_norm)) / 2);
	eArrive->m_vert = m_next->m_vert;
	eArrive->m_pair = eLeave;
	eArrive->m_face = m_pair->m_face;
	eArrive->m_next = m_pair;
	eArrive->m_text = m_pair->m_text;
	eArrive->m_norm = m_pair->m_norm;
	// fix original edges
	m_pair->m_vert = vRet;
	m_pair->prev()->m_next = eArrive;
	m_pair->m_text = new Point((*(m_pair->m_text) + *(m_pair->m_next->m_text)) / 2);
	m_pair->m_norm = new Point((*(m_pair->m_norm) + *(m_pair->m_next->m_norm)) / 2);
	m_next = eLeave;
	// fix vertices
	if (m_next->m_vert->m_edge == m_pair)
		m_next->m_vert->m_edge = eArrive;
	vRet->m_edge = eLeave;
	return vRet;
}

void HEEdge::Delete(set<HEObject*>& deletedObjects)
{
	Delete(true, deletedObjects);
}

void HEEdge::Delete(bool first, set<HEObject*>& deletedObjects)
{
	HEVert* vLeave = m_vert;
	HEVert* vArrive = m_next->m_vert;
	HEEdge* eLeave = prev();
	HEEdge* eArrive = m_next;

	// It's a triangle
	if (m_next->m_next->m_next == this)
	{
		eLeave->m_pair->m_pair = eArrive->m_pair;
		eArrive->m_pair->m_pair = eLeave->m_pair;
		if (vArrive->m_edge == eArrive)
			vArrive->m_edge = eArrive->m_pair->m_next;
		deletedObjects.insert(eLeave);
		if (eLeave->m_vert->m_edge == eLeave)
			eLeave->m_vert->m_edge = eArrive->m_pair;
		deletedObjects.insert(eArrive);
		deletedObjects.insert(m_face);
	}
	// Not a triangle
	else
	{
		eLeave->m_next = eArrive;
		if (m_face->m_edge == this)
			m_face->m_edge = eArrive;
	}
	if (vLeave->m_edge == this)
		vLeave->m_edge = eLeave->m_pair;
	if (first == true)
	{
		// move to center
		vLeave->m_vert = (vLeave->m_vert + vArrive->m_vert) / 2;
		HEVert::EdgeIterator iter =  vArrive->beginEdge();
		do
		{
			// redirecting
			iter->m_vert = m_vert;
			++iter;
		} while (iter != vArrive->endEdge());
		m_pair->Delete(false, deletedObjects);
	}
	deletedObjects.insert(this);
}

void HEEdge::DeleteWithoutMove(set<HEObject*>& deletedObjects)
{
	prev()->m_next = m_pair->m_next;
	m_pair->prev()->m_next = m_next;
	HEFace::EdgeIterator iter = m_pair->m_face->beginEdge();
	do
	{
		iter->m_face = m_face;
		++iter;
	} while (iter != m_pair->m_face->endEdge());
	if (m_vert->m_edge == this)
		m_vert->m_edge = m_pair->m_next;
	if (m_pair->m_vert->m_edge == m_pair)
		m_pair->m_vert->m_edge = m_next;
	if (m_face->m_edge == this)
		m_face->m_edge = m_next;
	deletedObjects.insert(this);
	deletedObjects.insert(m_pair);
	deletedObjects.insert(m_pair->m_face);
}