#include "stdafx.h"
#include "World.h"
#include "HalfEdge.h"
#include <algorithm>

World::World(int windowwidth, int windowheight, GLdouble eyex, GLdouble eyey, GLdouble eyez,
	GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz)
	: m_WindowWidth(windowwidth), m_WindowHeight(windowheight),	m_Eye(eyex, eyey, eyez),
	m_Center(centerx, centery, centerz), m_Up(upx, upy, upz), m_SelectSum(0.0, 0.0, 0.0),
	lastx(m_WindowWidth / 2), lasty(m_WindowHeight / 2)
{
	m_Up.Normalize();
}

World::~World()
{
	map<string, Material*>::iterator iter;
	for (iter = m_Materials.begin(); iter != m_Materials.end(); ++iter)
		delete iter->second;
	for (vector<HEVert*>::size_type nIndex = 0; nIndex < m_Vertices.size(); ++nIndex)
		delete m_Vertices[nIndex];
	for (vector<HEFace*>::size_type nIndex = 0; nIndex < m_Faces.size(); ++nIndex)
		delete m_Faces[nIndex];
}

void World::Init(const char* name)
{
	ReadOBJ(name, m_Vertices, m_TextPoints, m_NormVectors, m_Faces, m_Materials);
}

Vector World::GetUp()
{
	return m_Up;
}

void World::SetUp(const Vector& up)
{
	m_Up = up.Normalize();
}

void World::AddVert(HEVert* const vert)
{
	m_Vertices.push_back(vert);
}

void World::AppendVerts(vector<HEVert*>& verts)
{
	for (vector<HEVert*>::size_type nIndex = 0; nIndex < verts.size(); ++nIndex)
		AddVert(verts[nIndex]);
}

void World::AddFace(HEFace* const face)
{
	m_Faces.push_back(face);
}

void World::AppendFaces(vector<HEFace*>& faces)
{
	for (vector<HEFace*>::size_type nIndex = 0; nIndex < faces.size(); ++nIndex)
		AddFace(faces[nIndex]);
}

void World::Orient()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, (GLdouble)m_WindowWidth / m_WindowHeight, Parameters::zNear, Parameters::zFar);
	gluLookAt(m_Eye.m_x, m_Eye.m_y, m_Eye.m_z, m_Center.m_x, m_Center.m_y, m_Center.m_z,
		m_Up.m_x, m_Up.m_y, m_Up.m_z);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void World::Draw()
{
	Vector vForward = (m_Center - m_Eye).Normalize();
	Vector vRight = vForward.OuterProduct(GetUp());
	Point Left = (m_Eye + 2 * Parameters::zNear * vForward - vRight / 2000);
	Point Right = (m_Eye + 2 * Parameters::zNear * vForward + vRight / 2000);
	Point Up = (m_Eye + 2 * Parameters::zNear * vForward + GetUp() / 2000);
	Point Down = (m_Eye + 2 * Parameters::zNear * vForward - GetUp() / 2000);

	Material::SetMaterialWhite();
	glLineWidth(1.0f);

	// Draw axes
	glBegin(GL_LINES);
		glVertex3d(m_Eye.m_x - 100.0, 0.0, 0.0);
		glVertex3d(m_Eye.m_x + 100.0, 0.0, 0.0);
		glVertex3d(0.0, m_Eye.m_y - 100.0, 0.0);
		glVertex3d(0.0, m_Eye.m_y + 100.0, 0.0);
		glVertex3d(0.0, 0.0, m_Eye.m_z - 100.0);
		glVertex3d(0.0, 0.0, m_Eye.m_z + 100.0);
	glEnd();

	// Draw cursor
	glBegin(GL_LINES);
		glVertex3d(Left.m_x, Left.m_y, Left.m_z);
		glVertex3d(Right.m_x, Right.m_y, Right.m_z);
		glVertex3d(Up.m_x, Up.m_y, Up.m_z);
		glVertex3d(Down.m_x, Down.m_y, Down.m_z);
	glEnd();

	// Draw Objects
	glPushMatrix();
	glScalef(Parameters::fMagnification, Parameters::fMagnification, Parameters::fMagnification);

	vector<HEFace*> TransparentFaces;
	for (vector<HEFace*>::size_type nIndex = 0; nIndex < m_Faces.size(); ++ nIndex)
	{
		if (m_Faces[nIndex] != NULL && m_Faces[nIndex]->m_mtl != NULL)
		{
			if (m_Faces[nIndex]->m_mtl->IsTransparent() == false)
				m_Faces[nIndex]->Draw();
			else
				TransparentFaces.push_back(m_Faces[nIndex]);
		}
		else
			m_Faces[nIndex]->Draw();
	}

	// Draw Transparent Objects
	glDepthMask(GL_FALSE);
	for (vector<HEFace*>::size_type nIndex = 0; nIndex < TransparentFaces.size(); ++ nIndex)
		TransparentFaces[nIndex]->Draw();
	glDepthMask(GL_TRUE);

	// Draw Active Object
	Material::SetMaterialWhite();
	if (!m_objSelected.empty())
		for (set<HEObject*>::iterator iter = m_objSelected.begin(); iter != m_objSelected.end(); ++iter)
			(*iter)->DrawSelected();
	switch (Parameters::status)
	{
	case Parameters::TRANSLATE:
		Utility::DrawTranslateAxis(m_SelectSum / m_vertSelected.size());
		break;
	case Parameters::SCALE:
		Utility::DrawScaleAxis(m_SelectSum / m_vertSelected.size());
		break;
	case Parameters::ROTATE:
		Utility::DrawRotateAxis(m_SelectSum / m_vertSelected.size());
		break;
	default:
		break;
	}
	glPopMatrix();
}

