
//////////////////////////////////////////////////////////////////
//
//	FileName	:	Vivian.h
//	Author		:	SakuraSinojun
//	Description	:	Main Window and Function
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////


#pragma once

#include "windows.h"
#include "typedef.h"


BOOL	run();
BOOL	VVCreateWindow();
BOOL	VVRegisterClass();
BOOL	OnIdle(long count);
BOOL	PreTranslateMessage(MSG * msg);
BOOL	OnEraseBkGnd();

void OnCreate(HWND hWnd);
BOOL OnDestroy();
BOOL OnClose();
void OnPaint();
void OnLButtonDown(WPARAM wParam,CPoint point);
void OnRButtonDown(WPARAM wParam,CPoint point);
void OnMouseMove(WPARAM wParam,CPoint point);






