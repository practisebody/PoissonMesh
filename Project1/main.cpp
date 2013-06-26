#include "stdafx.h"
#include "World.h"
#include "HalfEdge.h"

World theWorld(0.5, 0.5, 0.5, 0.0, 0.0, 0.0, -0.5, -0.5, 1.0);

void PrintfInfo()
{
	static int frame = 0, time, timebase = 0;
	static char fps[1024];
	char scale[1024];
	
	Material::SetMaterialWhite();
	glMatrixMode(GL_PROJECTION);// 选择投影矩阵
	glPushMatrix();// 保存原矩阵
	glLoadIdentity();// 装入单位矩阵
	glOrtho(0, Parameters::nWindowWidth, 0, Parameters::nWindowHeight, -1, 1);// 位置正投影
	glMatrixMode(GL_MODELVIEW);// 选择Modelview矩阵
	glPushMatrix();// 保存原矩阵
	glLoadIdentity();// 装入单位矩阵*/

	// left up
	INITPRINTINFOLEFTUP;
	sprintf(scale, "Scale: %.3e x", Parameters::fMagnification);
	Utility::Print(scale);
	PRINTINFOLEFTUPMOVENEXT;
	Utility::Print("Display Mode: ");
	Utility::Print((Parameters::bLine) ? ("Line") : ("Fill"));
	PRINTINFOLEFTUPMOVENEXT;
	Utility::Print("Hidden Face Removal: ");
	Utility::Print((Parameters::bHiddenFaceRemoval) ? ("On") : ("Off"));
	PRINTINFOLEFTUPMOVENEXT;
	Utility::Print("Modify Mode:");
	Utility::Print((char*)Parameters::status);
	PRINTINFOLEFTUPMOVENEXT;
	Utility::Print(theWorld.m_objSelected.empty() == true ? "No object been selected" : "Object selected");
	PRINTINFOLEFTUPMOVENEXT;
	if (GetKeyState(VK_CONTROL) < 0)
	{
		Utility::Print("Ctrl ");
	}
	if (GetKeyState(VK_SHIFT) < 0)
	{
		Utility::Print("Shift ");
	}
	if (GetKeyState(VK_MENU) < 0)
	{
		Utility::Print("Alt ");
	}
	PRINTINFOLEFTUPMOVENEXT;
	Utility::PrintHelp(theWorld.m_objSelected.empty() == false, GetKeyState(VK_SHIFT) < 0, GetKeyState(VK_CONTROL) < 0, GetKeyState(VK_MENU) < 0);

	// left down
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000)
	{
		sprintf(fps, "FPS: %4.2f", frame * 1000.0 / (time - timebase));
		timebase = time;		
		frame = 0;
	}
	glRasterPos2i(Parameters::nMargin, Parameters::nMargin);
	Utility::Print(fps);

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

	// Blend(Alpha)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Cursor
	glutSetCursor(GLUT_CURSOR_NONE);

	glutPositionWindow(0, 0);

	// Read obj
	if (argc == 1)
		theWorld.Init("bunny.obj");
	else
		theWorld.Init(argv[1]);

	// Help
	Utility::InitHelp();
}

void OnDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Polygon
	if (Parameters::bLine == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	theWorld.OnOrient();
	theWorld.OnDraw();
	PrintfInfo();

	glutSwapBuffers();
}

void OnKeyDown(unsigned char key, int x, int y)
{
	theWorld.OnKeyDown(key, glutGetModifiers());
}
 
void OnSpecialKeyDown(int key, int x, int y)
{
	theWorld.OnSpecialKeyDown(key, glutGetModifiers());
}

void OnMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		theWorld.dragx = x;
		theWorld.dragy = y;
	}
	theWorld.OnMouseClick(button, state, glutGetModifiers());
}

void FixMouse(int& x, int& y)
{
	if (x < Parameters::nWindowWidth / 4)
	{
		theWorld.lastx += Parameters::nWindowWidth / 4;
		theWorld.dragx += Parameters::nWindowWidth / 4;
		x += Parameters::nWindowWidth / 4;
		SetCursorPos(x, y);
	}
	if (x > Parameters::nWindowWidth / 4 * 3)
	{
		theWorld.lastx -= Parameters::nWindowWidth / 4;
		theWorld.dragx -= Parameters::nWindowWidth / 4;
		x -= Parameters::nWindowWidth / 4;
		SetCursorPos(x, y);
	}
	if (y < Parameters::nWindowHeight / 4)
	{
		theWorld.lasty += Parameters::nWindowHeight / 4;
		theWorld.dragy += Parameters::nWindowWidth / 4;
		y += Parameters::nWindowHeight / 4;
		SetCursorPos(x, y);
	}
	if (y > Parameters::nWindowHeight / 4 * 3)
	{
		theWorld.lasty -= Parameters::nWindowHeight / 4;
		theWorld.dragy -= Parameters::nWindowWidth / 4;
		y -= Parameters::nWindowHeight / 4;
		SetCursorPos(x, y);
	}
}

