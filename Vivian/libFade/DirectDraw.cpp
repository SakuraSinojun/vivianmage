

#include "..\stdafx.h"

#include "DirectDraw.h"

//#include <list>
#include <algorithm>
#include <functional>

#include "Layer.h"


using namespace std;



CDirectDraw::CDirectDraw ()
{
	dd=NULL;
	primary=NULL;
	back=NULL;
}

CDirectDraw::~CDirectDraw() 
{
	//exit(0);
	//ReleaseAllObjects();

}



//向列表中添加绘图页
void * CDirectDraw::Add(CDDSurface *s) 
{
	//surface.push_back (s);
	return CLayer::AddLayer ((void *)s);
}

//从列表中删除
void CDirectDraw::Remove(CDDSurface *s) 
{
	//surface.remove(s);
	CLayer * _layer=CLayer::first;
	CLayer * _ly;

	while(_layer!=NULL)
	{
		_ly=_layer->next;
		if(_layer->Get()==(void *)s)
			delete _layer;
		_layer=_ly;
	}
}


//建立DirectDraw对象
bool CDirectDraw::Create(HWND hWnd, int width, int height, int mode) 
{
	//建立DirectDraw
	//HRESULT result = DirectDrawCreateEx(NULL,(VOID **)&dd,IID_IDirectDraw,NULL);
	HRESULT result=DirectDrawCreate(NULL,&dd,NULL);
	if(result !=DD_OK)
	{
		TRACE("DirectDrawCreate failed");
		return false;
	}
	
	//FullScreen
	result=dd->SetCooperativeLevel (hWnd,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	if(result !=DD_OK)
	{
		TRACE("SetCooperativeLevel failed.");
		return false;
	}

	
	//更改显示模式
	if(mode==FullColor)
	{
		result=dd->SetDisplayMode (width,height,32);
		if(result !=DD_OK)
			result=dd->SetDisplayMode (width,height,24);
	}else{
		result=dd->SetDisplayMode (width,height,16);
	}
	if(result!=DD_OK)
	{
		TRACE("SetDisplayMode failed.");
		return false;
	}
	

	//建立主绘图页
	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize =sizeof(ddsd);
	ddsd.dwFlags =DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps =DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount =1;
	result=dd->CreateSurface (&ddsd,&primary,NULL);
	if(result!=DD_OK)
	{
		TRACE("CreateSurface failed.");
		return false;
	}

	//建立背景缓冲区
	DDSCAPS ddscaps;
	::ZeroMemory (&ddscaps,sizeof(ddscaps));
	ddscaps.dwCaps =DDSCAPS_BACKBUFFER;
	result=primary->GetAttachedSurface(&ddscaps,&back);
	if(result!=DD_OK)
	{
		TRACE("GetAttachedSurface failed.");
		return false;
	}
	return true;

}

//释放所有图象
void CDirectDraw::ReleaseAllObjects() 
{
	if(dd)
	{
		//for_each(surface.begin(),surface.end(),mem_fun(&CDDSurface::Release));
		CLayer * _layer=CLayer::first;
		while(_layer!=NULL)
		{
			((CDDSurface *)_layer->Get())->Release();
			_layer=_layer->next;
		}
		if(primary)
		{
			primary->Release();
			primary=NULL;
		}
		dd->Release();
		dd=NULL;
	}
}


HRESULT CDirectDraw::RestoreAll() 
{
	//for_each(surface.begin (),surface.end(),mem_fun(&CDDSurface::Restore));
	
	CLayer * _layer=CLayer::first;
	while(_layer!=NULL)
	{
		((CDDSurface *)_layer->Get())->Restore ();
		_layer=_layer->next;
	}


	HRESULT result=primary->Restore ();
	if(result!=DD_OK)
		return result;
	return DD_OK;
}


//绘图
bool CDirectDraw::Draw ()
{
#ifndef CLEAR_BEFORE_DRAWING
#define CLEAR_BEFORE_DRAWING
#endif
#ifdef CLEAR_BEFORE_DRAWING
	DDBLTFX ddbltfx;
	ZeroMemory(&ddbltfx,sizeof(ddbltfx));
	ddbltfx.dwSize=sizeof(ddbltfx);
	ddbltfx.dwFillColor=0;
	back->Blt(NULL,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
#endif
	DDSURFACEDESC ddsd;
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize =sizeof(ddsd);
	back->GetSurfaceDesc(&ddsd);
	
	DWORD result=DD_OK;
	do
	{
		/*
		for(surface_list::iterator p=surface.begin ();p!=surface.end ();p++)
		{
			result=(*p)->Draw(back,ddsd);
			if(result==DDERR_SURFACELOST)
			{
				if(RestoreAll()!=DD_OK)
					return false;
				break;
			}else if(result!=DD_OK){
				TRACE("ERROR!");
				return false;
			}
		}
		*/

		CLayer * _layer=CLayer::first;	
		while(_layer!=NULL)
		{
			result=((CDDSurface *)_layer->Get())->Draw (back,ddsd);
			if(result==DDERR_SURFACELOST)
			{
				if(RestoreAll()!=DD_OK)
					return false;
				break;
			}else if(result!=DD_OK){
				TRACE("DIRECT DRAW DRAW ERROR!");
				return false;
			}
			_layer=_layer->next;
		}

	}while(result!=DD_OK);
	return true;
}


//Flip
HRESULT CDirectDraw::Flip ()
{
	HRESULT result=DD_OK;

	while(true)
	{
		result=primary->Flip(NULL,0);
		if(result==DD_OK)
			break;
		if(result==DDERR_SURFACELOST)
		{
			result=RestoreAll();
			if(result!=DD_OK)
				break;
		}
		if(result!=DDERR_WASSTILLDRAWING)
			break;
	}
	return result;
}









