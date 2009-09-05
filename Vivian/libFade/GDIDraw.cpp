#include "GDIDraw.h"

CGDIDraw::CGDIDraw(void)
{
	m_hdc=NULL;
	m_bitmap=NULL;
	back=NULL;
	m_hWnd=NULL;

}

CGDIDraw::~CGDIDraw(void)
{
	if(back)
	{
		DeleteDC(back);
	}
	if(m_bitmap)
	{
		DeleteObject(m_bitmap);
	}
	
	::ReleaseDC(m_hWnd,m_hdc);

}

bool CGDIDraw::Create(HWND hWnd,int width,int height)
{
	m_hdc=::GetDC(hWnd);
	m_hWnd=hWnd;

	size.cx=width;
	size.cy=height;

	if(m_bitmap)
	{
		DeleteObject(m_bitmap);
	}
	m_bitmap=::CreateCompatibleBitmap (m_hdc,width,height);
	
	if(m_bitmap==NULL)return false;

	if(back==NULL)
	{
		back=::CreateCompatibleDC (m_hdc);
		if(back==NULL)return false;
	}
	SelectObject(back,m_bitmap);
	return true;

}

CLayer * CGDIDraw::Add(CGDISurface *s)
{
	return CLayer::AddLayer ((void *)s);
}

void CGDIDraw::Remove(CGDISurface *s)
{
	CLayer * _layer=CLayer::first;
	CLayer * _ly;

	while(_layer!=NULL)
	{
		_ly=_layer->next;
		if(_layer->Get()==(void *)s)
			delete _layer;
		_layer=_ly;
	}

}

bool CGDIDraw::Draw()
{
	CLayer * _layer=CLayer::first ;
	while(_layer!=NULL)
	{
		((CGDISurface *)(_layer->Get()))->Draw (back,this->m_hWnd);
		_layer=_layer->next;
	}	
	return true;
}

HRESULT CGDIDraw::Flip()
{
	BitBlt(m_hdc,0,0,size.cx,size.cy,back,0,0,SRCCOPY);
	return 1;
}


