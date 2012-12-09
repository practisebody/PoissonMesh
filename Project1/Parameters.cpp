#include "stdafx.h"
#include "Parameters.h"

const GLdouble Parameters::PI = 3.141592653589793238;
const int Parameters::MAX_LINE_LENGTH = 1000;
const int Parameters::MAX_PREFIX_LENGTH = 10;
const GLdouble Parameters::zNear = 0.01;
const GLdouble Parameters::zFar = 1000;
GLfloat Parameters::fMagnification = 1.0f;
bool Parameters::bLine = false;
GLfloat& Parameters::fIntersectRadius = fMagnification;
Parameters::states Parameters::status = TRANSLATE;

GLfloat Parameters::fRevisedMaginification()
{
	return log(fMagnification + 1) + 1;
}