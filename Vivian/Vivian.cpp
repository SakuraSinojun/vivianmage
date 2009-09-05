
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

#include "libFade\libGameWindow.h"

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

	return 0;

}

///////////////////////////////////////////////////////////////////////////////////////
//
//
//			消息处理函数
//
//
//////////////////////////////////////////////////////////////////////////////////////

CMapWnd * win1;
CTextWnd * win2;
CAnimation * win3;
CGameWnd * win5;

CGameWnd * win4;


void OnCreate(HWND hWnd)
{
	win1=NEW CMapWnd();
	win2=NEW CTextWnd();
	win3=NEW CAnimation();
	win5=NEW CGameWnd();
	win4=NEW CGameWnd();

	win1->Create ("CGDATA\\bg.bmp","CGDATA\\bg1.bmp","CGDATA\\bg2.bmp","CGDATA\\bg3.bmp");
	win1->SetSplitPoint (CPoint(320,240));
	

	
	win2->SetTextColor (RGB(255,0,0));
	win2->CreateText (100,100,"Text test.");
	

	
	IMAGEFRAME frame;
	::ZeroMemory (&frame,sizeof(frame));
	frame.colorkey =true;
	frame.nDelay =50;
	lstrcpyA(frame.filename,"CGDATA\\Explosion.bmp");
	frame.rect.top =0;
	frame.rect.bottom =100;
	frame.iFadeLevel =255;
	for(int i=268;i<=670;i+=67)
	{
		frame.rect.left =i;
		frame.rect.right =i+67;
		win3->AddFrame (&frame);
	}
	win3->Start(win3->ANIMATIONTYPE_IMAGE);
	win3->MoveWindow (50,50);
	win3->ShowWindow (true);
	win3->CircleAnimation (win3->ANIMATIONTYPE_IMAGE ,true);

	win5->Create ("CGDATA\\char.bmp");
	win5->SetColorKey (true);

	win5->MoveWindow (320,0);
	

	
}

void OnClose()
{
	delete win1;
	delete win2;
	delete win3;
	delete win5;
	delete win4;

	::PostQuitMessage (0);
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
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}




