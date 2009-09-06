
//////////////////////////////////////////////////////////////////
//
//	FileName	:	MapWnd.cpp
//	Author		:	SakuraSinojun
//	Description	:	this class is used to draw a joint map spliced by 4 pictures
//	
//	Version		:	1.0.0.1
//	Date		:	2009.9.6
//
//	Copyright(c):	 2009-2010 Sakura
//
//////////////////////////////////////////////////////////////////



#include "MapWnd.h"

CMapWnd::CMapWnd(void):
	point(0,0)
{
}

CMapWnd::~CMapWnd(void)
{
}

//����������ͼƬƴ�Ӷ��ɵĵ�ͼ��������˳��Ϊ�����ϣ����ϣ����£����¡�
void CMapWnd::Create(const char * file0,const char * file1,const char * file2,const char * file3)
{
#define SETMAP(quote) SetMap(quote,file##quote)
	
	SETMAP(0);
	SETMAP(1);
	SETMAP(2);
	SETMAP(3);

#undef SETMAP

	SetSplitPoint(CPoint(0,0));

	ShowWindow(true);

}

//��ʾ��ͼ
void  CMapWnd::ShowWindow(bool show)
{
	for(int i=0;i<4;i++)
		map[i].ShowWindow (show);

}

//�趨��ͼCG�����Ϊ0~3
void CMapWnd::SetMap(int index,const char * filename)
{
	if(index>3 || index<0)
		return;
	map[index].Load(filename);

}

//���÷ָ��㡣����ͼƬ���Դ˵�Ϊ���������š�
void CMapWnd:: SetSplitPoint(CPoint point)
{
	this->point=point;
	
	map[0].MoveWindow (point.x-map[0].GetSize().cx,point.y-map[0].GetSize().cy);
	map[1].MoveWindow (point.x,point.y-map[0].GetSize().cy);
	map[2].MoveWindow (point.x-map[0].GetSize().cx,point.y);
	map[3].MoveWindow (point.x,point.y);

}

//ȡ�÷ָ���
CPoint CMapWnd:: GetSplitPoint()
{
	return point;
}

