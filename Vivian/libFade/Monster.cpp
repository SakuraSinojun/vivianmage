
//////////////////////////////////////////////////////////////////
//
//	FileName	:	Monster.cpp
//	Author		:	SakuraSinojun
//	Description	:	a class for each monster
//					inherit from this class when you are to put/draw/kill/modify a monster
//
//	Version		:	1.0.0.1
//	Date		:	2009.9.2
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////




#include "Monster.h"

#include "DirectDraw.h"


extern CDirectDraw DirectDraw;

CMonster::CMonster (LPCSTR filename,CRect rect,CPoint point,int ZOrder)
{
}
CMonster::~CMonster ()
{

}

void CMonster::Modify(CPoint point,LPCSTR filename,CRect rect,int ZOrder)
{
}



