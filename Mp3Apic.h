/************************************************************************/
/* *��ȡMP3����Ƕר��ͼƬ 
** ���ݷŵ�buffer��  buffer���Ǳ�׼ѹ����jpeg����png����
** ʹ����ǵ�ɾ��
**BY yuqingbin
/************************************************************************/
#pragma once
#include "ImageDecoder.h"   //����IMageͷ�ļ�
class Mp3Apic
{
public:
	Mp3Apic(HWND hWnd);
	~Mp3Apic(void);
	BOOL ReadMp3Apic(TCHAR *inFile,char **Buffer);  //���ݷ���*Buffer
	int MP3Load(TCHAR *Filename, unsigned char **Buffer, int *Width, int *Height, int *Depth);
	
	
	HWND g_hWnd;
	DWORD FrameSize;
};