HEObject* World::GetNearestObject()
{
	Vector vForward = (m_Center - m_Eye).Normalize();
	int Nearest = -1;
	double LeastDis = Parameters::zFar, Distance;
	HEFace* faceRet = NULL;
	HEEdge* edgeRet = NULL;
	HEVert* vertRet = NULL;
	for (vector<HEFace*>::size_type nIndex = 0; nIndex < m_Faces.size(); ++nIndex)
	{
		Distance = m_Faces[nIndex]->Intersect(m_Eye, vForward);
		if (Distance > Parameters::zNear && LeastDis > Distance)
		{
			Nearest = nIndex;
			LeastDis = Distance;
		}
	}
	if (Nearest >= 0)
		faceRet =  m_Faces[Nearest];
	// A Face Been Selected
	if (faceRet != NULL)
	{
		HEFace::EdgeIterator iter = faceRet->beginEdge();
		do
		{
			Distance = iter->Intersect(m_Eye, vForward);
			LeastDis = Parameters::zFar;
			if (Distance > Parameters::zNear && LeastDis > Distance)
			{
				edgeRet = &(*iter);
				LeastDis = Distance;
			}
			++iter;
		} while (iter != faceRet->endEdge()); 
		// An Edge Been Selected
		if (edgeRet != NULL)
		{
			HEEdge::VertIterator iter = edgeRet->beginVert();
			do
			{
				Distance = iter->Intersect(m_Eye, vForward);
				LeastDis = Parameters::zFar;
				if (Distance > Parameters::zNear && LeastDis > Distance)
				{
					vertRet = &(*iter);
					LeastDis = Distance;
				}
				++iter;
			} while (iter != edgeRet->endVert());
			if (vertRet != NULL)
				return vertRet;
			else
				return edgeRet;
		}
		else
			return faceRet;
	}
	return NULL;
}

void World::RecalculateSum()
{
	new (&m_SelectSum) Point(0.0, 0.0, 0.0);
	for (set<HEVert*>::iterator vertIter = m_vertSelected.begin(); vertIter != m_vertSelected.end(); ++vertIter)
	{
		m_SelectSum += (*vertIter)->m_vert;
	}
}

