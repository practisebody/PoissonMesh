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