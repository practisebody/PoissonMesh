#include "stdafx.h"
#include "Utility.h"
#include "HEFace.h"

const Point Utility::minPoint = Point(DBL_MIN, DBL_MIN, DBL_MIN);
const Point Utility::maxPoint = Point(DBL_MAX, DBL_MAX, DBL_MAX);
map<int, string> Utility::HelpInfo;

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

void Utility::Print(const char* string)
{
	for (const char* c = string; *c != '\0';)
	{
		for (; *c != '\n' && *c != '\0'; ++c)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		if (*c == '\n')
		{
			PRINTINFOLEFTUPMOVENEXT;
			++c;
		}
	}
}

void Utility::InitHelp()
{
	string strShift = "Move Mode\n";
	string strCtrl = "Ctrl + Q : Exit\nCtrl + A : Select All\n";
	string strAlt = "Alt + F4 : Exit\n";
	string strNull = "Broswing Mode\n";
	// Object Selected
	{
		// shift + ctrl + alt
		HelpInfo.insert(pair<int, string>( ANYOBJECTSELECTED | GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT,
			""));
		// shift + ctrl
		HelpInfo.insert(pair<int, string>( ANYOBJECTSELECTED | GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_CTRL | NOT(GLUT_ACTIVE_ALT),
			""));
		// shift + alt
		HelpInfo.insert(pair<int, string>( ANYOBJECTSELECTED | GLUT_ACTIVE_SHIFT | NOT(GLUT_ACTIVE_CTRL) | GLUT_ACTIVE_ALT,
			""));
		// shift
		HelpInfo.insert(pair<int, string>( ANYOBJECTSELECTED | GLUT_ACTIVE_SHIFT | NOT(GLUT_ACTIVE_CTRL) | NOT(GLUT_ACTIVE_ALT),
			strShift + ""));
		// ctrl + alt
		HelpInfo.insert(pair<int, string>( ANYOBJECTSELECTED | NOT(GLUT_ACTIVE_SHIFT) | GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT,
			"Scaling Mode"));
		//  ctrl
		HelpInfo.insert(pair<int, string>( ANYOBJECTSELECTED | NOT(GLUT_ACTIVE_SHIFT) | GLUT_ACTIVE_CTRL | NOT(GLUT_ACTIVE_ALT),
			strCtrl + "Multiple Selection Mode"));
		// alt
		HelpInfo.insert(pair<int, string>( ANYOBJECTSELECTED | NOT(GLUT_ACTIVE_SHIFT) | NOT(GLUT_ACTIVE_CTRL) | GLUT_ACTIVE_ALT,
			strAlt + "Special InSert/Delete Mode\nFocus Rotation Mode"));
		// no key
		HelpInfo.insert(pair<int, string>( ANYOBJECTSELECTED | NOT(GLUT_ACTIVE_SHIFT) | NOT(GLUT_ACTIVE_CTRL) | NOT(GLUT_ACTIVE_ALT),
			strNull + ""));
	}

	// No Object Selected
	{
		// shift + ctrl + alt
		HelpInfo.insert(pair<int, string>( NOT(ANYOBJECTSELECTED) | GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT,
			""));
		// shift + ctrl
		HelpInfo.insert(pair<int, string>( NOT(ANYOBJECTSELECTED) | GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_CTRL | NOT(GLUT_ACTIVE_ALT),
			""));
		// shift + alt
		HelpInfo.insert(pair<int, string>( NOT(ANYOBJECTSELECTED) | GLUT_ACTIVE_SHIFT | NOT(GLUT_ACTIVE_CTRL) | GLUT_ACTIVE_ALT,
			""));
		// shift
		HelpInfo.insert(pair<int, string>( NOT(ANYOBJECTSELECTED) | GLUT_ACTIVE_SHIFT | NOT(GLUT_ACTIVE_CTRL) | NOT(GLUT_ACTIVE_ALT),
			strShift + ""));
		// ctrl + alt
		HelpInfo.insert(pair<int, string>( NOT(ANYOBJECTSELECTED) | NOT(GLUT_ACTIVE_SHIFT) | GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT,
			"Scaling Mode"));
		//  ctrl
		HelpInfo.insert(pair<int, string>( NOT(ANYOBJECTSELECTED) | NOT(GLUT_ACTIVE_SHIFT) | GLUT_ACTIVE_CTRL | NOT(GLUT_ACTIVE_ALT),
			strCtrl + "Multiple Selection Mode"));
		// alt
		HelpInfo.insert(pair<int, string>( NOT(ANYOBJECTSELECTED) | NOT(GLUT_ACTIVE_SHIFT) | NOT(GLUT_ACTIVE_CTRL) | GLUT_ACTIVE_ALT,
			strAlt));
		// no key
		HelpInfo.insert(pair<int, string>( NOT(ANYOBJECTSELECTED) | NOT(GLUT_ACTIVE_SHIFT) | NOT(GLUT_ACTIVE_CTRL) | NOT(GLUT_ACTIVE_ALT),
			strNull + ""));
	}
}

void Utility::PrintHelp(bool bSelect, bool bShift, bool bCtrl, bool bAlt)
{
	Print(HelpInfo.find(bSelect * ANYOBJECTSELECTED + bCtrl * GLUT_ACTIVE_CTRL + bAlt * GLUT_ACTIVE_ALT + bShift * GLUT_ACTIVE_SHIFT)->second.c_str());
}

void Utility::GetNormalizedDis(const Point center, const vector<HEFace*> faces, vector<GLdouble>& distances, vector<Point>& centers)
{
	GLdouble dDis, dMaxDis = 0;
	distances.push_back(0);	
	centers.push_back(Point());
	for (vector<HEFace*>::size_type i = 1; i < faces.size(); ++i)
	{
		Point pTestCenter(0, 0, 0);
		int number = 0;
		HEFace::VertIterator iter = faces[i]->beginVert();
		do
		{
			pTestCenter += (Vector)(&*iter);
			++number;
			++iter;
		}
		while (iter != faces[i]->endVert());
		pTestCenter /= number;
		centers.push_back(pTestCenter);
		dDis = (center - pTestCenter).Module();
		distances.push_back(dDis);
		Utility::SetMax<GLdouble>(dMaxDis, dDis);
	}
	for (vector<GLdouble>::size_type i = 1; i < distances.size(); ++i)
	{
		distances[i] /= dMaxDis;
	}
}

void Utility::GetRotationMatrix(const Direction dir, const GLdouble angle, GLdouble Rotation[4][4])
{
	memset(Rotation, 0, sizeof(GLdouble) * 4 * 4);
	Rotation[0][0] = Rotation[1][1] = Rotation[2][2] = 1.0l;
	switch (dir)
	{
	case DIR_X:
		Rotation[1][1] = cos(angle); Rotation[1][2] = - sin(angle);
		Rotation[2][1] = sin(angle); Rotation[2][2] = cos(angle);
		break;
	case DIR_Y:
		Rotation[0][0] = cos(angle); Rotation[0][2] = - sin(angle);
		Rotation[2][0] = sin(angle); Rotation[2][2] = cos(angle);
		break;
	case DIR_Z:
		Rotation[0][0] = cos(angle); Rotation[0][1] = - sin(angle);
		Rotation[1][0] = sin(angle); Rotation[1][1] = cos(angle);
		break;
	default:
		MYASSERT(false);
		break;
	}
}