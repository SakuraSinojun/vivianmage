

//////////////////////////////////////////////////////////////////
//
//	FileName	:	Draw.cpp
//	Author		:	SakuraSinojun
//	Description	:	Main interface of drawing.
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////



#include "Draw.h"

#include "DirectDraw.h"
#include "GDIDraw.h"


#include "..\Memory.h"

#include <string>

namespace CDraw
{
	//��������
	bool	_win;						//���ڻ�����
	bool	_idlemsg;					//��Ӧidle
	int		_winwidth;					//���ڿ�
	int		_winheight;					//���ڸ�
	bool	_showfps=true;				//��ʾFPS
	
	WNDPROC	_windowprocedure=NULL;		//��Ϣ�ص�����ָ��

	

#ifdef __USE_DIRECTDRAW__
	CDirectDraw DirectDraw;				//DirectDraw��ͼ��
#endif
	CGDIDraw	GDIDraw;				//GDI��ͼ��
	HWND		m_hWnd=NULL;			//�����ھ��
	HINSTANCE	m_hInstance;			//ʵ�����
	char		FPS[100];				//FPS��Ϣ


#ifdef __USE_DIRECTDRAW__
	//ȡ��Direct��ͼ��
	CDirectDraw& GetDirectDraw()
	{
		return DirectDraw;
	}
#endif


	//ȡ��GDI��ͼ��
	CGDIDraw&	GetGDIDraw()
	{
		return GDIDraw;
	}

	//���ڹ��̻ص�����	
	LRESULT CALLBACK _WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_ERASEBKGND:
			return true;
			break;
		case WM_CREATE:
			m_hWnd=hwnd;
			Attach(hwnd);
			break;
		default:
			break;
		}
		if(_windowprocedure!=NULL)
		{
			return _windowprocedure(hwnd,uMsg,wParam,lParam);
		}else{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	
	
	//����FPS.
	void CalcFPS()
	{
		static DWORD oTime=0;
		static LONG	framecount=0;
		framecount++;
		double fps;
		DWORD t=GetTickCount();
		if(oTime==0)
		{
			oTime=t;
			return;
		}
		if(t-oTime>=1000)
		{
			fps=((double)framecount)*1000/(double)(t-oTime);
			framecount=0;
			sprintf_s(FPS,"FPS: %.2f",fps);
			oTime=t;
		}
	}

	//OnIdleѭ����
	BOOL OnIdle(LONG count)
	{
		CalcFPS();

		HDC hdc;

		if(_win)
		{
			GDIDraw.Draw ();
			GDIDraw.Flip ();
			if(_showfps)
			{
				hdc=GetDC(m_hWnd);
				TextOutA(hdc,0,0,FPS,lstrlenA(FPS));
				ReleaseDC(m_hWnd,hdc);
			}
		}else{
#ifdef __USE_DIRECTDRAW__
			DirectDraw.Draw ();
			if(_showfps)
			{
				DirectDraw.GetDrawable ()->GetDC(&hdc);
				TextOutA(hdc,0,0,FPS,lstrlenA(FPS));
				DirectDraw.GetDrawable ()->ReleaseDC (hdc);
			}
			DirectDraw.Flip ();
#endif
		}
		
		::SetWindowText (m_hWnd,FPS);
		return TRUE;
	}


	//��������
	void CreateGameWindow(HINSTANCE hInstance ,int width,int height,bool win,bool onidle)
	{
		WNDCLASSEX	wcex;
		CRect		rect;
		MSG			msgCur;
		LONG		count=0;
		bool		idle=true;


		_win=win;
		_idlemsg=onidle;
		_winwidth=width;
		_winheight=height;
		m_hInstance=hInstance;

		//ע�ᴰ����
		wcex.cbClsExtra = 0;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.cbWndExtra = 0;
		wcex.hbrBackground = (HBRUSH)::GetStockObject (BLACK_BRUSH);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hInstance = m_hInstance;
		wcex.lpfnWndProc = _WindowProcedure;
		wcex.lpszClassName = TEXT("VVWindow");
		wcex.lpszMenuName = NULL;
		wcex.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassEx(&wcex))
		{
			::MessageBox(NULL,"��������ʧ��","ERROR",MB_OK);
			exit(0);
		}
		
		//�������ڴ�С
		rect=CRect(0,0,width-4,height-4);
		::AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,FALSE);

		//��������
		m_hWnd = CreateWindowEx(0, TEXT("VVWindow"), 
									TEXT("VVRPG"), 
									(_win)?(WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX):WS_POPUP,
									CW_USEDEFAULT, CW_USEDEFAULT, rect.Width() , rect.Height (), 
									HWND_DESKTOP, NULL, m_hInstance, NULL);
		if(m_hWnd==NULL)
		{
			::MessageBox(NULL,"��������ʧ��","ERROR",MB_OK);
			exit(0);
		}

		//��ʾ
		ShowWindow(m_hWnd, SW_SHOW);


		//��Ϣѭ��
		if(onidle)			//��Ӧidle
		{
			while(true)
			{
				if (::PeekMessage(&msgCur, NULL, 0, 0, PM_NOREMOVE))
				{
					if (!::GetMessage(&msgCur, NULL, 0, 0))
						return;

					//if (!PreTranslateMessage(&msgCur))
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
		}else{			//����Ӧidle
			while(GetMessage(&msgCur, NULL, 0, 0))  {
				TranslateMessage(&msgCur);
				DispatchMessage(&msgCur);
			}
		}


	}

	
	//�趨�ص�����
	void SetWindowProcedure(WNDPROC _proc)
	{
		_windowprocedure=_proc;
	}


	//���ӵ�����
	void Attach(HWND hWnd)
	{
		if(_win)
		{
			GDIDraw.Create (m_hWnd,_winwidth,_winheight);
		}else{

#ifdef __USE_DIRECTDRAW__
			if(DirectDraw.Create (m_hWnd,_winwidth,_winheight,CDirectDraw::FullColor))
			{
			}else{
				::MessageBox (m_hWnd,"DirectDraw��ʼ������!","ERROR",MB_OK | MB_ICONINFORMATION);
				exit(0);
			}
#endif

		}
	}


	//������ͼҳ��CSurface�಻�����������Ե��ô˺������˴�Ӧ���ƹ���ģʽ��
	CSurface * CreateNewSurface()
	{
		if(_win)
		{
			return NEW CGDISurface();
		}else{
#ifdef __USE_DIRECTDRAW__
			return NEW CDDSurface();
#endif
		}
	}


};





