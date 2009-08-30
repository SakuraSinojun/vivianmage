
//////////////////////////////////////////////////////////////////
//
//	FileName	:	MainWnd.cpp
//	Author		:	SakuraSinojun
//	Description	:	a class of the main window
//	
//	Version		:	1.0.0.1
//	Date		:	2009.8.30
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////


#include "MainWnd.h"

#pragma comment (lib,"MSIMG32.LIB")

#define __MainWnd_Debug_




extern CMainWnd * pMainWnd;


#ifdef __MainWnd_Debug_

wchar_t	*	charfile[3]={TEXT("CGDATA\\CHAR.bmp"),TEXT("CGDATA\\CHAR1.bmp"),TEXT("CGDATA\\CHAR2.bmp")};
int			charconst=0;

#endif


void CALLBACK TimerProc(HWND hWnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{

	pMainWnd->m_point.x+=8;
	pMainWnd->m_point.y+=6;
	
	
	charconst++;

	if(charconst>=30)charconst=0;
	
	pMainWnd->OnPaint ();


	if(pMainWnd->m_point.x>GAME_WINDOW_WIDTH/2)
	{
		pMainWnd->m_point=CPoint(0,0);
		::KillTimer (hWnd,1);
	}

	
}


CMainWnd::CMainWnd(HWND hWnd)
{
	this->m_hWnd=hWnd;

	this->m_hdc =::CreateCompatibleDC(::GetDC(hWnd));
	this->m_bitmap =::CreateCompatibleBitmap (::GetDC(hWnd),GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);
	this->m_hdc_background =::CreateCompatibleDC(::GetDC(hWnd));
	this->m_bitmap_background =::CreateCompatibleBitmap (::GetDC(hWnd),GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);
	this->m_hdc_character =::CreateCompatibleDC(::GetDC(hWnd));
	this->m_bitmap_character =::CreateCompatibleBitmap (::GetDC(hWnd),GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);


	::SelectObject (m_hdc,m_bitmap);
	::SelectObject (m_hdc_background,m_bitmap_background);
	::SelectObject (m_hdc_character,m_bitmap_character);

	for(int i=0;i<=9;i++)
	{
		this->m_layer [i].bDraw =FALSE;
	}

	this->m_ly_background =&m_layer[0];
	this->m_ly_character =&m_layer[1];
	this->m_ly_mask =&m_layer[2];
	this->m_ly_animate =&m_layer[3];
	this->m_ly_text =&m_layer[4];

	this->m_ly_background ->SetDC (m_hdc_background);
	this->m_ly_animate ->SetDC(m_hdc);
	this->m_ly_character ->SetDC(m_hdc_character);
	this->m_ly_mask ->SetDC(m_hdc);
	this->m_ly_text ->SetDC (m_hdc);

	this->m_ly_background->bDraw =TRUE;
	this->m_ly_character->bDraw=TRUE;
	this->m_ly_mask->bDraw=TRUE;
	this->m_ly_animate->bDraw=TRUE;
	this->m_ly_text->bDraw=TRUE;


	this->m_point =CPoint(0,0);
	pMainWnd=this;
}

CMainWnd::~CMainWnd() 
{
	::DeleteDC(m_hdc);
	::DeleteObject (this->m_bitmap );
	
}


BOOL CMainWnd::OnPaint ()
{

	DrawBackground();
	DrawCharacter();
	
	::BitBlt (::GetDC(m_hWnd),0,0,GAME_WINDOW_WIDTH+1,GAME_WINDOW_HEIGHT+1,this->m_hdc,0,0,SRCCOPY);
	
	return TRUE;
}

void CMainWnd::DrawBackground()
{


	BKGNDPARAMETER bgParameter;

	memset(&bgParameter,0,sizeof(bgParameter));
	bgParameter.b4pics =TRUE;
	//bgParameter.point =CPoint(GAME_WINDOW_WIDTH/2,GAME_WINDOW_HEIGHT/2);
	bgParameter.point=m_point;
	bgParameter.part_topleft=TEXT("bg.bmp");
	bgParameter.part_topright =TEXT("CGDATA\\bg1.bmp");
	bgParameter.part_bottomleft =TEXT("CGDATA\\bg2.bmp");
	bgParameter.part_bottomright =TEXT("CGDATA\\bg3.bmp");

	this->m_ly_background ->DrawBackground (&bgParameter);

	::BitBlt (m_hdc,0,0,GAME_WINDOW_WIDTH+1,GAME_WINDOW_HEIGHT+1,this->m_hdc_background,0,0,SRCCOPY);
	
}

void CMainWnd::DrawCharacter()
{
	
	this->m_ly_character ->DrawCharacter (charfile[(int)(charconst/10)]);

	COLORREF	transColor=::GetPixel (this->m_hdc_character ,1,1);
	
	::TransparentBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,m_hdc_character,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,transColor);

}



///////////////////////////////////////////////////////////////////////////////////////////
//
//
//				ÊÂ¼þÏìÓ¦
//
//
///////////////////////////////////////////////////////////////////////////////////////////


void CMainWnd::OnLButtonDown(WPARAM wParam,CPoint point)
{
	::SetTimer(m_hWnd,1,10,TimerProc);

}

