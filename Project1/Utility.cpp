#include "stdafx.h"
#include "Utility.h"

const Point Utility::minPoint = Point(DBL_MIN, DBL_MIN, DBL_MIN);
const Point Utility::maxPoint = Point(DBL_MAX, DBL_MAX, DBL_MAX);

void Utility::DrawBasicAxis(const Point& here)
{
	LineWidth::DrawAxis();
	glBegin(GL_LINES);
		Material::SetColorRGB(MATERTIAL_RED);
		glVertex3d(here.m_x, here.m_y, here.m_z);
		glVertex3d(here.m_x + Parameters::fAxisLength * 1.2, here.m_y, here.m_z);
		Material::SetColorRGB(MATERTIAL_GREEN);
		glVertex3d(here.m_x, here.m_y, here.m_z);
		glVertex3d(here.m_x, here.m_y + Parameters::fAxisLength * 1.2, here.m_z);
		Material::SetColorRGB(MATERTIAL_BLUE);
		glVertex3d(here.m_x, here.m_y, here.m_z);
		glVertex3d(here.m_x, here.m_y, here.m_z + Parameters::fAxisLength * 1.2);
	glEnd();
	LineWidth::PopLineWidth();
}

void Utility::DrawCircle(GLdouble R, int n)
{
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < n; ++i)
		glVertex2f(R * cos(2 * Parameters::PI / n * i), R * sin(2 * Parameters::PI / n * i));
	glEnd();
}

void Utility::DrawTranslateAxis(const Point& here)
{
	DrawBasicAxis(here);
}

void Utility::DrawScaleAxis(const Point& here)
{
	DrawBasicAxis(here);
	LineWidth::DrawAxis();
	Material::SetMaterialWhite();
	glBegin(GL_LINE_LOOP);
		Material::SetColorRGB(MATERTIAL_RED);
		glVertex3d(here.m_x + Parameters::fAxisLength * 0.5, here.m_y, here.m_z);
		Material::SetColorRGB(MATERTIAL_GREEN);
		glVertex3d(here.m_x, here.m_y + Parameters::fAxisLength * 0.5, here.m_z);
		Material::SetColorRGB(MATERTIAL_BLUE);
		glVertex3d(here.m_x, here.m_y, here.m_z + Parameters::fAxisLength * 0.5);
	glEnd();
	LineWidth::PopLineWidth();
}

void Utility::DrawRotateAxis(const Point& here)
{
	DrawBasicAxis(here);
	LineWidth::DrawAxis();
	glPushMatrix();
	glTranslated(here.m_x, here.m_y, here.m_z);
	glPushMatrix();
		Material::SetColorRGB(MATERTIAL_BLUE);
		DrawCircle(Parameters::fAxisLength * 0.9, 360);
	glPopMatrix();
	glPushMatrix();
		Material::SetColorRGB(MATERTIAL_RED);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		DrawCircle(Parameters::fAxisLength * 0.9, 360);
	glPopMatrix();
	glPushMatrix();
		Material::SetColorRGB(MATERTIAL_GREEN);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		DrawCircle(Parameters::fAxisLength * 0.9, 360);
	glPopMatrix();
	glPopMatrix();
	LineWidth::PopLineWidth();
}

void Utility::DrawCube(const Point& leftdown, const Vector& scale)
{
	Point center = leftdown + scale / 2;
	LineWidth::DrawAxis();
	glPushMatrix();
		glTranslated(center.m_x, center.m_y, center.m_z);
		glScaled(1.2 * scale.m_x, 1.2 * scale.m_y, 1.2 * scale.m_z);
		glutWireCube(1.0);
	glPopMatrix();
	LineWidth::PopLineWidth();
}

void Utility::SetMin(Point& a, const Point& b)
{
	SetMin(a.m_x, b.m_x);
	SetMin(a.m_y, b.m_y);
	SetMin(a.m_z, b.m_z);
}

void Utility::SetMax(Point& a, const Point& b)
{
	SetMax(a.m_x, b.m_x);
	SetMax(a.m_y, b.m_y);
	SetMax(a.m_z, b.m_z);
}