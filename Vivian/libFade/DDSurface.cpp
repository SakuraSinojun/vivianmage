


#include "..\stdafx.h"
#include "DDSurface.h"

#include "DirectDraw.h"
#include "preLoad.h"



#include "Draw.h"

//extern CDirectDraw CDraw::DirectDraw;	//DirectDraw对象。

CDirectDraw * dd;

CDDSurface::CDDSurface() :
	src_pos(0,0),
	draw_pos(0,0),
	size(0,0),
	surface(0),
	show(false),
	colorkey(false),
	backbuffer(false),
	dst_bits(NULL),
	painter(NULL)
{
	iFadeLevel=255;
	dd=&CDraw::GetDirectDraw ();
}

CDDSurface::~CDDSurface() 
{
	if(surface && !backbuffer)
		surface->Release ();

	if(dst_bits!=NULL)
	{
		delete dst_bits;
	}


}


//取得bitmask的分量
static int BitShift(DWORD bits)
{
	for(int i=0;i<32;i++)
	{
		if(bits &1)
			return i;
		bits>>=1;
	}
	return -1;
}


//建立绘图页
HRESULT CDDSurface::Create(int width, int height) 
{
	bool sysmem=false;
	DDSURFACEDESC ddsd;
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize =sizeof(ddsd);
	ddsd.dwFlags =DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps =DDSCAPS_OFFSCREENPLAIN;
	if(sysmem)
		ddsd.ddsCaps.dwCaps |=DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth =width;
	ddsd.dwHeight =height;
	size.cx =width;
	size.cy=height;
	draw_size=size;
	HRESULT result=(*dd)->CreateSurface(&ddsd,&surface,0);
	if(result==DD_OK)
	{
		DDBLTFX ddbltfx;
		ZeroMemory(&ddbltfx,sizeof(ddbltfx));
		ddbltfx.dwSize=sizeof(ddbltfx);
		ddbltfx.dwFillColor=0;
		surface->Blt(NULL,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);	
		result=GetPixelFormat();
	}
	return result;

}


//建立主绘图页
HRESULT CDDSurface::CreatePrimary() 
{
	DDSURFACEDESC	ddsd;
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize =sizeof(ddsd);
	ddsd.dwFlags =DDSD_CAPS| DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps =DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount =1;
	HRESULT result=(*dd)->CreateSurface (&ddsd,&surface,0);
	if(result==DD_OK)
		result=GetPixelFormat();
	size=dd->GetSize ();
	
	return result;
}

//取得背景绘图页
HRESULT CDDSurface::AttachBackBuffer(CDDSurface &_surface) 
{
	backbuffer=true;
	DDSCAPS ddscaps;
	ZeroMemory(&ddscaps,sizeof(ddscaps));
	ddscaps.dwCaps =DDSCAPS_BACKBUFFER;
	HRESULT result=_surface->GetAttachedSurface(&ddscaps,&surface);
	if(result==DD_OK)
		result=GetPixelFormat();
	size=_surface.GetSize ();
	return result;

}


//取得像素格式
HRESULT CDDSurface::GetPixelFormat() 
{
	memset(&pixelformat,0,sizeof(pixelformat));
	pixelformat.dwSize =sizeof(pixelformat);
	HRESULT result=surface->GetPixelFormat (&pixelformat);
	rshift=BitShift(pixelformat.dwRBitMask );
	rrshift=8-BitShift(~(pixelformat.dwRBitMask >>rshift));
	gshift=BitShift(pixelformat.dwGBitMask );
	grshift=8-BitShift(~(pixelformat.dwGBitMask >>gshift));
	bshift=BitShift(pixelformat.dwBBitMask );
	brshift=8-BitShift(~(pixelformat.dwBBitMask >>bshift));
	return result;
}


HRESULT CDDSurface::Release() 
{
	HRESULT result=DD_OK;
	if(surface && !backbuffer)
		result=surface->Release ();
	surface=0;
	return result;
}

//恢复Lost的绘图页
HRESULT CDDSurface::Restore()
{
	if(surface && !backbuffer)
	{
		HRESULT result=surface->Restore ();
		if(result==DD_OK)
			return RestoreImage();
		return result;
	}
	return DD_OK;
}

//重新读入图片
HRESULT CDDSurface::RestoreImage ()
{
	if(!path.empty ())
		return Load();
	return DD_OK;
}

