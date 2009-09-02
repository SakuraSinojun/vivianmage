

#pragma once

#include "..\stdafx.h"


#include "Layer.h"


class CMainWnd
{
public:
	CMainWnd(HWND hWnd);

	~CMainWnd();


public:

	BOOL	OnPaint();
	void	OnCreate();
	void	OnLButtonDown(WPARAM wParam,CPoint point);
	BOOL	OnIdle(LONG lcount);

	void	DrawBackground();
	void	DrawCharacter();
	void	DrawMonster(LPCSTR filename,CRect rectInPic,CPoint point2Draw);
	void	DrawMonster2(LPCSTR filename,CRect rectInPic,CPoint point2Draw);
	void	DrawMonster3(LPCSTR filename,CRect rectInPic,CPoint point2Draw);
	void	DrawChat(LPCSTR filename);

	int		AddText(LPCTSTR filename,LPCTSTR text);
	void	ModifyTextPic(int index,LPCTSTR filename);	
	void	ModifyText(int index,LPCTSTR text);
	void	KillText(int index);

public:
	CLayer	*	m_ly_background;
	CLayer	*	m_ly_character;
	CLayer	*	m_ly_monster;
	CLayer	*	m_ly_mask;
	CLayer	*	m_ly_animate;
	CLayer	*	m_ly_text;


private:

	CLayer m_layer[10];

	HWND	m_hWnd;
	HDC		m_wnd_dc;

	HDC		m_hdc;
	HBITMAP	m_bitmap;
	
	HDC		m_hdc_background;
	HDC		m_hdc_character;
	HDC		m_hdc_monster;
	HDC		m_hdc_mask;
	HDC		m_hdc_animate;
	HDC		m_hdc_text;

	HBITMAP	m_bitmap_background;
	HBITMAP	m_bitmap_character;
	HBITMAP m_bitmap_monster;
	HBITMAP m_bitmap_text;


//temp:
public:
	CPoint m_point;


};








