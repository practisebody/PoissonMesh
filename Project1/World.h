#pragma once

#include "stdafx.h"
#include "obj.h"
#include "Object.h"

class World
{
public:
	World(int windowwidth, int windowheight, GLdouble eyex, GLdouble eyey, GLdouble eyez,
		GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz);
	~World();
	enum states
	{
		TRANSLATE,
		SCALE,
		ROTATE,
	};
	void Init(const char* name);
	Vector GetUp();
	void SetUp(const Vector& up);
	void AddFace(HEFace* const face);
	void Orient();
	void Draw();
	void MouseClick(int modifiers);
	void OnKeyDown(unsigned char key, int x, int y);
	void OnMouseDrag(Vector dir);
public:
	const int m_WindowWidth;
	const int m_WindowHeight;
	Point m_Eye, m_Center;
	Point m_SelectSum;
	set<HEObject*> m_objSelected;
	set<HEVert*> m_vertSelected;
	int lastx, lasty;
private:
	void DeleteObjects(set<HEObject*>& deletedObjects);
	HEObject* GetNearestObject();
	Vector m_Up;
	map<string, Material*> m_Materials;
	vector<HEVert*> m_Vertices;   // read from obj, start from 1
	vector<HEFace*> m_Faces;
	vector<Vector*> m_NormVectors;
	vector<Point*> m_TextPoints;  // read from obj, start from 1
	vector<GLuint> m_Textures;
	enum states status;
};