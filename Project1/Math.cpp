#include "stdafx.h"
#include "Math.h"
#include "HalfEdge.h"
#include <time.h>

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
		//distances[i] = 1 - ((exp(distances[i] / dMaxDis) - 1) / range);
		distances[i] = 1 - distances[i] / dMaxDis;
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

void Math::InitPoisson(vector<HEFace*>& faces)
{
	time_t start = time(NULL);
	// do something
	time_t stop = time(NULL);
	printf("Init: %d ms\n", stop - start);
}

void Math::CalcPoisson(vector<HEFace*>& faces)
{
	time_t start = time(NULL);
	// do something
	time_t stop = time(NULL);
	printf("Calc: %d ms\n", stop - start);
}