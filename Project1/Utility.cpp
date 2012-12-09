#include "stdafx.h"
#include "Utility.h"

void Utility::DrawTranslateAxis(Point point)
{
	glLineWidth(5.0f);
	glBegin(GL_LINES);
		Material::SetColorRGB(MATERTIAL_RED);
		glVertex3d(point.m_x, point.m_y, point.m_z);
		glVertex3d(point.m_x + Parameters::fRevisedMaginification() * 10, point.m_y, point.m_z);
		Material::SetColorRGB(MATERTIAL_GREEN);
		glVertex3d(point.m_x, point.m_y, point.m_z);
		glVertex3d(point.m_x, point.m_y + Parameters::fRevisedMaginification() * 10, point.m_z);
		Material::SetColorRGB(MATERTIAL_BLUE);
		glVertex3d(point.m_x, point.m_y, point.m_z);
		glVertex3d(point.m_x, point.m_y, point.m_z + Parameters::fRevisedMaginification() * 10);
	glEnd();
	glLineWidth(1.0f);
}

void Utility::DrawScaleAxis(Point point)
{
	Material::SetMaterialWhite();
	glLineWidth(5.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3d(point.m_x + Parameters::fRevisedMaginification() * 10, point.m_y, point.m_z);
		glVertex3d(point.m_x, point.m_y + Parameters::fRevisedMaginification() * 10, point.m_z);
		glVertex3d(point.m_x, point.m_y, point.m_z + Parameters::fRevisedMaginification() * 10);
	glEnd();
	glLineWidth(1.0f);
}

void Utility::DrawRotateAxis(Point point)
{
	glLineWidth(5.0f);
	glPushMatrix();
	glPushMatrix();
		DrawCircle(Parameters::fRevisedMaginification() * 10, 360);
	glPopMatrix();
	glPushMatrix();
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		DrawCircle(Parameters::fRevisedMaginification() * 10, 360);
	glPopMatrix();
	glPushMatrix();
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		DrawCircle(Parameters::fRevisedMaginification() * 10, 360);
	glPopMatrix();
	glPopMatrix();
	glLineWidth(1.0f);
}

void Utility::DrawCircle(GLdouble R, int n)
{
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < n; ++i)
		glVertex2f(R * cos(2*Parameters::PI / n * i), R * sin(2 * Parameters::PI / n * i));
	glEnd();
}