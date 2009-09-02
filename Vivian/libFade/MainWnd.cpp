
//////////////////////////////////////////////////////////////////
//
//	FileName	:	MainWnd.cpp
//	Author		:	SakuraSinojun
//	Description	:	a class of the main window
//					主窗口的所有操作与消息响应都在这里完成。
//
//	Version		:	1.0.0.1
//	Date		:	2009.8.30
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////


#include "MainWnd.h"

#pragma comment (lib,"MSIMG32.LIB")

#include "DirectDraw.h"

#include "preLoad.h"


//pMainWnd指向了主窗口。
extern CMainWnd * pMainWnd;

//extern	LPDIRECTDRAWSURFACE	lpDDSPrimary;
//extern	LPDIRECTDRAWSURFACE	lpDDSBack;
//extern	LPDIRECTDRAW		lpDD;
//extern	LPDIRECTDRAWSURFACE lpSurface;

extern	CDirectDraw DirectDraw;

extern char FPS[100];

//配置
extern bool _win;


//以下是测试各绘图结构所用代码。
#define __MainWnd_Debug_

#ifdef __MainWnd_Debug_

char	*	charfile[3]={"CGDATA\\CHAR.bmp","CGDATA\\CHAR1.bmp","CGDATA\\CHAR2.bmp"};
int			charconst=0;
char	*	bgfile[4]={"CGDATA\\bg.bmp","CGDATA\\bg1.bmp","CGDATA\\bg2.bmp","CGDATA\\bg3.bmp"};

char	 *	monsterfile="CGDATA\\witch.bmp";

struct MONSTERWAY
{
	int		increment_of_x;
	int		increment_of_y;
	int		step_count;
	CRect	rect[3];
};

MONSTERWAY	monster1way[4]={{-4,3,20,CRect(1,1,53,75),CRect(54,1,106,75),CRect(107,1,159,75)},
							{4,3,100,CRect(1,76,53,150),CRect(54,76,106,150),CRect(107,76,159,150)},
							{4,-3,20,CRect(1,151,53,225),CRect(1,151,53,225),CRect(1,151,53,225)},
							{-4,-3,100,CRect(1,226,53,300),CRect(1,226,53,300),CRect(1,226,53,300)}};
int	monster1waycount=0;
int monster1waynow=0;
CPoint monster1point=CPoint(100,10);
int monster1actioncount=0;
int	monster1count=0;

MONSTERWAY	monster2way[4]={{4,-3,100,CRect(1,151,53,225),CRect(1,151,53,225),CRect(1,151,53,225)},
							{4,3,20,CRect(1,76,53,150),CRect(54,76,106,150),CRect(107,76,159,150)},
							{-4,3,100,CRect(1,1,53,75),CRect(54,1,106,75),CRect(107,1,159,75)},
							{-4,-3,20,CRect(1,226,53,300),CRect(1,226,53,300),CRect(1,226,53,300)}};
int	monster2waycount=0;
int monster2waynow=0;
CPoint monster2point=CPoint(10,300);
int monster2actioncount=0;
int	monster2count=0;


int ExplosionCount=268;

#endif



