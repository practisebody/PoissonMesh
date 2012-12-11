#pragma once

#include <assert.h>
#define MYDEBUG

#ifdef DEBUG
#define MYASSERT(_Expression) assert(_Expression)
#else
#define MYASSERT(_Expression) /* _Expression */
#endif

#ifdef MYDEBUG
#define MYTEMP 
#else
#define MYTEMP d2m3i9n9n4un4989jf023kjk1
#endif

// stl
#include <vector>
#include <set>
#include <map>
#include <string>
using namespace std;

// bmp
#include <Windows.h>
// 3ds
//#include <ddraw.h>

// openGL
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glew.h>
#include <gl/glut.h>