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
	virtual HRESULT Draw(HDC hdc,HWND hWnd);
	HRESULT Load(const char * name);
	HRESULT SetColorKey(bool bColorKey);
	
	void CGDISurface::ColorKeyFade(HDC hdc,CSize size);

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
	CPoint GetDrawPos()
	{
		return this->draw_pos;
	}

	void SetFadeLevel(int level=255);


	COLORREF GetPTColor(){return ptcolor;}
	HDC Get(){return m_hdc;}
	
	//LPDIRECTDRAWSURFACE operator->(){return surface;}

	void SetPaintWnd(CPaintWnd * _paintwnd);

protected:

	HDC				m_hdc;			//��������λͼ��DC
	HBITMAP			m_hbitmap;		//����λͼ��Ҳ�������ڴ����λͼ��
	BITMAP			bitmap;			//
	char			path[MAX_PATH];	//BMP·��
	CPoint			src_pos;		//��Դ����
	CPoint			draw_pos;		//Ŀ������
	CSize			size;			//��ʾ��С
	CSize			draw_size;		//��ͼ��С
	bool			show;			//��ǰsurface��ʾʱΪtrue;
	bool			colorkey;		//ʹ��colorkeyʱ��ֵΪtrue

	COLORREF		ptcolor;		//(1,1)����ɫ��
	int				iFadeLevel;		//0~255����͸���ȡ�
	char	*		dst_bits;
	char	*		bmp_bits;
	
	CPaintWnd	*	painter;

};


//ȡ�ÿ��
inline int CGDISurface::Width() const
{
	return size.cx;
}

//ȡ�ø߶�
inline int CGDISurface::Height () const
{
	return size.cy ;
}

//ȡ��ɫ��
inline int CGDISurface::Depth () const
{
	return bitmap.bmBitsPixel  ;
}

//ȡ�û�ͼ����
inline void CGDISurface::GetRect(CRect *r) const 
{
	r->left =draw_pos.x;
	r->top =draw_pos.y;
	r->right=draw_pos.x+size.cx;
	r->bottom =draw_pos.y+size.cy;
}

//ȡ�óߴ�
inline CSize CGDISurface::GetSize() const 
{
	return size;
}

//�趨��ͼ��㣨���Ͻǣ�
inline void CGDISurface::SetDrawPos(int x, int y) 
{
	draw_pos.x=x;
	draw_pos.y=y;
}

//����
inline void CGDISurface::SetDrawPos(CPoint point) 
{
	draw_pos=point;
}

//ȡ�û�ͼ���(���Ͻ�)
inline CPoint CGDISurface::GetDrawPos () const
{
	return draw_pos;
}

//�趨��Դ���
inline void CGDISurface::SetSrcPos (int x,int y)
{
	src_pos.x=x;
	src_pos.y=y;
}

//ȡ����Դ���
inline CPoint CGDISurface::GetSrcPos() const 
{
	return src_pos;
}

//��ʾ��ͼҳ
inline void CGDISurface::Show(bool _show)
{
	show=_show;
}

//���û�ͼԴ����
inline void CGDISurface::SetSrcRect (CRect& rect)
{
	SetSrcPos(rect.left,rect.top);
	draw_size=rect.Size();
}
