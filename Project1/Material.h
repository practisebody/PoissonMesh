#pragma once

#include "stdafx.h"

struct Material
{
	Material();
	static void SetMaterialWhite();
	static void SetMaterialDefault();
	void SetMaterial();
	bool IsTransparent();
public:
	GLfloat Ka[3], Kd[3], Ks[3], Ns, d;
	GLuint Kd_map;
	static map<string, GLuint> m_Textures;
private:
	static const GLfloat Material_White[4];
	static const GLfloat Material_White_Shininess[1];
	static const GLfloat Default_Ambient[4];
	static const GLfloat Default_Diffuse[4];
	static const GLfloat Default_Specular[4];
	static const GLfloat Default_Shininess[1];
	static const GLfloat Default_Emission[4];
	static Material* lastMaterial;
};