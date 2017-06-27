/**************************************************************************
** ��Ƶ�б�ʹ�ã�AudioListHdcV2�б���DC��input��Ƶ�ļ����飨�ļ�����·������
**               input��Ƶ�ļ��ܸ���
** ��Ƶ�б�ʹ�ã�����
**
** ����Ķ�����Ҫɾ����ɾ���껹��ʹ�õ��� AudioFile��VideoFile��PictureFile
**                                        g_scene->mp3text  g_scene->bmptext
                                          new �Ķ���ʹ��ǰ Ҫ�ж��Ƿ�Ϊ��
										  ��Ȼ��ɾ��
**************************************************************************/
#include "UsbSd.h"
#include "Mp3Apic.h"
#include "Scene.h"
#include "Overlay.h"
#include "eUtil.h"
#include "CeFileManager.h"
#include "ShellOS.h"

extern ShellOS *shellOS;


//#include "textPro.h"
#include <commctrl.h> 
#include "atlstr.h"   
#include "TxtEdit.h"
#include "public.h"
#include "ElecBook.h"
#include <algorithm>
using namespace std;

extern MSearchFile* SFile;

COverlay VideoOverlay;
//////////////////�ⲿ����///////////////////////////
extern Scene *g_scene;
/////////////////////////////////////////////
#define  SINGLE_CLICK        8                      //��������
#define  MOUSE_MOVE          15                      //mouse move
////////////////////��ʱ��////////////////////////////
#define TIME_AUDISLID        186     //��Ƶ��������ʱ��
#define TIME_OVERLAYSHOW     187
#define TIME_VIDEOFULL       188
#define TIME_TEMP            189
#define TIME_MEMORYLEAK      190
#define TIME_PICSCANREFRESH  191
#define TIME_MOVELIST        192
#define TIME_STOPMOVE        193
#define TIME_DBCLICK         194

///////////////////////////////  
#define ENABLEOVERLAY   1        //Overlay�Ƿ���Ч
#define ENABLEEBOOK     0        //�������Ƿ���Ч

#define AGELISTLEN      70
///////////////////////////////////////global varibale/////////////////////////////////////////
HBITMAP hbmpBkGround;
HBRUSH holdbrush,holdbrush1;
BITMAP bmp;
////////////////////
HDC hdcSlide;          //�б�����
HDC hdcLaba;           //��Ƶ����С����
HDC AudioListHdcV2;    //��Ƶ�б���
HDC HdcAudioSel;          //��Ƶѡ��Ч��
HFONT FontAudiList;
///////////////////////////////
HDC hdcVideoList; //��Ƶ
HDC hdcVidTemp;

HDC VideoListHdcV2;

BOOL MOVEVIDEOLIST = FALSE;
int  VideoPlayNum = 0;
int  Viddwpos = 0;
int  Vidpos = 0;
BOOL VIDEOPLAYING = FALSE;   //��Ƶ���ڲ���
BOOL VIDEOLISTSEL = FALSE;  //ѡ��Ҫ���ŵ���Ƶ
int  Dbclick = 0;    
LONGLONG  AudioLength = 0;
///////////////////////////////
/*
����
*/
GLuint Mp3Moren;
GLuint BmpMoren;

int pos = 0;
int Auddwpos = 0;
int Audpos = 0;
int ChickPos = 0;
int Picpos =0;
int Picdwpos = 0;
BOOL MULTISCAN = FALSE;
BOOL MOVEAUDIOLISTFLAG = FALSE; 
int Movet1 =0;
int Movet2 = 0;
int ListMove = 0;
BOOL AUDIOLISTSEL = FALSE;
BOOL AUDIOPLAYING = FALSE;        //��Ƶ���ű�ʾ
int  AudioPlayNum = 0;            //��ǰ����ʱ��
int  PicFulldwpos = 0;
BOOL SURFACEINIT = TRUE; //��ʼ������
BOOL AUDIOPLINGFACE;
                                 
BOOL  RandMode = FALSE;     //�������ģʽ
BOOL  CycMode = TRUE;     //ѭ������ģʽ

BOOL VIDEOFACE = FALSE;    //��Ƶ���Ž���
BOOL DBWINDOW = FALSE;
BOOL VIDEOFULL = FALSE;
BOOL OVERLAYSHOW = FALSE;
BOOL LABASHOW = FALSE;
//����������
BOOL SCANEBOOK = FALSE;
BOOL EBOOKSEL = FALSE;
BOOL MOVEEBOOKLIST = FALSE;
BOOL EBOOKLISTSEL = FALSE;
int  EbookSelnum = 0;
int  Ebookpos = 0;
int  Ebookdwpos = 0;


#define LINE_LENGTH 100//84
#define LINAGE 13  //ÿҳ��ʾ������
#define KEYPAGE_SPACE 8  //�ؼ�ҳ���
HANDLE hThread;
CRITICAL_SECTION txt_critical_section; 
ElecBook* txtBook;
DWORD WINAPI TxtThread(LPVOID lpParameter);  //�������Ű��߳�
BOOL  INIT = FALSE;

////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////��������߳�///////////////////////////
//CRITICAL_SECTION g_cs;

DWORD WINAPI ThreadLoadPic0(LPVOID lpParam)
{
  CUsbSd *usb = (CUsbSd *)lpParam;
  for (int i=0;i<usb->PictureTcount;i++)
  {
	usb->PictureLoad(i);
	KillTimer(usb->g_UsbWnd,TIME_PICSCANREFRESH);
	SetTimer(usb->g_UsbWnd,TIME_PICSCANREFRESH,1,NULL);
  }
  return 1;
}

/*�򿪳���ʱ��createwindow�ĳ�ʼ��*/
void CUsbSd::Doinit(HWND hWnd)
{
  DoLoadPicture();       //��ʼ�Ľ���ʹ��ͼƬ
  PicCurrentNum = 0;
  PICSEL = FALSE;
  AUDSEL = FALSE;
  VIDSEL = FALSE;
  SINGLESCAN = FALSE;
  MENUSHOW = TRUE;
  AUDIOPLINGFACE = FALSE;                      //��Ƶ���Ž���
  FULLPIC = FALSE;

  Mp3Moren = g_scene->LoadBMP(&img,L"\\Storage Card\\USBDATA\\mp3moren.bmp");
  BmpMoren = g_scene->LoadBMP(&img,L"\\Storage Card\\USBDATA\\moren.bmp");
   /*
	��Ƶ�б���hdc����
	����hdcʱʹ�õ�hbmp����Ҫ��ʹ����ʱɾ��
	*/
   /// CreateVideoListDC(hWnd,602,423,L"\\Storage Card\\USBDATA\\VideoList.bmp");  //V2    ׼����Ƶ����
    
	/*
	��Ƶ�б���hdc����
	*/
    DoCreateAudiDCmem(hWnd);  //������Ƶ���ȡ���������ѡ��Ч��
	CreateAudioListDC(hWnd,603,490,L"\\Storage Card\\USBDATA\\AudiList.bmp");   //V2    ׼����Ƶ����
	FontAudiList = CreateFont(35,0,0,0,FW_LIGHT,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH,_T("΢���ź�"));

}
/*
���뱾�����ʼ��new���ࡢ���顢������hdcmem��������ȫ�����塢ͼƬ������Ƶ����
ͼƬ������������Ƶ����������ͼƬ����ͼ������ת������

���Ƴ�������ʱ��Ҫɾ�������㡢ȫ�ִ�����Ҫɾ��������״̬Ҫ��ʼ����ѡ��״̬Ҫ��ʼ��
*/
void CUsbSd::DoFileInit(HWND hWnd,ManagerMode mode,FileFor filetype)            //ģ���ʼ�� ��������ʱ��ʼ����ģ��
{
   g_UsbWnd = hWnd;
#if  0
	
	SetTimer(hWnd,TIME_MEMORYLEAK,500,NULL);
#endif
#if  0
	int t1 = GetTickCount();
#endif

	//��ʼ�� ��Ƶ����Ƶ��ͼƬ�������� �õ��Ķ���   
	//�ж��Ƿ����³�ʼ��
	//��ָ��ָ��� �ڴ���յ�
    
	//��ʼ���ٽ�������
	//InitializeCriticalSection(&txt_critical_section);  
	 pos = 0;
     Auddwpos = 0;
	 Audpos = 0;
     ChickPos = 0;
     Picpos =0;
	 Picdwpos = 0;
	 VideoPlayNum = 0;
	 Viddwpos = 0;
	 Vidpos = 0;
	 Ebookpos = 0;
	 Ebookdwpos = 0;
	 SCANEBOOK = FALSE;

  if((PictureTcount == 0) || (AudioTCount==0 )||(VideoTCount==0))  //��ʼһ��
  {
	if(mode == SD)
	{
	  switch(filetype)
	  {
	    case AUDIO:
		  { 
			  AudioTCount = SFile->SDAudioVector.size();
			  AudioFile = new FileInfoUsb[AudioTCount];
            for (int i=0;i<AudioTCount;i++)
            {
				lstrcpy(AudioFile[i].path,SFile->SDAudioVector[i].pathname);
				lstrcpy(AudioFile[i].name,SFile->SDAudioVector[i].filename);
            }
		  }
		break;
		case VIDEO:
			{ 
				VideoTCount = SFile->SDVideoVector.size();
				VideoFile = new FileInfoUsb[VideoTCount];
				for (int i=0;i<VideoTCount;i++)
				{
					lstrcpy(VideoFile[i].path,SFile->SDVideoVector[i].pathname);
					lstrcpy(VideoFile[i].name,SFile->SDVideoVector[i].filename);
				}
			}
			break;
		case PICTURE:
			{ 
				PictureTcount = SFile->SDPicVector.size();
				PictureFile = new FileInfoUsb[PictureTcount];
				for (int i=0;i<PictureTcount;i++)
				{
					lstrcpy(PictureFile[i].path,SFile->SDPicVector[i].pathname);
					lstrcpy(PictureFile[i].name,SFile->SDPicVector[i].filename);
				}
			}
			break;
		case TEXT:
			{ 
				EbookTCount = SFile->SDTxtVector.size();
				EbookFile = new FileInfoUsb[EbookTCount];
				for (int i=0;i<EbookTCount;i++)
				{
					lstrcpy(EbookFile[i].path,SFile->SDTxtVector[i].pathname);
					lstrcpy(EbookFile[i].name,SFile->SDTxtVector[i].filename);
				}
			}
			break;
		default:
			break;

	  }
	}
	if(mode == USB)
	{
		switch(filetype)
		{
		case AUDIO:
			{ 
				AudioTCount = SFile->USBAudioVector.size();
				AudioFile = new FileInfoUsb[AudioTCount];
				for (int i=0;i<AudioTCount;i++)
				{
					lstrcpy(AudioFile[i].path,SFile->USBAudioVector[i].pathname);
					lstrcpy(AudioFile[i].name,SFile->USBAudioVector[i].filename);
				}
			}
			break;
		case VIDEO:
			{ 
				VideoTCount = SFile->USBVideoVector.size();
				VideoFile = new FileInfoUsb[VideoTCount];
				for (int i=0;i<VideoTCount;i++)
				{
					lstrcpy(VideoFile[i].path,SFile->USBVideoVector[i].pathname);
					lstrcpy(VideoFile[i].name,SFile->USBVideoVector[i].filename);
				}
			}
			break;
		case PICTURE:
			{ 
				PictureTcount = SFile->USBPicVector.size();
				PictureFile = new FileInfoUsb[PictureTcount];
				for (int i=0;i<PictureTcount;i++)
				{
					lstrcpy(PictureFile[i].path,SFile->USBPicVector[i].pathname);
					lstrcpy(PictureFile[i].name,SFile->USBPicVector[i].filename);
				}
			}
			break;
		case TEXT:
			{ 
				EbookTCount = SFile->USBTxtVector.size();
				EbookFile = new FileInfoUsb[EbookTCount];
				for (int i=0;i<EbookTCount;i++)
				{
					lstrcpy(EbookFile[i].path,SFile->USBTxtVector[i].pathname);
					lstrcpy(EbookFile[i].name,SFile->USBTxtVector[i].filename);
				}
			}
			break;
		default:
			break;

		}
	}
	if(mode == Local)
	{
		switch(filetype)
		{
		case AUDIO:
			{ 
				AudioTCount = SFile->LocalAudioVector.size();
				AudioFile = new FileInfoUsb[AudioTCount];
				for (int i=0;i<AudioTCount;i++)
				{
					lstrcpy(AudioFile[i].path,SFile->LocalAudioVector[i].pathname);
					lstrcpy(AudioFile[i].name,SFile->LocalAudioVector[i].filename);
				}
			}
			break;
		case VIDEO:
			{ 
				VideoTCount = SFile->LocalVideoVector.size();
				VideoFile = new FileInfoUsb[VideoTCount];
				for (int i=0;i<VideoTCount;i++)
				{
					lstrcpy(VideoFile[i].path,SFile->LocalVideoVector[i].pathname);
					lstrcpy(VideoFile[i].name,SFile->LocalVideoVector[i].filename);
				}
			}
			break;
		case PICTURE:
			{ 
				PictureTcount = SFile->LocalPicVector.size();
				PictureFile = new FileInfoUsb[PictureTcount];
				for (int i=0;i<PictureTcount;i++)
				{
					lstrcpy(PictureFile[i].path,SFile->LocalPicVector[i].pathname);
					lstrcpy(PictureFile[i].name,SFile->LocalPicVector[i].filename);
				}
			}
			break;
		case TEXT:
			{ 
				EbookTCount = SFile->LocalTxtVector.size();
				EbookFile = new FileInfoUsb[EbookTCount];
				for (int i=0;i<EbookTCount;i++)
				{
					lstrcpy(EbookFile[i].path,SFile->LocalTxtVector[i].pathname);
					lstrcpy(EbookFile[i].name,SFile->LocalTxtVector[i].filename);
				}
			}
			break;
		default:
			break;

		}
	}
 }
	
#if 0
	int t2 = GetTickCount();
	TCHAR str[100];
	_stprintf(str,L"%s%d",L"init",t2-t1);
	OutputDebugString(str);
	OutputDebugString(L"\n");
#endif
}