//绘图
HRESULT CDDSurface::Draw(LPDIRECTDRAWSURFACE draw, DDSURFACEDESC &ddsd) 
{
	if(painter!=NULL)
	{
		HDC hdc;
		draw->GetDC(&hdc);
		painter->prePaint (hdc);
		draw->ReleaseDC(hdc);
	}

	if(!show || !surface)
		return DD_OK;
	int width=ddsd.dwWidth ;
	int height=ddsd.dwHeight ;


	CRect dst(draw_pos.x,draw_pos.y,draw_pos.x+draw_size.cx,draw_pos.y+draw_size.cy);
	if(dst.left >=width || dst.right <=0 || dst.top >=height || dst.bottom <=0)				
		return DD_OK;

	if(dst.left <0)
		dst.left =0;
	if(dst.right >width)
		dst.right=width;
	if(dst.top<0)
		dst.top =0;
	if(dst.bottom >height)
		dst.bottom =height;

	CRect src(CPoint(src_pos.x+dst.left-draw_pos.x,src_pos.y+dst.top-draw_pos.y),dst.Size ());
	

	DDBLTFX ddbltfx;
	ZeroMemory(&ddbltfx,sizeof(ddbltfx));
	ddbltfx.dwSize =sizeof(ddbltfx);

	
	HRESULT res;

	SetColorKey(colorkey);

	if(this->iFadeLevel>=255 || iFadeLevel<=0)
	{
		res=draw->Blt (&dst,surface,&src,DDBLT_WAIT | (colorkey?DDBLT_KEYSRC:0),&ddbltfx);
	}else{
		ColorKeyFade(draw,this->Get());
		res=DD_OK;
	}
	
	if(painter!=NULL)
	{
		HDC hdc;
		draw->GetDC(&hdc);
		painter->OnPaint (hdc);
		draw->ReleaseDC(hdc);
	}
	
	return res;

}


void CDDSurface::ColorKeyFade(LPDIRECTDRAWSURFACE lps1, LPDIRECTDRAWSURFACE lps3)
{

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
	rc_src.right =rc_src.left+draw_size.cx;
	rc_src.bottom =rc_src.top+draw_size.cy;


	DDSURFACEDESC sppp1;
	DDSURFACEDESC sppp3;

	memset(&sppp1,0,sizeof(sppp1));
	memset(&sppp3,0,sizeof(sppp3));


	sppp1.dwSize=sizeof(sppp1);
	sppp3.dwSize=sizeof(sppp3);

	//锁定表面
	lps1->Lock(NULL, &sppp1, DDLOCK_WAIT, NULL);
	lps3->Lock(NULL, &sppp3, DDLOCK_WAIT, NULL);


	//获取像素
	char *dst;
	char *src;
	
	char *dst_buffer=(char *)sppp1.lpSurface;
	char *src_buffer=(char *)sppp3.lpSurface ;

	int lp3=(int)sppp3.lPitch;
	int lp1=(int)sppp1.lPitch;

	int depth=this->Depth ()/8;

	COLORREF t1;
	COLORREF t2;

	BYTE r1,r2;
	BYTE g1,g2;
	BYTE b1,b2;

	int i,j;
	
	ptcolor=0;
	memcpy(&ptcolor,src_buffer,depth);

	for(j=rc_dst.top ;j<rc_dst.bottom;j++)
	{
		for(i=rc_dst.left ;i<rc_dst.right ;i++)
		{
			
			dst=dst_buffer+j*lp1+i*depth;
			src=src_buffer+(rc_src.top+j-rc_dst.top)*lp3+(rc_src.left+i-rc_dst.left)*depth;
			
			if(colorkey)
			{
				t2=0;
				memcpy(&t2,src,depth);
				if(t2==ptcolor)
					continue;
			}

			t1=0;
			t2=0;
			memcpy(&t1,dst,depth);
			memcpy(&t2,src,depth);
			
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
			memcpy(dst,&t1,depth);
		
		}
	}

	//释放表面
	lps3->Unlock(0);
	lps1->Unlock(0);



}



void CDDSurface::SetFadeLevel(int level)
{
	iFadeLevel=level;
	
	/*
	if(level==255)
		return;
	if(size.cx== 0 || size.cy==0)
	{
		return;
	}
	if(dst_bits!=NULL)
	{
		delete dst_bits;
	}
	dst_bits=(char *)NEW DWORD[size.cx *size.cy];
	*/

}



//加载BMP文件
HRESULT CDDSurface::Load(const char * name) 
{

	bool sysmem=false;

	HDC memDC=pl::Load(name,&bitmap);
	m_hdc=memDC;

	path=name;
	HRESULT result=DD_OK;
	if(!surface)
	{
		result=Create(bitmap.bmWidth ,bitmap.bmHeight);
		if(result!=DD_OK)
			return result;
	}

	HDC hdc;
	result=surface->GetDC(&hdc);
	if(result==DD_OK)
	{
		result=BitBlt(hdc,0,0,bitmap.bmWidth ,bitmap.bmHeight ,memDC,0,0,SRCCOPY);
		surface->ReleaseDC (hdc);
	}
	
	ptcolor=::GetPixel (memDC,1,1);


	return result;

}

