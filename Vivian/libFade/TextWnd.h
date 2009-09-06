

//////////////////////////////////////////////////////////////////
//
//	FileName	:	TextWnd.h
//	Author		:	SakuraSinojun
//	Description	:	this class is used to draw text on the surface
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////



#pragma once
#include "gamewnd.h"
#include <windows.h>


class CTextWnd :public CGameWnd
{
public:
	CTextWnd(void);
	virtual ~CTextWnd(void);
	
	bool Create(const char *filename);
	bool Create(int width,int height);

	void CreateText(int x,int y,const char * text);
	void SetBkColor(COLORREF color){_bkcolor=color;}
	void SetTextColor(COLORREF color){_textcolor=color;}
	void SetBkMode(int mode){_bkmode=mode;}


protected:
	void OnPaint(HDC hdc);
	void prePaint(HDC hdc);

private:
	int			_x;
	int			_y;
	char	*	_text;
	
	COLORREF	_bkcolor;
	COLORREF	_textcolor;
	int			_bkmode;


};
