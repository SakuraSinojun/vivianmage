
//////////////////////////////////////////////////////////////////
//
//	FileName	:	Draw.h
//	Author		:	SakuraSinojun
//	Description	:	Main interface of drawing.
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////




#pragma once

#include "..\stdafx.h"

#include "Layer.h"
#include "Surface.h"
#include "DirectDraw.h"
#include "GDIDraw.h"

namespace CDraw
{

#ifdef __USE_DIRECTDRAW__
	CDirectDraw&	GetDirectDraw();
#endif

	CGDIDraw&		GetGDIDraw();

	void CreateGameWindow(HINSTANCE hInstance,int width=640,int height=480,bool win=true,bool onidle=true);
	void Attach(HWND hWnd);
	
	void SetWindowProcedure(WNDPROC _proc);
	CSurface * CreateNewSurface();


};



