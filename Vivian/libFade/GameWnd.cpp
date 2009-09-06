

//////////////////////////////////////////////////////////////////
//
//	FileName	:	GameWnd.cpp
//	Author		:	SakuraSinojun
//	Description	:	this class is used to create a graphic window in the Game
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////






#include "GameWnd.h"

CGameWnd::CGameWnd(void):
	layer(NULL),
	surface(NULL)
{
	surface=CDraw::CreateNewSurface ();
	surface->SetPaintWnd (this);
	layer=(CLayer *)surface->Add ();
	ShowWindow(false);
}

CGameWnd::~CGameWnd(void)
{
	if(layer!=NULL)
	{
		delete layer;
	}
	if(surface!=NULL)
	{
		delete surface;
	}
	layer=NULL;
	surface=NULL;
}

//加载位图以建立窗口
bool CGameWnd::Create(int width,int height)
{	
	return (surface->Create(width,height)!=0);
}

//加载位图。
bool CGameWnd::Load (const char *filename)
{
	if(surface->Load (filename)==0)
	{
		TRACE("位图加载错误，在游戏窗口创建时。");
		return false;
	}
	surface->Show ();
	
	return true;
}

//设定裁切位图区域，在这个区域内的图像将被绘制到绘图表面(Surface)上。
void CGameWnd::UseBitmapRect(CRect& rect)
{
	surface->SetSrcRect (rect);
}	

//移动窗口
void CGameWnd::MoveWindow(int x,int y)
{
	surface->SetDrawPos (x,y);
}

//取得窗口位置 
CPoint CGameWnd::GetWindowPos()
{
	return surface->GetDrawPos();
}

//显示窗口
void CGameWnd::ShowWindow(bool show=true)
{
	surface->Show (show);
}

//窗口前置
void CGameWnd::MoveForward()
{
	layer->MoveForward ();	
}

//窗口后置
void CGameWnd::MoveBehind()
{
	layer->MoveBehind ();
}

//将窗口放置到另一窗口上方
void CGameWnd::SetWindowOn(CGameWnd * _window)
{
	layer->SetAfter (_window->GetLayer ());
}

//将窗口带到最前
void CGameWnd::BringWindowToTop()
{
	while(layer->MoveForward ())
	{
	}
}

//使用透明色以创建不规则形状窗口
void CGameWnd::SetColorKey(bool colorkey)
{
	surface->SetColorKey (colorkey);
}

//设定Alpha等级以将当前窗口融合到背景之上，即设定当前窗口的透明度。范围为0~255,0和255都表示不透明。
void CGameWnd::SetFadeLevel(int level)
{
	surface->SetFadeLevel (level);
}

//返回窗口尺寸。
CSize CGameWnd::GetSize()
{
	return CSize(surface->Width(),surface->Height ());

}




