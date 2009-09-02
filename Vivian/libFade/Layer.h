
#pragma once

#include "..\stdafx.h"


#include "DirectDraw.h"



class CLayer
{
public:
	CLayer();
	~CLayer();

	BOOL SetDC(HDC hdc);
	

	void	DrawBackground(PBKGNDPARAMETER pBGParameter);
	void	DrawCharacter(LPCSTR filename,CPoint point);
	void	DrawMonster(LPCSTR filename,CRect rectInPic,CPoint point2Draw);

public:
	BOOL bDraw;

private:
	HDC	m_hdc;

	HDC m_hdc_background_1;
	HDC m_hdc_background_2;
	HDC m_hdc_background_3;
	HDC m_hdc_background_4;
	HDC	m_hdc_character;

	char file_character[255];
	

private:
	
	CDDSurface m_dds_background_topleft;
	CDDSurface m_dds_background_topright;
	CDDSurface m_dds_background_bottomleft;
	CDDSurface m_dds_background_bottomright;

	CDDSurface m_dds_char;

	char filechar[255];
	char file1[255];
	char file2[255];
	char file3[255];
	char file4[255];
	

	CDDSurface m_Monster;

};
