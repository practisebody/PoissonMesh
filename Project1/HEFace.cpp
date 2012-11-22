#include "stdafx.h"
#include "HEFace.h"

void HEFace::Draw()
{
	m_mtl->SetMaterial();
	glBegin(GL_POLYGON);
	EdgeIterator iter = beginEdge();
	do
	{
		if (m_mtl->IsTransparent() == false)
			glTexCoord2f(iter->m_text->m_x, iter->m_text->m_y);
		glVertex3f(iter->m_vert->m_vert.m_x, iter->m_vert->m_vert.m_y, iter->m_vert->m_vert.m_z);
		++iter;
	} while (iter != endEdge());
	glEnd();
}

void HEFace::DrawSelected()
{
	EdgeIterator iter = beginEdge();
	do
	{
		iter->DrawSelected();
		//iter->m_vert->DrawSelected();
		++iter;
	} while (iter != endEdge());
}

double HEFace::Intersect(const Point& D, const Vector& E)
{
	Point A(m_edge->m_vert);
	Vector B(m_edge->m_next->m_vert), C(m_edge->m_next->m_next->m_vert);
	C = (C - B);
	B = (B - A);
	Vector vNormal = B.OuterProduct(C);
	double t = (A * vNormal - D * vNormal) / (E * vNormal);
	if (t < Parameters::zFar && t > Parameters::zNear)
	{
		if (InPlane(D + t * E))
		{
			return t;
		}
	}
	return -1.0;
}

void HEFace::operator+=(Vector vector)
{
	EdgeIterator iter = beginEdge();
	do
	{
		*iter->m_vert += vector;
		++iter;
	} while (iter != endEdge());
}

void HEFace::ToVerts(vector<HEVert*>& vector)
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

void HEFace::Delete()
{

}

bool HEFace::InPlane(const Point& point)
{
	EdgeIterator lastEdge = beginEdge();
	EdgeIterator iter = beginEdge();
	++iter;
	Vector lastVector = Point(lastEdge->m_vert) - point;
	Vector vector = Point(iter->m_vert) - point;
	Vector vNormal = vector.OuterProduct(lastVector);
	do
	{
		lastEdge = iter;
		++iter;
		lastVector = vector;
		vector = Point(iter->m_vert) - point;
		if (vNormal * (vector.OuterProduct(lastVector)) < 0)
			return false;
	} while (iter != endEdge());
	return true;
}