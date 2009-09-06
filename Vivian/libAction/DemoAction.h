#pragma once
#include "action.h"

#include "..\libFade\libFade.h"

class KeyAction
{
public:
	virtual ~KeyAction(){}
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)=0;
protected:
	KeyAction(){}
};

class CMainMenu : KeyAction
{
public:
	CMainMenu();
	~CMainMenu();


	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	void Create();
	void Destroy();

private:
	CGameWnd * win;
	CCursor	* cursor;

};


class CShop : KeyAction
{
public:
	CShop():win(NULL){}
	~CShop(){if(win)delete win;}

	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	void Create();
	void Destroy()
	{
		if(win)
			delete win;
		win=NULL;
	}

private:
	CGameWnd * win;
};


class CDemoAction :
	public KeyAction
{
public:
	CDemoAction(void);
	~CDemoAction(void);

	void Create(){};
	void Destroy(){};
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnCreate();
	void Pause();

private:

	CMapWnd * win1;
	CTextWnd * win2;
	CAnimation * win3;
	CCursor * win4;
	CGameWnd * win5;

	CMainMenu * pMainMenu;
	CShop	* pShop;

};