//渲染函数。。。
//由OnIdle调用的函数。。渲染事件全部在这里完成。。
extern LONG framecount;
extern DWORD oTime;
void Render();
void CALLBACK TimerProc(HWND hWnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
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

	::SetWindowTextA(hWnd,FPS);

	Render();
}
void Render()
{

	pMainWnd->m_point.x+=4;
	pMainWnd->m_point.y+=3;
	//pMainWnd->m_point =CPoint(320,240);
	
	
	char * temp;

	charconst++;
	monster1count++;
	monster1actioncount++;
	monster2count++;
	monster2actioncount++;

	if(charconst>=30)charconst=0;
	if(monster1actioncount>=12)monster1actioncount=0;
	if(monster2actioncount>=12)monster2actioncount=0;





	if(pMainWnd->m_point.x>=GAME_WINDOW_WIDTH)
	{
		pMainWnd->m_point=CPoint(0,0);
		//::KillTimer (hWnd,1);
		temp=bgfile[0];
		bgfile[0]=bgfile[3];
		bgfile[3]=temp;

		temp=bgfile[1];
		bgfile[1]=bgfile[2];
		bgfile[2]=temp;
	}

	

	if(monster1count>monster1way[monster1waynow].step_count)
	{
		monster1count=0;
		monster1waynow++;
		if(monster1waynow>=4)
			monster1waynow=0;
	}
	if(monster2count>monster2way[monster2waynow].step_count)
	{
		monster2count=0;
		monster2waynow++;
		if(monster2waynow>=4)
			monster2waynow=0;
	}

	monster1point.x+=monster1way[monster1waynow].increment_of_x;
	monster1point.y+=monster1way[monster1waynow].increment_of_y ;
	monster2point.x+=monster2way[monster2waynow].increment_of_x ;
	monster2point.y+=monster2way[monster2waynow].increment_of_y ;


	pMainWnd->DrawBackground();
	

	pMainWnd->DrawMonster (monsterfile,monster1way[monster1waynow].rect[(int)(monster1actioncount/4)],monster1point);
	
	pMainWnd->DrawCharacter();

	
	pMainWnd->DrawMonster2(monsterfile,monster2way[monster2waynow].rect[(int)(monster2actioncount/4)],monster2point);


	
	ExplosionCount+=(67/4);

	if(ExplosionCount>=737)ExplosionCount=268;

	pMainWnd->DrawMonster3 ("CGDATA\\Explosion.bmp",CRect((int)(ExplosionCount/67)*67,0,(int)(ExplosionCount/67)*67+66,100),CPoint(320,240));


	pMainWnd->DrawChat("CGDATA\\chat.bmp");

	pMainWnd->OnPaint ();

	
}


CMainWnd::CMainWnd(HWND hWnd)
{
	this->m_hWnd=hWnd;
	this->m_wnd_dc=::GetDC(hWnd);	


	//为各绘图Layer分配DC和画布。
	this->m_hdc =::CreateCompatibleDC(m_wnd_dc);
	this->m_bitmap =::CreateCompatibleBitmap (m_wnd_dc,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);
	this->m_hdc_background =::CreateCompatibleDC(m_wnd_dc);
	this->m_bitmap_background =::CreateCompatibleBitmap (m_wnd_dc,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);
	this->m_hdc_character =::CreateCompatibleDC(m_wnd_dc);
	this->m_bitmap_character =::CreateCompatibleBitmap (m_wnd_dc,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);
	this->m_hdc_monster =::CreateCompatibleDC (m_wnd_dc);
	this->m_bitmap_monster=::CreateCompatibleBitmap (m_wnd_dc,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);
	this->m_hdc_text=::CreateCompatibleDC(m_wnd_dc);
	this->m_bitmap_text=::CreateCompatibleBitmap (m_hdc_text,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);



	::SelectObject (m_hdc,m_bitmap);
	::SelectObject (m_hdc_background,m_bitmap_background);
	::SelectObject (m_hdc_character,m_bitmap_character);
	::SelectObject (m_hdc_monster,m_bitmap_monster);


	//以下代码留待扩展
	for(int i=0;i<=9;i++)
	{
		this->m_layer [i].bDraw =FALSE;
	}


	//调整指向。
	this->m_ly_background =&m_layer[0];
	this->m_ly_character =&m_layer[1];
	this->m_ly_mask =&m_layer[2];
	this->m_ly_animate =&m_layer[3];
	this->m_ly_text =&m_layer[4];
	this->m_ly_monster =&m_layer[5];

	//设定画布
	this->m_ly_background ->SetDC (m_hdc_background);

	this->m_ly_animate ->SetDC(m_hdc);
	this->m_ly_character ->SetDC(m_hdc_character);
	this->m_ly_mask ->SetDC(m_hdc);
	this->m_ly_text ->SetDC (m_hdc);
	this->m_ly_monster ->SetDC (m_hdc_monster);

	//待扩展
	this->m_ly_background->bDraw =TRUE;
	this->m_ly_character->bDraw=TRUE;
	this->m_ly_mask->bDraw=TRUE;
	this->m_ly_animate->bDraw=TRUE;
	this->m_ly_text->bDraw=TRUE;
	this->m_ly_monster ->bDraw =TRUE;

	//测试地图用变量。
	this->m_point =CPoint(320,240);

	
	//确定pMainWnd指向
	pMainWnd=this;

}

