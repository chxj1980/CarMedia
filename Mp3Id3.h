/////////////////////////////////////////////////////////////
//��ȡMP3�����������ҡ�ר������Ϣ
//
//BY  yuqingbin
/////////////////////////////////////////////////////////////


#pragma once
//#include "afx.h"
#include "tchar.h"
#include "atlstr.h" 


typedef struct pp //MP3��Ϣ�Ľṹ
{
	char Identify[3]; //TAG������ĸ//����������������ǲ����ļ���Ϣ����	
	char Title[31];   //��������30���ֽ�  
	char Artist[31];  //��������30���ֽ�
	char Album[31];   //������Ƭ��30���ֽ�
	char Year[5];	  //�꣬4���ֽ�
	char Comment[29]; //ע�ͣ�28���ֽ�
	unsigned char reserved;  //����λ��1���ֽ�
	unsigned char reserved2; //����λ��1���ֽ�
	unsigned char reserved3; //����λ��1���ֽ�
} MP3INFO;


//MP3INFO* ReadMp3Id3(CString strPlayListName);  //lpTitle ������  lpArtist ������  lpAlbum ר����
MP3INFO* ReadMp3Id3(TCHAR* strPlayListName);  //lpTitle ������  lpArtist ������  lpAlbum ר����