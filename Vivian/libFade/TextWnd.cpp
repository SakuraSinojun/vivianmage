

//////////////////////////////////////////////////////////////////
//
//	FileName	:	TextWnd.cpp
//	Author		:	SakuraSinojun
//	Description	:	this class is used to draw text on the surface
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////




#include "TextWnd.h"

#include "..\stdafx.h"



CTextWnd::CTextWnd(void)
{
	_x=0;
	_y=0;
	_text=NULL;
	
	_bkcolor=RGB(255,255,255);
	_textcolor=0;
	_bkmode=TRANSPARENT;

	SetColorKey(true);
	ShowWindow(true);
}

CTextWnd::~CTextWnd(void)
{
	if(_text!=NULL)
	{
		delete[] _text;
	}
}


void CTextWnd::OnPaint(HDC hdc)
{
	::SetBkColor (hdc,_bkcolor);
	::SetTextColor (hdc,_textcolor);
	::SetBkMode (hdc,_bkmode);
	::TextOutA(hdc,_x,_y,_text,lstrlenA(_text));
}

void CTextWnd::prePaint (HDC hdc)
{

}


//加载背景位图
bool CTextWnd::Create(const char *filename)
{
	bool res=CGameWnd::Load (filename);
	this->SetColorKey (true);
	return 	res;
}

//不加载位图而仅仅建立背景页。
bool CTextWnd::Create(int width,int height)
{
	HRESULT res=CGameWnd::Create (width,height);
	ShowWindow(true);
	SetColorKey (true);

	return (res!=0);
}

//创建文字
void CTextWnd::CreateText(int x,int y,const char * text)
{
	_x=x;
	_y=y;
	int l=lstrlenA(text);

	if(_text!=NULL)
	{
		delete[] _text;
	}
	_text=NEW char[l+1];
	lstrcpyA(_text,text);

	SetColorKey(true);
	ShowWindow(true);


}




