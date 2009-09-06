
//////////////////////////////////////////////////////////////////
//
//	FileName	:	GameWnd.h
//	Author		:	SakuraSinojun
//	Description	:	this class is used to create a graphic window in the Game
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////




#pragma once


#include "..\stdafx.h"
#include "Draw.h"


class CGameWnd	: public CPaintWnd
{
public:
	CGameWnd(void);
	virtual ~CGameWnd(void);
	
	bool Create(int width,int height);
	
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

	
		

protected:
	virtual void prePaint(HDC hdc){};
	virtual void OnPaint(HDC hdc){};
	
	CLayer* GetLayer(){return layer;}

	CLayer * layer;
	CSurface * surface;

};
