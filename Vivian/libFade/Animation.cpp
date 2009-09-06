

//////////////////////////////////////////////////////////////////
//
//	FileName	:	Animation.cpp
//	Author		:	SakuraSinojun
//	Description	:	this class is used to create an animation based on frame.
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////




#include "Animation.h"

CAnimation::CAnimation(void):
	bImageRunning(false),
	bTrackRunning(false),
	image_first(NULL),
	image_last(NULL),
	image_current(NULL),
	track_first(NULL),
	track_last(NULL),
	track_current(NULL),
	image_delaycount(0),
	track_delaycount(0),
	bImageCircle(false),
	bTrackCircle(false),
	image_oTick(0),
	track_oTick(0)
{
}

CAnimation::~CAnimation(void)
{
	this->RemoveAllFrames (this->ANIMATIONTYPE_IMAGE );
	this->RemoveAllFrames (this->ANIMATIONTYPE_TRACK );
}


//开始动画。参数可选为：0--跳帧动画，1--轨迹动画
void CAnimation::Start(int _AnimationType)
{
	if(surface==NULL)
	{
		surface=CDraw::CreateNewSurface();
		surface->Add ();
	}
	
	surface->SetPaintWnd (this);

	if(_AnimationType==ANIMATIONTYPE_IMAGE)
	{
		bImageRunning=true;
		if(image_current ==NULL)
		{
			image_current =image_first;
		}
		this->Load (image_current->filename);
		this->UseBitmapRect (image_current->rect);
		this->SetColorKey (image_current->colorkey);
		this->SetFadeLevel (image_current->iFadeLevel);

	}else if(_AnimationType==ANIMATIONTYPE_TRACK)
	{
		bTrackRunning=true;
		if(track_current==NULL)
		{
			track_current=track_first;
		}
		this->MoveWindow (track_current->x,track_current->y);
	}else{
	}
}

//暂停动画。参数可选为：0--跳帧动画，1--轨迹动画
void CAnimation::Pause(int _AnimationType)
{
	if(_AnimationType==ANIMATIONTYPE_IMAGE)
	{
		bImageRunning=false;
	}else if(_AnimationType==ANIMATIONTYPE_TRACK)
	{
		bTrackRunning=false;
	}else
	{
	}
}

//停止动画并返回初始状态。参数可选为：0--跳帧动画，1--轨迹动画
void CAnimation::Stop(int _AnimationType)
{
	if(_AnimationType==ANIMATIONTYPE_IMAGE)
	{
		bImageRunning=false;
		image_current =image_first;
		image_delaycount=0;
		if(image_first!=NULL)
		{
			Load(image_first->filename);
			SetColorKey(image_first->colorkey);
			this->UseBitmapRect (image_first->rect);
			SetFadeLevel(image_first->iFadeLevel);
		}
	}else if(_AnimationType==ANIMATIONTYPE_TRACK)
	{
		bTrackRunning=false;
		track_current=track_first;
		track_delaycount=0;
		if(track_first!=NULL)
		{
			MoveWindow(track_first->x,track_first->y);
		}
	}else
	{
	}
}

//动画运行状态。参数可选为：0--跳帧动画，1--轨迹动画
bool CAnimation::IsRunning(int _AnimationType)
{
	if(_AnimationType==ANIMATIONTYPE_IMAGE)
	{
		return bImageRunning ;
	}else if(_AnimationType==ANIMATIONTYPE_TRACK)
	{
		return bTrackRunning;
	}else
	{
		return false;
	}
}

//增加跳帧动画帧
int CAnimation::AddFrame(PIMAGEFRAME _frame)
{
	IMAGEFRAME * frame=NEW IMAGEFRAME(*_frame);
	frame->next=NULL;
	frame->prev=NULL;
	
	if(this->image_first ==NULL)
	{
		image_first=frame;
	}else{
		image_last->next=frame;
		frame->prev=image_last;
	}
	image_last=frame;
	//return GetFrameCount(ANIMATIONTYPE_IMAGE);
	return 0;
}

//增加轨迹动画帧
int CAnimation::AddFrame(PTRACKFRAME _frame)
{
	if(_frame->nDelay <=0)
		return 0;

	TRACKFRAME * frame=NEW TRACKFRAME(*_frame);
	frame->next=NULL;
	frame->prev=NULL;
	
	if(this->track_first ==NULL)
	{
		track_first=frame;
	}else{
		track_last->next=frame;
		frame->prev=track_last;
	}
	track_last=frame;
	//return GetFrameCount(ANIMATIONTYPE_TRACK);
	return 1;
}

//没用到这个函数。调用则一定会返回0值。
int CAnimation::GetFrameCount(int _AnimationType)
{
	int count=0;

	if(_AnimationType==ANIMATIONTYPE_IMAGE)
	{
	
	}else if(_AnimationType==ANIMATIONTYPE_TRACK)
	{
	
	}else
	{
	}
	return 0;

}

