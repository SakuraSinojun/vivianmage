


//////////////////////////////////////////////////////////////////
//
//	FileName	:	Layer.cpp
//	Author		:	SakuraSinojun
//	Description	:	this is the cavans on which we draw.
//	
//	Version		:	1.0.0.1
//	Date		:	2009.8.30
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////




#include "Layer.h"
#include "preLoad.h"



CLayer * CLayer::first=NULL;
CLayer * CLayer::last=NULL;


CLayer::CLayer(void * _surface)
{
	surface=_surface;
	prev=NULL;
	next=NULL;

}

CLayer::~CLayer()
{
	Remove();
}

CLayer * CLayer::AddLayer(void * _surface)
{
	CLayer * _layer=new CLayer(_surface);
	if(last==NULL)
	{
		first=_layer;
	}else{
		last->next =_layer;
		_layer->prev=last;
	}
	last=_layer;
	return _layer;

}

//删除layer.
void CLayer::RemoveLayer(CLayer * _layer)
{
	delete _layer;
}

//从Layer链表中移除
void CLayer::Remove ()
{
	CLayer * prev;
	CLayer * next;
	prev=this->prev;
	next=this->next;
	if(prev!=NULL)
	{
		prev->next=next;
	}
	if(next!=NULL)
	{
		next->prev=prev;
	}

}

//前置
bool CLayer::MoveForward()
{
	if(next==NULL)
	{
		return false;
	}

	CLayer * _ly=next;
	return _ly->MoveBehind ();

}

//后置
bool CLayer::MoveBehind()
{
	if(prev==NULL)
	{
		return false;
	}

	CLayer * lyprev;
	CLayer * lynext;
	lynext=this->prev;
	lyprev=lynext->prev;
	this->Remove ();
	this->prev=lyprev;
	if(lyprev!=NULL)
	{
		lyprev->next =this;
	}else{
		CLayer::first =this;
	}
	this->next=lynext;
	lynext->prev=this;

	return true;

}

//于_layer之后绘制，即放置于_layer上方。
void CLayer::SetAfter(CLayer * _layer)
{
	CLayer * lynext;
	
	if(_layer==NULL)
		return;
	
	this->Remove ();

	lynext=_layer->next;
	_layer->next=this;
	this->prev=_layer;
	this->next=lynext;
	if(lynext!=NULL)
	{
		lynext->prev=this;
	}else{
		last=this;
	}

}

//于_layer之前绘制，即放置于_layer下方。
void CLayer::SetBefore(CLayer * _layer)
{
	CLayer * lybf;

	if(_layer==NULL)
		return;
	
	this->Remove();
	lybf=_layer->prev;
	_layer->prev=this;
	this->next=_layer;
	this->prev=lybf;
	if(lybf!=NULL)
	{
		lybf->next=this;
	}else{
		first=this;
	}

}





#ifdef __LAYER_
#undef __LAYER_
#endif

