

#pragma once

#include "..\stdafx.h"



typedef struct _MONSTER
{
	
	int HP;
	int MP;
	int SP;
	
	int STR;
	int DEX;
	int CON;
	int INT;
	int WIS;
	int CHA;
	
	int level;
	int exp;

}MONSTER,*LMONSTER;

class CMonster
{
public:
	CMonster(LPCSTR filename,CRect rect,CPoint point,int ZOrder);
	~CMonster();
	
	void Modify(CPoint point,LPCSTR filename=NULL,CRect rect=CRect(),int ZOrder=0);

	void SetStatus(LMONSTER monster);


};


