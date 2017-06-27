///////////////////////////////////////////////////////////
//DBHandle ���ݿ⴦����
//��¼�ṹ����������ID��ԭͼ·�����ü�ͼ·��
//BY yuqingbin
///////////////////////////////////////////////////////////
#pragma once
#include "windows.h"
#define MAX_IDLEN 64
#define MAX_PATH  260
#define PID_ID  MAKELONG(CEVT_I2,1) 
#define PID_PATH  MAKELONG(CEVT_LPWSTR,2)
#define PID_CUTPATH  MAKELONG(CEVT_LPWSTR,3)
#define numOfProps  3

typedef struct  
{
	INT strID;                     
	TCHAR strPATH[MAX_PATH];
	TCHAR strCUTPATH[MAX_PATH];
}MYRECORDINFO;   //��¼�ṹ

typedef struct
{
	int nItem;
	MYRECORDINFO myRecordinfo;
}MYLVDATA; 

class DBHandle
{
public:
	DBHandle(void);
	~DBHandle(void);
	HANDLE  myOpenDataBase(HWND,int*);    //�����ݱ�
	void myGetItemRecord(int,MYLVDATA*); //��ȡ���ǵڼ�����¼
	BOOL mySetItemRecord(MYLVDATA*);    //��Ӽ�¼�Զ���ӵ�β��
	BOOL myDeleteItemRecord(int);    //ɾ����¼
	void DoCreateDB(HWND hWnd,TCHAR *DBPath,int Mode);     //�򿪻򴴽����ݿ� DBPath ���ݿ�·��  Mode���򿪷�ʽ
	int  totalNum ;
};
