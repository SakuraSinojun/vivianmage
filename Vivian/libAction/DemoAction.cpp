#include "DemoAction.h"

KeyAction * pKey;
CDemoAction * pAction;


CDemoAction::CDemoAction(void)
{
	pMainMenu=NEW CMainMenu();
	pShop=NEW CShop();
	win1=NEW CMapWnd();
	win2=NEW CTextWnd();
	win3=NEW CAnimation();
	win5=NEW CGameWnd();
	win4=NEW CCursor();

	pAction=this;
}

CDemoAction::~CDemoAction(void)
{
	delete pMainMenu;
	delete pShop;
	delete win1;
	delete win2;
	delete win3;
	delete win5;
	delete win4;
}

void CDemoAction::OnCreate ()
{
	pKey=this;


	win1->Create ("CGDATA\\bg.bmp","CGDATA\\bg1.bmp","CGDATA\\bg2.bmp","CGDATA\\bg3.bmp");
	win1->SetSplitPoint (CPoint(320,240));
	
	
	win2->SetTextColor (RGB(255,0,0));
	win2->CreateText (100,100,"Text test.");
	
	
	IMAGEFRAME frame;
	::ZeroMemory (&frame,sizeof(frame));
	frame.colorkey =true;
	frame.nDelay =50;
	lstrcpyA(frame.filename,"CGDATA\\Explosion.bmp");
	frame.rect.top =0;
	frame.rect.bottom =100;
	frame.iFadeLevel =255;
	for(int i=268;i<=670;i+=67)
	{
		frame.rect.left =i;
		frame.rect.right =i+67;
		win3->AddFrame (&frame);
	}
	win3->Start(win3->ANIMATIONTYPE_IMAGE);
	win3->MoveWindow (50,50);
	win3->ShowWindow (true);
	win3->CircleAnimation (win3->ANIMATIONTYPE_IMAGE ,true);
	TRACKFRAME _frame;
	::ZeroMemory (&_frame,sizeof(_frame));
	_frame.nDelay =5000;
	_frame.x=580;
	_frame.y=400;
	win3->AddFrame (&_frame);
	_frame.x=100;
	_frame.y=100;
	win3->AddFrame (&_frame);
	win3->CircleAnimation (win3->ANIMATIONTYPE_TRACK ,true);
	win3->Start (win3->ANIMATIONTYPE_TRACK);


	::ZeroMemory (&frame,sizeof(frame));
	frame.colorkey =false;
	frame.iFadeLevel =128;
	frame.nDelay =50;
	lstrcpyA(frame.filename,"CGDATA\\Cursor2.bmp");
	frame.rect.top =0;
	frame.rect.bottom =71;
	frame.rect.left =0;
	frame.rect.right =251;
	win4->AddFrame (&frame);
	frame.rect.left =252;
	frame.rect.right=503;
	win4->AddFrame (&frame);

	win4->Start(win4->ANIMATIONTYPE_IMAGE);
	//win4->SetFadeLevel (128);
	win4->MoveWindow (50,50);
	win4->ShowWindow (true);
	win4->CircleAnimation (win4->ANIMATIONTYPE_IMAGE ,true);


	win5->Create ("CGDATA\\char.bmp");
	win5->SetColorKey (true);

	win5->MoveWindow (128,0);
	//win5->SetFadeLevel (128);
	pMainMenu->Create ();


}

void CDemoAction::Pause()
{

}


void CDemoAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(pKey!=this)
	{
		pKey->OnKeyDown (nChar,nRepCnt,nFlags);
		return;
	}
	
	if(nChar==27)
	{
		//OnClose();
		Pause();
		pMainMenu->Create ();
	}else if(nChar==80){
		if(win3->IsRunning (win3->ANIMATIONTYPE_TRACK))
		{
			win3->Pause (win3->ANIMATIONTYPE_TRACK);
		}else{
			win3->Start(win3->ANIMATIONTYPE_TRACK);
		}
	}else if(nChar==32){
		win3->Stop(win3->ANIMATIONTYPE_TRACK);
	}else if(nChar==40){
		if(!win4->IsMoving ())
		{
			CPoint point=win4->GetWindowPos ();
			if(point.y==50)
			{
				point.y=100;
			}else{
				point.y=50;
			}
			win4->MoveTo (point.x,point.y,100);
		}
	}

}



CMainMenu::CMainMenu ():
	win(NULL),
	cursor(NULL)
{
	win=NEW CGameWnd();
	cursor =NEW CCursor();
}

CMainMenu::~CMainMenu()
{
	delete win;
	delete cursor;
}


void CMainMenu::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar==40)
	{
		if(!cursor->IsMoving ())
		{
			CPoint point=cursor->GetWindowPos ();
			if(point.y<306)
			{
				point.y+=80;
				cursor->MoveTo (point.x,point.y,100);
			}
		}
	}else if(nChar==38){
		if(!cursor->IsMoving ())
		{
			CPoint point=cursor->GetWindowPos ();
			if(point.y>146)
			{
				point.y-=80;
				cursor->MoveTo (point.x,point.y,100);
			}
		}
	}else if(nChar==13){
		CPoint point;
		point=cursor->GetWindowPos ();
		if(point.y==306)
		{
			//pKey=pAction;
			//Destroy();
			PostQuitMessage(0);
		}else if(point.y==146)
		{
			Destroy();
			pKey=pAction;
		}else if(point.y==226)
		{
			
		}
	}

}

void CMainMenu::Destroy()
{
	if(win)
		win->ShowWindow (false);
	if(cursor)
		cursor->ShowWindow (false);
}


void CMainMenu::Create()
{	
	cursor->Load("CGDATA\\Cursor.bmp");
	win->Load("CGDATA\\MainMenu.bmp");
	
	win->ShowWindow (true);
	cursor->ShowWindow (true);


	win->BringWindowToTop ();
	cursor->BringWindowToTop ();
	
	win->SetColorKey (true);
	cursor->SetColorKey (true);

	//win->SetFadeLevel (224);
	cursor->SetFadeLevel (128);

	cursor->MoveWindow (100,146);
	
	pKey=this;

}



void CShop::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}


void CShop::Create()
{
}



