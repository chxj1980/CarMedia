/************************************************************************/
/* USBģ����
USB��ģ�鹦��ʵ��
*/
/************************************************************************/
#pragma once
#include "directshow.h"
#include "Char2Wide.h"
#include "ImageDecoder.h"
#include "MP3INF.h"
#include "Mp3Apic.h"
#include "Scene.h"
#include "CutPicBmpYu.h"
#include "CeFileManager.h"

//enum ListType{Audio,Video};

typedef struct FileInfoUsb   /*��Ƶ�ļ���Ϣ*/
{
  TCHAR name[_MAX_FNAME];
  TCHAR path[_MAX_PATH];
};

typedef struct    /*ͼƬ����*/
{
	BYTE Data[100*100*3];
	BOOL Turn;
}PicBuffer;

class CUsbSd 
{
public:
	CUsbSd(void);
	~CUsbSd(void);
	void DoDelObject();
	void DoPaintInit(HWND hWnd);
	void DoPaintSelPic(HWND hWnd,BOOL FLAG);
	void DoPaintSelVid(HWND hWnd,BOOL FLAG);
	void DoPaintSelAud(HWND hWnd,BOOL FLAG);
    void DoPaintSelEbk(HWND hWnd,BOOL FLAG);
	

	void DoPaintWindows(HWND,int,int,int,int,HBITMAP,int,int); //��ͼ
	void DoLoadPicture();
	void DoFileInit(HWND hWnd,ManagerMode mode,FileFor filetype);                 //��ʼ��  �ŵ���������ʱ

	void DoPaintHengliang(HWND);
	void DoPaintAudioPliFace(HWND);
	void DoPaintAudioSlider(HWND,INT);

	int DoLbuttonDown(HWND,INT,INT);   //��Ӧ���������Ϣ WM_LBUTTONDWON
	int DoMouseMove(HWND,INT,INT);     //��Ӧ����ƶ���Ϣ WM_MOVE
	int DoLbuttonUp(HWND,INT,INT);     //��Ӧ���̧����Ϣ WM_LBUTTONUP
	int DoLDbClick(HWND,LPARAM);
	int DoTime(HWND,INT);              //��Ӧ��ʱ����Ϣ   WM_TIMER
	int DoPaint(HWND);                //��ӦWM_PAINT��Ϣ
    void Doinit(HWND);

	void DoAudioMode(HWND);
	void DoDrawTotalTime(HWND);
	void DoDrawCurrentTime(HWND);
	void DoPaintPicBrowface(HWND);
	void DoPaintVideoFace(HWND);
	void DoPaintAudMode(HWND);
	void DoFullScanPic(HWND);
	void PaintVideoName(HWND);
	void DoPiantVideoTime();
	
   
	void MoveVideoList(HWND hWnd,int ypos);                               //��Ƶ�б�
    void CreateVideoListDC(HWND hWnd,int width,int height,TCHAR *filebk);
	

	void DoCreateAudiDCmem(HWND hWnd);
	void CreateAudioListDC(HWND hWnd,int width,int height,TCHAR *filebk); //��Ƶ�б�
	void MoveAudioList(HWND hWnd,int ypos);

	void MoveEbookList(HWND hWnd,int ypos);
	
    
	void PictureInit(int PicAllCount);          //ͼƬ����� ��ʼ����������
    void PictureLoad(int firstnum,int numCount);    //�������� ���
	BOOL PictureLoad(int number);    //��������  ����
	void PictureLoad(int number,BYTE *buffer);    //��������  ����
	

	PictureCutYu *JpegPicCut;
	CutPicBmpYu  *BmpPicCut;
	Mp3Apic      *Mp3Texture;
	BOOL PicIsBmp(TCHAR *FilePath);

    PicBuffer *lpPicBuffer;

private:
	HANDLE  PicThread;
	TGAImage img;
	MediaControl *Media;
	MP3INF *mp3;
	

public:
	HWND  VidWnd;
	HWND g_UsbWnd;
    BOOL PICSEL,AUDSEL,VIDSEL;     //�˵������ʾ 
	BOOL MENUSHOW ;       //��ʾ�˵�����
	BOOL BROWPIC ;      //ͼƬ�����ʾ
	


	BOOL SINGLESCAN;
	int  PicNum;   /*0-19*/
	BOOL FULLPIC;
	int PicCurrentNum; /*0-pictotalnum����*/


	FileInfoUsb *AudioFile;  //��Ƶ
	FileInfoUsb *PictureFile;
	FileInfoUsb *VideoFile;
	FileInfoUsb *EbookFile;

	int AudioTCount;   //��Ƶ�ܸ��� ʹ�ð�
	int VideoTCount;   //��Ƶ�ܸ��� ʹ�ð�
	int PictureTcount; //ͼƬ
	int EbookTCount;
	
};
