





#pragma once

#ifndef __DirectDraw_h__
#define __DirectDraw_h__

#include <list>

#include "ddraw.h"
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"ddraw.lib")


#include "DDSurface.h"

class CDDSurface;

class CDirectDraw
{
public:
	enum{
		HignColor,
		FullColor,
	};

public:
	CDirectDraw();
	~CDirectDraw();
	bool Create(HWND hWnd,int width,int height,int mode);
	void ReleaseAllObjects();
	HRESULT RestoreAll();
	HRESULT Flip();
	void * Add(CDDSurface *s);
	void Remove(CDDSurface *s);
	virtual bool Draw();
	CSize GetSize() const { return size;}
	LPDIRECTDRAW operator->(){return dd;}
	LPDIRECTDRAWSURFACE& GetDrawable(){return back;}
	LPDIRECTDRAWSURFACE& GetPrimary(){return primary;}

protected:
	typedef std::list<CDDSurface *> surface_list;
	LPDIRECTDRAW		dd;			//DirectDraw¶ÔÏó
	LPDIRECTDRAWSURFACE primary;	//Ö÷»æÍ¼Ò³
	LPDIRECTDRAWSURFACE back;		//±³¾°»æÍ¼Ò³

	surface_list		surface;	//»æÍ¼Ò³ÁÐ±í
	CSize				size;
};




#endif