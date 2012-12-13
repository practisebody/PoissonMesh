#pragma once

#include "stdafx.h"
#include "obj.h"
#include "Object.h"

class World
{
public:
	World(GLdouble eyex, GLdouble eyey, GLdouble eyez,
		GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz);
	~World();
	void Init(const char* name);
	Vector GetUp();
	void SetUp(const Vector& up);
	void AddVert(HEVert* const vert);
	void AppendVerts(vector<HEVert*>& verts);
	void AddFace(HEFace* const face);
	void AppendFaces(vector<HEFace*>& faces);
	void OnOrient();
	void OnDraw(GLenum mode = GL_RENDER);
	void ReCalculateSum();
	void FixingSelectedVetices();
	void OnMouseClick(int button, int state, int modifiers);
	void OnSpecialKeyDown(int key, int modifiers);
	void OnKeyDown(unsigned char key, int modifiers);
	void OnMouseDrag(GLdouble scale, Direction dir, Direction mindir);
	int SizeOfFaces();
public:
	Point m_Eye, m_Center;
	Point m_SelectSum;
	set<HEObject*> m_objSelected;
	set<HEVert*> m_vertSelected;
	Point m_vSelectedMax, m_vSelectedMin;
	int lastx, lasty;
	int dragx, dragy;
private:
	void DeleteObjects(set<HEObject*>& deletedObjects);
	HEObject* GetNearestObject();
	Vector m_Up;
	map<string, Material*> m_Materials;
	vector<HEVert*> m_Vertices;   // read from obj, start from 1
public:
	vector<HEFace*> m_Faces;
	vector<Vector*> m_NormVectors;
	vector<Point*> m_TextPoints;  // read from obj, start from 1
	vector<GLuint> m_Textures;
	Direction m_DragDir;
};