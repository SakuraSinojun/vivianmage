
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

#include "libFade\MainWnd.h"

#include "libFade\DirectDraw.h"

#include "libFade\preLoad.h"


#define _ONIDLE_
//#undef	_ONIDLE_



//窗口、句柄
HWND m_hWnd;
HINSTANCE m_hInstance;
CMainWnd * pMainWnd;


//FPS计算
DWORD	oTime=0;
LONG	framecount=0;
char FPS[100];

//DirectDraw
CDirectDraw DirectDraw;
void	InitD3D();  
void	Cleanup();
void	iRender();

//设置选项
bool	_win=false;			//是否在窗口模式运行。窗口下不能用DirectDraw，而是用GDI绘图，刷新速率需自行控制。


//-----------------------------------------------------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	m_hInstance=hInstance;
	pMainWnd=NULL;

	
	//pl::test();

	if(lstrcmpA(lpCmdLine,"-w")==0)
	{
		_win=true;
	}else{
		_win=false;
	}

	//_win=true;
	
	int res= run();


	delete pMainWnd;
	return res;

}



///////////////////////////////////////////////////////////////////////////////////////
//
//
//							窗口实现
//
//
//////////////////////////////////////////////////////////////////////////////////////

#ifdef 折叠
#else

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch(uMsg)
	{
		case WM_CREATE:
			OnCreate(hwnd);
			break;
		case WM_DESTROY:
			return OnDestroy();
			break;
		case WM_CLOSE:
			return OnClose();
		case WM_PAINT:
			::BeginPaint(hwnd, &ps);
			OnPaint();
			::EndPaint(hwnd,&ps);
			return TRUE;
		case WM_ERASEBKGND:
			if(OnEraseBkGnd())
				return TRUE;
			break;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam,CPoint(lParam));
			break;
		case WM_LBUTTONDOWN:
			OnLButtonDown(wParam,CPoint(lParam));
			break;
		case WM_RBUTTONDOWN:
			OnRButtonDown(wParam,CPoint(lParam));
			break;
		case WM_TIMER:
			//DirectDraw.Flip ();
			break;
		default:
			break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int run()
{
	bool idle = true;
	long count = 0;
	MSG  msgCur;

	VVCreateWindow();
	ShowWindow(m_hWnd, SW_SHOW);

	
#ifdef _ONIDLE_
	while(true)
	{
		if (::PeekMessage(&msgCur, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!::GetMessage(&msgCur, NULL, 0, 0))
				return msgCur.wParam;

			if (!PreTranslateMessage(&msgCur))
			{
				::TranslateMessage(&msgCur);
				::DispatchMessage(&msgCur);
			}
			idle = true;
			count = 0;
		}else if (idle){
			if(count==MAXLONG)
				count=0;
			if(!OnIdle(count++))
				idle = false;
		}else{
			::WaitMessage();
		}
	}
#else
	while(GetMessage(&msgCur, NULL, 0, 0))  {
		TranslateMessage(&msgCur);
		DispatchMessage(&msgCur);
	}
#endif

	return 0;

}

BOOL VVCreateWindow()
{
	if(!VVRegisterClass())
		return FALSE;
	
	
	CRect rect;
	rect=CRect(0,0,GAME_WINDOW_WIDTH-4,GAME_WINDOW_HEIGHT-4);
	::AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,FALSE);


	m_hWnd = CreateWindowEx(0, TEXT("VVWindow"), 
								TEXT("VVRPG"), 
								(_win)?(WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX):WS_POPUP,
								CW_USEDEFAULT, CW_USEDEFAULT, rect.Width() , rect.Height (), 
								HWND_DESKTOP, NULL, m_hInstance, NULL);
	if(m_hWnd==NULL)
		return FALSE;
	
	InitD3D();

	return TRUE;
}

BOOL VVRegisterClass()
{
	WNDCLASSEX wcex;

	wcex.cbClsExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)::GetStockObject (BLACK_BRUSH);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = m_hInstance;
	wcex.lpfnWndProc = WindowProcedure;
	wcex.lpszClassName = TEXT("VVWindow");
	wcex.lpszMenuName = NULL;
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wcex))
		return FALSE;
	return TRUE;

}
#endif





///////////////////////////////////////////////////////////////////////////////////////
//
//
//			消息处理函数,这些消息将全部由渲染类完成。
//
//
//////////////////////////////////////////////////////////////////////////////////////


BOOL OnIdle(long count)
{
	framecount++;	
	DWORD t=::GetTickCount ();
	double fps;
	if(oTime==0)
	{
		oTime=t;
	}else{
		if(t-oTime>=1000)
		{
			fps=((double)framecount)*1000.0/(double)(t-oTime);
			oTime=t;
			framecount=0;
			sprintf_s(FPS,"FPS:%3.2f",fps);
		}
	}

	::SetWindowTextA(m_hWnd,FPS);

	pMainWnd->OnIdle (count);

	return TRUE;

}
BOOL PreTranslateMessage(MSG * msg)
{
	return FALSE;
}

void OnCreate(HWND hWnd)
{
	pMainWnd=new CMainWnd(hWnd);
	//::SetTimer (hWnd,1,16,NULL);
	pMainWnd->OnCreate ();


}

BOOL OnDestroy()
{
	PostQuitMessage(0);
	return TRUE;
}

BOOL OnClose()
{
	Cleanup();
	//if(6==::MessageBox(m_hWnd,TEXT("确定要退出？"),TEXT("结束游戏"),MB_YESNO))
	{
		PostQuitMessage(0);
	}
	return TRUE;
}

void OnLButtonDown(WPARAM wParam,CPoint point)
{
	pMainWnd->OnLButtonDown (wParam,point);
}

void OnRButtonDown(WPARAM wParam,CPoint point)
{

}

void OnMouseMove(WPARAM wParam,CPoint point)
{

}


void OnPaint()
{
	//pMainWnd->OnPaint ();
}

BOOL OnEraseBkGnd()
{
	return TRUE;
}



//-----------------------------------------------------------------------------------------------------------------------
void	InitD3D()
{
	if(_win)
	{

	}else{
#ifdef __DEBUG_DD_
		if(DirectDraw.Create (m_hWnd,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,CDirectDraw::FullColor))
		{
		}else{
		}
		return ;
#endif
	}

}

void	Cleanup()
{

}

void iRender()
{
	
}