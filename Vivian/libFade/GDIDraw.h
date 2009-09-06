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
	bool		Draw();		//��ͼ��
	HRESULT		Flip();		//��ҳ��

private:
	HWND		m_hWnd;
	HDC			m_hdc;		//���ջ�ͼĿ��
	HDC			back;		//������ͼҳ
	CSize		size;		//��ͼҳ��С
	CLayer	*	first;		//��ͼҳ����ͷָ��
	HBITMAP		m_bitmap;	//������ͼ����

};