//再次加载bmp
HRESULT CDDSurface::Load()
{
	HDC memDC=pl::Load(path.c_str(),&bitmap);
	m_hdc=memDC;
	
	HRESULT result=DD_OK;
	HDC hdc;

	result=surface->GetDC(&hdc);
	if(result==DD_OK)
	{
		result=BitBlt(hdc,0,0,bitmap.bmWidth ,bitmap.bmHeight ,memDC,0,0,SRCCOPY);
		surface->ReleaseDC (hdc);
	}
	
	ptcolor=::GetPixel (memDC,1,1);

	return result;
}


//设定透明色
HRESULT CDDSurface::SetColorKey(bool bColorKey) 
{
	colorkey=bColorKey;
	HDC hdc;
	surface->GetDC(&hdc);
	this->ptcolor =GetPixel(hdc,1,1);
	surface->ReleaseDC(hdc);

	COLORREF color=this->ptcolor;
	DWORD flag=DDCKEY_SRCBLT;
	if(surface)
	{
		DDCOLORKEY ddck;
		DWORD co=Color(GetRValue(color),GetGValue(color),GetBValue(color));
		ddck.dwColorSpaceLowValue =ddck.dwColorSpaceHighValue =co;
		HRESULT result=surface->SetColorKey (flag,&ddck);
		if(result==DD_OK)
			colorkey=true;
		return result;
	}
	return DDERR_GENERIC;
}

HRESULT CDDSurface::Fade(CDDSurface *s1, CDDSurface *s2, int level)
{
	CSurfaceLock Lock0(*this);
	CSurfaceLock Lock1(*s1);
	CSurfaceLock Lock2(*s2);
	if (!Lock0.IsOK() || !Lock1.IsOK() || !Lock2.IsOK())
		return DDERR_GENERIC;

	switch (Depth()) {
	  case 16:
		{
			for (int y = 0; y < size.cy; y++) {
				WORD *p = (WORD *)Lock0.GetBits(0, y);
				const WORD *q1 = (const WORD *)Lock1.GetBits(0, y);
				const WORD *q2 = (const WORD *)Lock2.GetBits(0, y);
				for (int x = 0; x < size.cx; x++) {
					DWORD data1 = *q1++;
					DWORD data2 = *q2++;
					DWORD r1 = s1->Red(data1);
					DWORD g1 = s1->Green(data1);
					DWORD b1 = s1->Blue(data1);
					DWORD r = r1 + (s2->Red(data2) - r1) * level / 256;
					DWORD g = g1 + (s2->Green(data2) - g1) * level / 256;
					DWORD b = b1 + (s2->Blue(data2) - b1) * level / 256;
					*p++ = (WORD)Color(r, g, b);
				}
			}
		}
		break;

	  case 24:
		{
			for (int y = 0; y < size.cy; y++) {
				BYTE *p = (BYTE *)Lock0.GetBits(0, y);
				const BYTE *q1 = (const BYTE *)Lock1.GetBits(0, y);
				const BYTE *q2 = (const BYTE *)Lock2.GetBits(0, y);
				for (int x = 0; x < size.cx; x++) {
					*p++ = q1[0] + (q2[0] - q1[0]) * level / 256;
					*p++ = q1[1] + (q2[1] - q1[1]) * level / 256;
					*p++ = q1[2] + (q2[2] - q1[2]) * level / 256;
					q1 += 3;
					q2 += 3;
				}
			}
		}
		break;

	  case 32:
		{
			for (int y = 0; y < size.cy; y++) {
				DWORD *p = (DWORD *)Lock0.GetBits(0, y);
				const DWORD *q1 = (const DWORD *)Lock1.GetBits(0, y);		//这里是不是加个COLORKEY的判断。。以仅仅Cross一部分。。
				const DWORD *q2 = (const DWORD *)Lock2.GetBits(0, y);
				for (int x = 0; x < size.cx; x++) {
					DWORD data1 = *q1++;
					DWORD data2 = *q2++;
					DWORD r1 = s1->Red(data1);
					DWORD g1 = s1->Green(data1);
					DWORD b1 = s1->Blue(data1);
					DWORD r = r1 + (s2->Red(data2) - r1) * level / 256;
					DWORD g = g1 + (s2->Green(data2) - g1) * level / 256;
					DWORD b = b1 + (s2->Blue(data2) - b1) * level / 256;
					*p++ = Color(r, g, b);
				}
			}
		}
		break;
	}
	return DD_OK;
}


