#include "stdafx.h"
#include "HEVert.h"

HEVert::HEVert(GLfloat _x, GLfloat _y, GLfloat _z) : m_vert(_x, _y, _z), m_edge(NULL)
{
}

void HEVert::DrawSelected()
{
	glLineWidth(5.0f);
	glPushMatrix();
	glTranslated(m_vert.m_x, m_vert.m_y, m_vert.m_z);
	glutWireCube(0.5);
	glPopMatrix();
	glLineWidth(1.0f);
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

void HEVert::operator+=(Vector vector)
{
	m_vert += vector;
}

void HEVert::ToVerts(vector<HEVert*>& vector)
{
	vector.clear();
	vector.push_back(this);
}

void HEVert::Delete(set<HEObject*>& deletedObjects)
{
}