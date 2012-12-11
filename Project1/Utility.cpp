#include "stdafx.h"
#include "Utility.h"

const Point Utility::minPoint = Point(DBL_MIN, DBL_MIN, DBL_MIN);
const Point Utility::maxPoint = Point(DBL_MAX, DBL_MAX, DBL_MAX);

void Utility::DrawBasicAxis(const Point& here)
{
	LineWidth::DrawAxis();
	glBegin(GL_LINES);
		Material::SetColorRGB(MATERTIAL_RED);
		glVertex3d(here[0], here[1], here[2]);
		glVertex3d(here[0] + Parameters::fAxisLength * 1.2, here[1], here[2]);
		Material::SetColorRGB(MATERTIAL_GREEN);
		glVertex3d(here[0], here[1], here[2]);
		glVertex3d(here[0], here[1] + Parameters::fAxisLength * 1.2, here[2]);
		Material::SetColorRGB(MATERTIAL_BLUE);
		glVertex3d(here[0], here[1], here[2]);
		glVertex3d(here[0], here[1], here[2] + Parameters::fAxisLength * 1.2);
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
		glVertex3d(here[0] + Parameters::fAxisLength * 0.5, here[1], here[2]);
		Material::SetColorRGB(MATERTIAL_GREEN);
		glVertex3d(here[0], here[1] + Parameters::fAxisLength * 0.5, here[2]);
		Material::SetColorRGB(MATERTIAL_BLUE);
		glVertex3d(here[0], here[1], here[2] + Parameters::fAxisLength * 0.5);
	glEnd();
	LineWidth::PopLineWidth();
}

void Utility::DrawRotateAxis(const Point& here)
{
	DrawBasicAxis(here);
	LineWidth::DrawAxis();
	glPushMatrix();
	Translated(here);
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
		Translated(center);
		Scaled(1.2 * scale);
		glutWireCube(1.0);
	glPopMatrix();
	LineWidth::PopLineWidth();
}

void Utility::Translated(const _Triple& triple)
{
	glTranslated(triple[0], triple[1], triple[2]);
}

void Utility::Scaled(const _Triple& triple)
{
	glScaled(triple[0], triple[1], triple[2]);
}

void Utility::SetMin(Point& a, const Point& b)
{
	for (int i = 0; i < 3; ++i)
		SetMin(a[i], b[i]);
}

void Utility::SetMax(Point& a, const Point& b)
{
	for (int i = 0; i < 3; ++i)
		SetMax(a[i], b[i]);
}