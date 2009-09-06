#pragma once
#include "animation.h"

class CCursor :
	public CAnimation
{
public:
	CCursor(void);
	~CCursor(void);

	bool IsMoving(){return this->IsRunning(CAnimation::ANIMATIONTYPE_TRACK);}
	void MoveTo(int x,int y,int time)
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
