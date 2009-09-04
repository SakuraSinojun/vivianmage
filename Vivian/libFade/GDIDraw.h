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
	HDC			m_hdc;		//���ջ�ͼĿ��
	HDC			back;		//������ͼҳ
	CSize		size;		//��ͼҳ��С
	CLayer	*	first;		//��ͼҳ����ͷָ��
	HBITMAP		m_bitmap;	//������ͼ����

};
