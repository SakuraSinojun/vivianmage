

#pragma once


#include <windows.h>
#include "..\typedef.h"

#include "Layer.h"


class CMainWnd
{
public:
	CMainWnd(HWND hWnd);

	~CMainWnd();


public:

	BOOL	OnPaint();
	void	OnLButtonDown(WPARAM wParam,CPoint point);
	BOOL	OnIdle(LONG lcount);

	void	DrawBackground();
	void	DrawCharacter();
	void	DrawMonster(LPCTSTR filename,CRect rectInPic,CPoint point2Draw);

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


//temp:
public:
	CPoint m_point;

};
