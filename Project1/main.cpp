#include "stdafx.h"
#include "World.h"

World theWorld(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 
			   0.5, 0.5, 0.5, 0.0, 0.0, 0.0, -0.5, -0.5, 1.0);

void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(buffer,"FPS:%4.2f",
			frame*1000.0/(time-timebase));
		timebase = time;		
		frame = 0;
	}

	//glutSetWindowTitle(buffer);
	char *c;
	glMatrixMode(GL_PROJECTION);// 选择投影矩阵
	glPushMatrix();// 保存原矩阵
	glLoadIdentity();// 装入单位矩阵
	glOrtho(0,480,0,480,-1,1);// 位置正投影
	glMatrixMode(GL_MODELVIEW);// 选择Modelview矩阵
	glPushMatrix();// 保存原矩阵
	glLoadIdentity();// 装入单位矩阵*/
	glRasterPos2f(10,10);
	for (c=buffer; *c != '\0'; c++) {		
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);// 选择投影矩阵
	glPopMatrix();// 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);// 选择Modelview矩阵
	glPopMatrix();// 重置为原保存矩阵	
}

void Init(int argc, char** argv)
{
	// Light
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	GLfloat position[4] = { 10, 0, 0, 1 };
	GLfloat ambient[4] = { 0.1, 0.1, 0.1, 1 };
	GLfloat diffuse[4] = { 0.2, 0.2, 0.2, 1 };
	GLfloat specular[4] = { 0.0, 0.0, 0.0, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT , ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE , diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);

	// Hidden Surface Removal
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Depth Test
	glEnable(GL_DEPTH_TEST);

	// Texture
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Color(Material)
	// glEnable(GL_COLOR_MATERIAL);
	// glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Blend(Alpha)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Cursor
	glutSetCursor(GLUT_CURSOR_NONE);

	glutPositionWindow(0, 0);

	// Read obj
	if (argc == 1)
		theWorld.Init("dog.obj");
	else
		theWorld.Init(argv[1]);
}

void OnKeyDown(unsigned char key, int x, int y)
{
	theWorld.OnKeyDown(key, x, y);
	glutPostRedisplay();
}

