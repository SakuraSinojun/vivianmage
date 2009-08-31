
#pragma once

#include <windows.h>
#include "..\typedef.h"


class CLayer
{
public:
	CLayer();
	~CLayer();

	BOOL SetDC(HDC hdc);
	

	void	DrawBackground(PBKGNDPARAMETER pBGParameter);
	void	DrawCharacter(LPCTSTR filename);
	void	DrawMonster(LPCTSTR filename,CRect rectInPic,CPoint point2Draw);

public:
	BOOL bDraw;

private:
	HDC	m_hdc;

};
