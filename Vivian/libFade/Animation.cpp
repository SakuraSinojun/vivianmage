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
	bTrackCircle(false)
{
}

CAnimation::~CAnimation(void)
{
	this->RemoveAllFrames (this->ANIMATIONTYPE_IMAGE );
	this->RemoveAllFrames (this->ANIMATIONTYPE_TRACE );
}

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

	}else if(_AnimationType==ANIMATIONTYPE_TRACE)
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

void CAnimation::Pause(int _AnimationType)
{
	if(_AnimationType==ANIMATIONTYPE_IMAGE)
	{
		
	}else if(_AnimationType==ANIMATIONTYPE_TRACE)
	{
	
	}else
	{
	}
}

void CAnimation::Stop(int _AnimationType)
{
	if(_AnimationType==ANIMATIONTYPE_IMAGE)
	{
		bImageRunning=false;
		image_current =image_first;
		image_delaycount=0;
	}else if(_AnimationType==ANIMATIONTYPE_TRACE)
	{
		bTrackRunning=false;
		track_current=track_first;
		image_delaycount=0;
	}else
	{
	}
}

bool CAnimation::IsRunning(int _AnimationType)
{
	if(_AnimationType==ANIMATIONTYPE_IMAGE)
	{
		return bImageRunning ;
	}else if(_AnimationType==ANIMATIONTYPE_TRACE)
	{
		return bTrackRunning;
	}else
	{
		return false;
	}
}

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

int CAnimation::AddFrame(PTRACKFRAME _frame)
{
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
	//return GetFrameCount(ANIMATIONTYPE_TRACE);
	return 0;
}

int CAnimation::GetFrameCount(int _AnimationType)
{
	int count=0;

	if(_AnimationType==ANIMATIONTYPE_IMAGE)
	{
	
	}else if(_AnimationType==ANIMATIONTYPE_TRACE)
	{
	
	}else
	{
	}
	return 0;

}

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
	}else if(_AnimationType==ANIMATIONTYPE_TRACE)
	{
		TRACKFRAME * frame=track_first;
		TRACKFRAME * _f=frame;
		while(frame!=NULL)
		{
			_f=frame->next;
			delete frame;
			frame=_f;
		}	
	}else
	{
	}
}

void CAnimation::CircleAnimation(int _AnimationType,bool bCircle)
{
	
	if(_AnimationType==ANIMATIONTYPE_IMAGE)
	{
		bImageCircle=bCircle;
	}else if(_AnimationType==ANIMATIONTYPE_TRACE)
	{
		bTrackCircle=bCircle;
	}else
	{
	}
}

void CAnimation::OnPaint(HDC hdc)
{
}

void CAnimation::DrawImage()
{
	static DWORD oTick=0;
	DWORD t;
	PIMAGEFRAME _imagenext;
	
	//没启动图片动画
	if(!bImageRunning)
	{
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
	if(oTick==0)
	{
		//第一次刷新。
		oTick=t;
		return;
	}
	
	//每次绘制前先计算当前已用掉时间。
	image_delaycount +=(t-oTick);
	oTick=t;
	
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

void CAnimation::DrawTrack()
{
	static DWORD oTick=0;
	DWORD t;
	PTRACKFRAME _tracknext;
	
	//没启动图片动画
	if(!bTrackRunning)
	{
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
	if(oTick==0)
	{
		//第一次刷新。
		oTick=t;
		return;
	}
	
	//每次绘制前先计算当前已用掉时间。
	track_delaycount +=(t-oTick);
	oTick=t;
	
	//到了跳到下一帧的时间。
	if(track_delaycount>=_tracknext->nDelay)
	{
		this->MoveWindow (_tracknext->x,_tracknext->y);
		track_delaycount=track_delaycount-_tracknext->nDelay;
		track_current=_tracknext;
	}


}

void CAnimation::prePaint(HDC hdc)
{
	DrawImage();
	DrawTrack();
}

