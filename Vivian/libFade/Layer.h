


//////////////////////////////////////////////////////////////////
//
//	FileName	:	Layer.h
//	Author		:	SakuraSinojun
//	Description	:	this is the cavans on which we draw.
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////



#pragma once

#include "..\stdafx.h"


#include "DirectDraw.h"



class CLayer
{
public:
	CLayer(){prev=NULL;next=NULL;surface=NULL;}
	CLayer(void * _surface);
	~CLayer();

	void * Get(){return surface;}
	void SetSurface(void * _surface){surface=_surface;}


	bool MoveForward();
	bool MoveBehind();
	void SetAfter(CLayer * _layer);
	void SetBefore(CLayer * _layer);
	
	static CLayer * AddLayer(void * _surface);
	static void RemoveLayer(CLayer * _layer);

	void Remove();

public:
	CLayer * next;
	CLayer * prev;
	static CLayer * first;
	static CLayer * last;

	
private:
	
	void * surface;

};


