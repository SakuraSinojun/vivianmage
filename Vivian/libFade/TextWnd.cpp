




#include "TextWnd.h"

#include "..\stdafx.h"



CTextWnd::CTextWnd(void)
{
	this->surface =NULL;
	this->layer=NULL;
	_x=0;
	_y=0;
	_text=NULL;
	
	_bkcolor=RGB(255,255,255);
	_textcolor=0;
	_bkmode=TRANSPARENT;

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


bool CTextWnd::Create(const char *filename)
{
	bool res=CGameWnd::Create (filename);
	surface->SetPaintWnd (this);
	this->SetColorKey (true);

	return 	res;

}

bool CTextWnd::Create(int width,int height)
{
	if(surface!=NULL)
		return false;
	
	surface=CDraw::CreateNewSurface ();
	HRESULT res=surface->Create (width,height);
	surface->Show ();
	surface->Add ();
	surface->SetColorKey (true);
	surface->SetPaintWnd (this);

	return (res!=0);
}

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

	if(surface==NULL)
	{
		Create(640,480);
		surface->SetColorKey(true);
	}


}