#ifdef __LAYER_
/*

extern	CDirectDraw			DirectDraw;
extern	char				FPS[100];


//设置项
extern bool _win;


CLayer::CLayer()
	:m_hdc(NULL)
{

	m_hdc_background_1=NULL;
	m_hdc_background_2=NULL;
	m_hdc_background_3=NULL;
	m_hdc_background_4=NULL;
	m_hdc_character=NULL;
}

CLayer::~CLayer() 
{
	if(_win)
	{

	}
}

BOOL CLayer::SetDC (HDC hdc)
{
	if(!m_hdc)
	{
		m_hdc=hdc;
	
		if(_win)
		{
		}
		return TRUE;
	}
	return FALSE;
}


void CLayer::DrawBackground(PBKGNDPARAMETER pBGParameter)
{	
	static bool bIsFirstDraw=true;
	PBKGNDPARAMETER  p=pBGParameter;
	CPoint point1,point2,point3,point4;
	BITMAP b1,b2,b3,b4;

	if(_win)
	{
		m_hdc_background_1=pl::Load (p->file1,&b1);
		m_hdc_background_2=pl::Load (p->file2,&b2);
		m_hdc_background_3=pl::Load (p->file3,&b3);
		m_hdc_background_4=pl::Load (p->file4,&b4);

		BitBlt(m_hdc,0,0,p->point.x,p->point.y,m_hdc_background_1,b1.bmWidth-p->point.x,b1.bmHeight -p->point.y,SRCCOPY);
		BitBlt(m_hdc,p->point.x,0,GAME_WINDOW_WIDTH-p->point.x,p->point.y,m_hdc_background_2,0,b1.bmHeight -p->point.y,SRCCOPY);
		BitBlt(m_hdc,0,p->point.y,p->point.x,GAME_WINDOW_HEIGHT-p->point.y,m_hdc_background_3,b1.bmWidth-p->point.x,0,SRCCOPY);
		BitBlt(m_hdc,p->point.x,p->point.y,GAME_WINDOW_WIDTH-p->point.x,GAME_WINDOW_HEIGHT-p->point.y,m_hdc_background_4,0,0,SRCCOPY);

	}else{
		
		if(lstrcmpA(file1,p->file1)!=0)
		{
			lstrcpyA(file1,p->file1);
			
			m_dds_background_topleft.Load (DirectDraw,p->part_topleft);
		}
		if(lstrcmpA(file2,p->file2)!=0)
		{
			lstrcpyA(file2,p->file2);
			
			m_dds_background_topright.Load(DirectDraw,p->part_topright);
		}
		if(lstrcmpA(file3,p->file3)!=0)
		{
			lstrcpyA(file3,p->file3);	
			m_dds_background_bottomleft.Load(DirectDraw,p->part_bottomleft);
		}
		if(lstrcmpA(file4,p->file4)!=0)
		{
			lstrcpyA(file4,p->file4);
			m_dds_background_bottomright.Load(DirectDraw,p->part_bottomright);
		}
		
		pl::Load (p->file1,&b1);
		pl::Load(p->file2,&b2);
		pl::Load (p->file3,&b3);
		pl::Load(p->file4,&b4);
		
		point1=CPoint(p->point.x-b1.bmWidth,p->point.y-b1.bmHeight );
		point2=CPoint(p->point.x,p->point.y-b2.bmHeight);
		point3=CPoint(p->point.x-b3.bmWidth ,p->point.y);
		point4=p->point;
		
		m_dds_background_topleft.SetDrawPos (point1);
		m_dds_background_topright.SetDrawPos (point2);
		m_dds_background_bottomleft.SetDrawPos (point3);
		m_dds_background_bottomright.SetDrawPos (point4);
		
		if(bIsFirstDraw)
		{
			bIsFirstDraw=false;
			DirectDraw.Add (&m_dds_background_topleft);
			DirectDraw.Add (&m_dds_background_topright);
			DirectDraw.Add (&m_dds_background_bottomleft);
			DirectDraw.Add (&m_dds_background_bottomright);
			m_dds_background_topleft.Show();
			m_dds_background_topright.Show();
			m_dds_background_bottomleft.Show();
			m_dds_background_bottomright.Show();
		}
	}

}

void CLayer::DrawCharacter(LPCSTR filename,CPoint point)
{
	static bool bIsFirstDraw=true;
	//HBITMAP		hBitmap;
	COLORREF	color;
	BITMAP		b_char;


	if(_win)
	{
		m_hdc_character=pl::Load (filename,&b_char);

		//StretchBlt函数那是相当之慢。。。于是这里做了一点修改。。
		//::StretchBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,m_hdc_character,0,0,1,1,SRCCOPY);		
		color=::GetPixel (m_hdc_character,1,1);
		::SetBkColor (m_hdc,color);
		CRect rc=CRect(0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);
		::ExtTextOutW(m_hdc,0,0,ETO_OPAQUE,&rc,NULL,0,0);

		::BitBlt (m_hdc,(GAME_WINDOW_WIDTH-b_char.bmWidth)/2-1,(GAME_WINDOW_HEIGHT-b_char.bmHeight)/2-1,b_char.bmWidth ,b_char.bmHeight ,m_hdc_character,0,0,SRCCOPY); 
		
	}else{
		
		if(lstrcmpA(filechar,filename)!=0)
		{
			lstrcpyA(filechar,filename);
			m_dds_char.Load(DirectDraw,filename);
			color=m_dds_char.GetPTColor ();
			m_dds_char.Show ();
			m_dds_char.SetColorKey (color);
			m_dds_char.SetDrawPos (point);
		}

		if(bIsFirstDraw)
		{
			DirectDraw.Add (&m_dds_char);
			bIsFirstDraw=false;
			m_dds_char.Show ();
		}

	}

}

void CLayer::DrawMonster(LPCSTR filename,CRect rectInPic,CPoint point2Draw)
{


	if(_win)
	{
		HDC memDC=pl::Load (filename);
		
		//::StretchBlt (m_hdc,0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT,memDC,0,0,1,1,SRCCOPY);
		COLORREF color=::GetPixel (memDC,1,1);
		::SetBkColor (m_hdc,color);
		CRect rc=CRect(0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);
		::ExtTextOutW(m_hdc,0,0,ETO_OPAQUE,&rc,NULL,0,0);
	
		::BitBlt (m_hdc,point2Draw.x,point2Draw.y,rectInPic.Width (),rectInPic.Height (),memDC,rectInPic.left ,rectInPic.top ,SRCCOPY);		
	}else{

	}

}

*/


#endif



