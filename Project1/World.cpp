#include "stdafx.h"
#include "World.h"
#include "HalfEdge.h"
#include <algorithm>

World::World(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz,
			 GLdouble upx, GLdouble upy, GLdouble upz)
	: m_Eye(eyex, eyey, eyez), m_Center(centerx, centery, centerz), m_Up(upx, upy, upz), m_DragDir(DIR_NULL), 
	lastx(Parameters::nWindowWidth / 2), lasty(Parameters::nWindowHeight / 2)
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

void World::OnOrient()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, (GLdouble)Parameters::nWindowWidth / Parameters::nWindowHeight, Parameters::zNear, Parameters::zFar);
	gluLookAt(m_Eye[0], m_Eye[1], m_Eye[2], m_Center[0], m_Center[1], m_Center[2],
		m_Up[0], m_Up[1], m_Up[2]);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void World::OnDraw(GLenum mode)
{
	Vector vForward = (m_Center - m_Eye).Normalize();
	Vector vRight = vForward.OuterProduct(GetUp());
	Point Left = (m_Eye + 2 * Parameters::zNear * vForward - vRight / 2000);
	Point Right = (m_Eye + 2 * Parameters::zNear * vForward + vRight / 2000);
	Point Up = (m_Eye + 2 * Parameters::zNear * vForward + GetUp() / 2000);
	Point Down = (m_Eye + 2 * Parameters::zNear * vForward - GetUp() / 2000);

	Material::SetMaterialWhite();
	LineWidth::DrawNormal();

	// Draw axes
	glBegin(GL_LINES);
		glVertex3d(m_Eye[0] - 100.0, 0.0, 0.0);
		glVertex3d(m_Eye[0] + 100.0, 0.0, 0.0);
		glVertex3d(0.0, m_Eye[1] - 100.0, 0.0);
		glVertex3d(0.0, m_Eye[1] + 100.0, 0.0);
		glVertex3d(0.0, 0.0, m_Eye[2] - 100.0);
		glVertex3d(0.0, 0.0, m_Eye[2] + 100.0);
	glEnd();

	// Draw cursor
	glBegin(GL_LINES);
		glVertex3d(Left[0], Left[1], Left[2]);
		glVertex3d(Right[0], Right[1], Right[2]);
		glVertex3d(Up[0], Up[1], Up[2]);
		glVertex3d(Down[0], Down[1], Down[2]);
	glEnd();

	// Draw Objects
	glPushMatrix();
	glScalef(Parameters::fMagnification, Parameters::fMagnification, Parameters::fMagnification);

	vector<HEFace*> TransparentFaces;
	for (vector<HEFace*>::size_type nIndex = 0; nIndex < m_Faces.size(); ++ nIndex)
	{
		if (mode == GL_SELECT)
			glLoadName(nIndex);
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
	Vector _scale = m_vSelectedMax - m_vSelectedMin;
	Material::SetMaterialWhite();
	if (m_objSelected.size() <= Parameters::nMaxDrawNumber || _scale.Module() <= Parameters::fMaxDrawSize)
	{
		if (m_objSelected.empty() == false)
			for (set<HEObject*>::iterator iter = m_objSelected.begin(); iter != m_objSelected.end(); ++iter)
				(*iter)->DrawSelected();
	}
	else
		Utility::DrawCube(m_vSelectedMin, _scale);
	switch (Parameters::status)
	{
	case States::TRANSLATE:
		Utility::DrawTranslateAxis(m_SelectSum / m_vertSelected.size());
		break;
	case States::SCALE:
		Utility::DrawScaleAxis(m_SelectSum / m_vertSelected.size());
		break;
	case States::ROTATE:
		Utility::DrawRotateAxis(m_SelectSum / m_vertSelected.size());
		break;
	default:
		break;
	}
	glPopMatrix();
	LineWidth::PopLineWidth();
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

void World::ReCalculateSum()
{
	new (&m_SelectSum) Point(0.0, 0.0, 0.0);
	if (m_vertSelected.empty() == false)
	{
		m_vSelectedMax = Utility::minPoint;
		m_vSelectedMin = Utility::maxPoint;
		for (set<HEVert*>::iterator vertIter = m_vertSelected.begin(); vertIter != m_vertSelected.end(); ++vertIter)
		{
			Utility::SetMax(m_vSelectedMax, (*vertIter)->m_vert);
			Utility::SetMin(m_vSelectedMin, (*vertIter)->m_vert);
			m_SelectSum += (Vector)(*vertIter)->m_vert;
		}
		Parameters::fAxisLength = (m_vSelectedMax - m_vSelectedMin).Module() / 3 + Parameters::fRevisedMaginification();
	}
}

void World::FixingSelectedVetices()
{
	vector<HEVert*> verts;
	m_vertSelected.clear();
	for (set<HEObject*>::iterator iter = m_objSelected.begin(); iter != m_objSelected.end(); ++iter)
	{
		(*iter)->ToVerts(verts);
		for (vector<HEVert*>::iterator vertIter = verts.begin(); vertIter != verts.end(); ++vertIter)
		{
			m_vertSelected.insert(*vertIter);
		}
	}
}

void World::OnMouseClick(int button, int state, int modifiers)
{
	vector<HEVert*>::iterator vertIter;
	HEObject* obj = GetNearestObject();
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		switch (state)
		{
		case GLUT_DOWN:
			m_DragDir = DIR_NULL;
			break;
		case GLUT_UP:
			// if it's a selection
			if (m_DragDir == DIR_NULL)
			{
				// If Ctrl Not Down
				if ((modifiers & GLUT_ACTIVE_CTRL) == 0)
				{
					m_objSelected.clear();
					//m_vertSelected.clear();
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
						//	obj->ToVerts(verts);
						//	for (vertIter = verts.begin(); vertIter != verts.end(); ++vertIter)
						//	{
						//		m_vertSelected.insert(*vertIter);
						//	}
						}
						// if selected
						else
						{
							m_objSelected.erase(iter);
							//m_vertSelected.clear();
							//for (iter = m_objSelected.begin(); iter != m_objSelected.end(); ++iter)
							//{
							//	(*iter)->ToVerts(verts);
							//	for (vertIter = verts.begin(); vertIter != verts.end(); ++vertIter)
							//	{
							//		m_vertSelected.insert(*vertIter);
							//	}
							//}
						}
					}
				}
			}
			FixingSelectedVetices();
			ReCalculateSum();
			m_DragDir = DIR_NULL;
			break;
		default:
			break;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		switch (state)
		{
		case GLUT_DOWN:
			Parameters::status = ((int)Parameters::status + 1) % 3;
			break;
		case GLUT_UP:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
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
		if (Parameters::bLine == true)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case GLUT_KEY_F4:
		// F4
		if ((modifiers & GLUT_ACTIVE_ALT) == 0)
		{
			Parameters::bHiddenFaceRemoval = !Parameters::bHiddenFaceRemoval;
			if (Parameters::bHiddenFaceRemoval == true)
				glEnable(GL_CULL_FACE);
			else
				glDisable(GL_CULL_FACE);
		}
		// alt + F4
		else
			exit(0);
		break;
	//case GLUT_KEY_UP:
	//	Parameters::fMagnification *= 1.1;
	//	break;
	//case GLUT_KEY_DOWN:
	//	Parameters::fMagnification *= 0.9;
	//	break;
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
		ReCalculateSum();
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
	case 'a' - 'a' + 1:
		if ((modifiers & GLUT_ACTIVE_CTRL) != 0)
		{
			for (vector<HEFace*>::iterator iter = m_Faces.begin(); iter != m_Faces.end(); ++iter)
			{
				if (dynamic_cast<HENullFace*>(*iter) == NULL)
					m_objSelected.insert(*iter);
			}
			for (vector<HEVert*>::iterator iter = ++m_Vertices.begin(); iter != m_Vertices.end(); ++iter)
				m_vertSelected.insert(*iter);
		}
		ReCalculateSum();
		break;
	case 'q' - 'a' + 1:
		if ((modifiers & GLUT_ACTIVE_CTRL) != 0)
			exit(0);
	case 27:
		m_objSelected.clear();
		m_vertSelected.clear();
		break;
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
		OnMouseDrag(Parameters::fRevisedMaginification(), (Direction)0, (Direction)0);
		break;
	case 'h':
		OnMouseDrag(Parameters::fRevisedMaginification(), (Direction)1, (Direction)1);
		break;
	case 'j':
		OnMouseDrag(Parameters::fRevisedMaginification(), (Direction)2, (Direction)2);
		break;
	case 't':
		OnMouseDrag(- Parameters::fRevisedMaginification(), (Direction)0, (Direction)0);
		break;
	case 'y':
		OnMouseDrag(- Parameters::fRevisedMaginification(), (Direction)1, (Direction)1);
		break;
	case 'u':
		OnMouseDrag(- Parameters::fRevisedMaginification(), (Direction)2, (Direction)2);
		break;
	case 127:
		// if select one object
		if (m_objSelected.size() == 1)
		{
			if ((modifiers & GLUT_ACTIVE_ALT) == 0)
				(*m_objSelected.begin())->Delete(deletedObjects);
			else
			{
				HEEdge* edge;
				if ((edge = dynamic_cast<HEEdge*>(*m_objSelected.begin())) != NULL)
					edge->DeleteWithoutMove(deletedObjects);
			}
		}
		// if multipule objects selected, only deal with faces
		else
		{
			for (set<HEObject*>::iterator iter = m_objSelected.begin(); iter != m_objSelected.end(); ++iter)
			{
				HEFace* face;
				if ((face = dynamic_cast<HEFace*>(*iter)) != NULL)
				{
					face->Delete(deletedObjects);
				}
			}
		}
		m_objSelected.clear();
		m_vertSelected.clear();
		DeleteObjects(deletedObjects);
		break;
	//case 'x':
	//	Parameters::status = ((int)Parameters::status + 1) % 3;
	default:
		break;
	}
	ReCalculateSum();
}

void World::OnMouseDrag(GLdouble scale, Direction dir, Direction mindir)
{
	GLdouble angle = scale / (Parameters::nWindowWidth / 40) * 2 * Parameters::PI;
	Point pCenter(m_SelectSum / m_vertSelected.size());
	Vector bases[3] =
	{
		Vector(1.0, 0.0, 0.0),
		Vector(0.0, 1.0, 0.0),
		Vector(0.0, 0.0, 1.0),
	};
	GLdouble Rotation[4][4];
	MYASSERT(dir >= DIR_X && dir <= DIR_Z);
	vector<GLdouble> distances;
	vector<Point> centers;
	Utility::GetNormalizedDis(pCenter, m_Faces, distances, centers);
	Vector vScale = (scale >= 0 ? scale : scale / (1 - scale)) * bases[dir] + Vector(1.0, 1.0, 1.0);
	if (Parameters::status == States::ROTATE)
		dir = mindir;
	if (m_DragDir != DIR_NULL)
		dir = m_DragDir;
	else
		m_DragDir = dir;
	switch (Parameters::status)
	{
	case States::TRANSLATE:
		for (set<HEVert*>::iterator iter = m_vertSelected.begin(); iter != m_vertSelected.end(); ++iter)
			**iter += scale * bases[dir];
		break;
	case States::SCALE:
		for (set<HEVert*>::iterator iter = m_vertSelected.begin(); iter != m_vertSelected.end(); ++iter)
			**iter = pCenter + ((*iter)->m_vert - pCenter).VectorProduct(vScale);
		for (vector<HEFace*>::size_type i = 1; i < m_Faces.size(); ++i)
		{
			GLdouble calibratedScale = distances[i] * scale;
			vScale = (calibratedScale >= 0 ? calibratedScale : calibratedScale / (1 - calibratedScale)) * bases[dir];
			HEFace::VertIterator iter = m_Faces[i]->beginVert();
			do
			{
				*iter = centers[i] + (iter->m_vert - centers[i]).VectorProduct(vScale + Vector(1.0, 1.0, 1.0));
				++iter;
			}
			while (iter != m_Faces[i]->endVert());
		}
		break;
	case States::ROTATE:
		Utility::GetRotationMatrix(mindir, angle, Rotation);
		for (set<HEVert*>::iterator iter = m_vertSelected.begin(); iter != m_vertSelected.end(); ++iter)
			**iter = pCenter + (Vector)Transform(Rotation, (*iter)->m_vert - pCenter);
		for (vector<HEFace*>::size_type i = 1; i < m_Faces.size(); ++i)
		{
			Utility::GetRotationMatrix(mindir, distances[i] * angle, Rotation);
			HEFace::VertIterator iter = m_Faces[i]->beginVert();
			do
			{
				*iter = centers[i] + (Vector)Transform(Rotation, (iter->m_vert - centers[i]));
				++iter;
			}
			while (iter != m_Faces[i]->endVert());
		}
		if (m_DragDir == DIR_NULL)
			m_DragDir = (Direction)mindir;
		break;
	default:
		MYASSERT(false);
		break;
	}
	ReCalculateSum();
}

int World::SizeOfFaces()
{
	return m_Faces.size();
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