CMainWnd::~CMainWnd() 
{
	::DeleteDC(m_hdc);
	::DeleteObject (this->m_bitmap );
	::DeleteDC (m_hdc_character);
	::DeleteDC (m_hdc_background);
	::DeleteDC (m_hdc_monster);
	::DeleteObject (m_bitmap_background);
	::DeleteObject (m_bitmap_character);
	::DeleteObject (m_bitmap_monster);

}


BOOL CMainWnd::OnPaint ()
{

	if(_win)
	{
		::BitBlt (m_wnd_dc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,m_hdc,0,0,SRCCOPY);
	}else{
		DirectDraw.Draw ();
		DirectDraw.Flip ();
	}


	return TRUE;
}


//绘制背景用测试函数。
//此函数完成背景的四张CG拼接。
void CMainWnd::DrawBackground()
{

	BKGNDPARAMETER bgParameter;				//BKGNDPARAMETER结构存储了四张CG的信息。

	memset(&bgParameter,0,sizeof(bgParameter));
	bgParameter.b4pics =TRUE;					//由四张拼接而不是单一背景显示。
	bgParameter.point=m_point;					//接鏠坐标
	bgParameter.part_topleft=bgfile[0];			//左上图
	bgParameter.part_topright =bgfile[1];		//右上图
	bgParameter.part_bottomleft =bgfile[2];		//左下图
	bgParameter.part_bottomright =bgfile[3];	//右下图

	this->m_ly_background ->DrawBackground (&bgParameter);		//背景layer的绘制。
	
	if(_win)
	{
		//m_hdc是个临时DC，所有的中间绘制过程全部画到m_hdc上后再一起blt到前景上。
		//COLORREF	transColor=::GetPixel (this->m_hdc_background ,1,1);
		::BitBlt (m_hdc,0,0,GAME_WINDOW_WIDTH+1,GAME_WINDOW_HEIGHT+1,this->m_hdc_background,0,0,SRCCOPY);	
	}

}

void CMainWnd::DrawCharacter()
{
	this->m_ly_character ->DrawCharacter (charfile[(int)(charconst/10)],CPoint(64,-32));

	if(_win)
	{
		COLORREF	transColor=::GetPixel (this->m_hdc_character ,1,1);
		::TransparentBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,m_hdc_character,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,transColor);
	}else{
	
	}
}

