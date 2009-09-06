

#pragma once


#include "..\stdafx.h"
#include "Draw.h"


class CGameWnd	: public CPaintWnd
{
public:
	CGameWnd(void);
	virtual ~CGameWnd(void);
	
	bool Create(const char * filename);
	
	bool Load(const char *filename);

	void ShowWindow(bool show);
	
	void MoveForward();
	void MoveBehind();
	void SetWindowOn(CGameWnd * _window);
	void BringWindowToTop();
	
	void MoveWindow(int x,int y);
	CPoint GetWindowPos();
	void UseBitmapRect(CRect& rect);

	void SetColorKey(bool colorkey);
	void SetFadeLevel(int level=255);
	
	CSize GetSize();

	CLayer* GetLayer(){return layer;}
		

protected:
	virtual void prePaint(HDC hdc){};
	virtual void OnPaint(HDC hdc){};


	CLayer * layer;
	CSurface * surface;

};
