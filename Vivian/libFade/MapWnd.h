#pragma once
#include "gamewnd.h"

class CMapWnd 
{
public:
	CMapWnd(void);
	~CMapWnd(void);

	void Create(const char * file1,const char * file2,const char * file3,const char * file4);
	void ShowWindow(bool show);

	void SetMap(int index,const char * filename);

	void SetSplitPoint(CPoint point);
	CPoint GetSplitPoint();

private:
	CGameWnd map[4];
	CPoint point;

};
