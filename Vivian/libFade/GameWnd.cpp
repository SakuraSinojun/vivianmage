#include "GameWnd.h"

CGameWnd::CGameWnd(void):
	layer(NULL),
	surface(NULL)
{
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

bool CGameWnd::Create(const char * filename)
{	
	
	if(surface!=NULL)
	{
		return false;
	}
	
	surface=CDraw::CreateNewSurface ();
	
	if(surface->Load (filename)==0)
	{
		TRACE("位图加载错误，在游戏窗口创建时。");
		return false;
	}
	layer=(CLayer *)surface->Add ();
	surface->Show ();

	return true;
}

bool CGameWnd::Load (const char *filename)
{
	if(surface==NULL)
	{
		surface=CDraw::CreateNewSurface ();
		if(surface->Load (filename)==0)
		{
			TRACE("位图加载错误，在装载到绘图页时。");
			return false;
		}
		layer=(CLayer *)surface->Add ();
	}else{
		if(surface->Load (filename)==0)
		{
			TRACE("位图加载错误，在装载到绘图页时。");
			return false;
		}
	}
	return true;
}

void CGameWnd::UseBitmapRect(CRect& rect)
{
	surface->SetSrcRect (rect);
}	

void CGameWnd::MoveWindow(int x,int y)
{
	surface->SetDrawPos (x,y);
}

CPoint CGameWnd::GetWindowPos()
{
	return surface->GetDrawPos();
}

void CGameWnd::ShowWindow(bool show)
{
	surface->Show (show);
}

void CGameWnd::MoveForward()
{
	layer->MoveForward ();	
}

void CGameWnd::MoveBehind()
{
	layer->MoveBehind ();
}

void CGameWnd::SetWindowOn(CGameWnd * _window)
{
	layer->SetAfter (_window->GetLayer ());
}

void CGameWnd::BringWindowToTop()
{
	while(layer->MoveForward ())
	{
	}
}

void CGameWnd::SetColorKey(bool colorkey)
{
	surface->SetColorKey (colorkey);
}

void CGameWnd::SetFadeLevel(int level)
{
	surface->SetFadeLevel (level);
}

CSize CGameWnd::GetSize()
{
	return CSize(surface->Width(),surface->Height ());

}




