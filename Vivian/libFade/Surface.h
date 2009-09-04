

#pragma once

#include "..\stdafx.h"

class CSurface
{
public:
	virtual ~CSurface(){}
	
	virtual HRESULT Create(int width,int height)=0;
	virtual HRESULT Load(const char * name)=0;
	virtual HRESULT SetColorKey(COLORREF color=RGB(0,255,0))=0;

	virtual void SetDrawPos(int x,int y)=0;
	virtual void SetDrawPos(CPoint point)=0;
	virtual void SetSrcPos(int x,int y)=0;
	virtual void SetSrcRect(CRect& rect)=0;
	virtual void Show(bool _show=true)=0;
	virtual void * Add()=0;

	//void SetFadeLevel(int level=256);

protected:
	CSurface(){}
	

};