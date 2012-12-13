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

#define ACCESSDENYVOID {}
#define ACCESSDENYDOUBLE { return 0.0; }
#define ACCESSDENYPOINT { return Point(); }
#define ACCESSDENYVECTOR { return Vector(); }

#define PRINTINFOLEFTUPMOVENEXT glRasterPos2i(Parameters::nMargin, Parameters::nWindowHeight - ++Parameters::nCount * Parameters::nMargin)
#define INITPRINTINFOLEFTUP Parameters::nCount = 0; PRINTINFOLEFTUPMOVENEXT

#define ANYOBJECTSELECTED 8
#define NOT(_bool) 0