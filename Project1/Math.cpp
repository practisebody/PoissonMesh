#include "stdafx.h"
#include "Math.h"
#include "HalfEdge.h"
#include <time.h>

matrix::DOK* Math::Poisson = NULL;

void Math::GetNormalizedDis(const Point& center, const vector<HEFace*>& faces, vector<GLdouble>& distances, vector<Point>& centers)
{
	GLdouble dDis, dMaxDis = 0;
	distances.push_back(0);	
	centers.push_back(Point());
	for (vector<HEFace*>::size_type i = 1; i < faces.size(); ++i)
	{
		Point pTestCenter(0, 0, 0);
		int number = 0;
		HEFace::VertIterator iter = faces[i]->beginVert();
		do
		{
			pTestCenter += (Vector)(iter->m_vert);
			++number;
			++iter;
		}
		while (iter != faces[i]->endVert());
		pTestCenter /= number;
		centers.push_back(pTestCenter);
		dDis = (center - pTestCenter).Module();
		distances.push_back(dDis);
		Utility::SetMax<GLdouble>(dMaxDis, dDis);
	}
	//GLdouble range = exp(1) - 1;
	for (vector<GLdouble>::size_type i = 1; i < distances.size(); ++i)
	{
		distances[i] = exp(- distances[i] * distances[i] * 50);
		//distances[i] = 1 - distances[i] / dMaxDis;
	}
}

void Math::GetRotationMatrix(const Direction& dir, const GLdouble& angle, GLdouble Rotation[4][4])
{
	memset(Rotation, 0, sizeof(GLdouble) * 4 * 4);
	Rotation[0][0] = Rotation[1][1] = Rotation[2][2] = 1.0l;
	switch (dir)
	{
	case DIR_X:
		Rotation[1][1] = cos(angle); Rotation[1][2] = - sin(angle);
		Rotation[2][1] = sin(angle); Rotation[2][2] = cos(angle);
		break;
	case DIR_Y:
		Rotation[0][0] = cos(angle); Rotation[0][2] = - sin(angle);
		Rotation[2][0] = sin(angle); Rotation[2][2] = cos(angle);
		break;
	case DIR_Z:
		Rotation[0][0] = cos(angle); Rotation[0][1] = - sin(angle);
		Rotation[1][0] = sin(angle); Rotation[1][1] = cos(angle);
		break;
	default:
		MYASSERT(false);
		break;
	}
}

void Math::CalcGradient(const vector<HEFace*>& faces)
{
	GLdouble Rotation[4][4];
	memset(Rotation, 0, sizeof(Rotation));
	for (vector<HEFace*>::size_type i = 0; i < faces.size(); ++i)
	{
		HEFace::EdgeIterator iter = faces[i]->beginEdge();
		Point a[3] = {
			iter->m_vert->m_vert,
			iter->m_next->m_vert->m_vert,
			iter->m_next->m_next->m_vert->m_vert,
		};
		Vector norm = (a[2] - a[1]).OuterProduct(a[1] - a[0]).Normalize();
		// row 0
		Rotation[0][0] = norm[0] * norm[0];
		Rotation[0][1] = norm[0] * norm[1] - norm[2];
		Rotation[0][2] = norm[0] * norm[2] + norm[1];
		// row 1
		Rotation[1][0] = norm[0] * norm[1] + norm[2];
		Rotation[1][1] = norm[1] * norm[1];
		Rotation[1][2] = norm[1] * norm[2] - norm[0];
		// row 2
		Rotation[2][0] = norm[0] * norm[2] - norm[1];
		Rotation[2][1] = norm[1] * norm[2] + norm[0];
		Rotation[2][2] = norm[2] * norm[2];
		GLdouble area = faces[i]->CalcArea();
		do
		{
			iter->m_vert->m_Gradient = (Vector)Transform(Rotation, iter->m_next->m_vert->m_vert - iter->prev()->m_vert->m_vert) / 2 / area;
			++iter;
		}
		while (iter != faces[i]->endEdge());
	}
}

