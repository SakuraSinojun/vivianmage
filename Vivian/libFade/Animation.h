

#pragma once
#include "gamewnd.h"

#include "..\stdafx.h"

class IMAGEFRAME
{
public:
	char		filename[260];		//ͼƬ·��
	CRect		rect;				//ͼƬ����
	bool		colorkey;			//͸������ɫ
	int			iFadeLevel;			//�ںϵȼ�
	int			nDelay;				//��ǰһ֡����ǰ֡���ӳ�
	IMAGEFRAME*	next;				//must be NULL
	IMAGEFRAME* prev;				//must be NULL

};
class TRACKFRAME
{
public:
	int x;
	int y;
	int nDelay;
	TRACKFRAME* next;
	TRACKFRAME* prev;
};

typedef IMAGEFRAME *PIMAGEFRAME;
typedef TRACKFRAME *PTRACKFRAME;


class CAnimation : public CGameWnd
{
public:
	enum{
		ANIMATIONTYPE_IMAGE,
		ANIMATIONTYPE_TRACE,
	};

	CAnimation(void);
	virtual ~CAnimation(void);
	
	//void SetFPS(int _fps){fps=_fps;}			//����FPS��


	void Start(int _AnimationType);				//��ʼ����
	void Pause(int _AnimationType);				//��ͣ����
	void Stop(int _AnimationType);				//ֹͣ����
	bool IsRunning(int _AnimationType);			//����״̬


	void CircleAnimation(int _AnimationType,bool bCircle);

	//���������������
	
	int AddFrame(PIMAGEFRAME _frame);
	int AddFrame(PTRACKFRAME _frame);

	int GetFrameCount(int _AnimationType);
	void RemoveAllFrames(int _AnimationType);

	//�������������


	void OnPaint(HDC hdc);
	void prePaint(HDC hdc);


protected:

	void DrawImage();
	void DrawTrack();


	//int fps;
	
	bool bImageRunning;
	bool bTrackRunning;
	bool bImageCircle;
	bool bTrackCircle;

	IMAGEFRAME * image_first;
	IMAGEFRAME * image_last;
	IMAGEFRAME * image_current;

	TRACKFRAME * track_first;
	TRACKFRAME * track_last;
	TRACKFRAME * track_current;

	int image_delaycount;
	int track_delaycount;

};
