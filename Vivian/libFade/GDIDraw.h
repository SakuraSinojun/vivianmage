//////////////////////////////////////////////////////////////////
//
//	FileName	:	GDIDraw.h
//	Author		:	SakuraSinojun
//	Description	:	this is the class to use GDISurface.
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////








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
	bool		Draw();		//»æÍ¼¡£
	HRESULT		Flip();		//·­Ò³¡£

private:
	HWND		m_hWnd;
	HDC			m_hdc;		//×îÖÕ»æÍ¼Ä¿±ê
	HDC			back;		//±³¾°»æÍ¼Ò³
	CSize		size;		//»æÍ¼Ò³´óÐ¡
	CLayer	*	first;		//»æÍ¼Ò³Á´±íÍ·Ö¸Õë
	HBITMAP		m_bitmap;	//±³¾°»æÍ¼»­²¼

};
