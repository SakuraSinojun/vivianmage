








#ifndef __DirectDraw_h__
#define __DirectDraw_h__

#include <list>

#include "ddraw.h"
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"ddraw.lib")


#include "DDSurface.h"

class CDirectDraw
{
public:
	enum{
		HignColor,
		FullColor,
	};

public:
	CDirectDraw();
	~CDirectDraw();
	bool Create(HWND hWnd,int width,int height,int mode);
	void ReleaseAllObjects();
	HRESULT RestoreAll();
	HRESULT Flip();
	void Add(CDDSurface *s);
	void Remove(CDDSurface *s);
	virtual bool Draw();
	CSize GetSize() const { return size;}
	LPDIRECTDRAW operator->(){return dd;}
	LPDIRECTDRAWSURFACE& GetDrawable(){return back;}
	LPDIRECTDRAWSURFACE& GetPrimary(){return primary;}

protected:
	typedef std::list<CDDSurface *> surface_list;
	LPDIRECTDRAW		dd;			//DirectDraw对象
	LPDIRECTDRAWSURFACE primary;	//主绘图页
	LPDIRECTDRAWSURFACE back;		//背景绘图页

	surface_list		surface;	//绘图页列表
	CSize				size;
};


//向列表中添加绘图页
inline void CDirectDraw::Add(CDDSurface *s) 
{
	surface.push_back (s);
}

//从列表中删除
inline void CDirectDraw::Remove(CDDSurface *s) 
{
	surface.remove(s);
}

#endif