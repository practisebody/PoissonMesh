#include "stdafx.h"
#include "HEVert.h"
#include "HEFace.h"

HEVert::HEVert(GLdouble _x, GLdouble _y, GLdouble _z) : m_vert(_x, _y, _z), m_edge(NULL), m_realVert(NULL)
{
}

HEVert::HEVert(const Point& point) : m_vert(point[0] , point[1], point[2]), m_edge(NULL), m_realVert(NULL)
{
	//m_vert = m_vert / Parameters::fMagnification;
}

void HEVert::DrawSelected()
{
	LineWidth::DrawHighLight();
	DrawSelectedInner();
	LineWidth::PopLineWidth();
}

void HEVert::DrawSelectedInner()
{
	glPushMatrix();
	Utility::Translated(m_vert);
	Material::SetColorRGB(MATERTIAL_RED);
	glutWireCube(0.0005);
	glPopMatrix();
}

double HEVert::Intersect(const Point& point, const Vector& dir)
{
	GLdouble a = dir * dir;
	GLdouble b = 2 * (point - (Point)this) * dir;
	GLdouble c = (point - (Point)this).Module(2) - Parameters::fIntersectRadius * Parameters::fIntersectRadius;
	GLdouble delta = b * b - 4 * a * c;
	if (delta < 0)
		return -1.0;
	else
		return -2 * c / (b + sqrt(delta));
}

void HEVert::ToVerts(vector<HEVert*>& vector)
{
	vector.clear();
	vector.push_back(this);
}

HEVert* HEVert::InsertVertex(vector<HEFace*>& /*faces*/)
{
	return NULL;
}

void HEVert::Delete(set<HEObject*>& deletedObjects)
{
	EdgeIterator iter = beginEdge();
	bool first = true;
	do
	{
		iter->m_pair->prev()->m_next = iter->m_next;
		if (iter->m_next->m_vert->m_edge == iter->m_pair)
			iter->m_next->m_vert->m_edge = iter->m_next;
		deletedObjects.insert(&*iter);
		deletedObjects.insert(iter->m_pair);
		if (first == true)
		{
			first = false;
			iter->m_face->m_edge = iter->m_next;
		}
		else
			deletedObjects.insert(iter->m_face);
		++iter;
	} while (iter != endEdge());
}

void HEVert::operator+=(Vector vector)
{
	m_vert += vector;
}

void HEVert::operator=(Point point)
{
	m_vert = point;
}

void HEVert::Update(const vector<HEVert*>& verts)
{
	m_vert = verts[m_realVert]->m_vert;
}