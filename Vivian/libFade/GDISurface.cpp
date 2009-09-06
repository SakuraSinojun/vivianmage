
//////////////////////////////////////////////////////////////////
//
//	FileName	:	GDISurface.cpp
//	Author		:	SakuraSinojun
//	Description	:	this is the cavans on which we draw use GDI.
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////




#include "GDISurface.h"

#include "preLoad.h"
#include "Draw.h"


CGDISurface::CGDISurface(void):
	src_pos(0,0),
	draw_pos(0,0),
	size(0,0),
	show(false),
	colorkey(false),
	dst_bits(NULL),
	bmp_bits(NULL),
	painter(NULL)
{

	iFadeLevel=255;
	m_hbitmap=NULL;
	HDC hdc=::GetDC(NULL);
	m_hdc=::CreateCompatibleDC (hdc);
	::ReleaseDC(NULL,hdc);

}

CGDISurface::~CGDISurface(void)
{
	if(m_hbitmap)
	{
		DeleteObject(m_hbitmap);
	}
	DeleteDC(m_hdc);
	if(dst_bits!=NULL)
	{
		delete dst_bits;
	}
	if(bmp_bits!=NULL)
	{
		delete bmp_bits;
	}

}

//不加载位图而直接建立绘图页。
HRESULT CGDISurface::Create(int width,int height)
{
	if(m_hbitmap!=NULL)
	{
		DeleteObject(m_hbitmap);
		m_hbitmap=NULL;
	}


	HDC hdc=::GetDC(NULL);								//取得屏幕DC
	UINT iPlanes=GetDeviceCaps(hdc,PLANES);				//调色盘
	UINT iBitsPixel=GetDeviceCaps(hdc,BITSPIXEL);		//比特每像素
	
	
	//分配图像内存空间
	if(bmp_bits==NULL)
	{
		bmp_bits=NEW char[width * height *iBitsPixel+1];
	}
	memset(bmp_bits,0,width*height*iBitsPixel+1);

	//建立位图
	m_hbitmap=::CreateBitmap(width,height,iPlanes,iBitsPixel,bmp_bits);

	//按建立的位图填充bitmap结构
	bitmap.bmBits =bmp_bits;
	bitmap.bmBitsPixel =iBitsPixel;
	bitmap.bmHeight =height;
	bitmap.bmWidth =width;
	bitmap.bmPlanes =iPlanes;
	bitmap.bmWidthBytes =width * iBitsPixel;
	bitmap.bmType =0;

	if(m_hbitmap)
	{
		SelectObject(m_hdc,m_hbitmap);
	}

	ReleaseDC(NULL,hdc);

	
	size.cx=width;
	size.cy=height;
	draw_size=size;

	return HRESULT(m_hbitmap);

}


//同时设定了ColorKey和Fade两种渲染效果的渲染函数
void CGDISurface::ColorKeyFade(HDC hdc,CSize size)
{
	if(dst_bits==NULL)
		return;


	CRect rc_dst(draw_pos,draw_size);
	CRect rc_src;
	
	if(rc_dst.left <0)
		rc_dst.left =0;
	if(rc_dst.top <0)
		rc_dst.top =0;
	if(rc_dst.right >size.cx)
		rc_dst.right =size.cx ;
	if(rc_dst.bottom >size.cy)
		rc_dst.bottom =size.cy;

	rc_src.left =src_pos.x+rc_dst.left-draw_pos.x;
	rc_src.top =src_pos.y+rc_dst.top-draw_pos.y;
	rc_src.right =rc_src.left+rc_dst.Width ();
	rc_src.bottom =rc_src.top+rc_dst.Height ();



	int depth1=GetDeviceCaps(hdc,BITSPIXEL)/8; 	
	int depth2=bitmap.bmBitsPixel/8;
	
	HBITMAP hbitmap=::CreateCompatibleBitmap (hdc,size.cx,size.cy);
	HDC		memDC=::CreateCompatibleDC (hdc);
	SelectObject(memDC,hbitmap);
	BitBlt(memDC,0,0,size.cx,size.cy,hdc,0,0,SRCCOPY);
	::GetBitmapBits (hbitmap,size.cx *size.cy *depth1,this->dst_bits);
	
	//演染开始
	
	char *dst;
	char *src;
	
	COLORREF t1;
	COLORREF t2;

	BYTE r1,r2;
	BYTE g1,g2;
	BYTE b1,b2;

	int i;
	int j;
	int od,os;

	char *src_bits=(char *)bitmap.bmBits ;

	ptcolor=0;
	memcpy(&ptcolor,src_bits,depth2);


	for(j=rc_dst.top;j<rc_dst.bottom ;j++)
	{
		for(i=rc_dst.left;i<rc_dst.right;i++)
		{
			od=((j)*size.cx+i)*depth1;
			os=(((bitmap.bmHeight-(j-rc_dst.top)-rc_src.top-1)*bitmap.bmWidth)+ (rc_src.left+i-rc_dst.left))*depth2;
			
			dst=od+dst_bits;
			src=os+src_bits;
			
			t2=0;
			memcpy(&t2,src,depth2);

			if(t2==ptcolor)
				continue;

			t1=0;
			t2=0;
			memcpy(&t1,dst,depth1);
			memcpy(&t2,src,depth2);
			
			r1=GetRValue(t1);
			g1=GetGValue(t1);
			b1=GetBValue(t1);
			r2=GetRValue(t2);
			g2=GetGValue(t2);
			b2=GetBValue(t2);
			
			r1=r1-(r1-r2)*iFadeLevel/256;
			g1=g1-(g1-g2)*iFadeLevel/256;
			b1=b1-(b1-b2)*iFadeLevel/256;

			t1=RGB(r1,g1,b1);
			memcpy(dst,&t1,depth1);
		}
	}

	//演染完毕

	::SetBitmapBits (hbitmap,size.cx * size.cy * depth1,this->dst_bits);
	BitBlt(hdc,0,0,size.cx,size.cy,memDC,0,0,SRCCOPY);
	DeleteObject(hbitmap);
	DeleteDC(memDC);

}

