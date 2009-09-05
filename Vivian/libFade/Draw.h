



#pragma once

#include "..\stdafx.h"

#include "Layer.h"
#include "Surface.h"
#include "DirectDraw.h"
#include "GDIDraw.h"

namespace CDraw
{

	
	CDirectDraw&	GetDirectDraw();
	CGDIDraw&		GetGDIDraw();

	void CreateGameWindow(HINSTANCE hInstance,int width=640,int height=480,bool win=true,bool onidle=true);
	void Attach(HWND hWnd);
	
	void SetWindowProcedure(WNDPROC _proc);
	CSurface * CreateNewSurface();


};



