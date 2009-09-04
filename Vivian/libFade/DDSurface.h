


//////////////////////////////////////////////////////////////////
//
//	FileName	:	DDSurface.h
//	Author		:	SakuraSinojun
//	Description	:	DirectDrawSurface
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////



#pragma once

#ifndef __DDSerface_h__
#define __DDSerface_h__

//#include <windows.h>
#include "..\StdAfx.h"


#include "ddraw.h"
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"ddraw.lib")

#include <string>

#include "Surface.h"

class CDirectDraw;
//class CDibSection;

class CDDSurface:public CSurface
{
public:
	CDDSurface();
	~CDDSurface();

	HRESULT Create(int width,int height);
	HRESULT CreatePrimary();
	HRESULT AttachBackBuffer(CDDSurface &_surface);
	HRESULT Restore();
	HRESULT Release();
	virtual HRESULT RestoreImage();
	virtual HRESULT Draw(LPDIRECTDRAWSURFACE draw,DDSURFACEDESC &ddsd);
	HRESULT Load(const char * name );
	HRESULT GetPixelFormat();
	HRESULT Fade(CDDSurface *s1,CDDSurface *s2,int level);
	HRESULT FadeFast(CDDSurface *s1,CDDSurface *s2,int level);

	HRESULT SetColorKey(COLORREF color=RGB(0,255,0));
	DWORD Color(DWORD r,DWORD g,DWORD b) const;
	DWORD Red(DWORD color)const;
	DWORD Green(DWORD color)const;
	DWORD Blue(DWORD color)const;

	int Width() const;
	int Height() const;
	int Depth() const;
	void GetRect(CRect *r) const;
	CSize GetSize() const;
	void SetDrawPos(int x,int y);
	void SetDrawPos(CPoint point);
	CPoint GetDrawPos() const;
	void  SetSrcPos(int x,int y);
	CPoint GetSrcPos()const;
	void SetSrcRect(CRect& rect);
	void Show(bool _show=true);

	COLORREF GetPTColor(){return ptcolor;}
	LPDIRECTDRAWSURFACE Get(){return surface;}
	LPDIRECTDRAWSURFACE operator->(){return surface;}

	void * Add();

protected:
	HRESULT Load();

protected:

	LPDIRECTDRAWSURFACE	surface;		//绘图页
	DDPIXELFORMAT		pixelformat;	//像素格式
	std::string			path;			//BMP路径
	CPoint				src_pos;		//来源坐标
	CPoint				draw_pos;		//目标坐标
	CSize				size;			//显示大小
	CSize				draw_size;		//绘图大小
	bool				backbuffer;		//若是背景缓冲区，则为true
	bool				show;			//显示时为true;
	bool				colorkey;		//使用colorkey时此值为true
	int					rshift;			//红色分量
	int					gshift;
	int					bshift;
	int					rrshift;		//右分量
	int					grshift;
	int					brshift;
	COLORREF			ptcolor;		//(1,1)点颜色；

};

class CSurfaceLock
{
public:
	CSurfaceLock(CDDSurface & _surface)
	{
		memset(&ddsd,0,sizeof(ddsd));
		ddsd.dwSize =sizeof(ddsd);
		surface=_surface.Get();
		for(int retry=0;retry<2;retry++)
		{
			result=surface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
			if(result==DD_OK)
			{
				bytes_per_pixel=(ddsd.ddpfPixelFormat.dwRGBBitCount+7)/8;
				bytes_per_line=ddsd.lPitch;
			}
			if(result!=DDERR_SURFACELOST)
				break;
			surface->Release ();
		}

	}
	~CSurfaceLock()
	{
		if(result==DD_OK)
			surface->Unlock (NULL);
	}
	bool IsOK() const{return result==DD_OK;}
	HRESULT Result()const{return result;}
	void *GetBits(int x,int y)const
	{
		return (void*)((char*)ddsd.lpSurface +y*bytes_per_line+x*bytes_per_pixel);
	}

protected:
	LPDIRECTDRAWSURFACE	surface;
	DDSURFACEDESC		ddsd;
	HRESULT				result;
	int					bytes_per_pixel;
	int					bytes_per_line;
};


//配合像素格式合成颜色
inline DWORD CDDSurface::Color (DWORD r,DWORD g,DWORD b) const
{
	return  ( (r >> rrshift) << rshift ) & pixelformat.dwRBitMask
		  | ( (g >> grshift) << gshift ) & pixelformat.dwGBitMask
		  | ( (b >> brshift) << bshift ) & pixelformat.dwBBitMask;

}

//取得红色。
inline DWORD CDDSurface::Red (DWORD color) const
{
	return ( ( color & pixelformat.dwRBitMask ) >> rshift ) << rrshift;
}

//取得绿色。
inline DWORD CDDSurface::Green (DWORD color) const
{
	return ( ( color & pixelformat.dwGBitMask ) >> gshift ) << grshift;
}

//取得蓝色。
inline DWORD CDDSurface::Blue (DWORD color) const
{
	return ( ( color & pixelformat.dwBBitMask ) >> bshift ) << brshift;
}

//取得宽度
inline int CDDSurface::Width() const
{
	return size.cx;
}

//取得高度
inline int CDDSurface::Height () const
{
	return size.cy ;
}

//取得色深
inline int CDDSurface::Depth () const
{
	return pixelformat.dwRGBBitCount ;
}

//取得绘图坐标
inline void CDDSurface::GetRect(CRect *r) const 
{
	r->left =draw_pos.x;
	r->top =draw_pos.y;
	r->right=draw_pos.x+size.cx;
	r->bottom =draw_pos.y+size.cy;
}

//取得尺寸
inline CSize CDDSurface::GetSize() const 
{
	return size;
}

//设定绘图起点（左上角）
inline void CDDSurface::SetDrawPos(int x, int y) 
{
	draw_pos.x=x;
	draw_pos.y=y;
}

//重载
inline void CDDSurface::SetDrawPos(CPoint point) 
{
	draw_pos=point;
}

//取得绘图起点(左上角)
inline CPoint CDDSurface::GetDrawPos () const
{
	return draw_pos;
}

//设定来源起点
inline void CDDSurface::SetSrcPos (int x,int y)
{
	src_pos.x=x;
	src_pos.y=y;
}

//取得来源起点
inline CPoint CDDSurface::GetSrcPos() const 
{
	return src_pos;
}

//显示绘图页
inline void CDDSurface::Show(bool _show)
{
	show=_show;
}

//设置绘图源区域
inline void CDDSurface::SetSrcRect (CRect& rect)
{
	SetSrcPos(rect.left,rect.top);
	draw_size=rect.Size();
}

#endif