void World::OnMouseClick(int modifiers)
{
	vector<HEVert*> verts;
	vector<HEVert*>::iterator vertIter;
	HEObject* obj = GetNearestObject();
	// If Ctrl Not Down
	if ((modifiers & GLUT_ACTIVE_CTRL) == 0)
	{
		new (&m_SelectSum) Point(0.0, 0.0, 0.0);
		m_objSelected.clear();
		m_vertSelected.clear();
	}
	// If Alt Not Down
	if ((modifiers & GLUT_ACTIVE_ALT) == 0)
	{
		if (obj != NULL)
		{
			set<HEObject*>::iterator iter = m_objSelected.find(obj);
			// if not selected
			if (iter == m_objSelected.end())
			{
				m_objSelected.insert(obj);
				obj->ToVerts(verts);
				for (vertIter = verts.begin(); vertIter != verts.end(); ++vertIter)
				{
					m_SelectSum += (*vertIter)->m_vert;
					m_vertSelected.insert(*vertIter);
				}
			}
			// if selected
			else
			{
				m_objSelected.erase(iter);
				m_vertSelected.clear();
				new (&m_SelectSum) Point(0.0, 0.0, 0.0);
				for (iter = m_objSelected.begin(); iter != m_objSelected.end(); ++iter)
				{
					(*iter)->ToVerts(verts);
					for (vertIter = verts.begin(); vertIter != verts.end(); ++vertIter)
					{
						m_SelectSum += (*vertIter)->m_vert;
						m_vertSelected.insert(*vertIter);
					}
				}
			}
		}
	}
	// If Alt Down
	//else
	//{
	//	m_objSelected.clear();
	//	m_vertSelected.clear();
	//	set<HEVert*> verts[3];
	//	int last = 0, now = 1, next = 2;
	//	HEVert* pVert = dynamic_cast<HEVert*>(obj);
	//	if (pVert != NULL)
	//	{
	//		verts[now].insert(pVert);
	//		m_objSelected.insert(pVert);
	//		m_vertSelected.insert(pVert);
	//	}
	//	while (verts[now].empty() == false)
	//	{
	//		for (set<HEVert*>::iterator _iter = verts[now].begin(); _iter != verts[now].end(); ++_iter)
	//		{
	//			pVert = *_iter;
	//			HEVert::VertIterator iter = pVert->beginVert();
	//			do
	//			{
	//				if (verts[last].find(&(*iter)) == verts[last].end()
	//					&& verts[now].find(&(*iter)) == verts[now].end())
	//				{
	//					verts[next].insert(&(*iter));
	//					m_objSelected.insert(&(*iter));
	//					m_vertSelected.insert(&(*iter));
	//				}
	//				++iter;
	//			} while (iter != pVert->endVert());
	//		}
	//		printf("%d %d %d\n", verts[last].size(), verts[now].size(), verts[next].size());
	//		OnDraw();
	//		now = (now + 1) % 3;
	//		last = (now + 2) % 3;
	//		next = (now + 1) % 3;
	//		verts[next].clear();
	//	}
	//}
}

void World::OnSpecialKeyDown(int key, int modifiers)
{
	HEVert* tempVert;
	vector<HEVert*> verts;
	vector<HEFace*> faces;
	switch (key)
	{
	case GLUT_KEY_F3:
		Parameters::bLine = !Parameters::bLine;
		break;
	case GLUT_KEY_UP:
		Parameters::fMagnification *= 1.1;
		break;
	case GLUT_KEY_DOWN:
		Parameters::fMagnification *= 0.9;
		break;
	case GLUT_KEY_INSERT:
		m_vertSelected.clear();
		for (set<HEObject*>::iterator iter = m_objSelected.begin(); iter != m_objSelected.end(); ++iter)
		{
			// alt not down
			if ((modifiers & GLUT_ACTIVE_ALT) == 0)
			{
				tempVert = (*iter)->InsertVertex(faces);
				if (tempVert != NULL)
				{
					m_vertSelected.insert(tempVert);
					AddVert(tempVert);
				}
				if (faces.size() != 0)
					AppendFaces(faces);
			}
			// alt down
			else
			{
				HEFace* face;
				if ((face = dynamic_cast<HEFace*>(*iter)) != NULL)
					face->InsertVertices(verts, faces);
				for(vector<HEVert*>::size_type nIndex = 0; nIndex < verts.size(); ++nIndex)
					m_vertSelected.insert(verts[nIndex]);
				AppendVerts(verts);
				if (faces.size() != 0)
					AppendFaces(faces);
			}
		}
		m_objSelected.clear();
		for (set<HEVert*>::iterator iter = m_vertSelected.begin(); iter != m_vertSelected.end(); ++iter)
			m_objSelected.insert(*iter);
		RecalculateSum();
		break;
	default:
		break;
	}
}

