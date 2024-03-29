

#pragma once

#include "..\stdafx.h"


//继承自CPaintWnd的类，在SetPaintWnd(this)之后可在每一帧绘制的前后对绘图页做出调整。
class CPaintWnd
{
public:
	virtual void prePaint(HDC hdc)=0;
	virtual void OnPaint(HDC hdc)=0;
};


//CSurface 是一个抽象类，继承自它的CGDISurface和CDDSurface用于建立绘图页
//当绘制每一帧时，所有绘图页的内容将被绘制到屏幕缓冲区中，Flip翻页时，绘冲区中的内容将被绘制到屏幕上。
class CSurface
{
public:
	virtual ~CSurface(){}
	
	virtual HRESULT Create(int width,int height)=0;			//建立绘图页
	virtual HRESULT Load(const char * name)=0;				//加载位图
	virtual HRESULT SetColorKey(bool bColorKey)=0;			//按ColorKey透明

	virtual void SetDrawPos(int x,int y)=0;					//绘制目标位置
	virtual void SetDrawPos(CPoint point)=0;				//绘制目标位置
	virtual CPoint GetDrawPos()=0;							//取得绘制位置
	virtual void SetSrcPos(int x,int y)=0;					//绘图来源位置
	virtual void SetSrcRect(CRect& rect)=0;					//绘图来源区域
	virtual void Show(bool _show=true)=0;					//显示绘图页
	virtual void * Add()=0;									//将绘图页添加到绘图页链表中

	virtual void SetFadeLevel(int level=255)=0;				//设定融合等级，255为不融合。

	virtual int Width() const=0;							//取得绘图页宽
	virtual int Height() const=0;							//取得绘图页高

	virtual void SetPaintWnd(CPaintWnd * _paintwnd)=0;		//设定绘图回调类，以在绘制前后对DC进行处理。

protected:
	CSurface(){}
	

};