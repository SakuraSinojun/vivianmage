

#pragma once


#include "Geometry.h"



typedef struct tagBKGNDPARAMETER
{	
	BOOL		b4pics;
	CPoint		point;
	LPCSTR		part_topleft;
	LPCSTR		part_topright;
	LPCSTR		part_bottomleft;
	LPCSTR		part_bottomright;
#define file1 part_topleft
#define file2 part_topright
#define file3 part_bottomleft
#define file4 part_bottomright
}BKGNDPARAMETER,*PBKGNDPARAMETER;





#define GAME_WINDOW_WIDTH	640
#define GAME_WINDOW_HEIGHT	480




#define TRACE(quote) MessageBox(NULL,TEXT(quote),TEXT(""),MB_OK);exit(0);