void OnSpecialKeyDown(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F3:
		Parameters::bLine = !Parameters::bLine;
		break;
	case GLUT_KEY_UP :
		Parameters::fMagnification *= 1.1;
		break;
	case GLUT_KEY_DOWN :
		Parameters::fMagnification *= 0.9;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void OnMouseClick(int button, int state, int x, int y)
{
	switch (state)
	{
	case GLUT_DOWN :
		theWorld.OnMouseClick(glutGetModifiers());
	case GLUT_UP :
		//theWorld.m_objSelected = NULL;
		break;
	}
}

void FixMouse(int& x, int& y)
{
	if (x < theWorld.m_WindowWidth / 4)
	{
		theWorld.lastx += theWorld.m_WindowWidth / 4;
		x += theWorld.m_WindowWidth / 4;
		SetCursorPos(x, y);
	}
	if (x > theWorld.m_WindowWidth / 4 * 3)
	{
		theWorld.lastx -= theWorld.m_WindowWidth / 4;
		x -= theWorld.m_WindowWidth / 4;
		SetCursorPos(x, y);
	}
	if (y < theWorld.m_WindowHeight / 4)
	{
		theWorld.lasty += theWorld.m_WindowHeight / 4;
		y += theWorld.m_WindowHeight / 4;
		SetCursorPos(x, y);
	}
	if (y > theWorld.m_WindowHeight / 4 * 3)
	{
		theWorld.lasty -= theWorld.m_WindowHeight / 4;
		y -= theWorld.m_WindowHeight / 4;
		SetCursorPos(x, y);
	}
}

void OnMouseMove(int x, int y)
{
	FixMouse(x, y);
	GLdouble HorizontalAngle = (GLdouble)(theWorld.lastx - x) / theWorld.m_WindowWidth * 2 * Parameters::PI;
	GLdouble VerticalAngle = (GLdouble)(theWorld.lasty - y) / theWorld.m_WindowHeight * 2 * Parameters::PI;
	Vector vForward = (theWorld.m_Center - theWorld.m_Eye);
	Vector vRight = vForward.OuterProduct(theWorld.GetUp());
	Vector u = vRight.Normalize();
	Vector v = vForward.Normalize();
	Vector n = theWorld.GetUp();
	GLdouble WorldtoView[3][4] =
	{
		{ u.m_x, u.m_y, u.m_z, -(u * theWorld.m_Eye) },
		{ v.m_x, v.m_y, v.m_z, -(v * theWorld.m_Eye) },
		{ n.m_x, n.m_y, n.m_z, -(n * theWorld.m_Eye) },
	};
	GLdouble HorizontalRotation[4][4] =
	{
		{ cos(HorizontalAngle), -sin(HorizontalAngle), 0, 0 },
		{ sin(HorizontalAngle), cos(HorizontalAngle), 0, 0 },
		{ 0, 0, 1, 0 },
	};
	GLdouble VerticalRotation[4][4] =
	{
		{ 1, 0, 0, 0 },
		{ 0, cos(VerticalAngle), -sin(VerticalAngle), 0 },
		{ 0, sin(VerticalAngle), cos(VerticalAngle), 0 },
	};
	GLdouble ViewtoWorld[4][4] =
	{
		{ u.m_x, v.m_x, n.m_x, theWorld.m_Eye.m_x },
		{ u.m_y, v.m_y, n.m_y, theWorld.m_Eye.m_y },
		{ u.m_z, v.m_z, n.m_z, theWorld.m_Eye.m_z },
	};
	theWorld.m_Center = (Transform(ViewtoWorld, Transform(HorizontalRotation,
		Transform(WorldtoView, theWorld.m_Center))));
	vForward = theWorld.m_Center - theWorld.m_Eye;
	vRight = vForward.OuterProduct(theWorld.GetUp());
	theWorld.m_Center = (Transform(ViewtoWorld, Transform(VerticalRotation,
		Transform(WorldtoView, theWorld.m_Center))));
	vForward = theWorld.m_Center - theWorld.m_Eye;
	theWorld.SetUp(-vForward.OuterProduct(vRight));
	glutPostRedisplay();
	theWorld.lastx = x;
	theWorld.lasty = y;
}

void OnMouseDrag(int x, int y)
{
	FixMouse(x, y);
	Vector vForward = (theWorld.m_Center - theWorld.m_Eye);
	if (theWorld.m_objSelected.empty())
		;
	else if (abs(x - theWorld.lastx) < 3 && abs(y - theWorld.lasty) < 3)
		return;
	else
	{
		Vector vForward = (theWorld.m_Center - theWorld.m_Eye);
		Vector vRight = vForward.OuterProduct(theWorld.GetUp());
		Vector u = vRight.Normalize();
		Vector v = vForward.Normalize();
		Vector n = theWorld.GetUp();
		GLdouble xu = u.m_x - (u * theWorld.m_Eye), xn = n.m_x - (n * theWorld.m_Eye);
		GLdouble yu = u.m_y - (u * theWorld.m_Eye), yn = n.m_y - (n * theWorld.m_Eye);
		GLdouble zu = u.m_z - (u * theWorld.m_Eye), zn = n.m_z - (n * theWorld.m_Eye);
		GLdouble cursoru = x - theWorld.lastx, cursorn = theWorld.lasty - y;
		GLdouble xscale = (xu * cursoru + xn * cursorn) / sqrt(xu * xu + xn * xn);
		GLdouble yscale = (yu * cursoru + yn * cursorn) / sqrt(yu * yu + yn * yn);
		GLdouble zscale = (zu * cursoru + zn * cursorn) / sqrt(zu * zu + zn * zn);
		if (fabs(xscale) > fabs(yscale) && fabs(xscale) > fabs(zscale))
			theWorld.OnMouseDrag(xscale, 0);
		if (fabs(yscale) > fabs(xscale) && fabs(yscale) > fabs(zscale))
			theWorld.OnMouseDrag(yscale, 1);
		if (fabs(zscale) > fabs(xscale) && fabs(zscale) > fabs(yscale))
			theWorld.OnMouseDrag(zscale, 2);
	}
	OnMouseMove(x, y);
}

void OnDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Polygon
	if (Parameters::bLine == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	theWorld.Orient();
	theWorld.Draw();
	Material::SetMaterialWhite();
	getFPS();

	glFlush();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(theWorld.m_WindowWidth, theWorld.m_WindowHeight);
	glutCreateWindow("Project1");

	Init(argc, argv);
	glutKeyboardFunc(OnKeyDown);
	glutSpecialFunc(OnSpecialKeyDown);
	glutMouseFunc(OnMouseClick);
	glutMotionFunc(OnMouseDrag);
	glutPassiveMotionFunc(OnMouseMove);
	glutDisplayFunc(OnDraw);
	glutMainLoop();
}