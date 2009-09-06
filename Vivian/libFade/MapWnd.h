//////////////////////////////////////////////////////////////////
//
//	FileName	:	MapWnd.h
//	Author		:	SakuraSinojun
//	Description	:	this class is used to draw a joint map spliced by 4 pictures
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////




#pragma once
#include "gamewnd.h"

class CMapWnd //: public CGameWnd
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
	void Create(int width,int height){}
	void Load(const char *file){}
	void MoveForward(){}						
	void MoveBehind(){}						
	void SetWindowOn(CGameWnd * _window){}	
	void BringWindowToTop(){}					
	void MoveWindow(int x,int y){}				
	CPoint GetWindowPos(){}						
	void UseBitmapRect(CRect& rect){}		
	void SetColorKey(bool colorkey){}		
	void SetFadeLevel(int level=255){}	
	CSize GetSize(){}

private:
	CGameWnd map[4];
	CPoint point;

};