void OnMouseMove(int x, int y)
{
	FixMouse(x, y);
	// if ctrl or alt not pressed
	if (GetKeyState(VK_CONTROL) >= 0 || GetKeyState(VK_MENU) >= 0)
	{
		GLdouble HorizontalAngle = (GLdouble)(theWorld.lastx - x) / Parameters::nWindowWidth * 2 * Parameters::PI;
		GLdouble VerticalAngle = (GLdouble)(theWorld.lasty - y) / Parameters::nWindowHeight * 2 * Parameters::PI;
		Vector vForward = (theWorld.m_Center - theWorld.m_Eye);
		Vector vRight = vForward.OuterProduct(theWorld.GetUp());
		Vector u = vRight.Normalize();
		Vector v = vForward.Normalize();
		Vector n = theWorld.GetUp();
		GLdouble WorldtoView[4][4] =
		{
			{ u[0], u[1], u[2], -(u * (Vector)theWorld.m_Eye) },
			{ v[0], v[1], v[2], -(v * (Vector)theWorld.m_Eye) },
			{ n[0], n[1], n[2], -(n * (Vector)theWorld.m_Eye) },
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
			{ u[0], v[0], n[0], theWorld.m_Eye[0] },
			{ u[1], v[1], n[1], theWorld.m_Eye[1] },
			{ u[2], v[2], n[2], theWorld.m_Eye[2] },
		};
		if (GetKeyState(VK_SHIFT) < 0)
		{
			theWorld.m_Eye -= (theWorld.lasty - y) / Parameters::fMaxDrawSize / vForward.Module() * vForward;
			theWorld.m_Center -= (theWorld.lasty - y) / Parameters::fMaxDrawSize / vForward.Module() * vForward;
			theWorld.m_Eye += (theWorld.lastx - x) / Parameters::fMaxDrawSize / vRight.Module() * vRight;
			theWorld.m_Center += (theWorld.lastx - x) / Parameters::fMaxDrawSize / vRight.Module() * vRight;
		}
		else if (GetKeyState(VK_MENU) < 0)
		{
			if (theWorld.m_objSelected.empty() == true)
				;
			// some object been selected
			else
			{
				theWorld.m_Center = Parameters::fMagnification * theWorld.m_SelectSum / theWorld.m_vertSelected.size();
				memset(WorldtoView, 0, sizeof(WorldtoView));
				WorldtoView[0][0] = WorldtoView[1][1] = WorldtoView[2][2] = 1.0;
				memset(ViewtoWorld, 0, sizeof(ViewtoWorld));
				ViewtoWorld[0][0] = ViewtoWorld[1][1] = ViewtoWorld[2][2] = 1.0;
				for (int i = 0; i < 3; ++i)
					WorldtoView[i][3] = - (ViewtoWorld[i][3] = theWorld.m_Center[i]);
				theWorld.m_Eye = Transform(ViewtoWorld, Transform(HorizontalRotation,
					Transform(WorldtoView, theWorld.m_Eye)));
				vForward = theWorld.m_Center - theWorld.m_Eye;
				vRight = vForward.OuterProduct(theWorld.GetUp());
				theWorld.m_Eye = Transform(ViewtoWorld, Transform(VerticalRotation,
					Transform(WorldtoView, theWorld.m_Eye)));
				vForward = theWorld.m_Center - theWorld.m_Eye;
				theWorld.SetUp(-vForward.OuterProduct(vRight));
			}
		}
		else
		{
			theWorld.m_Center = Transform(ViewtoWorld, Transform(HorizontalRotation,
				Transform(WorldtoView, theWorld.m_Center)));
			vForward = theWorld.m_Center - theWorld.m_Eye;
			vRight = vForward.OuterProduct(theWorld.GetUp());
			theWorld.m_Center = Transform(ViewtoWorld, Transform(VerticalRotation,
				Transform(WorldtoView, theWorld.m_Center)));
			vForward = theWorld.m_Center - theWorld.m_Eye;
			theWorld.SetUp(-vForward.OuterProduct(vRight));
		}
	}
	// if ctrl + alt pressed
	else
	{
		double scale = ((double)theWorld.lasty - y) / 1000;
		Parameters::fMagnification *= (scale <= 0 ? exp(scale) : log(scale + 1) + 1);
	}
	theWorld.lastx = x;
	theWorld.lasty = y;
}

