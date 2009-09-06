

//////////////////////////////////////////////////////////////////
//
//	FileName	:	Animation.h
//	Author		:	SakuraSinojun
//	Description	:	this class is used to create an animation based on frame.
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////



#pragma once
#include "gamewnd.h"

#include "..\stdafx.h"

class IMAGEFRAME
{
public:
	char		filename[260];		//图片路径
	CRect		rect;				//图片区域
	bool		colorkey;			//透明像素色
	int			iFadeLevel;			//融合等级
	int			nDelay;				//从前一帧到当前帧的延迟
	IMAGEFRAME*	next;				//must be NULL
	IMAGEFRAME* prev;				//must be NULL

};

class TRACKFRAME
{
public:
	int x;							//位置x
	int y;							//位置y
	int nDelay;						//帧时间（毫秒）
	TRACKFRAME* next;				//must be NULL
	TRACKFRAME* prev;				//must be NULL
};

typedef IMAGEFRAME *PIMAGEFRAME;
typedef TRACKFRAME *PTRACKFRAME;


class CAnimation : public CGameWnd
{
public:
	enum{
		ANIMATIONTYPE_IMAGE,
		ANIMATIONTYPE_TRACK,
	};

	CAnimation(void);
	virtual ~CAnimation(void);
	
	//void SetFPS(int _fps){fps=_fps;}			//动画FPS。


	void Start(int _AnimationType);				//开始动画
	void Pause(int _AnimationType);				//暂停动画
	void Stop(int _AnimationType);				//停止动画
	bool IsRunning(int _AnimationType);			//动画状态


	void CircleAnimation(int _AnimationType,bool bCircle);

	//下面是链表操作。
	
	int AddFrame(PIMAGEFRAME _frame);
	int AddFrame(PTRACKFRAME _frame);

	int GetFrameCount(int _AnimationType);
	void RemoveAllFrames(int _AnimationType);

	//链表操作结束。


protected:

	void OnPaint(HDC hdc);
	void prePaint(HDC hdc);


	void DrawImage();
	void DrawTrack();


	//int fps;
	
	bool bImageRunning;						//跳帧动画运行状态
	bool bTrackRunning;						//轨迹动画运行状态
	bool bImageCircle;						//跳帧动画循环状态
	bool bTrackCircle;						//轨迹动画循环状态

	IMAGEFRAME * image_first;
	IMAGEFRAME * image_last;
	IMAGEFRAME * image_current;

	TRACKFRAME * track_first;
	TRACKFRAME * track_last;
	TRACKFRAME * track_current;

	int image_delaycount;
	int track_delaycount;

	DWORD image_oTick;
	DWORD track_oTick;

};
