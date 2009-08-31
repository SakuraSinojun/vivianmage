
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
wchar_t *	bgfile[4]={TEXT("CGDATA\\bg.bmp"),TEXT("CGDATA\\bg1.bmp"),TEXT("CGDATA\\bg2.bmp"),TEXT("CGDATA\\bg3.bmp")};

wchar_t *	monsterfile=TEXT("CGDATA\\witch.bmp");

struct MONSTERWAY
{
	int		increment_of_x;
	int		increment_of_y;
	int		step_count;
	CRect	rect[3];
};

MONSTERWAY	monster1way[4]={{-8,6,10,CRect(1,1,53,75),CRect(54,1,106,75),CRect(107,1,159,75)},
							{8,6,50,CRect(1,76,53,150),CRect(54,76,106,150),CRect(107,76,159,150)},
							{8,-6,10,CRect(1,151,53,225),CRect(1,151,53,225),CRect(1,151,53,225)},
							{-8,-6,50,CRect(1,226,53,300),CRect(1,226,53,300),CRect(1,226,53,300)}};
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

//void CALLBACK TimerProc(HWND hWnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
void Render()
{

	pMainWnd->m_point.x+=8;
	pMainWnd->m_point.y+=6;
	
	
	wchar_t * temp;

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


	pMainWnd->DrawBackground();
	

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


	
	pMainWnd->DrawMonster (monsterfile,monster1way[monster1waynow].rect[(int)(monster1actioncount/4)],monster1point);
	
	pMainWnd->DrawCharacter();

	
	pMainWnd->DrawMonster (monsterfile,monster2way[monster2waynow].rect[(int)(monster2actioncount/4)],monster2point);


	
	ExplosionCount+=(67/2);

	if(ExplosionCount>=737)ExplosionCount=268;

	pMainWnd->DrawMonster (TEXT("CGDATA\\Explosion.bmp"),CRect((int)(ExplosionCount/67)*67,0,(int)(ExplosionCount/67)*67+66,100),CPoint(320,240));



	pMainWnd->OnPaint ();

	
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
	this->m_hdc_monster =::CreateCompatibleDC (::GetDC(hWnd));
	this->m_bitmap_monster=::CreateCompatibleBitmap (::GetDC(hWnd),GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);


	::SelectObject (m_hdc,m_bitmap);
	::SelectObject (m_hdc_background,m_bitmap_background);
	::SelectObject (m_hdc_character,m_bitmap_character);
	::SelectObject (m_hdc_monster,m_bitmap_monster);


	for(int i=0;i<=9;i++)
	{
		this->m_layer [i].bDraw =FALSE;
	}

	this->m_ly_background =&m_layer[0];
	this->m_ly_character =&m_layer[1];
	this->m_ly_mask =&m_layer[2];
	this->m_ly_animate =&m_layer[3];
	this->m_ly_text =&m_layer[4];
	this->m_ly_monster =&m_layer[5];

	this->m_ly_background ->SetDC (m_hdc_background);
	this->m_ly_animate ->SetDC(m_hdc);
	this->m_ly_character ->SetDC(m_hdc_character);
	this->m_ly_mask ->SetDC(m_hdc);
	this->m_ly_text ->SetDC (m_hdc);
	this->m_ly_monster ->SetDC (m_hdc_monster);

	this->m_ly_background->bDraw =TRUE;
	this->m_ly_character->bDraw=TRUE;
	this->m_ly_mask->bDraw=TRUE;
	this->m_ly_animate->bDraw=TRUE;
	this->m_ly_text->bDraw=TRUE;
	this->m_ly_monster ->bDraw =TRUE;

	this->m_point =CPoint(0,0);
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
	bgParameter.part_topleft=bgfile[0];
	bgParameter.part_topright =bgfile[1];
	bgParameter.part_bottomleft =bgfile[2];
	bgParameter.part_bottomright =bgfile[3];

	this->m_ly_background ->DrawBackground (&bgParameter);

	::BitBlt (m_hdc,0,0,GAME_WINDOW_WIDTH+1,GAME_WINDOW_HEIGHT+1,this->m_hdc_background,0,0,SRCCOPY);
	
}

void CMainWnd::DrawCharacter()
{
	
	this->m_ly_character ->DrawCharacter (charfile[(int)(charconst/10)]);

	COLORREF	transColor=::GetPixel (this->m_hdc_character ,1,1);
	
	::TransparentBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,m_hdc_character,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,transColor);

}

void CMainWnd::DrawMonster(LPCTSTR filename,CRect rectInPic,CPoint point2Draw)
{
	this->m_ly_monster ->DrawMonster (filename,rectInPic,point2Draw);
	COLORREF	transColor=::GetPixel (this->m_hdc_monster ,1,1);

	::TransparentBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,m_hdc_monster,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,transColor);


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
	//::SetTimer(m_hWnd,1,10,TimerProc);

}

BOOL CMainWnd::OnIdle(LONG lcount)
{
	Render();
	return TRUE;
}