void CUsbSd::DoDelObject()
{
	DeleteDC(AudioListHdcV2);
	DeleteDC(hdcSlide);
	DeleteDC(hdcLaba);
  
	if(hbmpBkGround)
		DeleteObject(hbmpBkGround);	
}

/*�򿪳���ʱ�½�����ʱ�ĳ�ʼ��*/
CUsbSd::CUsbSd()
{
  AudioFile = NULL;  //ָ���ʼ��  ʹ��ǰ��ַ�����ÿ�
  VideoFile = NULL;  
  PictureFile = NULL;
  EbookFile = NULL;
  lpPicBuffer = NULL;
 
#if ENABLEOVERLAY
  if(VideoOverlay.CreateSurface(800,87) == 0)   //����overlay
	  MessageBox(NULL,L"create overlay error",L"",NULL);
#endif
  JpegPicCut = new PictureCutYu(NULL);
  BmpPicCut = new CutPicBmpYu();
  Media =new MediaControl();
  Mp3Texture = new Mp3Apic(NULL);
  mp3 = new MP3INF();   //mp3��Ϣ
  //��ʼ��COM����
  CoInitializeEx(NULL, COINIT_MULTITHREADED);
}

CUsbSd::~CUsbSd(void)
{
	DoDelObject();
	if(AudioFile)
	{
		delete AudioFile;
		AudioFile = NULL;
	}
	if(VideoFile)
	{
		delete VideoFile;
		VideoFile = NULL;
	}
	if(PictureFile)
	{
 	  delete PictureFile;
	  PictureFile = NULL;
	}
	if(Media)
	{
		delete Media;
		Media = NULL;
	}
	if(mp3)
	{
		delete mp3;
		mp3 = NULL;
	}

	if(lpPicBuffer)
	{
		delete lpPicBuffer;
		lpPicBuffer = NULL;
	}
	//�ͷ�COM����
	CoUninitialize();
}
void CUsbSd::DoPaintInit(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	HDC hdcmem = CreateCompatibleDC(hdc);
   
	HBITMAP hbmphl = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\3-3.bmp");  //��ɫ����  5
	holdbrush = (HBRUSH)SelectObject(hdcmem,hbmphl);
	GetObject(hbmphl,sizeof(BITMAP),&bmp);
	BitBlt(hdc,0,0,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY); //**/
	SelectObject(hdcmem,holdbrush);
    
	HBITMAP hbmpBak = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\1.bmp"); //����
	holdbrush = (HBRUSH)SelectObject(hdcmem,hbmpBak);
	GetObject(hbmpBak,sizeof(BITMAP),&bmp);
	BitBlt(hdc,0,60,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
	SelectObject(hdcmem,holdbrush);

	DeleteDC(hdcmem);
	DeleteObject(hbmphl);
    DeleteObject(hbmpBak);
	ReleaseDC(hWnd,hdc);
}
void CUsbSd::DoPaintSelPic(HWND hWnd,BOOL FLAG)
{
	if (FLAG)
	{   
		HDC hdc = GetDC(hWnd);
		HDC hdcmem = CreateCompatibleDC(hdc);

		DoPaintWindows(hWnd,198,60,800-198,480-60,hbmpBkGround,198,60);
        HBITMAP hbmphl = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\3-3.bmp");  //��ɫ����  5
		holdbrush = (HBRUSH)SelectObject(hdcmem,hbmphl);  //�׺���
		GetObject(hbmphl,sizeof(BITMAP),&bmp);
		BitBlt(hdc,0,0,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
		SelectObject(hdcmem,holdbrush);

		HBITMAP hbmpBak = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\1.bmp"); //����
		holdbrush = (HBRUSH)SelectObject(hdcmem,hbmpBak);  //����
		GetObject(hbmpBak,sizeof(BITMAP),&bmp);
		BitBlt(hdc,0,60,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
		SelectObject(hdcmem,holdbrush);
    
		HBITMAP hbmppicsel = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\2.bmp"); //ѡ��ͼƬ�˵�
		holdbrush = (HBRUSH)SelectObject(hdcmem,hbmppicsel);  //ͼƬѡ��
		GetObject(hbmppicsel,sizeof(BITMAP),&bmp);
		BitBlt(hdc,0,60,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
		SelectObject(hdcmem,holdbrush);

		DeleteDC(hdcmem);
		DeleteObject(hbmphl);
		DeleteObject(hbmppicsel);
		DeleteObject(hbmpBak);
		ReleaseDC(hWnd,hdc);
	}
}

void CUsbSd::DoPaintSelVid(HWND hWnd,BOOL FLAG)
{
	if (FLAG)
	{   
		HDC hdc = GetDC(hWnd);
		HDC hdcmem = CreateCompatibleDC(hdc);

		HBITMAP hbmphl = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\videobk-1.bmp");  //��ɫ����  5
		holdbrush = (HBRUSH)SelectObject(hdcmem,hbmphl);   //����
		GetObject(hbmphl,sizeof(BITMAP),&bmp);
		BitBlt(hdc,0,0,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
		SelectObject(hdcmem,holdbrush);

		HBITMAP hbmpBak = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\1.bmp"); //����
		holdbrush = (HBRUSH)SelectObject(hdcmem,hbmpBak);  //���ĸ�
		GetObject(hbmpBak,sizeof(BITMAP),&bmp);
		BitBlt(hdc,0,60,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
		SelectObject(hdcmem,holdbrush);

        HBITMAP hbmpvisel = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\6.bmp");  //��Ƶ�˵�ѡ
		holdbrush = (HBRUSH)SelectObject(hdcmem,hbmpvisel);   //��Ƶѡ��˵�
		GetObject(hbmpvisel,sizeof(BITMAP),&bmp);
		BitBlt(hdc,0,166,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
		SelectObject(hdcmem,holdbrush);
		
		DeleteDC(hdcmem);
		
		DeleteObject(hbmpvisel);
		DeleteObject(hbmphl);
		DeleteObject(hbmpBak);
		ReleaseDC(hWnd,hdc);
	}
}
void CUsbSd::DoPaintSelAud(HWND hWnd,BOOL FLAG)
{
	if (FLAG)
	{   
		HDC hdc = GetDC(hWnd);
		HDC hdcmem = CreateCompatibleDC(hdc);

		HBITMAP hbmphl = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\musicBk1.bmp");  //��ɫ����  5
		holdbrush = (HBRUSH)SelectObject(hdcmem,hbmphl);  //����
		GetObject(hbmphl,sizeof(BITMAP),&bmp);
		BitBlt(hdc,0,0,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
		SelectObject(hdcmem,holdbrush);

		HBITMAP hbmpBak = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\1.bmp"); //����
		holdbrush = (HBRUSH)SelectObject(hdcmem,hbmpBak);  //����
		GetObject(hbmpBak,sizeof(BITMAP),&bmp);
		BitBlt(hdc,0,60,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
		SelectObject(hdcmem,holdbrush);
    
		HBITMAP hbmpausel = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\7.bmp");  //��Ƶ�˵�ѡ��
		holdbrush = (HBRUSH)SelectObject(hdcmem,hbmpausel);  //��Ƶѡ��
		GetObject(hbmpausel,sizeof(BITMAP),&bmp);
		BitBlt(hdc,0,271,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
		SelectObject(hdcmem,holdbrush);

		DeleteDC(hdcmem);
		DeleteObject(hbmpausel);
        DeleteObject(hbmphl);
		DeleteObject(hbmpBak);
		ReleaseDC(hWnd,hdc);
	}
}


void CUsbSd::DoPaintSelEbk(HWND hWnd,BOOL FLAG)
{
	if (FLAG)
	{   
		HDC hdc = GetDC(hWnd);
		HDC hdcmem = CreateCompatibleDC(hdc);

		HBITMAP hbmphl = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\3-3.bmp");  //��ɫ����  5
		holdbrush = (HBRUSH)SelectObject(hdcmem,hbmphl);  //����
		GetObject(hbmphl,sizeof(BITMAP),&bmp);
		BitBlt(hdc,0,0,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
		SelectObject(hdcmem,holdbrush);

		HBITMAP hbmpBak = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\1.bmp"); //����
		holdbrush = (HBRUSH)SelectObject(hdcmem,hbmpBak);  //����
		GetObject(hbmpBak,sizeof(BITMAP),&bmp);
		BitBlt(hdc,0,60,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
		SelectObject(hdcmem,holdbrush);

		HBITMAP hbmpausel = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\Ebook.bmp");  //��Ƶ�˵�ѡ��
		holdbrush = (HBRUSH)SelectObject(hdcmem,hbmpausel);  //��Ƶѡ��
		GetObject(hbmpausel,sizeof(BITMAP),&bmp);
		BitBlt(hdc,0,376,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
		SelectObject(hdcmem,holdbrush);

		DeleteDC(hdcmem);
		DeleteObject(hbmpausel);
		DeleteObject(hbmphl);
		DeleteObject(hbmpBak);
		ReleaseDC(hWnd,hdc);
	}
}


void CUsbSd::DoCreateAudiDCmem(HWND hWnd)
{
	HDC hdc  = GetDC(hWnd);
	HBITMAP hbmp,hbmplaba,hbmpslide,hbmpaudsel,hbmptmp;
	HDC hdcmem;
	hdcmem   = CreateCompatibleDC(hdc);
	
	hdcLaba  = CreateCompatibleDC(hdc);
	hdcSlide = CreateCompatibleDC(hdc);  //������
	HdcAudioSel = CreateCompatibleDC(hdc);//��Ƶѡ��

	hbmplaba = CreateCompatibleBitmap(hdc,32,28);
	hbmpslide= CreateCompatibleBitmap(hdc,17,49);
	hbmpaudsel = CreateCompatibleBitmap(hdc,602,AGELISTLEN);
	
	SelectObject(hdcLaba,hbmplaba);
	SelectObject(hdcSlide,hbmpslide);
	SelectObject(HdcAudioSel,hbmpaudsel);
	
	/*������ɾ����Ч����Ҫ��ʹ�������hdc��ʱ��ɾ��*/
	DeleteObject(hbmplaba);    
	DeleteObject(hbmpslide);
	DeleteObject(hbmpaudsel);

    HBITMAP	hbmpaub = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\13.bmp");  //��Ƶ���ű�ʾ
	holdbrush = (HBRUSH)SelectObject(hdcmem,hbmpaub); //����
	GetObject(hbmpaub,sizeof(BITMAP),&bmp); 
	BitBlt(hdcLaba,0,0,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
	SelectObject(hdcmem,holdbrush);
    
	HBITMAP hbmpsl = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\14.bmp");  //�б������
	holdbrush = (HBRUSH)SelectObject(hdcmem,hbmpsl); //������
	GetObject(hbmpsl,sizeof(BITMAP),&bmp); 
	BitBlt(hdcSlide,0,0,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
	SelectObject(hdcmem,holdbrush);

	HBITMAP hbmpaadsel = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\AudioSel.bmp");  //�б������
	holdbrush = (HBRUSH)SelectObject(hdcmem,hbmpaadsel); //���Ч��
	GetObject(hbmpaadsel,sizeof(BITMAP),&bmp); 
	BitBlt(HdcAudioSel,0,0,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
	SelectObject(hdcmem,holdbrush);
	
	DeleteDC(hdcmem);
	DeleteObject(hbmpsl);
	DeleteObject(hbmpaub);
	DeleteObject(hbmpaadsel);

	ReleaseDC(hWnd,hdc);
}


void CUsbSd::DoPaintWindows(HWND hWnd,int xPos,int yPos,int width,int height,HBITMAP phBitmap,int hxPos,int hyPos)
{
	HDC hdc = GetDC(hWnd);
	HDC hdcmem = CreateCompatibleDC(hdc);
	HBRUSH hBR = (HBRUSH)SelectObject(hdcmem,phBitmap);
	BitBlt(hdc,xPos,yPos,width,height,hdcmem,hxPos,hyPos,SRCCOPY);
	SelectObject(hdcmem,hBR);
	DeleteDC(hdcmem);
	ReleaseDC(hWnd,hdc);
}
void CUsbSd::DoLoadPicture()  //��ʼ������   
{
	//////////////////////////����ͼƬ/////////////////////////////	
	if(!hbmpBkGround)
		hbmpBkGround = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\BackGround.bmp");  //�������
}

void CUsbSd::DoPaintAudioPliFace(HWND hWnd)
{
	HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\musicBK2.bmp");   //��Ƶ���ű���
	DoPaintWindows(hWnd,0,0,800,64,hbmp,0,0); //����
	DeleteObject(hbmp);
    
	HBITMAP hmusicbb = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\musicbb.bmp");
	DoPaintWindows(hWnd,0,384,800,96,hmusicbb,0,0); //����������
	DeleteObject(hmusicbb);
	DoDrawTotalTime(hWnd);  //����ʱ��		

	DoPaintAudMode(hWnd);    //���Ʋ���ģʽ

	if(AUDIOPLAYING&&AUDIOPLINGFACE)
	{
		HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\push.bmp");
		DoPaintWindows(hWnd,360,398,81,82,hbmp,0,0);     //��ͣ
        DeleteObject(hbmp);
	}
	else
	{
		HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\play1.bmp");
		DoPaintWindows(hWnd,360,398,81,82,hbmp,0,0);     //����
        DeleteObject(hbmp);
	}

	///////////////////////////////������Ϣ�����ҡ�ר��������������ʾ/////////////////////////////////////
	MP3INFO mp3info = {0}; //�洢MP3��Ϣ�Ľṹ��

	if(mp3->ReadMp3Info(AudioFile[AudioPlayNum].path,&mp3info))
	{
		HDC hdc = GetDC(hWnd);
		SetBkMode(hdc,0);
		SetTextColor(hdc,RGB(0,0,255));

		RECT TextRc = {300,60,500,90};
		HFONT FontList = CreateFont(25,0,0,0,FW_LIGHT,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH,_T("΢���ź�"));
		HFONT fontold = (HFONT)SelectObject(hdc,FontList);
		SetTextCharacterExtra(hdc,0);
		DrawText(hdc,mp3info.Title,-1,&TextRc,DT_CENTER);
        SelectObject(hdc,fontold);
		DeleteObject(FontList);
		ReleaseDC(hWnd,hdc);
	}

	////////////////////////////////////////////////////////////////////
}
void CUsbSd::DoPaintAudioSlider(HWND hWnd,INT AuPos)  //AuPos>200
{
     HBITMAP hbmp1 = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\BlackMu-1.bmp");   //��Ƶ��������
	 DoPaintWindows(hWnd,AuPos,384,800-AuPos,14,hbmp1,0,0);    //�ڵ�
	 DeleteObject(hbmp1);
	 HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\BlueMu-6.bmp");    //��Ƶ��������
	 DoPaintWindows(hWnd,0,384,AuPos,14,hbmp,800-AuPos,0);  //����	
	 DeleteObject(hbmp);
}
int  CUsbSd::DoLDbClick(HWND hWnd,LPARAM lParam)
{
	int xpoint = LOWORD(lParam);
	int ypoint = HIWORD(lParam);
	if(hWnd == VidWnd)
	{
      hWnd = g_UsbWnd;
	 if((xpoint>0 && xpoint<431)&&(ypoint>0 && ypoint<282)&&VIDEOFACE&&VIDEOPLAYING&&!VIDEOFULL) //ȫ��
	 {
		
		KillTimer(hWnd,TIME_AUDISLID);
		SetTimer(hWnd,TIME_VIDEOFULL,500,NULL);  //����ȫ��������
		KillTimer(VidWnd,TIME_OVERLAYSHOW);

		VIDEOFACE = FALSE;                       //ȫ�����Ž���
		Media->DshowFullWindow(TRUE);  
        SetWindowPos(VidWnd,NULL,0,0,800,480,0);
		VIDEOFULL = TRUE;
		RECT rSrc = {0,0,800,87};
		RECT rDes = {0,393,800,480};
		OVERLAYSHOW = FALSE;
		SetTimer(hWnd,TIME_OVERLAYSHOW,3000,NULL);

		DoPiantVideoTime();
		return 0;
	 }
	 //��Ƶȫ����С��
	 if((xpoint>0 && xpoint<800)&&(ypoint>0 && ypoint<350)&&VIDEOPLAYING&&VIDEOFULL)
	 {
		
		SetTimer(hWnd,TIME_TEMP,1,NULL);   //��ת��ʱ��
		KillTimer(hWnd,TIME_VIDEOFULL); //�ر�ȫ��������
		KillTimer(hWnd,TIME_OVERLAYSHOW);
		VIDEOFACE = TRUE;                              //С��״̬
		OVERLAYSHOW = FALSE;
		VideoOverlay.UpdateOverlay(NULL,NULL,DDOVER_HIDE,0);  
		Media->DshowFullWindow(FALSE);

		SetWindowPos(VidWnd,NULL,183,101,431,282,0);

		DoPaintVideoFace(hWnd);
		DoDrawTotalTime(hWnd);
		PaintVideoName(hWnd);

		OVERLAYSHOW = FALSE;
		VIDEOFULL = FALSE;
		return 0;
	  }
	}
	/*��ͼ��ȫ�����ͼƬ*/
	if(SINGLESCAN&&!FULLPIC&&(xpoint>180 && xpoint<620)&&(ypoint>80 && ypoint<380))
	{
		FULLPIC = TRUE;
		DoFullScanPic(hWnd);
		return 0;
	}

	/*ȫ������ͼ���*/
	if(FULLPIC)
	{
		FULLPIC = FALSE;
		g_scene->PicInit( );
		PicCurrentNum = g_scene->PicSingleScan(PicNum,Picdwpos);
		return 0;
	}
}
int  CUsbSd::DoLbuttonDown(HWND hWnd,INT xpoint,INT ypoint)
{
	//��Ƶȫ����һ��
#if ENABLEOVERLAY           //�Ƿ��overlay
	if((xpoint>233 && xpoint<292)&&(ypoint>410 && ypoint<480)&&!VIDEOFACE&&VIDEOFULL&&OVERLAYSHOW&&VIDEOPLAYING)
	 {
		VideoOverlay.DrawImageToOverlay(L"\\Storage Card\\USBDATA\\fullpre2.bmp",233,5);
		do 
		{
			VideoPlayNum--;
			if(VideoPlayNum<0)
				VideoPlayNum = 0;
		} while (!Media->DshowCreate(VidWnd,VideoFile[VideoPlayNum].path,hWnd));
		Media->DshowFullWindow(TRUE); 
		Media->DshowPlay();
		AudioLength = Media->DshowGetDuration();    //��ʱ��
		VideoOverlay.DrawImageToOverlay(L"\\Storage Card\\USBDATA\\fullpre1.bmp",233,5);
		DoPiantVideoTime();
		return 0;
	}
	//��Ƶȫ����һ��
	if((xpoint>508 && xpoint<567)&&(ypoint>410 && ypoint<480)&&!VIDEOFACE&&VIDEOFULL&&OVERLAYSHOW&&VIDEOPLAYING)
	{
        VideoOverlay.DrawImageToOverlay(L"\\Storage Card\\USBDATA\\fullnext2.bmp",508,5);
		do 
		{
			VideoPlayNum++;
			if(VideoPlayNum>VideoTCount-1)
				VideoPlayNum = VideoTCount-1;
		} while (!Media->DshowCreate(VidWnd,VideoFile[VideoPlayNum].path,hWnd));

		  Media->DshowFullWindow(TRUE); 
		  Media->DshowPlay();
		  AudioLength = Media->DshowGetDuration();  //��ʱ��
		  VideoOverlay.DrawImageToOverlay(L"\\Storage Card\\USBDATA\\fullnext1.bmp",508,5);
		  DoPiantVideoTime();
		  return 0;
	}
    //��Ƶȫ�� ��ť����
	if((xpoint>360 && xpoint<441)&&(ypoint>398 && ypoint<480)&&!VIDEOFACE&&VIDEOFULL&&OVERLAYSHOW)
	{
		if(VIDEOPLAYING)
		{
		 VIDEOPLAYING = FALSE;
         Media->DshowPause();
		 BOOL bl = VideoOverlay.DrawImageToOverlay(L"\\Storage Card\\USBDATA\\fullplay2.bmp",360,5);
		
		 KillTimer(hWnd,TIME_OVERLAYSHOW);
		}
		else
		{
			Media->DshowPlay();
            VideoOverlay.DrawImageToOverlay(L"\\Storage Card\\USBDATA\\fullpush1.bmp",360,5);
			VIDEOPLAYING = TRUE;
			SetTimer(hWnd,TIME_OVERLAYSHOW,3000,NULL);
		}
		return 0;
	}
	
	
    //��Ƶȫ����ʾOverlay
	if((xpoint>0 && xpoint<800)&&(ypoint>350 && ypoint<480)&&VIDEOFULL&&VIDEOPLAYING)
	{
		KillTimer(hWnd,TIME_OVERLAYSHOW);
		RECT rSrc = {0,0,800,87};
		RECT rDes = {0,393,800,480};
		if(!OVERLAYSHOW)
		{
		 VideoOverlay.UpdateOverlay(&rDes,&rSrc,DDOVER_SHOW,0xbf);          // ����͸����
		 OVERLAYSHOW = TRUE;
		}
		SetTimer(hWnd,TIME_OVERLAYSHOW,3000,NULL);
		return 0;
	}
#endif

	if((xpoint>0 && xpoint<198)&&(ypoint>60 && ypoint<166)&&!PICSEL&&MENUSHOW) //ѡ��ͼƬ�˵�
	{
		Media->DshowUninit();
		PICSEL = TRUE;
		EBOOKSEL = FALSE;
		VIDSEL = FALSE;
		AUDSEL = FALSE;
		BROWPIC = FALSE;
		//DoPaintSelPic(hWnd,PICSEL);    
		
		AUDIOPLAYING = FALSE;             //ֹͣ��Ƶ����
		SURFACEINIT = FALSE;
		
		PictureInit(PictureTcount);  //PictureTcount ��������Ч��
		g_scene->PicInit();
		g_scene->PicMultiScan(Picdwpos,1); 

		return 0;
	}

	if((xpoint>0 && xpoint<198)&&(ypoint>166 && ypoint<271)&&!VIDSEL&&MENUSHOW)//ѡ����Ƶ�˵�
	{
//#if ENABLEOVERLAY
//		if(VideoOverlay.CreateSurface(800,87) == 0)   //����overlay
//			MessageBox(hWnd,L"create overlay error",L"",NULL);
//#endif
		// Media->DshowStop();
		Media->DshowUninit();
		PICSEL = FALSE;
		EBOOKSEL = FALSE;
		VIDSEL = TRUE;
		AUDSEL = FALSE;
		VIDEOFACE = FALSE;
		AUDIOPLAYING = FALSE;          //ֹͣ��Ƶ����
		AUDIOPLINGFACE = FALSE;
		DoPaintSelVid(hWnd,VIDSEL);
		MoveVideoList(hWnd,Viddwpos%70);        //
        
		return 0;
	}

	if((xpoint>0 && xpoint<198)&&(ypoint>271 && ypoint<376)&&!AUDSEL&&MENUSHOW) //ѡ����Ƶ�˵�
	{
		
		
		PICSEL = FALSE;
		EBOOKSEL = FALSE;
		VIDSEL = FALSE;   
		AUDSEL = TRUE;        //��Ƶ�˵�ѡ��
		VIDEOFACE = FALSE;
		VIDEOPLAYING = FALSE;
		AUDIOPLAYING = FALSE;

		DoPaintSelAud(hWnd,AUDSEL);
		Auddwpos = 0;

		MoveAudioList(hWnd,Auddwpos%AGELISTLEN);
	  /*
	   ����MP3ר��ͼƬ �������
	  */       
		if(g_scene->MP3TexId == NULL)    //�Ƿ���Ҫ��ʼ��            
		{
			g_scene->MP3TexId = new GLuint[AudioTCount];
			TGAImage img;
			int width = 0;
			int height = 0;
			int Depth = 0;
			
			for (int i=0;i<AudioTCount;i++)                             //����Ĭ������
			{
				BYTE *buffer = NULL;

				if(Mp3Texture->MP3Load(AudioFile[i].path,&buffer,&width,&height,&Depth))
				{
					g_scene->MP3TexId[i] = g_scene->LoadBMP(&img,buffer,width,height,FALSE);
				}
				else
				{
					g_scene->MP3TexId[i]=Mp3Moren;
				}
			}
		}
		return 0;
	}
#if ENABLEEBOOK
	if((xpoint>0 && xpoint<198)&&(ypoint>376 && ypoint<480)&&!EBOOKSEL&&MENUSHOW) //ѡ�е�����˵�
	{
		PICSEL = FALSE;
		VIDSEL = FALSE;   
		AUDSEL = FALSE;        //��Ƶ�˵�ѡ��
		VIDEOFACE = FALSE;
		VIDEOPLAYING = FALSE;
		EBOOKSEL = TRUE;
		DoPaintSelEbk(hWnd,TRUE);
		MoveEbookList(hWnd,Ebookdwpos%70);
		return 0;
	}
	if (EBOOKSEL&&(xpoint>198 && xpoint<800)&&(ypoint>60 && ypoint<480)) //����������б�����
	{
		MOVEEBOOKLIST = TRUE;
		Ebookpos = ypoint;
		ChickPos = ypoint;

		return 0;
	}
#endif
	///////////////////////////
	if (!AUDIOPLINGFACE&&AUDSEL&&(xpoint>198 && xpoint<800)&&(ypoint>75 && ypoint<480)) //�����Ƶ�˵����� �б�����
	{
		MOVEAUDIOLISTFLAG = TRUE;
		Audpos = ypoint;
		ChickPos = ypoint;
		Movet1 = GetTickCount();
		ListMove = ypoint;
		return 0;
	}
	if (!VIDEOFACE&&VIDSEL&&(xpoint>198 && xpoint<800)&&(ypoint>64 && ypoint<480)) //�����Ƶ�˵����� �б�����
	{
		ChickPos = ypoint;
		Vidpos = ypoint;
		MOVEVIDEOLIST = TRUE;
		return 0;
	}
	if ((AUDIOPLINGFACE||VIDEOFACE)&&(xpoint>233 && xpoint<292)&&(ypoint>410 && ypoint<480))   //��һ��
	{
		HBITMAP hbmp1 = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\up.bmp");
		DoPaintWindows(hWnd,233,398,59,82,hbmp1,0,0);
		DeleteObject(hbmp1);
		if(AUDIOPLINGFACE)
		{
			do 
			{
				AudioPlayNum--;
				if(AudioPlayNum<0)
					AudioPlayNum = 0;
			} while (!Media->DshowCreate(hWnd,AudioFile[AudioPlayNum].path,hWnd));
		     Media->DshowPlay();
		}
		if(VIDEOFACE)
		{
			do 
			{
				VideoPlayNum--;
				if(VideoPlayNum<0)
					VideoPlayNum = 0;
			} while (!Media->DshowCreate(VidWnd,VideoFile[VideoPlayNum].path,hWnd)); //�����ʽ��֧��  �Զ���ת��һ��            
		  Media->DshowPlay();
		  PaintVideoName(hWnd);
		}
		HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\up1.bmp");
		DoPaintWindows(hWnd,233,398,59,82,hbmp,0,0);
        DeleteObject(hbmp);
		AudioLength = Media->DshowGetDuration();
		KillTimer(hWnd,TIME_AUDISLID);
		SetTimer(hWnd,TIME_AUDISLID,1000,NULL);   //���Ʋ��Ž�����

		if(AUDIOPLINGFACE)
			AUDIOPLAYING = TRUE; //������Ƶ
		if(VIDEOFACE)
		{
			DoPaintWindows(hWnd,680,367,80,17,hbmpBkGround,680,367);
			DoDrawTotalTime(hWnd);
			VIDEOPLAYING=TRUE;
		}
		if(AUDIOPLINGFACE)
		{
			g_scene->AudioInit( );
			g_scene->AudioPrev(AudioPlayNum);
			Sleep(100);
			DoPaintAudioPliFace(hWnd);
		}
		return 0;
	}
	if ((AUDIOPLINGFACE||VIDEOFACE)&&(xpoint>360 && xpoint<441)&&(ypoint>409 && ypoint<480)) //������ͣ��
	{
		if(AUDIOPLAYING||VIDEOPLAYING)
		{
			HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\play1.bmp");
			DoPaintWindows(hWnd,360,398,81,82,hbmp,0,0);
			DeleteObject(hbmp);
			Media->DshowPause();
			if(VIDEOFACE)
				VIDEOPLAYING = FALSE;
			else
				AUDIOPLAYING = FALSE;
		}    
		else
		{
			HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\push.bmp");
			DoPaintWindows(hWnd,360,398,81,82,hbmp,0,0);
			DeleteObject(hbmp);
			Media->DshowPlay();
			if(VIDEOFACE)
				VIDEOPLAYING = TRUE;
			else
				AUDIOPLAYING = TRUE;
		}
		return 0;
	}
	if ((AUDIOPLINGFACE||VIDEOFACE)&&(xpoint>508 && xpoint<567)&&(ypoint>410 && ypoint<480))  //��Ƶ��һ�װ�ť
	{
		HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\down.bmp");
		DoPaintWindows(hWnd,508,398,60,82,hbmp,0,0);
		DeleteObject(hbmp);
		if(AUDIOPLINGFACE)
		{
			do 
			{
				AudioPlayNum++;
				if(AudioPlayNum>AudioTCount-1)
					AudioPlayNum = AudioTCount-1;
			} while (!Media->DshowCreate(hWnd,AudioFile[AudioPlayNum].path,hWnd));
		   Media->DshowPlay();
		}
		if(VIDEOFACE)
		{
			do 
			{
				VideoPlayNum++;
				if(VideoPlayNum>VideoTCount-1)
					VideoPlayNum = VideoTCount-1;
			} while (!Media->DshowCreate(VidWnd,VideoFile[VideoPlayNum].path,hWnd));
		   Media->DshowPlay();
		  PaintVideoName(hWnd);
		}
		
		AudioLength = Media->DshowGetDuration();
		KillTimer(hWnd,TIME_AUDISLID);
		SetTimer(hWnd,TIME_AUDISLID,1000,NULL); //����ý�������
		if(AUDIOPLINGFACE)
			AUDIOPLAYING = TRUE; //������Ƶ
		if(VIDEOFACE)
		{
			VIDEOPLAYING = TRUE;
			DoPaintWindows(hWnd,680,367,80,17,hbmpBkGround,680,367);
			DoDrawTotalTime(hWnd);
		}
        HBITMAP hbmp1 = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\down1.bmp");
		DoPaintWindows(hWnd,508,398,60,82,hbmp1,0,0);
        DeleteObject(hbmp1);
		if(AUDIOPLINGFACE)
		{
			g_scene->AudioInit( );
			g_scene->AudioNext(AudioPlayNum);
			Sleep(100);
			DoPaintAudioPliFace(hWnd);
		}  
		return 0;
	}
	if ((AUDIOPLINGFACE||VIDEOFACE)&&(xpoint>0 && xpoint<800)&&(ypoint>380 && ypoint<405))  //���������
	{
		Media->DshowSetPositions((LONGLONG)xpoint*AudioLength/800);
		return 0;
	}
	if (AUDIOPLINGFACE&&(xpoint>56&& xpoint<155)&&(ypoint>418 && ypoint<480))               //���ѭ������ 
	{
		if(!CycMode)
		{
			HBITMAP hbmp2= (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\loop.bmp");
			DoPaintWindows(hWnd,56,398,99,82,hbmp2,0,0);
            DeleteObject(hbmp2);
			HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\rand1.bmp");
			DoPaintWindows(hWnd,647,398,98,82,hbmp,0,0);
			DeleteObject(hbmp);
		}
		RandMode = FALSE;
		CycMode = TRUE;
		return 0;
	}
	if (AUDIOPLINGFACE&&(xpoint>647&& xpoint<746)&&(ypoint>418 && ypoint<480))            //���������� 
	{
		if(!RandMode)
		{
			HBITMAP hbmp2 = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\rand.bmp");
			DoPaintWindows(hWnd,647,398,98,82,hbmp2,0,0);
			DeleteObject(hbmp2);
			HBITMAP hbmp  = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\loop1.bmp");
			DoPaintWindows(hWnd,56,398,99,82,hbmp,0,0);
			DeleteObject(hbmp);
		}
		RandMode = TRUE;
		CycMode = FALSE;
		return 0;
	}
	if(PICSEL&&(xpoint>198&& xpoint<800)&&(ypoint>60 && ypoint<480)&&!BROWPIC)  //�����������ͼƬ����
	{
		g_scene->PicInit();
		g_scene->PicMultiScan(Picdwpos,0);
		PICSEL = FALSE;
		BROWPIC = TRUE;         //ͼƬ�����ʾ
		MENUSHOW = FALSE;
		return 0;
	}
	if(BROWPIC&&!FULLPIC&&(xpoint>698 && xpoint<800)&&(ypoint>0&& ypoint<60))    //����ͼƬ�˵�����
	{
		SINGLESCAN = FALSE;
		MULTISCAN  =  FALSE;
		FULLPIC = FALSE;
		HBITMAP hbmpBk2 = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\backButton2.bmp");
	    DoPaintWindows(hWnd,698,0,102,60,hbmpBk2,0,0);
        DeleteObject(hbmpBk2);
		return 0;
	}
	if(SCANEBOOK&&(xpoint>698 && xpoint<800)&&(ypoint>0&&ypoint<60))  //�����鷵�ز���
	{
		HBITMAP hbmpBk2 = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\backButton2.bmp");
		DoPaintWindows(hWnd,698,0,102,60,hbmpBk2,0,0);
		DeleteObject(hbmpBk2);
		return 0;
	}
	 /*ͼƬ������н����Buttondown*/
     if(BROWPIC&&!MENUSHOW)
	 {
		 ChickPos = xpoint;
		 Picpos = xpoint;
		 MULTISCAN = TRUE;
		 return 0;
	 }
	 if(SCANEBOOK)
	 {
		 POINT DownPoint;
		 DownPoint.x = xpoint;
		 DownPoint.y = ypoint;
		 txtBook->ButtonDown(hWnd,DownPoint);
		 return 0;
	 }
     if (AUDIOPLINGFACE&&(xpoint>0 && xpoint<800)&&(ypoint>60 && ypoint<380))
     {
		 ChickPos = xpoint;
     }
	return 0;

}
int  CUsbSd::DoMouseMove(HWND hWnd,INT xpoint,INT ypoint )
{
	/////////////////////////////����ͼƬ�б�/////////////////////////////
	/*������ͼ����ͼ*/
    if(MULTISCAN&&!FULLPIC&&(abs(xpoint-ChickPos)>=MOUSE_MOVE+15)&&SINGLESCAN) //��ͼ�������
	{
		Picpos = xpoint - Picpos;
		Picdwpos = Picdwpos - Picpos;
		int Four = (PictureTcount%4 == 0)?0:1;
		if (Picdwpos<0)
			Picdwpos=0;
		else if (Picdwpos>=(PictureTcount/4-5+Four)*160)
		{
			Picdwpos=(PictureTcount/4-5+Four)*160;
		}
        g_scene->PicInit();
        g_scene->PicSingleScan(PicNum,Picdwpos);
		Picpos = xpoint;
		return 0;
	}
	/*������ͼ����ͼ*/
	if(BROWPIC&&MULTISCAN&&!FULLPIC&&(abs(xpoint-ChickPos)>=MOUSE_MOVE)&&!SINGLESCAN)
	{
		Picpos = xpoint - Picpos;
		Picdwpos = Picdwpos - Picpos;
        int Four = (PictureTcount%4 == 0)?0:1;
		if (Picdwpos<0)
			Picdwpos=0;
		else if (Picdwpos>=(PictureTcount/4-5+Four)*160)
		{
			Picdwpos=(PictureTcount/4-5+Four)*160;
		}
		g_scene->PicInit( );
		g_scene->PicMultiScan(Picdwpos,0);
		Picpos = xpoint;

		return 0;
	}
	if((abs(ypoint-ChickPos)>=MOUSE_MOVE)&&MOVEEBOOKLIST) //�����������б�
	{

		Ebookpos = ypoint - Ebookpos;
		Ebookdwpos = Ebookdwpos - Ebookpos;
		if(Ebookdwpos <= 0)
			Ebookdwpos=0;
		if(Ebookdwpos >= (EbookTCount-6)*AGELISTLEN)   //70
			Ebookdwpos = (EbookTCount-6)*AGELISTLEN;
		if(EbookTCount<=6)
			Ebookdwpos = 0;
		
		MoveEbookList(hWnd,Ebookdwpos%AGELISTLEN);
		Ebookpos = ypoint;

		return 0;
	}
              /*������Ƶ�б�*/
	if((abs(ypoint-ChickPos)>=MOUSE_MOVE)&&MOVEAUDIOLISTFLAG&&!AUDIOPLINGFACE&&AUDSEL) //�ƶ���Ƶ�б�
	{
		//MessageBox(hWnd,L"",L"",NULL);
		Audpos = ypoint - Audpos;
		Auddwpos = Auddwpos - Audpos;
		if(Auddwpos <= 0)
			Auddwpos=0;
		if(Auddwpos >= (AudioTCount-6)*AGELISTLEN)   //70
			Auddwpos = (AudioTCount-6)*AGELISTLEN;
		if(AudioTCount<=6)
			Auddwpos = 0;
		//DoPaintAudiList(hWnd,Auddwpos);
		MoveAudioList(hWnd,Auddwpos%AGELISTLEN);
		Audpos = ypoint;

		return 0;
	}
	/////////////////////////////////������Ƶ�б�/////////////////////
	if((abs(ypoint-ChickPos)>=MOUSE_MOVE)&&MOVEVIDEOLIST&&!VIDEOFACE&&VIDSEL) 
	{
		Vidpos = ypoint - Vidpos;
		Viddwpos = Viddwpos - Vidpos;
		if(Viddwpos <= 0)
			Viddwpos=0;
		if(Viddwpos >= (VideoTCount-6)*AGELISTLEN)   //70
			Viddwpos = (VideoTCount-6)*AGELISTLEN;
		if(VideoTCount<=6)
			Viddwpos = 0;
		//DoPaintVideoList(hWnd,Viddwpos);
		MoveVideoList(hWnd,Viddwpos%70);
		Vidpos = ypoint;

		return 0;
	}
	return 0;
}
int  CUsbSd::DoLbuttonUp(HWND hWnd,INT xpoint,INT ypoint)
{   
	 if(AUDIOPLINGFACE&&((xpoint-ChickPos)>100)&&(xpoint>0 && xpoint<800)&&(ypoint>60 && ypoint<380))  //������һ��
	 {
	  do 
		{
			AudioPlayNum--;
			if(AudioPlayNum<0)
				AudioPlayNum = 0;
		} while (!Media->DshowCreate(hWnd,AudioFile[AudioPlayNum].path,hWnd));
		Media->DshowPlay();
		AudioLength = Media->DshowGetDuration();
		KillTimer(hWnd,TIME_AUDISLID);
		SetTimer(hWnd,TIME_AUDISLID,1000,NULL); //����ý�������
		AUDIOPLAYING = TRUE; //������Ƶ
		g_scene->AudioInit( );
		g_scene->AudioPrev(AudioPlayNum);
		Sleep(100);
		DoPaintAudioPliFace(hWnd);
		 return 0;
	 }
	 if(AUDIOPLINGFACE&&((xpoint-ChickPos)<-100)&&(xpoint>0 && xpoint<800)&&(ypoint>60 && ypoint<380))  //������һ��
	 {
		   do 
			{
				AudioPlayNum++;
				if(AudioPlayNum>AudioTCount-1)
					AudioPlayNum = AudioTCount-1;
			} while (!Media->DshowCreate(hWnd,AudioFile[AudioPlayNum].path,hWnd));
			Media->DshowPlay();
			AudioLength = Media->DshowGetDuration();
			KillTimer(hWnd,TIME_AUDISLID);
			SetTimer(hWnd,TIME_AUDISLID,1000,NULL); //����ý�������
		    AUDIOPLAYING = TRUE; //������Ƶ
			g_scene->AudioInit( );
			g_scene->AudioNext(AudioPlayNum);
			Sleep(100);
			DoPaintAudioPliFace(hWnd);
		 return 0;
	 }
	 if (MENUSHOW&&(xpoint>698 && xpoint<800)&&(ypoint>0 && ypoint<60))  //�˳�����ť
	 {
         Media->DshowStop();
		 Media->DshowUninit();
         KillTimer(hWnd,TIME_MEMORYLEAK);
		 HBITMAP hbmpBk2 = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\backButton2.bmp");
		 DoPaintWindows(hWnd,698,0,102,60,hbmpBk2,0,1);
		 DeleteObject(hbmpBk2);
		 //SendMessage(hWnd,WM_DESTROY,0,0);
		 if(shellOS->USBFROMMAIN==FALSE)
		 {
			 g_scene->DocManageInit();
			 g_scene->DocManageStat(24);
			 shellOS->Cur_Mode = _TOOL;
		 }
		 else
		 {
			 g_scene->MenuInit( );		
			 g_scene->MenuStat(shellOS->Mode_Front,shellOS->Cur_Bg);
			 shellOS->Cur_Mode=_MAIN;
			 shellOS->USBFROMMAIN=FALSE;
			 ShowWindow(shellOS->hChild,SW_SHOW);
		 }
		 AUDIOPLAYING = FALSE;  //ֹͣ��Ƶ����
		 return 0;
	 }

#if 1
	
	/*ȫ�������һ�š���һ��ͼƬ*/
	if(FULLPIC&&(abs(xpoint-ChickPos)>=(MOUSE_MOVE+50)))
	{
		  if((xpoint-ChickPos)>0)
		  {
			PicCurrentNum--;
			if(PicCurrentNum<0)
				PicCurrentNum = 0;
			else
			{
			  DoFullScanPic(hWnd);
              PicNum--;
			}
			if(PicNum<0)
				PicNum =0;
		  }
		  else
		  {
			 PicCurrentNum++;
			if(PicCurrentNum>PictureTcount-1)
              PicCurrentNum = PictureTcount-1;
			else
			{
			  DoFullScanPic(hWnd);
			  PicNum++;
			}
			
		  }
		 PicNum = PicNum%20;
		 Picdwpos = (PicCurrentNum-PicNum)/4*160;
		 int Four = (PictureTcount%4 == 0)?0:1;
		 if (Picdwpos<0)
			 Picdwpos=0;
		 else if (Picdwpos>=(PictureTcount/4-5+Four)*160)
		 {
			 Picdwpos=(PictureTcount/4-5+Four)*160;
			 PicNum = PicCurrentNum - Picdwpos/160*4;
		 }
		 return 0;
	}
	/*��ͼ������ض�ͼ���*/
	if(SINGLESCAN&&!FULLPIC&&(xpoint>640 && xpoint<800)&&(ypoint>76 && ypoint<380)&&abs(xpoint-ChickPos)<=SINGLE_CLICK)
	{
		SINGLESCAN = FALSE;
		MULTISCAN = TRUE;
		g_scene->PicInit();
		g_scene->PicMultiUp(PicNum,Picdwpos);
		return 0;
	}
	if(SINGLESCAN&&!FULLPIC&&(xpoint>0 && xpoint<160)&&(ypoint>76 && ypoint<380)&&abs(xpoint-ChickPos)<=SINGLE_CLICK)
	{
		SINGLESCAN = FALSE;
		MULTISCAN = TRUE;
		g_scene->PicInit();
		g_scene->PicMultiUp(PicNum,Picdwpos);
		return 0;
	}
    /*��ͼ���*/
	if(SINGLESCAN&&!FULLPIC) //�ײ���ѡ��
	{
		if(abs(xpoint-ChickPos)<=SINGLE_CLICK)
		{
		if((xpoint>0 && xpoint<160)&&(ypoint>380 && ypoint<480))
			PicNum=PicNum+4*(0-PicNum/4);
		else if((xpoint>160 && xpoint<320)&&(ypoint>380 && ypoint<480))
			PicNum=PicNum+4*(1-PicNum/4);
		else if((xpoint>320 && xpoint<480)&&(ypoint>380 && ypoint<480))
			PicNum=PicNum+4*(2-PicNum/4);
		else if((xpoint>480 && xpoint<640)&&(ypoint>380 && ypoint<480))
			PicNum=PicNum+4*(3-PicNum/4);
		else if((xpoint>640 && xpoint<800)&&(ypoint>380 && ypoint<480))
			PicNum=PicNum+4*(4-PicNum/4);
		}
		if((Picdwpos%160)>80)
			Picdwpos=Picdwpos+160-Picdwpos%160;
		else if((Picdwpos%160) <= 80)
			Picdwpos= Picdwpos-Picdwpos%160;
		g_scene->PicInit( );
		PicCurrentNum = g_scene->PicSingleScan(PicNum,Picdwpos);
		return 0;
	}
	/*��ͼ����ͼ���*/
    if(MULTISCAN&&!FULLPIC&&!SINGLESCAN)
	{
      MULTISCAN = FALSE;
	  if((Picdwpos%160)>80)
		  Picdwpos=Picdwpos+160-Picdwpos%160;
	  else if((Picdwpos%160) <= 80)
		  Picdwpos= Picdwpos-Picdwpos%160;
	 
	  if((abs(xpoint-ChickPos)<=SINGLE_CLICK))  
	  {   
		  int num = 0;
		  num = (ypoint - 76)/100 +(xpoint/160)*4;
		  printf("%d\n",num);
		  PicNum = num;
		  PicCurrentNum =PicNum + Picdwpos/160*4;
		  if(PicCurrentNum<=PictureTcount-1)
		  {
		   g_scene->PicInit();
		   PicCurrentNum = g_scene->PicMultiDown(num,Picdwpos);
		   Sleep(100);
		   SINGLESCAN = TRUE;
		   return 0;
		  }
	  }
	  g_scene->PicInit( );
	  g_scene->PicMultiScan(Picdwpos,0);
      return 0;
	}
	/* ��Ƶ���淵�ؼ�*/
	if(VIDEOFACE&&(xpoint>698 && xpoint<800)&&(ypoint>0 && ypoint<60)&&!VIDSEL)   
	{
		ShowWindow(VidWnd,SW_HIDE);
		HBITMAP hbmpBk2 = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\backButton2.bmp");
		DoPaintWindows(hWnd,698,0,102,60,hbmpBk2,0,0);
		DeleteObject(hbmpBk2);
		Media->DshowUninit();
		KillTimer(hWnd,TIME_AUDISLID);  //�ر���Ƶ��������ʱ��
		KillTimer(hWnd,TIME_VIDEOFULL); //�ر���Ƶ overlay ��ʱ��
		VIDEOFACE = FALSE;
		MENUSHOW = TRUE;
		VIDSEL = TRUE;
		VIDEOPLAYING = FALSE;
		DoPaintSelVid(hWnd,VIDSEL);
		MoveVideoList(hWnd,Viddwpos%70);
		return 0;
	}
	/*ͼƬ���淵�ؼ�*/
	if(BROWPIC&&(xpoint>698 && xpoint<800)&&(ypoint>0 && ypoint<60))
	{   
		BROWPIC = FALSE;
		MENUSHOW = TRUE;
		PICSEL = TRUE;
		DoPaintSelPic(hWnd,PICSEL);
		g_scene->PicInit( );
		g_scene->PicMultiScan(Picdwpos,1);
		return 0;
	}
	//�����鷵�ز���
    if(SCANEBOOK&&(xpoint>698 && xpoint<800)&&(ypoint>0&&ypoint<60))  
	{
       MENUSHOW  = TRUE;
	   SCANEBOOK = FALSE;
	   EBOOKSEL  = TRUE;
	   DoPaintSelEbk(hWnd,TRUE);
	   MoveEbookList(hWnd,Ebookdwpos%70);
       delete txtBook;
	   txtBook = NULL;
       return 0;
	}
	/*��Ƶ���淵�ؼ�*/
	if (!AUDSEL&&AUDIOPLINGFACE&&(xpoint>698 && xpoint<800)&&(ypoint>0 && ypoint<60))   //������Ƶ�˵���������
	{
		//Media->DshowStop();
		//Media->DshowUninit();
		KillTimer(hWnd,TIME_AUDISLID);  //��������ƽ�����
		HBITMAP hbmpBk2 = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\backButton2.bmp");
		DoPaintWindows(hWnd,698,0,102,60,hbmpBk2,0,0);
		DeleteObject(hbmpBk2);
        AUDSEL = TRUE;
		MENUSHOW = TRUE; 
		AUDIOPLINGFACE = FALSE;      //��Ƶ���Ž���ʧЧ
		DoPaintSelAud(hWnd,AUDSEL);
		MoveAudioList(hWnd,Auddwpos%AGELISTLEN); //����˵�����            
		return 0;
	}
#endif
#if ENABLEEBOOK            
	if(SCANEBOOK)                     //������
	{
		EnterCriticalSection(&(txt_critical_section));

		POINT UpPoint;
		UpPoint.x = xpoint;
		UpPoint.y = ypoint;

		txtBook->ButtonUp(hWnd,UpPoint);
		LeaveCriticalSection(&(txt_critical_section));
	}
	if(MOVEEBOOKLIST&&EBOOKSEL)            //ѡ�е������б�                            
	{
		if((Ebookdwpos%70) > 35)
		{
			Ebookdwpos = Ebookdwpos +70-(Ebookdwpos%70);   //70 one song height
		}
		else
			Ebookdwpos = Ebookdwpos - (Ebookdwpos%70);
		if(abs(ypoint-ChickPos) <=SINGLE_CLICK)                //ѡ��
		{
			if(((ypoint-60)/70+ Ebookdwpos/70) > (EbookTCount- 1))
				return 0;
			EbookSelnum = (ypoint-60)/70+ Ebookdwpos/70;                //60 �Ǻ����ĸ߶�
			if(EbookSelnum > (EbookTCount- 1))
			{
				
				return 0;                  
			}
			else
			{
				EBOOKLISTSEL = TRUE;
			}
			
			if(EBOOKLISTSEL)   //ѡ�е�����
			{
				EBOOKSEL = FALSE;        //���õ�����˵�������Ч
				MENUSHOW = FALSE;        //��ֹ�˵�����
				SCANEBOOK = TRUE;

				txtBook=new ElecBook(hWnd,EbookFile[EbookSelnum].path);
				hThread= CreateThread(NULL, 0,TxtThread, NULL, 0, NULL);	
				EnterCriticalSection(&(txt_critical_section));
				txtBook->PaintMain(hWnd,NULL,NULL,NULL);
				LeaveCriticalSection(&(txt_critical_section));
			}
			else
			{
				EBOOKLISTSEL = FALSE;
			}
	    }
		if(EBOOKSEL)
		{
			MoveEbookList(hWnd,Ebookdwpos%70);
		}
		MOVEEBOOKLIST = FALSE;

		return 0;
	}
#endif
	/////////////////////////////////////////ѡ����Ƶ�б�/////////////////////////////////////
	if(MOVEAUDIOLISTFLAG&&AUDSEL)  //��Ƶ�˵��б�������ʱ
	{
		if((Auddwpos%AGELISTLEN) > 35)
		{
			Auddwpos = Auddwpos +AGELISTLEN-(Auddwpos%AGELISTLEN);   //70 one song height
		}
		else
			Auddwpos = Auddwpos - (Auddwpos%AGELISTLEN);
		if(abs(ypoint-ChickPos) <=SINGLE_CLICK)                //ѡ����Ƶ��Ŀ����
		{
			if(((ypoint-60)/AGELISTLEN+ Auddwpos/AGELISTLEN) > (AudioTCount- 1))
			{
				   MOVEAUDIOLISTFLAG = FALSE;
				        return 0;
			}
			AudioPlayNum = (ypoint-60)/AGELISTLEN+ Auddwpos/AGELISTLEN;                    //60�Ǻ����ĸ߶�
			if(AudioPlayNum > (AudioTCount- 1))
			{
				MOVEAUDIOLISTFLAG = FALSE;
				return 0;                  
			}
			else
			{
				AUDIOLISTSEL = TRUE;
			}
			//������Ƶ
			if(AUDIOLISTSEL)        //ѡ�в�����Ƶ
			{
				MENUSHOW = FALSE;      //��ֹ�˵�����
				
				AUDIOPLAYING = TRUE;   //������Ƶ
				MoveAudioList(hWnd,Auddwpos%AGELISTLEN);
                AUDSEL = FALSE;        //������Ƶ�˵�������Ч
				AUDIOPLINGFACE = TRUE; //������Ƶ����

				while(!Media->DshowCreate(hWnd,AudioFile[AudioPlayNum].path,hWnd))
				{
					AudioPlayNum++;
					if(AudioPlayNum>AudioTCount-1)
						AudioPlayNum = AudioTCount-1;
				}
				Media->DshowPlay();
				AudioLength = Media->DshowGetDuration();
				KillTimer(hWnd,TIME_AUDISLID);
				SetTimer(hWnd,TIME_AUDISLID,1000,NULL); //������Ƶ������
				g_scene->AudioInit( );
				g_scene->AudioStat(AudioPlayNum);
				Sleep(100);
				DoPaintAudioPliFace(hWnd); //������Ƶ���Ž���
			}
			else
			{
				AUDIOPLAYING = FALSE;
			}
			///////////////////////////////////////////
		 }
		if(AUDSEL)
		{
   //         Movet2 = GetTickCount();
			//if((Movet2-Movet1)<300)
			//{
			//	ListMove = ypoint - ListMove;
			//	SetTimer(hWnd,TIME_STOPMOVE,(Movet2-Movet1)*5*abs(ListMove)/120,NULL);
			//	SetTimer(hWnd,TIME_MOVELIST,50,NULL);
			//}
			//else
			//{
			//	if((Auddwpos%AGELISTLEN) > 35)
			//	{
			//		Auddwpos = Auddwpos +AGELISTLEN-(Auddwpos%AGELISTLEN);   //70 one song height
			//	}
			//	else
			//		Auddwpos = Auddwpos - (Auddwpos%AGELISTLEN);
				MoveAudioList(hWnd,Auddwpos%AGELISTLEN);
			//}
		}
		MOVEAUDIOLISTFLAG = FALSE;
		return 0;
	}
	//////////////////////////////////////////////��Ƶ�б��Ų���///////////////////////////////////////////
	if(MOVEVIDEOLIST&&VIDSEL)
	{
		if((Viddwpos%AGELISTLEN) > 35)
		{
			Viddwpos = Viddwpos +AGELISTLEN-(Viddwpos%AGELISTLEN);   //70 one song height
		}
		else
			Viddwpos = Viddwpos - (Viddwpos%AGELISTLEN);
		if(abs(ypoint-ChickPos) <=SINGLE_CLICK)              //����б���
		{
			if(((ypoint-60)/AGELISTLEN+ Viddwpos/AGELISTLEN) > (VideoTCount- 1))
			{
				MOVEVIDEOLIST = FALSE;
				return 0;
			}
			VideoPlayNum = (ypoint-60)/AGELISTLEN+ Viddwpos/AGELISTLEN;                    //60�Ǻ����ĸ߶�
			if(VideoPlayNum > (VideoTCount- 1))
			{
				MOVEVIDEOLIST = FALSE;
				return 0;                  
			}
			else
			{
				VIDEOLISTSEL = TRUE;
			}
			if(VIDEOLISTSEL)
			{
				VIDEOFACE = TRUE;  //��Ƶ���沥��
				MENUSHOW = FALSE;
				VIDSEL = FALSE;
				DoPaintVideoFace(hWnd);

				//������Ƶ
				ShowWindow(VidWnd,SW_SHOW);
				Media->DshowCreate(VidWnd,VideoFile[VideoPlayNum].path,hWnd);
                PaintVideoName(hWnd);
				Media->DshowPlay();
				AudioLength = Media->DshowGetDuration();

				KillTimer(hWnd,TIME_AUDISLID);
				SetTimer(hWnd,TIME_AUDISLID,1000,NULL); 
				DoDrawTotalTime(hWnd);

				VIDEOPLAYING = TRUE;
				//Overlay ��������
#if ENABLEOVERLAY
				BOOL re = VideoOverlay.DrawBktoOverlay(L"\\Storage Card\\USBDATA\\VideoFull.bmp",0,0,800,87,0,0);   //87 
                     if(!re)
                       MessageBox(NULL,L"draw bk overlay error",L"",NULL);
#endif			
			}
		}
		if(VIDSEL)
		{
			MoveVideoList(hWnd,Viddwpos%70);
		}
		MOVEVIDEOLIST = FALSE;
		return 0;
	}
	
	return 0;
}
int  CUsbSd::DoTime(HWND hWnd,INT wParam)
{
	if(wParam == TIME_DBCLICK)
	{
		Dbclick = 0;
		KillTimer(hWnd,TIME_DBCLICK);
	}
	if(wParam == TIME_MOVELIST)
	{
		if(Auddwpos <= 0)
			Auddwpos=0;
		if(Auddwpos >= (AudioTCount-6)*AGELISTLEN)   //70
			Auddwpos = (AudioTCount-6)*AGELISTLEN;
		if(AudioTCount<=6)
			Auddwpos = 0;
		//DoPaintAudiList(hWnd,Auddwpos);
		MoveAudioList(hWnd,Auddwpos%AGELISTLEN);
		Auddwpos = Auddwpos - 2*ListMove/10;

	}
	if(wParam == TIME_STOPMOVE)
	{
		KillTimer(hWnd,TIME_MOVELIST);
		KillTimer(hWnd,TIME_STOPMOVE);
		ListMove = 0;       //��λ��������
	}
	if(wParam == TIME_PICSCANREFRESH&&(PICSEL||BROWPIC)&&!SINGLESCAN)       //����ͼƬʱBROWPIC �ж��Ƿ���Ҫˢ��
	{
		g_scene->PicInit();
		g_scene->PicMultiScan(Picdwpos,PICSEL); 
		
		KillTimer(hWnd,TIME_PICSCANREFRESH);
	}
	if(wParam == TIME_TEMP)   //
	{
		LONGLONG nowPos = Media->DshowGetCurrentPos();
		DoPaintAudioSlider(hWnd,(nowPos*800)/AudioLength);
		DoDrawCurrentTime(hWnd);
		SetTimer(hWnd,TIME_AUDISLID,1000,NULL);
		KillTimer(hWnd,TIME_TEMP);

		OutputDebugString(L"time5");
		OutputDebugString(L"\n");
	}
	if((wParam == TIME_AUDISLID)&&(AUDIOPLINGFACE||VIDEOPLAYING)) //��Ƶ���������ƶ�ʱ�� ����Ƶ���Ž�����Ч
	{
		LONGLONG nowPos = Media->DshowGetCurrentPos();
		DoPaintAudioSlider(hWnd,(nowPos*800)/AudioLength);
		DoDrawCurrentTime(hWnd);
		OutputDebugString(L"time4");
		OutputDebugString(L"\n");
	}
	if((wParam == TIME_OVERLAYSHOW)&&OVERLAYSHOW)     //overlay ������ʧ
	{
		RECT rSrc = {0,0,800,87};
		RECT rDes = {0,393,800,480};
		VideoOverlay.UpdateOverlay(&rDes,&rSrc,DDOVER_SHOW,0x00);
		OVERLAYSHOW = FALSE;
		KillTimer(hWnd,TIME_OVERLAYSHOW);

		OutputDebugString(L"time3");
		OutputDebugString(L"\n");
	}
    if((wParam == TIME_VIDEOFULL)&&VIDEOPLAYING)   //��Ƶȫ��ʱ�Ľ�����
	{
		LONGLONG nowPos = Media->DshowGetCurrentPos();
		
		VideoOverlay.DrawLineToOverlay2(86,86,86,(nowPos*800)/AudioLength,0);
		VideoOverlay.DrawLineToOverlay2(50,50,50,(nowPos*800)/AudioLength,1);
		VideoOverlay.DrawLineToOverlay2(60,60,60,(nowPos*800)/AudioLength,2);

		VideoOverlay.DrawLineToOverlay(52,126,125,(nowPos*800)/AudioLength,0);
		VideoOverlay.DrawLineToOverlay(0,187,184,(nowPos*800)/AudioLength,1);
		VideoOverlay.DrawLineToOverlay(52,126,125,(nowPos*800)/AudioLength,2);
        
		{
			int Second = Media->DshowGetCurrentPos()/10000000;  //��  
			TCHAR str[50];
			_stprintf(str,L"%02d:%02d",Second/60,Second%60);     //��ǰ����ʱ��
			VideoOverlay.DrawTextToOverlay(str,30,5,100,45);
		}
		OutputDebugString(L"time2");
		OutputDebugString(L"\n");
	}
	if(wParam == TIME_MEMORYLEAK)     //�ڴ�ʹ����
	{
		MEMORYSTATUS Usbmemory;
		GlobalMemoryStatus(&Usbmemory);
        HDC hdc = GetDC(hWnd);
		TCHAR str[100];
		_stprintf(str,L"%d",Usbmemory.dwMemoryLoad);
		RECT rc = {600,0,700,60};
		
		DrawText(hdc,str,-1,&rc,DT_CENTER);

		ReleaseDC(hWnd,hdc);
		OutputDebugString(L"time1");
		OutputDebugString(L"\n");
	}
	
	return 0;
}
int CUsbSd::DoPaint(HWND hWnd)
{
	/////��ʼ���� hbmp1,hbmp5
	if(!PICSEL&&!AUDSEL&&!VIDSEL&&!EBOOKSEL&&MENUSHOW&&SURFACEINIT) //�˵����ֵĳ�ʼ����
	{  
		DoPaintWindows(hWnd,0,0,800,480,hbmpBkGround,0,0); //����
		DoPaintInit(hWnd);
	}
	if(PICSEL)
	{
		PICSEL = TRUE;
		EBOOKSEL = FALSE;
		VIDSEL = FALSE;
		AUDSEL = FALSE;
		BROWPIC = FALSE;

		AUDIOPLAYING = FALSE;             //ֹͣ��Ƶ����
		SURFACEINIT = FALSE;

		PictureInit(PictureTcount);  //PictureTcount ��������Ч��
		g_scene->PicInit();
		g_scene->PicMultiScan(Picdwpos,1); 
	    return 0;
	}
	/////////////////////////////////////////////////////
	//��Ƶ�˵��б����
	if(AUDSEL)
	{
		PICSEL = FALSE;
		EBOOKSEL = FALSE;
		VIDSEL = FALSE;   
		AUDSEL = TRUE;        //��Ƶ�˵�ѡ��
		VIDEOFACE = FALSE;
		VIDEOPLAYING = FALSE;
		AUDIOPLAYING = FALSE;

		DoPaintSelAud(hWnd,AUDSEL);
		Auddwpos = 0;

		MoveAudioList(hWnd,Auddwpos%AGELISTLEN);
	  /*
	   ����MP3ר��ͼƬ �������
	  */       
		if(g_scene->MP3TexId == NULL)    //�Ƿ���Ҫ��ʼ��            
		{
			g_scene->MP3TexId = new GLuint[AudioTCount];
			TGAImage img;
			int width = 0;
			int height = 0;
			int Depth = 0;
			for (int i=0;i<AudioTCount;i++)                             //����Ĭ������
			{
				BYTE *buffer = NULL;

				if(Mp3Texture->MP3Load(AudioFile[i].path,&buffer,&width,&height,&Depth))
				{
					g_scene->MP3TexId[i] = g_scene->LoadBMP(&img,buffer,width,height,FALSE);
				}
				else
				{
					g_scene->MP3TexId[i]=Mp3Moren;
				}
			}
		}
		return 0;
	}
	///////////////////////////////////////////////////
	//��Ƶ�˵�����
	if(VIDSEL)
	{
		PICSEL = FALSE;
		EBOOKSEL = FALSE;
		VIDSEL = TRUE;
		AUDSEL = FALSE;
		VIDEOFACE = FALSE;
		AUDIOPLAYING = FALSE;          //ֹͣ��Ƶ����
		AUDIOPLINGFACE = FALSE;
		DoPaintSelVid(hWnd,VIDSEL);
		MoveVideoList(hWnd,Viddwpos%70);        //
	   return 0;
	}
	if(VIDEOFACE)
	{
		DoPaintVideoFace(hWnd);
		DoDrawTotalTime(hWnd);  //���»���ʱ��
		PaintVideoName(hWnd);   //���»�����Ƶ�ļ�����
		return 0;
	}
}

void CUsbSd::DoAudioMode(HWND hWnd)
{
	if(AUDIOPLINGFACE) //��Ƶ���Ž�����Ϣ
	{
	long lEventCode;
	if(Media->DshowGetMediaEvent(&lEventCode))
	{
		if(lEventCode == EC_COMPLETE) //���Ž���
		{
		  if(AUDIOPLAYING)
		  {
			if(RandMode)  //�������
			{
				AudioPlayNum = rand()%(AudioTCount-1);
			}
			else //ѭ������
			{
				AudioPlayNum++;
				if(AudioPlayNum > AudioTCount-1)
					AudioPlayNum = 0;
			}
            while(!Media->DshowCreate(hWnd,AudioFile[AudioPlayNum].path,hWnd))
			{
				AudioPlayNum++;
				if(AudioPlayNum>AudioTCount-1)
					AudioPlayNum = AudioTCount-1;
			}
			Media->DshowPlay();
			AudioLength = Media->DshowGetDuration();
			KillTimer(hWnd,TIME_AUDISLID);
			SetTimer(hWnd,TIME_AUDISLID,1000,NULL); //������Ƶ������
			g_scene->AudioInit( );
			g_scene->AudioPrev(AudioPlayNum);

            Sleep(100);
			if(AUDIOPLINGFACE)
				DoPaintAudioPliFace(hWnd);  //����ʱ���ܲ��ڲ��Ž���
			if(AUDSEL)
			{
				MoveAudioList(hWnd,Auddwpos%AGELISTLEN);
			}
		}
		}
	  }
	}
}
void CUsbSd::DoDrawTotalTime(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	int Second = AudioLength/10000000;  //��
	TCHAR str[50];
	_stprintf(str,L"%02d:%02d",Second/60,Second%60);
	SetBkMode(hdc,1);
	SetTextColor(hdc,RGB(41,36,33));
	RECT rc = {680,367,760,384};
	HFONT FontList = CreateFont(30,0,0,0,FW_LIGHT,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH,_T("΢���ź�"));
	HFONT fontold = (HFONT)SelectObject(hdc,FontList);
	SetTextCharacterExtra(hdc,1);
	DrawText(hdc,str,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
	SelectObject(hdc,fontold);
	DeleteObject(FontList);

	ReleaseDC(hWnd,hdc);
}
void CUsbSd::DoDrawCurrentTime(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	DoPaintWindows(hWnd,19,367,80,17,hbmpBkGround,19,367);   //ʱ��ĵ�
	int Second = Media->DshowGetCurrentPos()/10000000;  //��
	TCHAR str[50];
	_stprintf(str,L"%02d:%02d",Second/60,Second%60);
	SetBkMode(hdc,1); 
	SetTextColor(hdc,RGB(41,36,33));                 //241
	RECT rc = {19,367,100,384};
	HFONT FontList = CreateFont(30,0,0,0,FW_LIGHT,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH,_T("΢���ź�"));
	HFONT fontold = (HFONT)SelectObject(hdc,FontList);
	SetTextCharacterExtra(hdc,1);
	DrawText(hdc,str,-1,&rc,DT_VCENTER|DT_LEFT);
	SelectObject(hdc,fontold);
	DeleteObject(FontList);
	ReleaseDC(hWnd,hdc);
}
void CUsbSd::DoPaintAudMode(HWND hWnd)
{
	if(CycMode)
	{
		HBITMAP hbmp2= (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\loop.bmp");
		DoPaintWindows(hWnd,56,398,99,82,hbmp2,0,0);
		DeleteObject(hbmp2);
		HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\rand1.bmp");
		DoPaintWindows(hWnd,647,398,98,82,hbmp,0,0);
		DeleteObject(hbmp);
	}
	else
	{
		HBITMAP hbmp2 = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\rand.bmp");
		DoPaintWindows(hWnd,647,398,98,82,hbmp2,0,0);
        DeleteObject(hbmp2);
		HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\loop1.bmp");
		DoPaintWindows(hWnd,56,398,99,82,hbmp,0,0);
		DeleteObject(hbmp);
	}

}
void CUsbSd::DoPaintPicBrowface(HWND hWnd)
{
	DoPaintWindows(hWnd,0,0,800,480,hbmpBkGround,0,0);
	HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\picbrow.bmp");   //ͼƬ�������
	DoPaintWindows(hWnd,0,0,800,64,hbmp,0,0);
	DeleteObject(hbmp);
}
void CUsbSd::DoPaintVideoFace(HWND hWnd)
{
	DoPaintWindows(hWnd,0,64,800,320,hbmpBkGround,0,64);  //����
	HBITMAP hbmp2 = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\videobk-2.bmp"); 
	DoPaintWindows(hWnd,0,0,800,64,hbmp2,0,0);         //����
	DeleteObject(hbmp2);
	HBITMAP hvideobb = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\videobb.bmp"); 
	DoPaintWindows(hWnd,0,384,800,96,hvideobb,0,0);     //������
	DeleteObject(hvideobb);

	HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\push.bmp");
	DoPaintWindows(hWnd,360,398,81,82,hbmp,0,0);      //��ť
	DeleteObject(hbmp);

}

void CUsbSd::DoFullScanPic(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	RECT rc = {0,0,800,480};

	
	BYTE *pbuffer = NULL;
	BITMAPINFO bitmapInfo;
	ZeroMemory(&bitmapInfo,sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  //�������ø���ֵ

	bitmapInfo.bmiHeader.biWidth = 100;      //����ͼ�Ĵ�С
	bitmapInfo.bmiHeader.biHeight =100;   
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 24;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage	= 100*100*3;

	HBITMAP hbmp = CreateDIBSection(hdc,&bitmapInfo,DIB_RGB_COLORS,(void**)&pbuffer,0,0); 
	
	memcpy(pbuffer,lpPicBuffer[PicCurrentNum].Data,100*100*3);

	HDC hdcmem = CreateCompatibleDC(hdc);
	HBRUSH hold = (HBRUSH)SelectObject(hdcmem,hbmp);
	StretchBlt(hdc,0,0,800,480,hdcmem,0,0,100,100,SRCCOPY);        //����ͼ���
	SelectObject(hdcmem,hold);
    DeleteDC(hdcmem);
    DeleteObject(hbmp);

	ImageDecoder *ChosePic = new ImageDecoder(PictureFile[PicCurrentNum].path);   //ȫ��
	ChosePic->Decode();
	ChosePic->Draw(hdc,rc);
	if(ChosePic)
	{
	  delete ChosePic;
	  ChosePic = NULL;
	}

	ReleaseDC(hWnd,hdc);

}

void CUsbSd::PaintVideoName(HWND hWnd)
{
	DoPaintWindows(hWnd,300,64,200,36,hbmpBkGround,300,64);
	HDC hdc = GetDC(hWnd);
	SetBkMode(hdc,0);
	SetTextColor(hdc,RGB(0,0,255));

	RECT TextRc = {300,64,500,100};
	HFONT FontList = CreateFont(25,0,0,0,FW_LIGHT,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH,_T("΢���ź�"));
	HFONT fontold = (HFONT)SelectObject(hdc,FontList);
	SetTextCharacterExtra(hdc,0);
	DrawText(hdc,VideoFile[VideoPlayNum].name,-1,&TextRc,DT_CENTER);
	SelectObject(hdc,fontold);
	DeleteObject(FontList);
	ReleaseDC(hWnd,hdc);
}

void CUsbSd::DoPiantVideoTime()
{
	int Second = AudioLength/10000000;  //��
	TCHAR str[50];                                    //������ʱ��
	_stprintf(str,L"%02d:%02d",Second/60,Second%60);
	VideoOverlay.DrawTextToOverlay(str,680,5,100,45);
}
void CUsbSd::CreateVideoListDC(HWND hWnd,int width,int height,TCHAR *filebk)
{
	HDC hdc = GetDC(hWnd);
	VideoListHdcV2 = CreateCompatibleDC(hdc);
	HBITMAP hbmpTT = CreateCompatibleBitmap(hdc,width,height);
	SelectObject(VideoListHdcV2,hbmpTT);

	HBITMAP hbmpbk = (HBITMAP)SHLoadDIBitmap(filebk);
	HDC hdcmem = CreateCompatibleDC(hdc);
	HBRUSH hold = (HBRUSH)SelectObject(hdcmem,hbmpbk);
	BitBlt(VideoListHdcV2,0,0,width,height,hdcmem,0,0,SRCCOPY);

	SelectObject(hdcmem,hold);
	DeleteDC(hdcmem);
	/*����ɾ����Ч����Ҫ����ʱɾ��*/
	DeleteObject(hbmpTT);

	DeleteObject(hbmpbk);
	ReleaseDC(hWnd,hdc);
}

void CUsbSd::CreateAudioListDC(HWND hWnd,int width,int height,TCHAR *filebk)
{
	HDC hdc = GetDC(hWnd);
	AudioListHdcV2 = CreateCompatibleDC(hdc);
	HBITMAP hbmpTT = CreateCompatibleBitmap(hdc,width,height);
	SelectObject(AudioListHdcV2,hbmpTT);

	HBITMAP hbmpbk = (HBITMAP)SHLoadDIBitmap(filebk);
	HDC hdcmem = CreateCompatibleDC(hdc);
	HBRUSH hold = (HBRUSH)SelectObject(hdcmem,hbmpbk);
	BitBlt(AudioListHdcV2,0,0,width,height,hdcmem,0,0,SRCCOPY);

	SelectObject(hdcmem,hold);
	DeleteDC(hdcmem);
	DeleteObject(hbmpTT);
    DeleteObject(hbmpbk);
	ReleaseDC(hWnd,hdc);
	//return hdcmemList;
}
void CUsbSd::MoveAudioList(HWND hWnd,int ypos)    //ypos   
{ 
   if(!AUDIOPLINGFACE&&AUDSEL)                                                         //�б����ݣ���Ƶ�ܸ���     AudioSearch->PicCount
   {
	   HDC hdc = GetDC(hWnd);                                    //          ��Ƶ�ļ������� AudioFile[num].name
	//////////////////draw bkground//////////////////////
	HDC hdcmem = CreateCompatibleDC(hdc);
	HBITMAP hbmpTT = CreateCompatibleBitmap(hdc,603,490);           //603*490
	HBRUSH hold = (HBRUSH)SelectObject(hdcmem,hbmpTT);
	BitBlt(hdcmem,0,0,603,490,AudioListHdcV2,0,0,SRCCOPY);
	
    /////////////////draw play mark and slider//////////////////////
	
	if((abs(Auddwpos/AGELISTLEN - AudioPlayNum) < 6)&&AUDIOPLAYING)
	{
		BitBlt(hdcmem,0,(AudioPlayNum - Auddwpos/AGELISTLEN)*AGELISTLEN - Auddwpos%AGELISTLEN+ypos,602,AGELISTLEN,HdcAudioSel,0,0,SRCCOPY);
		//BitBlt(hdcmem,500,(AudioPlayNum - Auddwpos/70)*70 - Auddwpos%70 + 20+ypos,32,28,hdcLaba,0,0,SRCCOPY);     //С���� ���ű�ʾ
	}
	if(AudioTCount <= 6)
		BitBlt(hdcmem,570,AudioTCount*AGELISTLEN-AGELISTLEN,17,49,hdcSlide,0,0,SRCCOPY);    //������
	else
		BitBlt(hdcmem,570,(Auddwpos*340)/((AudioTCount-6)*AGELISTLEN)-49+AGELISTLEN+ypos,17,49,hdcSlide,0,0,SRCCOPY); //������  350=5*70
	/////////////write text/////////////////
	RECT rc = {50,0,603,70};
	int num = 0;
	num +=Auddwpos/AGELISTLEN;    //avglen 70      listpos:�б�ǰλ��
	SetBkMode(hdcmem,1);
	HFONT fontold = (HFONT)SelectObject(hdcmem,FontAudiList);
	SetTextCharacterExtra(hdcmem,1);
	for(int i=0;i<7&&num<AudioTCount;i++)    //
	{
		DrawText(hdcmem,AudioFile[num].name,-1,&rc,DT_VCENTER);
		rc.top=rc.top+AGELISTLEN;
		rc.bottom=rc.bottom+AGELISTLEN;
		num++;
	}
	SelectObject(hdcmem,fontold);
	
	///////////////////copy to screen/////////////////////
	BitBlt(hdc,198,64,602,420,hdcmem,1,ypos,SRCCOPY);               //1 ������Ӱ
	SelectObject(hdcmem,hold);
    DeleteDC(hdcmem);
    DeleteObject(hbmpTT);
	ReleaseDC(hWnd,hdc);
   }
}
void CUsbSd::MoveEbookList(HWND hWnd,int ypos)    //ypos   
{                                                             //�б����ݣ���Ƶ�ܸ���     AudioSearch->PicCount
	HDC hdc = GetDC(hWnd);                                    //          ��Ƶ�ļ������� AudioFile[num].name
	//////////////////draw bkground//////////////////////
	HDC hdcmem = CreateCompatibleDC(hdc);
	HBITMAP hbmpTT = CreateCompatibleBitmap(hdc,603,490);           //603*490
	HBRUSH hold = (HBRUSH)SelectObject(hdcmem,hbmpTT);
	BitBlt(hdcmem,0,0,603,490,AudioListHdcV2,0,0,SRCCOPY);

	/////////////////draw play mark and slider//////////////////////
	if(EbookTCount <= 6)
		BitBlt(hdcmem,570,EbookTCount*70-70,17,49,hdcSlide,0,0,SRCCOPY);    //������
	else
		BitBlt(hdcmem,570,(Ebookdwpos*350)/((EbookTCount-6)*70)-49+70+ypos,17,49,hdcSlide,0,0,SRCCOPY); //������  350=5*70
	/////////////write text/////////////////
	RECT rc = {50,0,603,70};
	int num = 0;
	num +=Ebookdwpos/70;    //avglen 70      listpos:�б�ǰλ��
	SetBkMode(hdcmem,1);
	HFONT FontList = CreateFont(35,0,0,0,FW_LIGHT,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH,_T("΢���ź�"));
	HFONT fontold = (HFONT)SelectObject(hdcmem,FontList);

	SetTextCharacterExtra(hdcmem,1);

	for(int i=0;i<7&&num<EbookTCount;i++)    //
	{
		DrawText(hdcmem,EbookFile[num].name,-1,&rc,DT_VCENTER);
		rc.top=rc.top+70;
		rc.bottom=rc.bottom+70;
		num++;
	}
	SelectObject(hdcmem,fontold);
	
	///////////////////copy to screen/////////////////////
	BitBlt(hdc,198,64,602,420,hdcmem,1,ypos,SRCCOPY);               //1 ������Ӱ
	SelectObject(hdcmem,hold);
	DeleteDC(hdcmem);
	DeleteObject(FontList);
	DeleteObject(hbmpTT);

	ReleaseDC(hWnd,hdc);
}
DWORD WINAPI TxtThread(LPVOID lpParameter)
{
	//txt�ļ��Ű�
	//TxtEdit cText;  //������Ĺ��캯�����ļ�
	txtBook->cText->FileOut.seekp(0,ios_base::beg);
	while (!(txtBook->cText->File.eof()))
	{
		EnterCriticalSection(&txt_critical_section);
		txtBook->cText->FileOut.clear();
		txtBook->cText->FileOut.seekp(txtBook->ExitPos);
		txtBook->cText->Read();  //��ʼ���ļ��������Ű洦��
		txtBook->ExitPos = txtBook->cText->FileOut.tellp();
		txtBook->cText->PageNum = txtBook->cText->LineNum/LINAGE; //��ǰ���Ű���ҳ��
		LeaveCriticalSection(&txt_critical_section);
	}

	if (txtBook->cText->LineNum%LINAGE)
	{
		txtBook->cText->PageNum += 1;
	}
	return 1;
}
void CUsbSd::MoveVideoList(HWND hWnd,int ypos)
{      
   if(!VIDEOFACE&&VIDSEL)    
   {
	HDC hdc = GetDC(hWnd);                                    //          ��Ƶ�ļ������� AudioFile[num].name
	//////////////////draw bkground//////////////////////
	HDC hdcmem = CreateCompatibleDC(hdc);
	HBITMAP hbmpTT = CreateCompatibleBitmap(hdc,603,490);           //603*490
	HBRUSH hold = (HBRUSH)SelectObject(hdcmem,hbmpTT);
	BitBlt(hdcmem,0,0,603,490,AudioListHdcV2,0,0,SRCCOPY);

	/////////////////draw play mark and slider//////////////////////
	if(VideoTCount <= 6)
		BitBlt(hdcmem,570,VideoTCount*AGELISTLEN-AGELISTLEN,17,49,hdcSlide,0,0,SRCCOPY);    //������
	else
		BitBlt(hdcmem,570,(Viddwpos*340)/((VideoTCount-6)*AGELISTLEN)-49+AGELISTLEN+ypos,17,49,hdcSlide,0,0,SRCCOPY); //������  350=5*70
	/////////////write text/////////////////
	RECT rc = {50,0,603,70};
	int num = 0;
	num +=Viddwpos/AGELISTLEN;    //avglen 70      listpos:�б�ǰλ��
	SetBkMode(hdcmem,1);
	HFONT fontold = (HFONT)SelectObject(hdcmem,FontAudiList);
	SetTextCharacterExtra(hdcmem,1);
	for(int i=0;i<7&&num<VideoTCount;i++)    //
	{
		DrawText(hdcmem,VideoFile[num].name,-1,&rc,DT_VCENTER);
		rc.top=rc.top+AGELISTLEN;
		rc.bottom=rc.bottom+AGELISTLEN;
		num++;
	}
	SelectObject(hdcmem,fontold);

	///////////////////copy to screen/////////////////////
	BitBlt(hdc,198,64,602,420,hdcmem,1,ypos,SRCCOPY);               //1 ������Ӱ
	SelectObject(hdcmem,hold);
	DeleteDC(hdcmem);
	DeleteObject(hbmpTT);
	ReleaseDC(hWnd,hdc);
   }
}

void CUsbSd::PictureInit(int PicAllCount)    //PicAllCount  ��������Ч��           ��ʼ��
{ 
	if(g_scene->BmpTexId == NULL) 
	{
       g_scene->BmpTexId=(GLuint *)calloc(PicAllCount,sizeof(GLuint));    //����ͼƬ�ĸ�����������
       
       for(int i=0;i<PicAllCount;i++)
	   {
        g_scene->BmpTexId[i] = BmpMoren;
	   }
 
       lpPicBuffer = new PicBuffer[PicAllCount];  //ͼƬ�������ݱ���
	   PicThread = CreateThread(NULL,0,ThreadLoadPic0,(LPVOID)this,NULL,NULL);  //����ͼƬ�߳�
	  
	}
}

void CUsbSd::PictureLoad(int firstnum,int numCount)
{
	for (int i=firstnum;i<numCount;i++)
	{  
      PictureLoad(i);
	}
}

BOOL CUsbSd::PictureLoad(int number)   //����������������ͼ
{
	BYTE *buffer = NULL;
	//BOOL Turn = TRUE;
    if(PicIsBmp(PictureFile[number].path))  //bmp�ļ�
	{
	   if(BmpPicCut->CutBmp(PictureFile[number].path,NULL,100,100,&buffer) == FALSE)     //buffer�����ڴ棬���ҽ�������
		   return 0;
	   //Turn = TRUE;
	}
	else  //��BMP�ļ�
	{
      if(JpegPicCut->CutJpeg(PictureFile[number].path,&buffer,100,100)==FALSE)
		  return 0;
	  //Turn = FALSE;
	}
    
	memcpy(lpPicBuffer[number].Data,buffer,100*100*3);  //���ݱ���

	g_scene->BmpTexId[number]=g_scene->LoadBMP(&img,buffer,100,100,TRUE);   //buffer����������������ɾ��buffer
	return 1;
}
void CUsbSd::PictureLoad(int number,BYTE *buffer)
{
    //g_scene->BmpTexId[number]=g_scene->LoadBMP(&img,buffer,100,100);   //buffer����������������ɾ��buffer
}
BOOL CUsbSd::PicIsBmp(TCHAR *FilePath)
{
	FILE *fp;
	fp = _wfopen(FilePath,L"rb");
	if(fp == NULL)
		return 0;
	BYTE type[2];
	fread(type,1,2,fp);
	if ((type[0]==0x42)&&(type[1]==0x4d))   //BM��ʾ
	{
		fclose(fp);
		return 1;
	}
	fclose(fp);
	return 0;
}