

#pragma once


#ifdef __USE_DIRECTDRAW__
//#undef __USE_DIRECTDRAW__
#endif


//���������ע����Ч�Ա���DirectDraw
//#define __USE_DIRECTDRAW_


#include "Memory.h"

#include <windows.h>

#include "Geometry.h"

#include "typedef.h"

#ifdef __USE_DIRECTDRAW__
#include <ddraw.h>
#pragma comment(lib,"ddraw.lib")
#endif

#pragma comment(lib,"winmm.lib")
#pragma comment (lib,"MSIMG32.LIB")







