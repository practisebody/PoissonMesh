#include "stdafx.h"
#include "HEFace.h"

void HEFace::Draw()
{
	m_mtl->SetMaterial();
	glBegin(GL_POLYGON);
	EdgeIterator iter = beginEdge();
	do
	{
		if (m_mtl && m_mtl->IsTransparent() == false)
			glTexCoord2d((*iter->m_text)[0], (*iter->m_text)[1]);
		glVertex3d(iter->m_vert->m_vert[0], iter->m_vert->m_vert[1], iter->m_vert->m_vert[2]);
		//glNormal3d(iter->m_norm->m_x, iter->m_norm->m_y, iter->m_norm->m_z);
		++iter;
	} while (iter != endEdge());
	glEnd();
}

void HEFace::DrawSelected()
{
	LineWidth::DrawHighLight();
	DrawSelectedInner();
	LineWidth::PopLineWidth();
}

void HEFace::DrawSelectedInner()
{
	// Draw Lines
	glBegin(GL_POLYGON);
	EdgeIterator eIter = beginEdge();
	do
	{
		glVertex3d(eIter->m_vert->m_vert[0], eIter->m_vert->m_vert[1], eIter->m_vert->m_vert[2]);
		++eIter;
	} while (eIter != endEdge());
	glEnd();
	// Draw Points
	VertIterator vIter = beginVert();
	do
	{
		vIter->DrawSelectedInner();
		++vIter;
	} while (vIter != endVert());
}

