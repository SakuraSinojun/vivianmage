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

void CAnimation::DrawImage()
{
	DWORD t;
	PIMAGEFRAME _imagenext;
	
	//û����ͼƬ����
	if(!bImageRunning)
	{
		image_oTick=0;
		return;
	}

	//û���κ�֡��ֱ����������
	if(image_current==NULL)
	{
		bImageRunning=false;
		return;
	}
	
	//ָ����һ֡
	_imagenext=image_current->next;
	
	//����Ѿ������һ֡�����ж��Ƿ�ѭ�����Ŷ�����
	if(_imagenext==NULL)
	{
		if(bImageCircle)
		{	
			//ѭ�����ţ���ָ���һ֡��
			_imagenext=image_first;
		}else{
			//���򶯻�ֹͣ��
			bImageRunning=false;
			image_current=image_first;
			image_delaycount=0;
			return;
		}
	}

	//�������һ֡������ѭ����ָ���˵�һ֡��
	//��Tick������ȥʱ�䡣
	t=::GetTickCount ();
	if(image_oTick==0)
	{
		//��һ��ˢ�¡�
		image_oTick=t;
		return;
	}
	
	//ÿ�λ���ǰ�ȼ��㵱ǰ���õ�ʱ�䡣
	image_delaycount +=(t-image_oTick);
	image_oTick=t;
	
	//����������һ֡��ʱ�䡣
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
	DWORD t;
	PTRACKFRAME _tracknext;
	int x,y;


	//û����ͼƬ����
	if(!bTrackRunning)
	{
		track_oTick=0;
		return;
	}

	//û���κ�֡��ֱ����������
	if(track_current==NULL)
	{
		bTrackRunning=false;
		return;
	}
	
	//ָ����һ֡
	_tracknext=track_current->next;
	
	//����Ѿ������һ֡�����ж��Ƿ�ѭ�����Ŷ�����
	if(_tracknext==NULL)
	{
		if(bTrackCircle)
		{	
			//ѭ�����ţ���ָ���һ֡��
			_tracknext=track_first;
		}else{
			//���򶯻�ֹͣ��
			bTrackRunning=false;
			track_current=track_first;
			track_delaycount=0;
			return;
		}
	}

	//�������һ֡������ѭ����ָ���˵�һ֡��
	//��Tick������ȥʱ�䡣
	t=::GetTickCount ();
	if(track_oTick==0)
	{
		//��һ��ˢ�¡�
		track_oTick=t;
		return;
	}
	
	//ÿ�λ���ǰ�ȼ��㵱ǰ���õ�ʱ�䡣
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
	//����������һ֡��ʱ�䡣
	if(track_delaycount>=_tracknext->nDelay)
	{
		track_delaycount=track_delaycount-_tracknext->nDelay;
		track_current=_tracknext;
	}


}

void CAnimation::prePaint(HDC hdc)
{
	DrawImage();
	DrawTrack();
}

