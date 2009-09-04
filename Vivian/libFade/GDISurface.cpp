
//////////////////////////////////////////////////////////////////
//
//	FileName	:	GDISurface.cpp
//	Author		:	SakuraSinojun
//	Description	:	this is the cavans on which we draw use GDI.
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.3
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////




#include "GDISurface.h"

#include "preLoad.h"
#include "Draw.h"


CGDISurface::CGDISurface(void):
	src_pos(0,0),
	draw_pos(0,0),
	size(0,0),
	show(false),
	colorkey(false)
{
	iFadeLevel=256;
	m_hbitmap=NULL;
	HDC hdc=::GetDC(NULL);
	m_hdc=::CreateCompatibleDC (hdc);
	::ReleaseDC(NULL,hdc);
}

CGDISurface::~CGDISurface(void)
{
	if(m_hbitmap)
	{
		DeleteObject(m_hbitmap);
		DeleteDC(m_hdc);
	}

}

HRESULT CGDISurface::Create(int width,int height)
{
	if(m_hbitmap!=NULL)
	{
		DeleteObject(m_hbitmap);
		m_hbitmap=NULL;
	}

	HDC hdc=::GetDC(NULL);
	m_hbitmap=::CreateCompatibleBitmap (hdc,width,height);

	if(m_hbitmap)
	{
		SelectObject(m_hdc,m_hbitmap);
	}

	ReleaseDC(NULL,hdc);

	size.cx=width;
	size.cy=height;
	draw_size=size;

	return HRESULT(m_hbitmap);

}

HRESULT CGDISurface::Draw(HDC hdc)
{
	if(!show)
		return 0;

	COLORREF color=::GetPixel (m_hdc,1,1);

	if(iFadeLevel==256)
	{
		if(colorkey)
		{
			::TransparentBlt (hdc,this->draw_pos.x,draw_pos.y,draw_size.cx,draw_size.cy,m_hdc,this->src_pos.x,src_pos.y,draw_size.cx,draw_size.cy,color);
		}else{
			::BitBlt (hdc,draw_pos.x,draw_pos.y,draw_size.cx,draw_size.cy,m_hdc,src_pos.x,src_pos.y,SRCCOPY);
		}
	}else{
		if(colorkey)
		{
		}else{
		}
	}
	return 0;
}

HRESULT CGDISurface::Load(const char * name)
{
	m_hdc=pl::Load(name,&bitmap);
	if(m_hdc)
	{
		if(m_hbitmap!=NULL)
		{
			DeleteObject(m_hbitmap);
			m_hbitmap=NULL;
		}
	}else{
		if(m_hbitmap!=NULL)
		{
			SelectObject(m_hdc,m_hbitmap);
		}
	}
	
	size.cx=bitmap.bmWidth ;
	size.cy=bitmap.bmHeight ;
	draw_size=size;

	lstrcpyA(path,name);
	return HRESULT(m_hbitmap);

}


HRESULT CGDISurface::SetColorKey(COLORREF color)
{
	colorkey=true;
	ptcolor=color;
	return 0;
}

void * CGDISurface::Add()
{

	return CDraw::GetGDIDraw().Add (this);
	
}







