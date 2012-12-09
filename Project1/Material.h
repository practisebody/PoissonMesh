#pragma once

#include "stdafx.h"

#define MATERTIAL_RED 1.0f, 0.0f, 0.0f
#define MATERTIAL_GREEN 0.0f, 1.0f, 0.0f
#define MATERTIAL_BLUE 0.0f, 0.0f, 1.0f

struct Material
{
	Material();
	static void SetMaterialWhite();
	static void SetColorRGB(GLfloat r, GLfloat g, GLfloat b);
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