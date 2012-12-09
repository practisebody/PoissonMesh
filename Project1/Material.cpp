#include "stdafx.h"
#include "Material.h"

map<string, GLuint> Material::m_Textures;
const GLfloat Material::Material_White[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat Material::Material_White_Shininess[1] = { 0.0f };
const GLfloat Material::Default_Ambient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat Material::Default_Diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat Material::Default_Specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat Material::Default_Shininess[1] = { 0.0f };
const GLfloat Material::Default_Emission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
Material* Material::lastMaterial = NULL;

Material::Material()
{
	memset(this, 0, sizeof(Material));
}

void Material::SetMaterialWhite()
{
	glDisable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Material_White);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Material_White);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Material_White_Shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Material_White);
	lastMaterial = NULL;
}

void Material::SetColorRGB(GLfloat r, GLfloat g, GLfloat b)
{
	GLfloat color[4] = { r, g, b, 1 };
	glDisable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Material_White_Shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
	lastMaterial = NULL;
}

void Material::SetMaterialDefault()
{
	glDisable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Default_Ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Default_Diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Default_Specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Default_Shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Default_Emission);
	lastMaterial = NULL;
}

void Material::SetMaterial()
{
	if (lastMaterial != this)
	{
		size_t size = sizeof(GLfloat) * 3;
		GLfloat Material_Ambient[4];
		GLfloat Material_Diffuse[4];
		GLfloat Material_Specular[4];
		GLfloat Material_Shininess = Ns;
		memcpy(Material_Ambient, Ka, size);
		memcpy(Material_Diffuse, Kd, size);
		memcpy(Material_Specular, Ks, size);
		Material_Ambient[3] = Material_Diffuse[3] = Material_Specular[3] = d;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Material_Ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Material_Diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Material_Specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Material_Shininess);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Kd_map);
		lastMaterial = this;
	}
}

bool Material::IsTransparent()
{
	return (fabs(d - 1.000f) > 0.001f);
}