double HEFace::Intersect(const Point& D, const Vector& E)
{
	Point A(m_edge->m_vert), _B(m_edge->m_next->m_vert), _C(m_edge->m_next->m_next->m_vert);
	Vector C = (_C - _B);
	Vector B = (_B - A);
	Vector vNormal = B.OuterProduct(C);
	double t = ((Vector)A * vNormal - (Vector)D * vNormal) / (E * vNormal);
	if (t < Parameters::zFar && t > Parameters::zNear)
	{
		if (InPlane(D + t * E))
		{
			return t;
		}
	}
	return -1.0;
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

HEVert* HEFace::InsertVertex(vector<HEFace*>& faces)
{
	bool first = true;
	Point pCenter(0.0, 0.0, 0.0);
	Vector* vNorm = new Vector(0.0, 0.0, 0.0);
	Point* vText = new Point(0.0, 0.0, 0.0);
	int nCount = 0;
	// edges around this face
	vector<HEEdge*> edges;
	EdgeIterator iter = beginEdge();
	do
	{
		pCenter += (Vector)iter->m_vert->m_vert;
		*vNorm += *iter->m_norm;
		*vText += (Vector)*iter->m_text;
		edges.push_back(&*iter);
		++nCount;
		++iter;
	} while (iter != endEdge());
	*vNorm = *vNorm / nCount;
	*vText = *vText / nCount;

	// new materials
	HEVert* vCenter = new HEVert(pCenter / nCount);
	// edges that leaving center
	HEEdge** leaveEdges = new HEEdge*[nCount];
	// edges that arriving center
	HEEdge** arriveEdges = new HEEdge*[nCount];
	for (int i = 0; i < nCount; ++i)
	{
		leaveEdges[i] = new HEEdge();
		arriveEdges[i] = new HEEdge();
	}
	for (int i = 0; i < nCount - 1; ++i)
		faces.push_back(new HEFace);
	faces.push_back(this);

	// link materails
	vCenter->m_edge = leaveEdges[0];
	for (int i = 0; i < nCount; ++i)
	{
		int nNext = (i + 1) % nCount;
		int nPrev = (i + nCount - 1) % nCount;
		leaveEdges[i]->m_vert = vCenter;
		leaveEdges[i]->m_pair = arriveEdges[i];
		leaveEdges[i]->m_face = faces[nNext];
		leaveEdges[i]->m_next = edges[nNext];
		leaveEdges[i]->m_text = vText;
		leaveEdges[i]->m_norm = vNorm;
		arriveEdges[i]->m_vert = edges[nNext]->m_vert;
		arriveEdges[i]->m_pair = leaveEdges[i];
		arriveEdges[i]->m_face = faces[i];
		arriveEdges[i]->m_next = leaveEdges[nPrev];
		arriveEdges[i]->m_text = edges[nNext]->m_text;
		arriveEdges[i]->m_norm = edges[nNext]->m_norm;
		edges[i]->m_face = faces[i];
		edges[i]->m_next = arriveEdges[i];
		faces[i]->m_edge = edges[i];
		faces[i]->m_mtl = m_mtl;
	}
	faces.pop_back();
	return vCenter;
}

void HEFace::InsertVertices(vector<HEVert*>& verts, vector<HEFace*>& faces)
{
	verts.clear();
	faces.clear();

	Point pCenter(0.0, 0.0, 0.0);
	Vector* vNorm = new Vector(0.0, 0.0, 0.0);
	Point* vText = new Point(0.0, 0.0, 0.0);
	int nCount = 0;
	vector<HEEdge*> edges;
	// scan around face
	EdgeIterator iter = beginEdge();
	do
	{
		pCenter += (Vector)iter->m_vert->m_vert;
		*vNorm += *iter->m_norm;
		*vText += (Vector)*iter->m_text;
		edges.push_back(&*iter);
		++nCount;
		++iter;
	} while (iter != endEdge());
	*vNorm = *vNorm / nCount;
	*vText = *vText / nCount;

	// new materials
	HEVert* vCenter = new HEVert(pCenter / nCount);
	// edges that leaving center
	HEEdge** leaveEdges = new HEEdge*[nCount];
	// edges that arriving center
	HEEdge** arriveEdges = new HEEdge*[nCount];
	for (int i = 0; i < nCount; ++i)
	{
		leaveEdges[i] = new HEEdge();
		arriveEdges[i] = new HEEdge();
		verts.push_back(edges[i]->InsertVertex(faces));
	}
	for (int i = 0; i < nCount - 1; ++i)
		faces.push_back(new HEFace);
	faces.push_back(this);

	// link materails
	vCenter->m_edge = leaveEdges[0];
	for (int i = 0; i < nCount; ++i)
	{
		int nNext = (i + 1) % nCount;
		int nPrev = (i + nCount - 1) % nCount;
		leaveEdges[i]->m_vert = vCenter;
		leaveEdges[i]->m_pair = arriveEdges[i];
		leaveEdges[i]->m_face = faces[nNext];
		leaveEdges[i]->m_next = edges[i]->m_next;
		leaveEdges[i]->m_text = vText;
		leaveEdges[i]->m_norm = vNorm;
		arriveEdges[i]->m_vert = edges[i]->m_next->m_vert;
		arriveEdges[i]->m_pair = leaveEdges[i];
		arriveEdges[i]->m_face = faces[i];
		arriveEdges[i]->m_next = leaveEdges[nPrev];
		arriveEdges[i]->m_text = edges[nNext]->m_text;
		arriveEdges[i]->m_norm = edges[nNext]->m_norm;
		edges[i]->m_face = faces[i];
		edges[i]->m_next->m_face = faces[nNext];
		edges[i]->m_next = arriveEdges[i];
		faces[i]->m_edge = edges[i];
		faces[i]->m_mtl = m_mtl;
	}
	faces.pop_back();
}

void HEFace::Delete(set<HEObject*>& deletedObjects)
{
	Point pCenter(0.0, 0.0, 0.0);
	int count = 3;
	while (m_edge->m_next->m_next->m_next != m_edge)
	{
		pCenter += (Vector)m_edge->m_vert->m_vert;
		++count;
		m_edge->Delete(deletedObjects);
	}
	pCenter += (Vector)m_edge->m_vert->m_vert;
	pCenter += (Vector)m_edge->m_next->m_vert->m_vert;
	pCenter += (Vector)m_edge->m_next->m_next->m_vert->m_vert;
	m_edge->Delete(deletedObjects);
	m_edge->m_next->m_pair->Delete(deletedObjects);
	pCenter = pCenter / count;
	m_edge->m_next->m_pair->m_vert->m_vert = pCenter;
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