////////////////////////////////////////////////////////////
//��ȡ��������
//
//BY  yuqingbin
////////////////////////////////////////////////////////////


#pragma once
#include "afx.h"


typedef struct 
{
	_int64 Time; //LONGLONG Time; //ʱ���
	 TCHAR    Lyric[1000];    //���һ��
}LYRIC; // //���ʱ����ṹ��

bool SplitLyric(CString path); //��ȡ���Ѹ���зֳɾ