//移除所有帧。参数可选为：0--跳帧动画，1--轨迹动画
void CAnimation::RemoveAllFrames(int _AnimationType)
{

	if(_AnimationType==ANIMATIONTYPE_IMAGE)
	{
		IMAGEFRAME * frame=image_first;
		IMAGEFRAME * _f=frame;
		while(frame!=NULL)
		{
			_f=frame->next;
			delete frame;
			frame=_f;
		}
		image_first=NULL;
		image_last=NULL;
	}else if(_AnimationType==ANIMATIONTYPE_TRACK)
	{
		TRACKFRAME * frame=track_first;
		TRACKFRAME * _f=frame;
		while(frame!=NULL)
		{
			_f=frame->next;
			delete frame;
			frame=_f;
		}	
		track_first=NULL;
		track_last=NULL;
	}else
	{
	}
}

//设定动画循环。参数可选为：0--跳帧动画，1--轨迹动画
void CAnimation::CircleAnimation(int _AnimationType,bool bCircle)
{
	
	if(_AnimationType==ANIMATIONTYPE_IMAGE)
	{
		bImageCircle=bCircle;
	}else if(_AnimationType==ANIMATIONTYPE_TRACK)
	{
		bTrackCircle=bCircle;
	}else
	{
	}
}

void CAnimation::OnPaint(HDC hdc)
{
}

//绘制Image动画
void CAnimation::DrawImage()
{
	DWORD t;
	PIMAGEFRAME _imagenext;
	
	//没启动图片动画
	if(!bImageRunning)
	{
		image_oTick=0;
		return;
	}

	//没有任何帧而直接启动动画
	if(image_current==NULL)
	{
		bImageRunning=false;
		return;
	}
	
	//指向下一帧
	_imagenext=image_current->next;
	
	//如果已经是最后一帧，则判断是否循环播放动画。
	if(_imagenext==NULL)
	{
		if(bImageCircle)
		{	
			//循环播放，则指向第一帧。
			_imagenext=image_first;
		}else{
			//否则动画停止。
			bImageRunning=false;
			image_current=image_first;
			image_delaycount=0;
			return;
		}
	}

	//不是最后一帧，或因循环而指向了第一帧。
	//用Tick计算用去时间。
	t=::GetTickCount ();
	if(image_oTick==0)
	{
		//第一次刷新。
		image_oTick=t;
		return;
	}
	
	//每次绘制前先计算当前已用掉时间。
	image_delaycount +=(t-image_oTick);
	image_oTick=t;
	
	//到了跳到下一帧的时间。
	if(image_delaycount>=_imagenext->nDelay)
	{
		this->Load (_imagenext->filename);
		this->UseBitmapRect (_imagenext->rect);
		this->SetColorKey (_imagenext->colorkey);
		this->SetFadeLevel (_imagenext->iFadeLevel);
		image_delaycount=image_delaycount-_imagenext->nDelay;
		image_current=_imagenext;
	}

}

//绘制Track动画。
void CAnimation::DrawTrack()
{
	DWORD t;
	PTRACKFRAME _tracknext;
	int x,y;


	//没启动图片动画
	if(!bTrackRunning)
	{
		track_oTick=0;
		return;
	}

	//没有任何帧而直接启动动画
	if(track_current==NULL)
	{
		bTrackRunning=false;
		return;
	}
	
	//指向下一帧
	_tracknext=track_current->next;
	
	//如果已经是最后一帧，则判断是否循环播放动画。
	if(_tracknext==NULL)
	{
		if(bTrackCircle)
		{	
			//循环播放，则指向第一帧。
			_tracknext=track_first;
		}else{
			//否则动画停止。
			bTrackRunning=false;
			track_current=track_first;
			track_delaycount=0;
			return;
		}
	}

	//不是最后一帧，或因循环而指向了第一帧。
	//用Tick计算用去时间。
	t=::GetTickCount ();
	if(track_oTick==0)
	{
		//第一次刷新。
		track_oTick=t;
		return;
	}
	
	//每次绘制前先计算当前已用掉时间。
	track_delaycount +=(t-track_oTick);
	track_oTick=t;
	
	x=_tracknext->x-track_current->x;
	y=_tracknext->y-track_current->y;
	x=x*track_delaycount/_tracknext->nDelay;
	y=y*track_delaycount/_tracknext->nDelay;

	if(track_delaycount<_tracknext->nDelay)
	{
		this->MoveWindow (track_current->x+x,track_current->y+y);
	}else{
		MoveWindow(_tracknext->x,_tracknext->y);
	}
	//到了跳到下一帧的时间。
	if(track_delaycount>=_tracknext->nDelay)
	{
		track_delaycount=track_delaycount-_tracknext->nDelay;
		track_current=_tracknext;
	}


}

//预渲染
void CAnimation::prePaint(HDC hdc)
{
	DrawImage();
	DrawTrack();
}

