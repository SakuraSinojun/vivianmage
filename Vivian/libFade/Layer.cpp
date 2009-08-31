


//////////////////////////////////////////////////////////////////
//
//	FileName	:	Layer.cpp
//	Author		:	SakuraSinojun
//	Description	:	this is the cavans on which we draw.
//	
//	Version		:	1.0.0.1
//	Date		:	2009.8.30
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////




#include "Layer.h"


CLayer::CLayer()
	:m_hdc(NULL)
{
}

CLayer::~CLayer() 
{
}

BOOL CLayer::SetDC (HDC hdc)
{
	if(!m_hdc)
	{
		m_hdc=hdc;
		return TRUE;
	}
	return FALSE;
}


void CLayer::DrawBackground(PBKGNDPARAMETER pBGParameter)
{
	HDC memDC=::CreateCompatibleDC (this->m_hdc );
	//HBITMAP memBitmap=::CreateCompatibleBitmap (m_hdc,GAME_WINDOW_WIDTH,GAME_WINDOWHEIGHT);
	
	PBKGNDPARAMETER  p=pBGParameter;
	HBITMAP hBitmap;
	BITMAP	bitmap;


	if(p->b4pics)
	{
		hBitmap=(HBITMAP)::LoadImage(NULL,p->part_topleft  ,
									IMAGE_BITMAP,0,0,
									LR_LOADFROMFILE | LR_CREATEDIBSECTION);
 		::SelectObject (memDC,hBitmap);
		::GetObject (hBitmap,sizeof(BITMAP),&bitmap);
		::BitBlt (m_hdc,0,0,p->point.x ,p->point.y,memDC,bitmap.bmWidth-p->point.x,bitmap.bmHeight -p->point.y,SRCCOPY);
		::DeleteObject (hBitmap);
		hBitmap=(HBITMAP)::LoadImage(NULL,p->part_topright  ,
									IMAGE_BITMAP,0,0,
									LR_LOADFROMFILE | LR_CREATEDIBSECTION);
 		::SelectObject (memDC,hBitmap);
		::GetObject (hBitmap,sizeof(BITMAP),&bitmap);
		::BitBlt (m_hdc,p->point.x ,0,GAME_WINDOW_WIDTH,p->point.y,memDC,0,bitmap.bmHeight -p->point.y,SRCCOPY);
		::DeleteObject (hBitmap);
		hBitmap=(HBITMAP)::LoadImage(NULL,p->part_bottomleft  ,
									IMAGE_BITMAP,0,0,
									LR_LOADFROMFILE | LR_CREATEDIBSECTION);
 		::SelectObject (memDC,hBitmap);
		::GetObject (hBitmap,sizeof(BITMAP),&bitmap);
		::BitBlt (m_hdc,0,p->point.y,p->point.x ,GAME_WINDOW_WIDTH,memDC,bitmap.bmWidth-p->point.x,0,SRCCOPY);
		::DeleteObject (hBitmap);
		hBitmap=(HBITMAP)::LoadImage(NULL,p->part_bottomright  ,
									IMAGE_BITMAP,0,0,
									LR_LOADFROMFILE | LR_CREATEDIBSECTION);
 		::SelectObject (memDC,hBitmap);
		::GetObject (hBitmap,sizeof(BITMAP),&bitmap);
		::BitBlt (m_hdc,p->point.x,p->point.y,GAME_WINDOW_WIDTH ,GAME_WINDOW_HEIGHT,memDC,0,0,SRCCOPY);
		::DeleteObject (hBitmap);
	}else{
		hBitmap=(HBITMAP)::LoadImage(NULL,p->part_topleft  ,
									IMAGE_BITMAP,0,0,
									LR_LOADFROMFILE | LR_CREATEDIBSECTION);
 		::SelectObject (memDC,hBitmap);
		::BitBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,memDC,0,0,SRCCOPY);
		::DeleteObject (hBitmap);
	}
	::DeleteDC(memDC);


}

void CLayer::DrawCharacter(LPCTSTR filename)
{
	BITMAP bitmap;

	HDC memDC=::CreateCompatibleDC (this->m_hdc);
	HBITMAP hBitmap=(HBITMAP)::LoadImage(NULL,filename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	::SelectObject (memDC,hBitmap);
	::GetObject (hBitmap,sizeof(bitmap),&bitmap);

	::StretchBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,memDC,0,0,1,1,SRCCOPY);
	
	::BitBlt (m_hdc,(GAME_WINDOW_WIDTH-bitmap.bmWidth)/2-1,(GAME_WINDOW_HEIGHT-bitmap.bmHeight)/2-1,bitmap.bmWidth ,bitmap.bmHeight ,memDC,0,0,SRCCOPY); 

	::DeleteObject (hBitmap);
	::DeleteDC(memDC);


}

void CLayer::DrawMonster(LPCTSTR filename,CRect rectInPic,CPoint point2Draw)
{
	
	HDC memDC=::CreateCompatibleDC(this->m_hdc);
	HBITMAP hBitmap=(HBITMAP)::LoadImage(NULL,filename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	::SelectObject (memDC,hBitmap);

	
	::StretchBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,memDC,0,0,1,1,SRCCOPY);

	::BitBlt (m_hdc,point2Draw.x,point2Draw.y,rectInPic.Width (),rectInPic.Height (),memDC,rectInPic.left ,rectInPic.top ,SRCCOPY);

	::DeleteObject (hBitmap);
	::DeleteDC (memDC);

}








