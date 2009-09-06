

//////////////////////////////////////////////////////////////////
//
//	FileName	:	Cursor.h
//	Author		:	SakuraSinojun
//	Description	:	this class is used to create a cursor;
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////





#pragma once
#include "animation.h"

class CCursor :
	public CAnimation
{
public:
	CCursor(void);
	~CCursor(void);

	//光标还在移动？
	bool IsMoving(){return this->IsRunning(CAnimation::ANIMATIONTYPE_TRACK);}
	
	//在time毫秒内将光标移动到(x,y)的位置。
	void MoveTo(int x,int y,int time=100)
	{
		if(IsMoving())return;
		this->RemoveAllFrames (CCursor::ANIMATIONTYPE_TRACK);
		TRACKFRAME frame;
		frame.nDelay=time;
		frame.next =0;
		frame.prev =0;
		frame.x=GetWindowPos().x;
		frame.y=GetWindowPos().y;
		AddFrame(&frame);
		frame.x=x;
		frame.y=y;
		AddFrame(&frame);
		this->CircleAnimation (ANIMATIONTYPE_TRACK,false);
		Stop(ANIMATIONTYPE_TRACK);
		Start(CAnimation::ANIMATIONTYPE_TRACK);
	}

};
