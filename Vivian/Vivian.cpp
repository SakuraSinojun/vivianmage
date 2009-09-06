
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

#include "libAction\DemoAction.h"

#include "Memory.h"


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
	CDraw::CreateGameWindow (hInstance,640,480,true);

	//::MessageBeep (-1);
	
#ifdef MEMCHECK
	int l=500;
	char * report=new char[l];
	
	while(!MemoryReport (report,l))
	{
		delete[] report;
		l=l*2;
		report=new char[l];
	}

	if(lstrlenA(report)!=0)
	{
		::MessageBox(NULL,report,"内存报告",MB_OK | MB_ICONINFORMATION);
	}

	delete[] report;
#endif

	return 0;

}

///////////////////////////////////////////////////////////////////////////////////////
//
//
//			消息处理函数
//
//
//////////////////////////////////////////////////////////////////////////////////////

CDemoAction * action;



void OnCreate(HWND hWnd)
{
	action=NEW CDemoAction();
	action->OnCreate ();
	
}

void OnClose()
{
	delete action;
	::PostQuitMessage (0);
}

void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	/*
	char temp[100];
	wsprintf(temp,"%d",nChar);
	::MessageBoxA(NULL,temp,temp,MB_OK);
	*/

	action->OnKeyDown (nChar,nRepCnt,nFlags);

}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch(uMsg)
	{
	case WM_CLOSE:
		OnClose();
		break;
	case WM_CREATE:
		OnCreate(hwnd);
		break;
	case WM_KEYDOWN:
		OnKeyDown(wParam,LOWORD(lParam),HIWORD(lParam));
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}