void Math::InitPoisson(const vector<HEFace*>& faces, vector<HEVert*>& rawVerts)
{
	time_t start = time(NULL);
	if (Poisson != NULL)
		delete Poisson;
	Poisson = new matrix::DOK(rawVerts.size() - 1, rawVerts.size() - 1);
	for (vector<HEFace*>::size_type i = 0; i < faces.size(); ++i)
		faces[i]->CalcArea();
	CalcGradient(faces);
	for (vector<HEFace*>::size_type i = 0; i < faces.size(); ++i)
	{
		HEFace::EdgeIterator iter = faces[i]->beginEdge();
		GLdouble area = faces[i]->CalcArea();
		do
		{
			HEVert* v0 = iter->prev()->m_vert;
			HEVert* v1 = iter->m_next->m_vert;
			HEVert* v2 = iter->m_vert;
			(*Poisson)(v0->m_realVert - 1, v0->m_realVert - 1) += v0->m_Gradient * v0->m_Gradient * area;
			(*Poisson)(v0->m_realVert - 1, v1->m_realVert - 1) += v0->m_Gradient * v1->m_Gradient * area;
			(*Poisson)(v0->m_realVert - 1, v2->m_realVert - 1) += v0->m_Gradient * v2->m_Gradient * area;
			++iter;
		}
		while (iter != faces[i]->endEdge());
	}
	time_t stop = time(NULL);
	printf("Init: %d s\n", stop - start);
}

void Math::CalcPoisson(const vector<HEFace*>& faces, vector<HEVert*>& rawVerts, const set<HEVert*>& fixedVerts)
{
	time_t start = time(NULL);
	matrix::vector B[3] = {
		matrix::vector(rawVerts.size() - 1),
		matrix::vector(rawVerts.size() - 1),
		matrix::vector(rawVerts.size() - 1),
	};
	CalcGradient(faces);
	for (vector<HEFace*>::size_type i = 0; i < faces.size(); ++i)
	{
		HEFace::EdgeIterator iter = faces[i]->beginEdge();
		GLdouble area = faces[i]->CalcArea();
		do
		{
			HEVert* v0 = iter->prev()->m_vert;
			HEVert* v1 = iter->m_next->m_vert;
			HEVert* v2 = iter->m_vert;
			GLdouble co[3]= {
				v0->m_Gradient * v0->m_Gradient * area,
				v0->m_Gradient * v1->m_Gradient * area,
				v0->m_Gradient * v2->m_Gradient * area,
			};
			for (int j = 0; j < 3; ++j)
				B[j][v0->m_realVert - 1] += co[0] * v0->m_vert[j] + co[1] * v1->m_vert[j] + co[2] * v2->m_vert[j];
			++iter;
		}
		while (iter != faces[i]->endEdge());
	}
	matrix::DOK A(*Poisson);
	for (set<HEVert*>::iterator iter = fixedVerts.begin(); iter != fixedVerts.end(); ++iter)
	{
		GLint real = (*iter)->m_realVert - 1;
		A(real, real) = 1;
		HEVert::EdgeIterator eIt = (*iter)->beginEdge();
		do
		{
			GLint neighbor = eIt->m_next->m_vert->m_realVert - 1;
			for (int i = 0; i < 3; ++i)
				B[i][neighbor] -= A(real, neighbor) * (*iter)->m_vert[i];
			A(real, neighbor) = A(neighbor, real) = 0;
			++eIt;
		}
		while (eIt != (*iter)->endEdge());
		for (int i = 0; i < 3; ++i)
			B[i][(*iter)->m_realVert - 1] = (*iter)->m_vert[i];
	}
	matrix::vector x = A.conjugate_gradient(B[0], B[0], B[0].size(), 0.000001);
	matrix::vector y = A.conjugate_gradient(B[1], B[1], B[0].size(), 0.000001);
	matrix::vector z = A.conjugate_gradient(B[2], B[2], B[0].size(), 0.000001);
	for (vector<HEVert*>::size_type i = 1; i < rawVerts.size(); ++i) {
		rawVerts[i]->m_vert[0] = x[i - 1];
		rawVerts[i]->m_vert[1] = y[i - 1];
		rawVerts[i]->m_vert[2] = z[i - 1];
	}
	time_t stop = time(NULL);
	printf("Calc: %d s\n", stop - start);
}