void World::OnKeyDown(unsigned char key, int modifiers)
{
	Vector vForward = m_Center - m_Eye;
	Vector vRight = vForward.OuterProduct(GetUp());
	set<HEObject*> deletedObjects;
	switch (key)
	{
	case 27:
		exit(0);
	case 'w':
		m_Eye += 0.01 / vForward.Module() * vForward;
		m_Center += 0.01 / vForward.Module() * vForward;
		break;
	case 's':
		m_Eye -= 0.01 / vForward.Module() * vForward;
		m_Center -= 0.01 / vForward.Module() * vForward;
		break;
	case 'a':
		m_Eye -= 0.01 / vRight.Module() * vRight;
		m_Center -= 0.01 / vRight.Module() * vRight;
		break;
	case 'd':
		m_Eye += 0.01 / vRight.Module() * vRight;
		m_Center += 0.01 / vRight.Module() * vRight;
		break;
	case 'g':
		OnMouseDrag(Parameters::fRevisedMaginification(),0);
		break;
	case 'h':
		OnMouseDrag(Parameters::fRevisedMaginification(), 1);
		break;
	case 'j':
		OnMouseDrag(Parameters::fRevisedMaginification(), 2);
		break;
	case 't':
		OnMouseDrag(- Parameters::fRevisedMaginification(), 0);
		break;
	case 'y':
		OnMouseDrag(- Parameters::fRevisedMaginification(), 1);
		break;
	case 'u':
		OnMouseDrag(- Parameters::fRevisedMaginification(), 2);
		break;
	case 127:
		// if select one object
		if (m_objSelected.size() == 1)
			if ((modifiers & GLUT_ACTIVE_ALT) == 0)
				(*m_objSelected.begin())->Delete(deletedObjects);
			else
			{
				HEEdge* edge;
				if ((edge = dynamic_cast<HEEdge*>(*m_objSelected.begin())) != NULL)
					edge->DeleteWithoutMove(deletedObjects);
			}
		// if multipule objects selected, only deal with faces
		else
			for (set<HEObject*>::iterator iter = m_objSelected.begin(); iter != m_objSelected.end(); ++iter)
			{
				HEFace* face;
				if ((face = dynamic_cast<HEFace*>(*iter)) != NULL)
				{
					face->Delete(deletedObjects);
				}
			}
		m_objSelected.clear();
		m_vertSelected.clear();
		DeleteObjects(deletedObjects);
		break;
	case 'x':
		Parameters::status = (Parameters::states)((Parameters::status + 1) % 3);
	default:
		break;
	}
	RecalculateSum();
}

void World::OnMouseDrag(GLdouble scale, int dir)
{
	GLdouble angle = scale / 10 * 2 * Parameters::PI;
	Point pCenter(m_SelectSum / m_vertSelected.size());
	Vector bases[3] =
	{
		Vector(1.0, 0.0, 0.0),
		Vector(0.0, 1.0, 0.0),
		Vector(0.0, 0.0, 1.0),
	};
	GLdouble Rotation[4][4] =
	{
		{ 1.0, 0.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 1.0, 0.0 },
	};
	switch (dir)
	{
	case 0:
		Rotation[1][1] = cos(angle); Rotation[1][2] = - sin(angle);
		Rotation[2][1] = sin(angle); Rotation[2][2] = cos(angle);
		break;
	case 1:
		Rotation[0][0] = cos(angle); Rotation[0][2] = - sin(angle);
		Rotation[2][0] = sin(angle); Rotation[2][2] = cos(angle);
		break;
	case 2:
		Rotation[0][0] = cos(angle); Rotation[0][1] = - sin(angle);
		Rotation[1][0] = sin(angle); Rotation[1][1] = cos(angle);
		break;
	default:
		assert(false);
		break;
	}
	assert(dir >=0 && dir <= 2);
	switch (Parameters::status)
	{
	case Parameters::TRANSLATE:
		for (set<HEVert*>::iterator iter = m_vertSelected.begin(); iter != m_vertSelected.end(); ++iter)
			**iter += scale * bases[dir];
		break;
	case Parameters::SCALE:
		for (set<HEVert*>::iterator iter = m_vertSelected.begin(); iter != m_vertSelected.end(); ++iter)
			**iter = ((*iter)->m_vert - pCenter).VectorProduct((exp(scale) - 1) * bases[dir] + Vector(1.0, 1.0, 1.0)) + pCenter;
		break;
	case Parameters::ROTATE:
		for (set<HEVert*>::iterator iter = m_vertSelected.begin(); iter != m_vertSelected.end(); ++iter)
			**iter = Transform(Rotation, (*iter)->m_vert - pCenter) + pCenter;
		break;
	default:
		assert(false);
		break;
	}
	RecalculateSum();
}

void World::DeleteObjects(set<HEObject*>& deletedObjects)
{
	HEFace* pFace;
	for (set<HEObject*>::iterator iter = deletedObjects.begin(); iter != deletedObjects.end(); ++iter)
	{
		if ((pFace = dynamic_cast<HEFace*>(*iter)) != NULL)
			m_Faces.erase(find(m_Faces.begin(), m_Faces.end(), pFace));
		delete *iter;
	}
}