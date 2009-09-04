
//////////////////////////////////////////////////////////////////
//
//	FileName	:	Vivian.cpp
//	Author		:	SakuraSinojun
//	Description	:	Main Window and Function
//	
//	Version		:	1.0.0.1
//	Date		:	2009.8.30
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////


#define __DEBUG_DD_

#include "Vivian.h"
#include "stdio.h"

#include "libFade\Draw.h"


#include "libFade\Draw.h"


HINSTANCE m_hInstance;
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	m_hInstance=hInstance;
	bool _win;		

	if(lstrcmpA(lpCmdLine,"-w")==0)
	{
		_win=true;
	}else{
		_win=false;
	}
	
	CDraw::SetWindowProcedure (WindowProcedure);
	CDraw::CreateGameWindow (hInstance,640,480,false);

	::MessageBeep (-1);

	return 0;

}

///////////////////////////////////////////////////////////////////////////////////////
//
//
//			消息处理函数
//
//
//////////////////////////////////////////////////////////////////////////////////////
void OnCreate(HWND hWnd)
{
	CSurface * surface=CDraw::NewSurface ();
	surface->Load ("CGDATA\\bg.bmp");
	surface->Show ();
	surface->Add();

}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CLOSE:
		::PostQuitMessage (0);
		break;
	case WM_CREATE:
		OnCreate(hwnd);
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}