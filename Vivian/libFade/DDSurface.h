


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

	LPDIRECTDRAWSURFACE	surface;		//��ͼҳ
	DDPIXELFORMAT		pixelformat;	//���ظ�ʽ
	std::string			path;			//BMP·��
	CPoint				src_pos;		//��Դ����
	CPoint				draw_pos;		//Ŀ������
	CSize				size;			//��ʾ��С
	CSize				draw_size;		//��ͼ��С
	bool				backbuffer;		//���Ǳ�������������Ϊtrue
	bool				show;			//��ʾʱΪtrue;
	bool				colorkey;		//ʹ��colorkeyʱ��ֵΪtrue
	int					rshift;			//��ɫ����
	int					gshift;
	int					bshift;
	int					rrshift;		//�ҷ���
	int					grshift;
	int					brshift;
	COLORREF			ptcolor;		//(1,1)����ɫ��

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


//������ظ�ʽ�ϳ���ɫ
inline DWORD CDDSurface::Color (DWORD r,DWORD g,DWORD b) const
{
	return  ( (r >> rrshift) << rshift ) & pixelformat.dwRBitMask
		  | ( (g >> grshift) << gshift ) & pixelformat.dwGBitMask
		  | ( (b >> brshift) << bshift ) & pixelformat.dwBBitMask;

}

//ȡ�ú�ɫ��
inline DWORD CDDSurface::Red (DWORD color) const
{
	return ( ( color & pixelformat.dwRBitMask ) >> rshift ) << rrshift;
}

//ȡ����ɫ��
inline DWORD CDDSurface::Green (DWORD color) const
{
	return ( ( color & pixelformat.dwGBitMask ) >> gshift ) << grshift;
}

//ȡ����ɫ��
inline DWORD CDDSurface::Blue (DWORD color) const
{
	return ( ( color & pixelformat.dwBBitMask ) >> bshift ) << brshift;
}

//ȡ�ÿ��
inline int CDDSurface::Width() const
{
	return size.cx;
}

//ȡ�ø߶�
inline int CDDSurface::Height () const
{
	return size.cy ;
}

//ȡ��ɫ��
inline int CDDSurface::Depth () const
{
	return pixelformat.dwRGBBitCount ;
}

//ȡ�û�ͼ����
inline void CDDSurface::GetRect(CRect *r) const 
{
	r->left =draw_pos.x;
	r->top =draw_pos.y;
	r->right=draw_pos.x+size.cx;
	r->bottom =draw_pos.y+size.cy;
}

//ȡ�óߴ�
inline CSize CDDSurface::GetSize() const 
{
	return size;
}

//�趨��ͼ��㣨���Ͻǣ�
inline void CDDSurface::SetDrawPos(int x, int y) 
{
	draw_pos.x=x;
	draw_pos.y=y;
}

//����
inline void CDDSurface::SetDrawPos(CPoint point) 
{
	draw_pos=point;
}

//ȡ�û�ͼ���(���Ͻ�)
inline CPoint CDDSurface::GetDrawPos () const
{
	return draw_pos;
}

//�趨��Դ���
inline void CDDSurface::SetSrcPos (int x,int y)
{
	src_pos.x=x;
	src_pos.y=y;
}

//ȡ����Դ���
inline CPoint CDDSurface::GetSrcPos() const 
{
	return src_pos;
}

//��ʾ��ͼҳ
inline void CDDSurface::Show(bool _show)
{
	show=_show;
}

//���û�ͼԴ����
inline void CDDSurface::SetSrcRect (CRect& rect)
{
	SetSrcPos(rect.left,rect.top);
	draw_size=rect.Size();
}

#endif






