


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