//设定绘图回调类。
void CGDISurface::SetPaintWnd(CPaintWnd * _paintwnd)
{
	painter=_paintwnd;
}

//渲染当前绘图面
HRESULT CGDISurface::Draw(HDC hdc,HWND hWnd)
{
	//预绘制
	if(painter!=NULL)
	{
		painter->prePaint (hdc);
	}

	//当前页显示与否
	if(!show)
		return 0;

	CRect rc_dst(draw_pos,draw_size);
	CRect rc_src;
	
	CRect rc;
	::GetClientRect (hWnd,&rc);

	if(rc_dst.left <0)
		rc_dst.left =0;
	if(rc_dst.top <0)
		rc_dst.top =0;
	if(rc_dst.right >rc.Width())				//这里应该是绘图页width
		rc_dst.right =rc.Width() ;
	if(rc_dst.bottom >rc.Height ())				//这里也应该是绘图页height
		rc_dst.bottom =rc.Height ();

	rc_src.left =src_pos.x+rc_dst.left-draw_pos.x;
	rc_src.top =src_pos.y+rc_dst.top-draw_pos.y;
	rc_src.right =rc_src.left+draw_size.cx;
	rc_src.bottom =rc_src.top+draw_size.cy;


	COLORREF color=::GetPixel (m_hdc,1,1);

	if(iFadeLevel>=255 || iFadeLevel<=0)
	{
		if(colorkey)
		{
			::TransparentBlt (hdc,rc_dst.left,rc_dst.top,rc_dst.Width (),rc_dst.Height(),m_hdc,rc_src.left,rc_src.top,rc_dst.Width(),rc_dst.Height(),color);
		}else{
			::BitBlt (hdc,rc_dst.left,rc_dst.top,rc_dst.Width (),rc_dst.Height(),m_hdc,rc_src.left,rc_src.top,SRCCOPY);
		}
	}else{
		if(colorkey)
		{
			ColorKeyFade(hdc,CSize(rc.Width(),rc.Height()));
		}else{
			BLENDFUNCTION bf;
			bf.AlphaFormat =0;
			bf.BlendFlags =0;
			bf.BlendOp =AC_SRC_OVER;
			bf.SourceConstantAlpha =iFadeLevel ;
			::AlphaBlend (hdc,rc_dst.left,rc_dst.top,rc_dst.Width (),rc_dst.Height(),m_hdc,rc_src.left,rc_src.top,rc_dst.Width(),rc_dst.Height(),bf);
		}
	}
	

	//绘图结束，OnPaint后续处理
	if(painter!=NULL)
	{
		painter->OnPaint (hdc);
	}

	return 0;

}

//设定透明度，0~255。0和255为不透明。
void CGDISurface::SetFadeLevel(int level)
{
	iFadeLevel=level;
	if(level!=255)
	{
		if(size.cx== 0 || size.cy==0)
		{
			return;
		}
		if(dst_bits!=NULL)
		{
			delete dst_bits;
		}
		dst_bits=(char *)NEW DWORD[800 *600];
	}
}

//加载位图到绘图页。
HRESULT CGDISurface::Load(const char * name)
{

	m_hdc=pl::Load(name,&bitmap);
	if(m_hdc)
	{
		if(m_hbitmap!=NULL)
		{
			DeleteObject(m_hbitmap);
			m_hbitmap=NULL;
		}
	}else{
		if(m_hbitmap!=NULL)
		{
			SelectObject(m_hdc,m_hbitmap);
		}
	}
	
	size.cx=bitmap.bmWidth ;
	size.cy=bitmap.bmHeight ;
	draw_size=size;

	lstrcpyA(path,name);
	
	return HRESULT(m_hdc);

}

//设置透明色。
HRESULT CGDISurface::SetColorKey(bool bColorKey)
{

	colorkey=bColorKey;
	ptcolor=GetPixel(m_hdc,1,1);
	return 0;

}

//将绘图页添加至绘图列表中。
void * CGDISurface::Add()
{
	return CDraw::GetGDIDraw().Add (this);	
}







