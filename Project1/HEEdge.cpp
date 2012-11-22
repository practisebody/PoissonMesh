#include "stdafx.h"
#include "HEEdge.h"
#include "HEFace.h"

HEEdge::HEEdge() : m_pair(NULL)
{
}

void HEEdge::DrawSelected()
{
	glLineWidth(5.0f);
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
		iter->DrawSelected();
		++iter;
	}
	while (iter != endVert());
	glLineWidth(1.0f);
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

void HEEdge::operator+=(Vector vector)
{
	VertIterator iter = beginVert();
	do
	{
		*iter += vector;
		++iter;
	}
	while (iter != endVert());
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
		deletedObjects.insert(eLeave);
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
			vLeave->m_edge = eArrive;
	if (first == true)
	{
		vector<HEEdge*> edges;
		HEVert::EdgeIterator iter =  vArrive->beginEdge();
		do
		{
			edges.push_back(&(*iter));
			++iter;
		} while (iter != vArrive->endEdge());
		// move to center
		vLeave->m_vert = (vLeave->m_vert + vArrive->m_vert) / 2;
		// redirecting
		for (vector<HEEdge*>::size_type i = 0; i < edges.size(); ++i)
			edges[i]->m_vert = m_vert;
		m_pair->Delete(false, deletedObjects);
	}
	deletedObjects.insert(this);
}