static __int64 expand64(unsigned data)
{
	return ((__int64)((data << 16) | data) << 32) | ((data << 16) | data);
}


HRESULT CDDSurface::FadeFast(CDDSurface *s1, CDDSurface *s2, int level)
{
	CSurfaceLock Lock0(*this);
	CSurfaceLock Lock1(*s1);
	CSurfaceLock Lock2(*s2);
	if (!Lock0.IsOK() || !Lock1.IsOK() || !Lock2.IsOK())
		return DDERR_GENERIC;

	switch (Depth()) {
	  case 16:
		{
			static __int64 maskr = expand64(pixelformat.dwRBitMask);
			static __int64 maskg = expand64(pixelformat.dwGBitMask);
			static __int64 maskb = expand64(pixelformat.dwBBitMask);
			__int64 lv = expand64(level);
			int len = size.cx / 4;
			for (int y = 0; y < size.cy; y++) {
				BYTE *p = (BYTE *)Lock0.GetBits(0, y);
				const BYTE *q1 = (const BYTE *)Lock1.GetBits(0, y);
				const BYTE *q2 = (const BYTE *)Lock2.GetBits(0, y);
				__asm	{
					mov			edi, dword ptr p
					mov			ebx, dword ptr q1
					mov			esi, dword ptr q2
					mov			edx, len
					movq		mm6, lv
					mov			ecx, this

L16:
					movq		mm0, [esi]
					movq		mm1, mm0
					movq		mm2, mm0
					pand		mm0, maskr			; mm0 = red
					pand		mm1, maskg			; mm1 = green
					pand		mm2, maskb			; mm2 = blue

					movq		mm3, [ebx]
					movq		mm4, mm3
					movq		mm5, mm3
					pand		mm3, maskr			; mm0 = red
					pand		mm4, maskg			; mm1 = green
					pand		mm5, maskb			; mm2 = blue

					movd		mm7, dword ptr [ecx].rshift
					psrlw		mm0, mm7
					psrlw		mm3, mm7
					psubw		mm0, mm3
					pmullw		mm0, mm6
					psraw		mm0, 8
					paddw		mm0, mm3
					psllw		mm0, mm7

					movd		mm7, dword ptr [ecx].gshift
					psrlw		mm1, mm7
					psrlw		mm4, mm7
					psubw		mm1, mm4
					pmullw		mm1, mm6
					psraw		mm1, 8
					paddw		mm1, mm4
					psllw		mm1, mm7

					movd		mm7, dword ptr [ecx].bshift
					psrlw		mm2, mm7
					psrlw		mm5, mm7
					psubw		mm2, mm5
					pmullw		mm2, mm6
					psraw		mm2, 8
					paddw		mm2, mm5
					psllw		mm2, mm7

					por			mm0, mm1
					por			mm0, mm2

					movq		[edi], mm0
					add			edi, 8
					add			esi, 8
					add			ebx, 8
					dec			edx
					jnz			L16
				}
			}
			__asm	emms
		}
		break;

	  case 24:
	  case 32:
		{
			__int64 lv = expand64(level);
			int len = size.cx * Depth() / 8 / 8;
			for (int y = 0; y < size.cy; y++) {
				BYTE *p = (BYTE *)Lock0.GetBits(0, y);
				const BYTE *q1 = (const BYTE *)Lock1.GetBits(0, y);
				const BYTE *q2 = (const BYTE *)Lock2.GetBits(0, y);
				__asm	{
					mov			edi, dword ptr p
					mov			ebx, dword ptr q1
					mov			esi, dword ptr q2
					mov			edx, len
					movq		mm6, lv
					pxor		mm7, mm7

L24:
					movq		mm0, [esi]
					movq		mm1, [ebx]
					movq		mm2, mm0
					movq		mm3, mm1

					punpcklbw	mm0, mm7
					punpcklbw	mm1, mm7
					punpckhbw	mm2, mm7
					punpckhbw	mm3, mm7

					psubw		mm0, mm1
					psubw		mm2, mm3
					pmullw		mm0, mm6
					pmullw		mm2, mm6
					psrlw		mm0, 8
					psrlw		mm2, 8
					paddb		mm0, mm1
					paddb		mm2, mm3

					packuswb	mm0, mm2

					movq		[edi], mm0
					add			edi, 8
					add			esi, 8
					add			ebx, 8
					dec			edx
					jnz			L24
				}
			}
			__asm	emms
		}
		break;
	}
	return DD_OK;
}


void * CDDSurface::Add()
{
	return (*dd).Add (this);
}


