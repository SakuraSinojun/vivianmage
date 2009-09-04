#pragma once

#include "GDISurface.h"
#include "..\stdafx.h"
#include "Layer.h"


class CGDIDraw
{
public:
	CGDIDraw(void);
	~CGDIDraw(void);
	
	bool		Create(HWND hWnd,int width,int height);
	CLayer *	Add(CGDISurface *s);
	void		Remove(CGDISurface *s);
	bool		Draw();
	HRESULT		Flip();

private:
	HWND		m_hWnd;
	HDC			m_hdc;		//最终绘图目标
	HDC			back;		//背景绘图页
	CSize		size;		//绘图页大小
	CLayer	*	first;		//绘图页链表头指针
	HBITMAP		m_bitmap;	//背景绘图画布

};
