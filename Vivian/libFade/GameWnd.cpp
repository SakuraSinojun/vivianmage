

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

//����λͼ�Խ�������
bool CGameWnd::Create(int width,int height)
{	
	return (surface->Create(width,height)!=0);
}

//����λͼ��
bool CGameWnd::Load (const char *filename)
{
	if(surface->Load (filename)==0)
	{
		TRACE("λͼ���ش�������Ϸ���ڴ���ʱ��");
		return false;
	}
	surface->Show ();
	
	return true;
}

//�趨����λͼ��������������ڵ�ͼ�񽫱����Ƶ���ͼ����(Surface)�ϡ�
void CGameWnd::UseBitmapRect(CRect& rect)
{
	surface->SetSrcRect (rect);
}	

//�ƶ�����
void CGameWnd::MoveWindow(int x,int y)
{
	surface->SetDrawPos (x,y);
}

//ȡ�ô���λ�� 
CPoint CGameWnd::GetWindowPos()
{
	return surface->GetDrawPos();
}

//��ʾ����
void CGameWnd::ShowWindow(bool show=true)
{
	surface->Show (show);
}

//����ǰ��
void CGameWnd::MoveForward()
{
	layer->MoveForward ();	
}

//���ں���
void CGameWnd::MoveBehind()
{
	layer->MoveBehind ();
}

//�����ڷ��õ���һ�����Ϸ�
void CGameWnd::SetWindowOn(CGameWnd * _window)
{
	layer->SetAfter (_window->GetLayer ());
}

//�����ڴ�����ǰ
void CGameWnd::BringWindowToTop()
{
	while(layer->MoveForward ())
	{
	}
}

//ʹ��͸��ɫ�Դ�����������״����
void CGameWnd::SetColorKey(bool colorkey)
{
	surface->SetColorKey (colorkey);
}

//�趨Alpha�ȼ��Խ���ǰ�����ںϵ�����֮�ϣ����趨��ǰ���ڵ�͸���ȡ���ΧΪ0~255,0��255����ʾ��͸����
void CGameWnd::SetFadeLevel(int level)
{
	surface->SetFadeLevel (level);
}

//���ش��ڳߴ硣
CSize CGameWnd::GetSize()
{
	return CSize(surface->Width(),surface->Height ());

}




