#pragma once

#include "..\stdafx.h"

#include "Surface.h"


class CGDIDraw;

class CGDISurface : public CSurface
{
public:
	CGDISurface(void);
	~CGDISurface(void);
	HRESULT Create(int width,int height);	
	virtual HRESULT Draw(HDC hdc);
	HRESULT Load(const char * name);
	HRESULT SetColorKey(COLORREF color=RGB(0,255,0));
	
	
	//HRESULT GetPixelFormat();
	//HRESULT Fade(CDDSurface *s1,CDDSurface *s2,int level);
	//HRESULT FadeFast(CDDSurface *s1,CDDSurface *s2,int level);
	/*
	DWORD Color(DWORD r,DWORD g,DWORD b) const;
	DWORD Red(DWORD color)const;
	DWORD Green(DWORD color)const;
	DWORD Blue(DWORD color)const;
	*/
	
	
	void * Add();

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
	void SetFadeLevel(int level=256){iFadeLevel=level;}

	COLORREF GetPTColor(){return ptcolor;}
	HDC Get(){return m_hdc;}
	
	//LPDIRECTDRAWSURFACE operator->(){return surface;}

protected:

	HDC				m_hdc;			//绘图DC
	HBITMAP			m_hbitmap;		//关联位图，也可能是内存兼容位图。
	BITMAP			bitmap;			//
	char			path[MAX_PATH];	//BMP路径
	CPoint			src_pos;		//来源坐标
	CPoint			draw_pos;		//目标坐标
	CSize			size;			//显示大小
	CSize			draw_size;		//绘图大小
	bool			show;			//当前surface显示时为true;
	bool			colorkey;		//使用colorkey时此值为true
	
	/*
	int				rshift;			//红色分量
	int				gshift;
	int				bshift;
	int				rrshift;		//右分量
	int				grshift;
	int				brshift;
	*/
	COLORREF		ptcolor;		//(1,1)点颜色；
	int				iFadeLevel;		//0~256交叉透明度。

};


//取得宽度
inline int CGDISurface::Width() const
{
	return size.cx;
}

//取得高度
inline int CGDISurface::Height () const
{
	return size.cy ;
}

//取得色深
inline int CGDISurface::Depth () const
{
	return bitmap.bmBitsPixel  ;
}

//取得绘图坐标
inline void CGDISurface::GetRect(CRect *r) const 
{
	r->left =draw_pos.x;
	r->top =draw_pos.y;
	r->right=draw_pos.x+size.cx;
	r->bottom =draw_pos.y+size.cy;
}

//取得尺寸
inline CSize CGDISurface::GetSize() const 
{
	return size;
}

//设定绘图起点（左上角）
inline void CGDISurface::SetDrawPos(int x, int y) 
{
	draw_pos.x=x;
	draw_pos.y=y;
}

//重载
inline void CGDISurface::SetDrawPos(CPoint point) 
{
	draw_pos=point;
}

//取得绘图起点(左上角)
inline CPoint CGDISurface::GetDrawPos () const
{
	return draw_pos;
}

//设定来源起点
inline void CGDISurface::SetSrcPos (int x,int y)
{
	src_pos.x=x;
	src_pos.y=y;
}

//取得来源起点
inline CPoint CGDISurface::GetSrcPos() const 
{
	return src_pos;
}

//显示绘图页
inline void CGDISurface::Show(bool _show)
{
	show=_show;
}

//设置绘图源区域
inline void CGDISurface::SetSrcRect (CRect& rect)
{
	SetSrcPos(rect.left,rect.top);
	draw_size=rect.Size();
}
