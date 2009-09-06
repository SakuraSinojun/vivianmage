

#pragma once

#include "..\stdafx.h"

class CPaintWnd
{
public:
	virtual void prePaint(HDC hdc)=0;
	virtual void OnPaint(HDC hdc)=0;
};


class CSurface
{
public:
	virtual ~CSurface(){}
	
	virtual HRESULT Create(int width,int height)=0;			//������ͼҳ
	virtual HRESULT Load(const char * name)=0;				//����λͼ
	virtual HRESULT SetColorKey(bool bColorKey)=0;			//��ColorKey͸��

	virtual void SetDrawPos(int x,int y)=0;					//����Ŀ��λ��
	virtual void SetDrawPos(CPoint point)=0;				//����Ŀ��λ��
	virtual CPoint GetDrawPos()=0;							//ȡ�û���λ��
	virtual void SetSrcPos(int x,int y)=0;					//��ͼ��Դλ��
	virtual void SetSrcRect(CRect& rect)=0;					//��ͼ��Դ����
	virtual void Show(bool _show=true)=0;					//��ʾ��ͼҳ
	virtual void * Add()=0;									//����ͼҳ��ӵ���ͼҳ������

	virtual void SetFadeLevel(int level=255)=0;				//�趨�ںϵȼ���255Ϊ���ںϡ�

	virtual int Width() const=0;							//ȡ�û�ͼҳ��
	virtual int Height() const=0;							//ȡ�û�ͼҳ��

	virtual void SetPaintWnd(CPaintWnd * _paintwnd)=0;		//�趨��ͼ�ص��࣬���ڻ���ǰ���DC���д���

protected:
	CSurface(){}
	

};