void OnMouseDrag(int x, int y)
{
	FixMouse(x, y);
	Vector vForward = (theWorld.m_Center - theWorld.m_Eye);
	if (abs(x - theWorld.lastx) > 3 || abs(y - theWorld.lasty) > 3)
	{
		if (theWorld.m_objSelected.empty() == true || theWorld.m_DragDir == DIR_FLAG || GetKeyState(VK_CONTROL) < 0)
		{
			GLuint* pickBuffer = new GLuint[theWorld.SizeOfFaces() * 4 + 16];
			GLint nPicks, vp[4];

			glSelectBuffer(theWorld.SizeOfFaces() + 1, pickBuffer); //设置选择缓冲区
			glRenderMode(GL_SELECT); //激活选择模式
			glInitNames();   //初始化名字堆栈

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();

			glGetIntegerv(GL_VIEWPORT, vp);
			gluPickMatrix((Parameters::nWindowWidth + (x - theWorld.dragx)) / 2,
				(Parameters::nWindowHeight - (y - theWorld.dragy)) / 2,
				abs(x - theWorld.dragx),
				abs(y - theWorld.dragy), vp);
			gluPerspective(45.0, (GLdouble)Parameters::nWindowWidth / Parameters::nWindowHeight, Parameters::zNear, Parameters::zFar);
			glMatrixMode(GL_MODELVIEW);
			gluLookAt(theWorld.m_Eye[0], theWorld.m_Eye[1], theWorld.m_Eye[2],
				theWorld.m_Center[0], theWorld.m_Center[1], theWorld.m_Center[2],
				theWorld.GetUp()[0], theWorld.GetUp()[1], theWorld.GetUp()[2]);

			//theWorld.OnOrient();
			glPushName(-1);
			theWorld.OnDraw(GL_SELECT);
			glPopName();

			//恢复投影变换
			glPopMatrix();

			//获得选择集并输出
			nPicks = glRenderMode(GL_RENDER);
			HEFace* face;
			for(GLint i = 1; i < nPicks; ++i)
			{
				face = theWorld.m_Faces[pickBuffer[4 * i + 3]];
				if (dynamic_cast<HENullFace*>(face) == NULL)
					theWorld.m_objSelected.insert(dynamic_cast<HEObject*>(face));
			}
			theWorld.m_DragDir = DIR_FLAG;
			theWorld.lastx = x;
			theWorld.lasty = y;
		}
		else
		{
			// some objects selected
			Vector vForward = (theWorld.m_Center - theWorld.m_Eye);
			Vector vRight = vForward.OuterProduct(theWorld.GetUp());
			Vector u = vRight.Normalize();
			Vector v = vForward.Normalize();
			Vector n = theWorld.GetUp();
			GLdouble xu = u[0] - (u * (Vector)theWorld.m_Eye), xn = n[0] - (n * (Vector)theWorld.m_Eye);
			GLdouble yu = u[1] - (u * (Vector)theWorld.m_Eye), yn = n[1] - (n * (Vector)theWorld.m_Eye);
			GLdouble zu = u[2] - (u * (Vector)theWorld.m_Eye), zn = n[2] - (n * (Vector)theWorld.m_Eye);
			GLdouble cursoru = x - theWorld.lastx, cursorn = theWorld.lasty - y;
			GLdouble xscale = (xu * cursoru + xn * cursorn) / sqrt(xu * xu + xn * xn);
			GLdouble yscale = (yu * cursoru + yn * cursorn) / sqrt(yu * yu + yn * yn);
			GLdouble zscale = (zu * cursoru + zn * cursorn) / sqrt(zu * zu + zn * zn);
			if (fabs(xscale) > fabs(yscale) && fabs(xscale) > fabs(zscale))
				if (fabs(yscale) > fabs(zscale))
					theWorld.OnMouseDrag(xscale, DIR_X, DIR_Z);
				else
					theWorld.OnMouseDrag(xscale, DIR_X, DIR_Y);
			if (fabs(yscale) > fabs(xscale) && fabs(yscale) > fabs(zscale))
				if (fabs(xscale) > fabs(zscale))
					theWorld.OnMouseDrag(yscale, DIR_Y, DIR_Z);
				else
					theWorld.OnMouseDrag(yscale, DIR_Y, DIR_X);
			if (fabs(zscale) > fabs(xscale) && fabs(zscale) > fabs(yscale))
				if (fabs(xscale) > fabs(yscale))
					theWorld.OnMouseDrag(zscale, DIR_Z, DIR_Y);
				else
					theWorld.OnMouseDrag(zscale, DIR_Z, DIR_X);
			//OnMouseMove(x, y);
			theWorld.lastx = x;
			theWorld.lasty = y;
		}
		OnDraw();
	}
}

void OnIdle()
{
	glutPostRedisplay();
}

#include "matrix.h"

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(Parameters::nWindowWidth, Parameters::nWindowHeight);
	glutCreateWindow("Project1");
	glewInit();

	Init(argc, argv);
	glutKeyboardFunc(OnKeyDown);
	glutSpecialFunc(OnSpecialKeyDown);
	glutMouseFunc(OnMouseClick);
	glutMotionFunc(OnMouseDrag);
	glutPassiveMotionFunc(OnMouseMove);
	glutDisplayFunc(OnDraw);
	glutIdleFunc(OnIdle);
	glutMainLoop();
}