void CMainWnd::DrawMonster(LPCSTR filename,CRect rectInPic,CPoint point2Draw)
{
	static	bool bFirstDraw=true;
	static	CDDSurface dds_monster;
	COLORREF	color;

	if(_win)
	{
		this->m_ly_monster ->DrawMonster (filename,rectInPic,point2Draw);
		COLORREF	transColor=::GetPixel (this->m_hdc_monster ,1,1);
		::TransparentBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,m_hdc_monster,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,transColor);
	}else{
		if(bFirstDraw)
		{
			dds_monster.Load (DirectDraw,filename);
			DirectDraw.Add (&dds_monster);	
			color=dds_monster.GetPTColor();
			dds_monster.SetColorKey (color);
			dds_monster.Show ();
			bFirstDraw=false;
		}
		dds_monster.SetDrawPos (point2Draw);
		dds_monster.SetSrcRect (rectInPic);
	}

}
void CMainWnd::DrawMonster2(LPCSTR filename,CRect rectInPic,CPoint point2Draw)
{
	static	bool bFirstDraw=true;
	static	CDDSurface dds_monster;
	COLORREF	color;

	if(_win)
	{
		this->m_ly_monster ->DrawMonster (filename,rectInPic,point2Draw);
		COLORREF	transColor=::GetPixel (this->m_hdc_monster ,1,1);
		::TransparentBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,m_hdc_monster,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,transColor);

	}else{
		if(bFirstDraw)
		{
			dds_monster.Load (DirectDraw,filename);
			DirectDraw.Add (&dds_monster);	
			color=dds_monster.GetPTColor();
			dds_monster.SetColorKey (color);
			dds_monster.Show ();
			bFirstDraw=false;
		}
		dds_monster.SetDrawPos (point2Draw);
		dds_monster.SetSrcRect (rectInPic);
	}

}
void CMainWnd::DrawMonster3(LPCSTR filename,CRect rectInPic,CPoint point2Draw)
{
	static	bool bFirstDraw=true;
	static	CDDSurface dds_monster;
	COLORREF	color;

	if(_win)
	{
		this->m_ly_monster ->DrawMonster (filename,rectInPic,point2Draw);
		COLORREF	transColor=::GetPixel (this->m_hdc_monster ,1,1);
		::TransparentBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,m_hdc_monster,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,transColor);

	}else{
		if(bFirstDraw)
		{
			dds_monster.Load (DirectDraw,filename);
			DirectDraw.Add (&dds_monster);	
			color=dds_monster.GetPTColor();
			dds_monster.SetColorKey (color);
			dds_monster.Show ();
			bFirstDraw=false;
		}
		dds_monster.SetDrawPos (point2Draw);
		dds_monster.SetSrcRect (rectInPic);
	}

}

void CMainWnd::DrawChat(LPCSTR filename)
{
	static	bool bFirstDraw=true;
	static	CDDSurface dds_chat;
	CDDSurface dds_temp;


	COLORREF	color;

	if(_win)
	{
		HDC memDC=pl::Load (filename);
		color=::GetPixel (memDC,1,1);
		::TransparentBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,memDC,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,color);
		TextOutA(m_hdc,210,380,FPS,lstrlenA(FPS));

	}else{
		
		if(bFirstDraw)
		{
			dds_chat.Load (DirectDraw,filename);
			DirectDraw.Add (&dds_chat);	
			color=dds_chat.GetPTColor();
			dds_chat.SetColorKey (color);
			dds_chat.Show ();
			bFirstDraw=false;
		}

		
		HDC hdc;
		dds_chat->GetDC(&hdc);
		//::SetBkMode (hdc,TRANSPARENT);
		::SetTextColor (hdc,RGB(0,0,0));
		TextOutA(hdc,210,380,FPS,lstrlenA(FPS));
		dds_chat->ReleaseDC (hdc);
		

	}


}


///////////////////////////////////////////////////////////////////////////////////////////
//
//
//				事件响应
//
//
///////////////////////////////////////////////////////////////////////////////////////////


void CMainWnd::OnLButtonDown(WPARAM wParam,CPoint point)
{

}

BOOL CMainWnd::OnIdle(LONG lcount)
{
	Render();
	return TRUE;
}


void CMainWnd::OnCreate()
{
	/*
	pl::Load ("CGDATA\\bg.bmp");
	pl::Load ("CGDATA\\bg1.bmp");
	pl::Load ("CGDATA\\bg2.bmp");
	pl::Load ("CGDATA\\bg3.bmp");
	pl::Load ("CGDATA\\char.bmp");
	pl::Load ("CGDATA\\char1.bmp");
	pl::Load ("CGDATA\\char2.bmp");
	pl::Load ("CGDATA\\chat.bmp");
	pl::Load ("CGDATA\\witch.bmp");
	*/

#ifndef _ONIDLE_
	::SetTimer(m_hWnd,1,31,TimerProc);
#endif

}