//#include "BMP.h"
#include <windows.h>
//#include <jpeg.h>
#include "comm1.h"
#include <assert.h>
#include "button.h"

#include <stdio.h> 
#include<stdlib.h>
#include "Tga.h"
#pragma once
#include "imageBMP.h"

#include "resource.h"
#include "TGA1.h"

char* _INT2CHAR(int m)
{
	int n;
	char s[100];
	char   fla=0; 
	int count=0;
	memset(s,0,100); 
	if   (m   <   0)   
	{ 
		fla   =   '- '; 
		m   =   -1*m; 
	} 


	//n=m%10;
	m=m/10;
	while(m > 0)
	{
		s[count]=m%10+'0';
		count++;
		//n=m%10;
		m=m/10;
	}
	s[count]='\0';
	for(n=0;n<=count/2;n++)
	{
			char s1;
			s1=s[n];
			s[n]=s[count-n-1];
			s[count-n-1]=s1;
	}

	return s;//(char)(((int)'0')+i);
}

int _CHAR2INT(char mychar)
{
	return (int)(mychar-'0');
}
HBITMAP CopyScreenToBitmap(LPRECT lpRect)

 //lpRect ����ѡ������

{

HDC       hScrDC, hMemDC;      

 // ��Ļ���ڴ��豸������

HBITMAP    hBitmap, hOldBitmap;   

 // λͼ���

int       nX, nY, nX2, nY2;      

// ѡ����������

int       nWidth, nHeight;      

// λͼ��Ⱥ͸߶�

int       xScrn, yScrn;         

// ��Ļ�ֱ���



   // ȷ��ѡ������Ϊ�վ���

   if (IsRectEmpty(lpRect))

     return NULL;

   //Ϊ��Ļ�����豸������

   hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

   //Ϊ��Ļ�豸�����������ݵ��ڴ��豸������

   hMemDC = CreateCompatibleDC(hScrDC);

   // ���ѡ����������

   nX = lpRect->left;

   nY = lpRect->top;

   nX2 = lpRect->right;

   nY2 = lpRect->bottom;

   // �����Ļ�ֱ���

   xScrn = GetDeviceCaps(hScrDC, HORZRES);

   yScrn = GetDeviceCaps(hScrDC, VERTRES);

   //ȷ��ѡ�������ǿɼ���

   if (nX < 0)

      nX = 0;

   if (nY < 0)

      nY = 0;

   if (nX2 > xScrn)

      nX2 = xScrn;

   if (nY2 > yScrn)

      nY2 = yScrn;

   nWidth = nX2 - nX;

   nHeight = nY2 - nY;

   // ����һ������Ļ�豸��������ݵ�λͼ

   hBitmap = CreateCompatibleBitmap

   (hScrDC, nWidth, nHeight);

   // ����λͼѡ���ڴ��豸��������

   hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

   // ����Ļ�豸�����������ڴ��豸��������

   BitBlt(hMemDC, 0, 0, nWidth, nHeight,

   hScrDC, nX, nY, SRCCOPY);

   //�õ���Ļλͼ�ľ��

   hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

   //��� 

   DeleteDC(hScrDC);

   DeleteDC(hMemDC);

   // ����λͼ���

   return hBitmap;



   

//�õ���Ļλͼ����Ժ�,����
//
//���԰���Ļ����ճ������������.

    //if (OpenClipboard(hWnd)) 

    // //hWndΪ���򴰿ھ��

    //  {

    //    //��ռ�����

    //    EmptyClipboard();

    //    //����Ļ����ճ������������,

    //    hBitmap Ϊ�ղŵ���Ļλͼ���

    //    SetClipboardData(CF_BITMAP, hBitmap);

    //    //�رռ�����

    //    CloseClipboard();

      }

  /* ����Ҳ���԰���Ļ������λͼ��ʽ�浽�����ļ���.*/

      

//int SaveBitmapToFile(HBITMAP hBitmap , 	LPSTR lpFileName) //hBitmap Ϊ�ղŵ���Ļλͼ���
//
//{    	 //lpFileName Ϊλͼ�ļ���
//
//	HDC            hDC;         
//
//  	 //�豸������
//
//      int            iBits;      
//
// 	//��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
//
//	WORD            wBitCount;   
//
//   	//λͼ��ÿ��������ռ�ֽ���
//
//      //�����ɫ���С�� λͼ�������ֽڴ�С ��
//
//      //λͼ�ļ���С �� д���ļ��ֽ���
//
//	DWORD           dwPaletteSize=0,
//
//				dwBmBitsSize,
//
//				 dwDIBSize, dwWritten;
//
//	BITMAP          Bitmap;        
//
//	//λͼ���Խṹ
//
//	BITMAPFILEHEADER   bmfHdr;        
//
//	//λͼ�ļ�ͷ�ṹ
//
//      BITMAPINFOHEADER   bi;            
//
//	//λͼ��Ϣͷ�ṹ 
//
//	LPBITMAPINFOHEADER lpbi;          
//
//	//ָ��λͼ��Ϣͷ�ṹ
//
//      HANDLE          fh, hDib, hPal,hOldPal=NULL;
//
//	//�����ļ��������ڴ�������ɫ����
//
//  
//
//   //����λͼ�ļ�ÿ��������ռ�ֽ���
//
//   hDC = CreateDC(_T("DISPLAY"),NULL,NULL,NULL);
//
//iBits = GetDeviceCaps(hDC, BITSPIXEL) * 
//
//GetDeviceCaps(hDC, PLANES);
//
//   DeleteDC(hDC);
//
//   if (iBits <= 1)
//
//      wBitCount = 1;
//
//   else if (iBits <= 4)
//
//      wBitCount = 4;
//
//   else if (iBits <= 8)
//
//      wBitCount = 8;
//
//   else if (iBits <= 24)
//
//      wBitCount = 24;
//
//   //�����ɫ���С
//
//   if (wBitCount <= 8)
//
//      dwPaletteSize = (1 << wBitCount) *
//
//      sizeof(RGBQUAD);
//
//   
//
//   //����λͼ��Ϣͷ�ṹ
//
//   GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
//
//   bi.biSize            = sizeof(BITMAPINFOHEADER);
//
//   bi.biWidth           = Bitmap.bmWidth;
//
//   bi.biHeight          = Bitmap.bmHeight;
//
//   bi.biPlanes          = 1;
//
//   bi.biBitCount         = wBitCount;
//
//   bi.biCompression      = BI_RGB;
//
//   bi.biSizeImage        = 0;
//
//   bi.biXPelsPerMeter     = 0;
//
//   bi.biYPelsPerMeter     = 0;
//
//   bi.biClrUsed         = 0;
//
//   bi.biClrImportant      = 0;
//
//
//
//   dwBmBitsSize = ((Bitmap.bmWidth *
//
//    wBitCount+31)/32)* 4
//
//	 *Bitmap.bmHeight ;
//
//   //Ϊλͼ���ݷ����ڴ�
//
//   hDib  = GlobalAlloc(GHND,dwBmBitsSize+
//
//	dwPaletteSize+sizeof(BITMAPINFOHEADER));
//
//   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
//
//   *lpbi = bi;
//
//   // �����ɫ��   
//
//   hPal = GetStockObject(DEFAULT_PALETTE);
//
//   if (hPal)
//
//   {
//
//      hDC  = GetDC(NULL);
//
//      hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
//
//      RealizePalette(hDC);
//
//   }
//
//   // ��ȡ�õ�ɫ�����µ�����ֵ
//
//   GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight,
//
//	 (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
//
//    +dwPaletteSize,
//
//	 (BITMAPINFOHEADER *)
//
//    lpbi, DIB_RGB_COLORS);
//
//   //�ָ���ɫ��   
//
//   if (hOldPal)
//
//   {
//
//      SelectPalette(hDC, hOldPal, TRUE);
//
//      RealizePalette(hDC);
//
//      ReleaseDC(NULL, hDC);
//
//   }
//
//   //����λͼ�ļ�    
//
//fh = CreateFile(lpFileName, GENERIC_WRITE, 
//
//0, NULL, CREATE_ALWAYS,
//
//         FILE_ATTRIBUTE_NORMAL | FILE_
//
//         FLAG_SEQUENTIAL_SCAN, NULL);
//
//   if (fh == INVALID_HANDLE_VALUE)
//
//      return FALSE;
//
//   // ����λͼ�ļ�ͷ
//
//   bmfHdr.bfType = 0x4D42;  // "BM"
//
//dwDIBSize    = sizeof(BITMAPFILEHEADER) 
//
//           + sizeof(BITMAPINFOHEADER)
//
//	        + dwPaletteSize + dwBmBitsSize;  
//
//   bmfHdr.bfSize = dwDIBSize;
//
//   bmfHdr.bfReserved1 = 0;
//
//   bmfHdr.bfReserved2 = 0;
//
//   bmfHdr.bfOffBits = (DWORD)sizeof
//
//   (BITMAPFILEHEADER) 
//
//      + (DWORD)sizeof(BITMAPINFOHEADER)
//
//     + dwPaletteSize;
//
//   // д��λͼ�ļ�ͷ
//
//WriteFile(fh, (LPSTR)&bmfHdr, sizeof
//
//(BITMAPFILEHEADER), &dwWritten, NULL);
//
//   // д��λͼ�ļ���������
//
//   WriteFile(fh, (LPSTR)lpbi, dwDIBSize, 
//
//   &dwWritten, NULL);
//
//   //���   
//
//   GlobalUnlock(hDib);
//
//   GlobalFree(hDib);
//
//   CloseHandle(fh);
//
//}
 

HBITMAP CopyScreenToBitmap(int &nWidth,int &nHeight)
{
    
    HDC  hScrDC,// ��Ļ�豸������ 
		hMemDC;   //���ڴ��豸������   
    // λͼ���
    HBITMAP  hBitmap, hOldBitmap;    
    // ��Ļ�ֱ���
    int  xScrn, yScrn;         
    
    //Ϊ��Ļ�����豸������
    hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
    //Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
    hMemDC = CreateCompatibleDC(hScrDC);
    // �����Ļ�ֱ���
    xScrn = GetDeviceCaps(hScrDC, HORZRES);
    yScrn = GetDeviceCaps(hScrDC, VERTRES);

    //�洢��Ļ�ĳ���
    nWidth = xScrn;
    nHeight = yScrn;
    
    // ����һ������Ļ�豸��������ݵ�λͼ
    hBitmap = CreateCompatibleBitmap(hScrDC, xScrn, yScrn);
    // ����λͼѡ���ڴ��豸��������, ��hBitmapָ���λͼѡ���ڴ�DC������ֵΪԭ����λͼholdbitmap���ȱ�����������ָ�

    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    // ����Ļ�豸�����������ڴ��豸��������,���豸DC��ͼ���Ƶ��ڴ�DC,�����Ͱ���Ļͼ�󱣴浽hbitmap��ָ���BITMAP�ṹ������


    BitBlt(hMemDC, 0, 0, xScrn,yScrn,hScrDC, 0, 0, SRCCOPY);
    //�õ���Ļλͼ�ľ��
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//HDC hdc=GetWindowDC(hwnd);//�������hwndָ��Ĵ��ڵ�DC
	//GetWindowRect(hwnd,&rect);//��ô��ھ���

	//BitBlt(hdc,0,0,rect.right-rect.left,rect.bottom-rect.top,hmemdc,0,0,SRCCOPY);//���ڴ�DC��ͼ����ʾ��������


    //��� 
    DeleteDC(hScrDC);
    DeleteDC(hMemDC);
    // ����λͼ���
    return hBitmap;

//	HBITMAP holdbitmap,hbitmap;
//HDC hdc,hscreendc,hmemdc;
//hdc=GetWindowDC(hwnd);//�������hwndָ��Ĵ��ڵ�DC
//GetWindowRect(hwnd,&rect);//��ô��ھ���
//hscreendc=CreateDC("DISPLAY",NULL,NULL,NULL);//���������Ļ���豸DC
//hmemdc=CreateCompatibleDC(hdc);//�������豸DC���ݵ��ڴ�DC
//hbitmap=CreateCompatibleBitmap(hscreendc,GetDeviceCaps(hdcScreen,HORZRES),GetDeviceCaps(hdcScreen,VERTRES)); //��������ĻDC���ݵ�λͼ
//holdbitmap=(HBITMAP)SelectObject(hmemdc,hbitmap);//��hbitmapָ���λͼѡ���ڴ�DC������ֵΪԭ����λͼholdbitmap���ȱ�����������ָ�
//BitBlt(hmemdc,0,0,rect.right-rect.left,rect.bottom-rect.top,hscreendc,0,0,SRCCOPY);//���豸DC��ͼ���Ƶ��ڴ�DC,�����Ͱ���Ļͼ�󱣴浽hbitmap��ָ���BITMAP�ṹ������
//
//
//�����ť������BitBlt��ʾ����:
//case WM_COMMAND:
//holdbitmap=(HBITMAP)SelectObject(hmemdc,hbitmap);//������ͼ���hbitmapѡ���ڴ�DC
//BitBlt(hdc,0,0,rect.right-rect.left,rect.bottom-rect.top,hmemdc,0,0,SRCCOPY);//���ڴ�DC��ͼ����ʾ��������

}

int Save16BitmapToFile(HBITMAP hBitmap,LPCTSTR lpFileName)//���������ñ���Ϊ16λ��ͼƬ
{
    

    HDC hDC=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
    HDC hOffDC=CreateCompatibleDC(hDC);
    SelectObject(hOffDC,hBitmap);
    
    BITMAP Bitmap;
    GetObject(hBitmap,sizeof(BITMAP),&Bitmap);
    
    HANDLE fh=CreateFile(lpFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if(fh == INVALID_HANDLE_VALUE )
        return FALSE;

    BITMAPFILEHEADER bfh;
    memset(&bfh,0,sizeof(bfh));
    bfh.bfType=0x4D42/*((WORD) ('M' << 8) | 'B')*/;
    bfh.bfSize= sizeof(bfh)+2*Bitmap.bmWidth*Bitmap.bmHeight+sizeof(BITMAPFILEHEADER);
    bfh.bfOffBits=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);
    DWORD dwWritten=0;
    WriteFile(fh,&bfh,sizeof(bfh),&dwWritten,NULL);
    BITMAPINFOHEADER bih;
    memset(&bih,0,sizeof(bih));
    bih.biSize=sizeof(bih);
    bih.biWidth=Bitmap.bmWidth;
    bih.biHeight=Bitmap.bmHeight;
    bih.biPlanes=1;
    bih.biBitCount=16;

    if( !WriteFile(fh,&bih,sizeof(bih),&dwWritten,NULL) )
    {
        return FALSE;
    }
    
    BITMAPINFO bitmapInfo;
    memset((void *)&bitmapInfo,0,sizeof(BITMAPINFO) );
    bitmapInfo.bmiHeader=bih;

    HDC hMemDC=CreateCompatibleDC(hDC);    
    BYTE *m_lpBitBmp=new BYTE[bfh.bfSize-sizeof(BITMAPFILEHEADER)];
    HBITMAP hDibBitmap=CreateDIBSection(hMemDC,&bitmapInfo,DIB_RGB_COLORS,(void **)&m_lpBitBmp,
        NULL,0);
    if(hDibBitmap != 0)
    {
        ::SelectObject(hMemDC,hDibBitmap);
        BitBlt(hMemDC,0,0,Bitmap.bmWidth,Bitmap.bmHeight,hOffDC,0,0,SRCCOPY);
        WriteFile(fh,m_lpBitBmp,bfh.bfSize-sizeof(BITMAPFILEHEADER),&dwWritten,NULL);
    }
    
    DeleteObject(hDibBitmap);
    DeleteDC(hDC);
    DeleteDC(hMemDC);

    CloseHandle(fh);

    return 1;

}



int Save24BitmapToFile(HBITMAP hBitmap,LPCTSTR lpFileName)//���������ñ���Ϊ24λ��ͼƬ
{
    

    HDC hDC=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
    HDC hOffDC=CreateCompatibleDC(hDC);
    SelectObject(hOffDC,hBitmap);
    
    BITMAP Bitmap;
    GetObject(hBitmap,sizeof(BITMAP),&Bitmap);
    
    void * fh=CreateFile(lpFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if(fh == INVALID_HANDLE_VALUE )
        return FALSE;

    BITMAPFILEHEADER bfh;
    memset(&bfh,0,sizeof(bfh));
    bfh.bfType=0x4D42/*((WORD) ('M' << 8) | 'B')*/;
    bfh.bfSize= sizeof(bfh)+3*Bitmap.bmWidth*Bitmap.bmHeight+sizeof(BITMAPFILEHEADER);
    bfh.bfOffBits=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);
    DWORD dwWritten=0;
    WriteFile(fh,&bfh,sizeof(bfh),&dwWritten,NULL);
    BITMAPINFOHEADER bih;
    memset(&bih,0,sizeof(bih));
    bih.biSize=sizeof(bih);
    bih.biWidth=Bitmap.bmWidth;
    bih.biHeight=Bitmap.bmHeight;
    bih.biPlanes=1;
    bih.biBitCount=24;

    if( !WriteFile(fh,&bih,sizeof(bih),&dwWritten,NULL) )
    {
        return FALSE;
    }
    
    BITMAPINFO bitmapInfo;
    memset((void *)&bitmapInfo,0,sizeof(BITMAPINFO) );
    bitmapInfo.bmiHeader=bih;

    HDC hMemDC=CreateCompatibleDC(hDC);    
  
    
	BYTE *m_lpBitBmp;//=(BYTE*)calloc(3*Bitmap.bmWidth*Bitmap.bmHeight,sizeof(BYTE));
	HBITMAP hDibBitmap=CreateDIBSection(hMemDC,&bitmapInfo,DIB_RGB_COLORS,(void **)&m_lpBitBmp,
        NULL,0);
    if(hDibBitmap != 0)
    {
        ::SelectObject(hMemDC,hDibBitmap);
        BitBlt(hMemDC,0,0,Bitmap.bmWidth,Bitmap.bmHeight,hOffDC,0,0,SRCCOPY);
        WriteFile(fh,m_lpBitBmp,3*Bitmap.bmWidth*Bitmap.bmHeight,&dwWritten,NULL);
    }
   
    DeleteObject(hDibBitmap);
 /*  if(m_lpBitBmp==NULL)
	printf("m_lpBitBmp======NULL\n");	
   else
		printf("m_lpBitBmp======nonNULL\n");	*/
	   DeleteDC(hMemDC);
	
    CloseHandle(fh);

 
    DeleteDC(hDC);

 return 1;
}


/*������ɫ/��ɫ�� 

λͼ���õ�һ��ѹ����������λͼͼƬ��ѡ�����д����Ե���������ɫ(ͨ��������256��)���Ƴ���ɫ��
Ȼ��ͼƬ��ԭ����ɫ����ɫ�����������ʾ��
����ԭͼƬ���Ա����������ѹ�����ʺ���ѹ����ҳͼ�ε���ɫ�����ٵ�ͼ�Σ�
���ʺ�ѹ����Ƭ��ɫ�ʷḻ��ͼ�Ρ� */

int Save32BitmapToFile(HBITMAP hBitmap,LPCTSTR lpFileName)//���������ñ���Ϊ32λ����ɫ���ͼƬ
{
    /*����ʹ����ɫ����ȥ��bih.biCompression=BI_BITFIELDS;��䣬Ȼ��
    ������bitmapInfo.bmiColors[0].rgbRed=255;
    bitmapInfo.bmiColors[0].rgbGreen=255;
    bitmapInfo.bmiColors[0].rgbBlue=255;*/

    HDC hDC=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
    HDC hOffDC=CreateCompatibleDC(hDC);
    SelectObject(hOffDC,hBitmap);
    
    BITMAP Bitmap;
    GetObject(hBitmap,sizeof(BITMAP),&Bitmap);
    
    HANDLE fh=CreateFile(lpFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if(fh == INVALID_HANDLE_VALUE )
        return FALSE;

    BITMAPFILEHEADER bfh;
    memset(&bfh,0,sizeof(bfh));
    bfh.bfType=0x4D42/*((WORD) ('M' << 8) | 'B')*/;
    bfh.bfSize= sizeof(bfh)+4*Bitmap.bmWidth*Bitmap.bmHeight+sizeof(BITMAPFILEHEADER);
    bfh.bfOffBits=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);
    
    DWORD dwWritten=0;
    WriteFile(fh,&bfh,sizeof(bfh),&dwWritten,NULL);
    BITMAPINFOHEADER bih;
    memset(&bih,0,sizeof(bih));
    bih.biSize=sizeof(bih);
    bih.biWidth=Bitmap.bmWidth;
    bih.biHeight=Bitmap.bmHeight;
    bih.biPlanes=1;
    bih.biBitCount=32;
    bih.biCompression=BI_BITFIELDS;
    

    if( !WriteFile(fh,&bih,sizeof(bih),&dwWritten,NULL) )
    {
        return FALSE;
    }
    
    BITMAPINFO bitmapInfo;
    memset((void *)&bitmapInfo,0,sizeof(BITMAPINFO) );
    bitmapInfo.bmiHeader=bih;
    bitmapInfo.bmiColors[0].rgbRed=255;
    bitmapInfo.bmiColors[0].rgbGreen=255;
    bitmapInfo.bmiColors[0].rgbBlue=255;
    

    HDC hMemDC=CreateCompatibleDC(hDC);    
    BYTE *m_lpBitBmp=new BYTE[4*Bitmap.bmWidth*Bitmap.bmHeight];
    HBITMAP hDibBitmap=CreateDIBSection(hMemDC,&bitmapInfo,DIB_RGB_COLORS,(void **)&m_lpBitBmp,
        NULL,0);
    if(hDibBitmap != 0)
    {
        ::SelectObject(hMemDC,hDibBitmap);
        BitBlt(hMemDC,0,0,Bitmap.bmWidth,Bitmap.bmHeight,hOffDC,0,0,SRCCOPY);
        WriteFile(fh,m_lpBitBmp,4*Bitmap.bmWidth*Bitmap.bmHeight,&dwWritten,NULL);
    }
    
    DeleteObject(hDibBitmap);
    DeleteDC(hDC);
    DeleteDC(hMemDC);

    CloseHandle(fh);

    return 1;

}


//int SaveJPEGToFile(HBITMAP hBitmap,LPCSTR lpFileName)//���������ñ���ΪjpegͼƬ
//{
//    
//
//    HDC hDC=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
//    HDC hOffDC=CreateCompatibleDC(hDC);
//    SelectObject(hOffDC,hBitmap);
//    
//    BITMAP Bitmap;
//    GetObject(hBitmap,sizeof(BITMAP),&Bitmap);
//    
//    HANDLE fh=CreateFile((LPCWSTR)lpFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
//        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
//    if(fh == INVALID_HANDLE_VALUE )
//        return FALSE;
//
//    BITMAPFILEHEADER bfh;
//    memset(&bfh,0,sizeof(bfh));
//    bfh.bfType=0x4D42/*((WORD) ('M' << 8) | 'B')*/;
//    bfh.bfSize= sizeof(bfh)+3*Bitmap.bmWidth*Bitmap.bmHeight+sizeof(BITMAPFILEHEADER);
//    bfh.bfOffBits=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);
//    DWORD dwWritten=0;
////    WriteFile(fh,&bfh,sizeof(bfh),&dwWritten,NULL);
//    BITMAPINFOHEADER bih;
//    memset(&bih,0,sizeof(bih));
//    bih.biSize=sizeof(bih);
//    bih.biWidth=Bitmap.bmWidth;
//    bih.biHeight=Bitmap.bmHeight;
//    bih.biPlanes=1;
//    bih.biBitCount=24;
//
//    /*if( !WriteFile(fh,&bih,sizeof(bih),&dwWritten,NULL) )
//    {
//        return FALSE;
//    }*/
//    
//    BITMAPINFO bitmapInfo;
//    memset((void *)&bitmapInfo,0,sizeof(BITMAPINFO) );
//    bitmapInfo.bmiHeader=bih;
//
//    HDC hMemDC=CreateCompatibleDC(hDC);    
//    BYTE *m_lpBitBmp=new BYTE[bfh.bfSize-sizeof(BITMAPFILEHEADER)];
//    HBITMAP hDibBitmap=CreateDIBSection(hMemDC,&bitmapInfo,DIB_RGB_COLORS,(void **)&m_lpBitBmp,
//        NULL,0);
//    if(hDibBitmap != 0)
//    {
//        ::SelectObject(hMemDC,hDibBitmap);
//        BitBlt(hMemDC,0,0,Bitmap.bmWidth,Bitmap.bmHeight,hOffDC,0,0,SRCCOPY);
//        //WriteFile(fh,m_lpBitBmp,bfh.bfSize-sizeof(BITMAPFILEHEADER),&dwWritten,NULL);
//        WriteJPEGFile(lpFileName, m_lpBitBmp,Bitmap.bmWidth,Bitmap.bmHeight,TRUE, 60);
//
//    }
//    
//    DeleteObject(hDibBitmap);
//    DeleteDC(hDC);
//    DeleteDC(hMemDC);
//
//    CloseHandle(fh);
//
//    return 1;
//
//}



//BOOL   CompoundDIB(HANDLE   hDIB,HANDLE   hDIBSrc,int   alpha)   
//  {   
//  LPVOID   lpvBuf=NULL;   //   Ŀ��ͼ������ָ��   
//  LPVOID   lpvBufSrc=NULL;   //   Դͼ����ָ��   
//    
//  //   //   Դͼ����Ϣ   //     
//  LPBITMAPINFO   lpbmif=(LPBITMAPINFO)hDIBSrc;     
//  LPBITMAPINFOHEADER   lpbmifh=(LPBITMAPINFOHEADER)lpbmif;   
//  //   ����ͼ������ƫ����   
//  UINT   nColors=lpbmifh->biClrUsed   ?   lpbmifh->biClrUsed   :   1<<lpbmifh->biBitCount;   
//  if   (   nColors   >256   )   
//  nColors=0;   //   �����ɫ������256ɫ����û�е�ɫ��   
//  lpvBufSrc=(LPVOID)((LPBYTE)lpbmif->bmiColors+nColors*sizeof(RGBQUAD));     
//  int   cxSrc=lpbmifh->biWidth;   //   Դͼ����   
//  int   cySrc=lpbmifh->biHeight;   //   Դͼ��߶�   
//  //   ����ͼ��ÿ�е��ֽ���(ͼ��λ��   x   ͼ����,������ܱ�2��������ÿ�к������һ��0�ֽ�)   
//  int   nBytesPerLineSrc=((cxSrc*lpbmifh->biBitCount+31)&~31)/8;     
//    
//  //   //   Ŀ��ͼ����Ϣ   //   
//  lpbmif=(LPBITMAPINFO)hDIB;   
//  lpbmifh=(LPBITMAPINFOHEADER)lpbmif;   
//  nColors=lpbmifh->biClrUsed   ?   lpbmifh->biClrUsed   :   1<<lpbmifh->biBitCount;   
//  if   (   nColors   >256   )   
//  nColors=0;   
//  lpvBuf=(LPVOID)((LPBYTE)lpbmif->bmiColors+nColors*sizeof(RGBQUAD));   
//  int   cx=lpbmifh->biWidth;   
//  int   cy=lpbmifh->biHeight;   
//  int   nBytesPerLine=((cx*lpbmifh->biBitCount+31)&~31)/8;   
//    
//  LPBYTE   lpbPnt=NULL;   
//  LPBYTE   lpbPntSrc=NULL;   
//  //   //   ͨ��alphaֵ�ϲ�����ͼ�������ֵ   //   
//  //   ���������24λ���ɫͼ��������ȵ�ͼ�����������Դ�����   
//  for   (   int   y=(cy<cySrc   ?   cy   :   cySrc);   y>0   ;y--   )   
//  {   
//  lpbPnt=(LPBYTE)lpvBuf+nBytesPerLine*(y-1);   
//  lpbPntSrc=(LPBYTE)lpvBufSrc+nBytesPerLineSrc*(y-1);   
//  for   (   int   x=0;   x<(cx<cxSrc   ?   cx   :   cxSrc);   x++   )   
//  {   
//  for   (   int   i=0   ;i<3   ;i++   )   
//  *lpbPnt++=(*lpbPnt*(255-alpha)+*(lpbPntSrc++)*alpha)/255;   
//  }   
//  }   
//  return   TRUE;   
//  }   
    
/*     �ص��ղ����۵����⣬��α��⻭����͸��ͼ��һ�����Ե���������һ��ղŽ��ܵ�����Alphaֵ�ϳ�ͼ�󷽷���
   ��������ںϳɵĹ����ж�̬�޸�Alphaֵ��ʹ�����������ִ�(������)0�������ȵ�(ǰ����)255,�������Ϳ���ʹǰ���𲽵���͸�����������ˡ�
   �������������������ɣ�   
    
      Alphaͨ��   
    
      ������˵�Ķ�̬�޸ĵ�Alphaֵ��һ����ʹ��һ��256���ĻҶ�ͼ��ʵ�ֵ�(���ŻҶ�ͼ�ͳ�ΪAlphaͨ��)���Ҷ�ͼ�ĸ���ֵ��Ӧ��ǰ��ͼƬ��Ӧ���Alphaֵ���Ҷ�ͼ�ĺ�ɫ������͸����(AlphaֵΪ0)����ɫ����Ϊ��͸������(AlphaֵΪ255)���ҶȲ��־���ǰ���ͱ������ںϲ��֡���һ���ϳ�Ч����!   
      Alphaͨ��   ǰ��ͼ     
       ����ͼ   �ϳ�ͼ     
    
      ���Կ���������Alphaͨ�����ϳɺ��ͼ��ǰ���ͱ����ǳ��������ں���һ���ˡ�   
      Alphaͨ���ϳ�ͼ����룺   
 */   
  BOOL   CompoundDIB(HDC   hDC,int   left,int   top,void *   hDIBDes,void*   hDIBSrc,void *   hDIBAlpha)   
  {   
          void *   lpvBufDes=NULL;   //   Ŀ��ͼ������ָ��(����)   
          void *   lpvBufSrc=NULL;   //   Դͼ����ָ��(ǰ��)   
          void *   lpvBufAlpha=NULL;   //   Alphaͨ������ָ��   
    
          //   //   Դͼ����Ϣ   //     
          BITMAPINFO*   lpbmif=(BITMAPINFO*)hDIBSrc; // LPBITMAPINFO   lpbmif=(LPBITMAPINFO)hDIBSrc;     
          BITMAPINFOHEADER*   lpbmifh=(BITMAPINFOHEADER*)lpbmif;  //LPBITMAPINFOHEADER   lpbmifh=(LPBITMAPINFOHEADER)lpbmif;   
          //   ����ͼ������ƫ����   
          UINT   nColors=lpbmifh->biClrUsed   ?   lpbmifh->biClrUsed   :   1<<lpbmifh->biBitCount;   
          if   (   nColors   >256   )   
                  nColors=0;   //   �����ɫ������256ɫ����û�е�ɫ��   
          lpvBufSrc=lpbmif->bmiColors+nColors;     
          int   cxSrc=lpbmifh->biWidth;   //   Դͼ����   
          int   cySrc=lpbmifh->biHeight;   //   Դͼ��߶�   
          //   ����ͼ��ÿ�е��ֽ���(ͼ��λ��   x   ͼ����,������ܱ�2��������ÿ�к������һ��0�ֽ�)   
          int   nBytesPerLineSrc=((cxSrc*lpbmifh->biBitCount+31)&~31)/8;     
    
          //   //   Ŀ��ͼ����Ϣ   //   
          lpbmif=(BITMAPINFO*)hDIBDes;   
          lpbmifh=(BITMAPINFOHEADER*)lpbmif;   
          nColors=lpbmifh->biClrUsed   ?   lpbmifh->biClrUsed   :   1<<lpbmifh->biBitCount;   
          if   (   nColors   >256   )   
                  nColors=0;   
          lpvBufDes=lpbmif->bmiColors+nColors;   
          int   cxDes=lpbmifh->biWidth;   
          int   cyDes=lpbmifh->biHeight;   
          int   nBytesPerLineDes=((cxDes*lpbmifh->biBitCount+31)&~31)/8;   
    
          //   //   Alphaͨ����Ϣ   //   
          lpbmif=(BITMAPINFO*)hDIBAlpha;   
          lpbmifh=(BITMAPINFOHEADER*)hDIBAlpha;   
          /*assert(lpbmifh->biWidth==cxSrc   &&   lpbmifh->biHeight==cySrc   &&     
                  lpbmifh->biBitCount==8   );   */
          nColors=lpbmifh->biClrUsed   ?   lpbmifh->biClrUsed   :   256;   
          lpvBufAlpha=lpbmif->bmiColors+nColors;   
          int   nBytesPerLineAlpha=((cxSrc*8+31)&~31)/8;   
    
          //   //   ������ȡ��ɫֵ��ָ��   //   
          BYTE*   lpbPntDes=NULL;   
          BYTE*   lpbPntSrc=NULL;   
          BYTE*   lpbPntAlpha=NULL;   
          //   //   ͨ��alphaֵ�ϲ�����ͼ�������ֵ   //   
          //   ���������24λ���ɫͼ��������ȵ�ͼ�����������Դ�����   
          for( int  y=cySrc; y>0; y-- )   
          {   
                  lpbPntDes=(BYTE*)lpvBufDes+nBytesPerLineDes*(cyDes-top-cySrc+y-1)+left*3;   
                  lpbPntSrc=(BYTE*)lpvBufSrc+nBytesPerLineSrc*(y-1);   
                  lpbPntAlpha=(BYTE*)lpvBufAlpha+nBytesPerLineAlpha*(y-1);   
                  for   (   int   x=0;   x<cxSrc;   x++   )   
                  {   
                          int   alpha=*lpbPntAlpha++;   
                          for   (   int i=0; i<3; i++  )   
                                  *lpbPntDes++=(*lpbPntDes*(255-alpha)+*(lpbPntSrc++)*alpha)/255;   
                  }   
          }   
		  //   ��buttonͼƬ����Ļ�� 
       /* SetDIBitsToDevice(hDC,left,top,cx,cy,0,0,0,cy,lpvBufFairy, 
                (LPBITMAPINFO)m_hDIBFairy,DIB_RGB_COLORS); */

          return   TRUE;   
    
  } 

  int findTransparentPixel(unsigned char threshold1,unsigned char threshold2,unsigned char threshold3,unsigned char r,unsigned char g, unsigned char b)
  {
	int count=0,drg,drb,dgb;
	int flag=0;
	if((r>threshold1)&&(g>threshold1)&&(b>threshold1))
	{
		drg=abs(r-g);
		drb=abs(r-b);
		dgb=abs(g-b);
		count=drg+drb+dgb;
		if(((drg<threshold2)&&(drb<threshold2)&&(dgb<threshold2))||(count<threshold3))
			flag=1;
	}
	return flag;
  }
unsigned char *  CompoundTGA(int   left,int   top, TCHAR *fnameBackgd,TCHAR *fnameForegd,BITMAPINFOHEADER*   bmifhOut, float Alpha)
{    
   int retBackgd,retForegd;
   unsigned char *bufBackgd,  /*Store backgound image pixel*/
	    *bufForegd,
	    *bufOut;  /*output image pixel buffer*/   
	   
	int widthBackgd, heightBackgd, bppBackgd;
	int widthForegd, heightForegd, bppForegd; 

    retBackgd = TGALoad( fnameBackgd, &bufBackgd, &widthBackgd, &heightBackgd, &bppBackgd);
	TCHAR ErrorMsg[MAX_PATH];
    if ( ! retBackgd )
	{   
   		printf("CompoundTGA:: Failed to load  %s  TGA image file !\n", fnameBackgd);
		_tsprintf_s(ErrorMsg,MAX_PATH,__TEXT("Failed to load  %s   TGA image file !\n"), fnameBackgd);
		OutputDebugString(ErrorMsg); 
	    OutputDebugString(_T("\r\n"));
	//	OutputDebugString(TEXT("SlipCalendarsRight CreateDIBSection false\r\n"));
      return NULL;
   }
	printf("bppBackgd=%d, widthBackgd=%d,heightBackgd=%d\n,",bppBackgd,widthBackgd,heightBackgd);
  

	bufOut= (unsigned char *) malloc(3 * (widthBackgd) * (heightBackgd));
	int t;
	if( bppBackgd == 32 )
	{
	   for(int i=heightBackgd-1;i>=0;i--)
	   {	
		   for(int j=0;j<widthBackgd;j++)
			{
				t=i*widthBackgd+j;
				bufOut[(t*3)]=bufBackgd[(t*4)];
				bufOut[(t*3)+1]=bufBackgd[(t*4)+1];
				bufOut[(t*3)+2]=bufBackgd[(t*4)+2];
			}
	   }
	}
	if(bppBackgd == 24)
	{
		for(int i=heightBackgd-1;i>=0;i--)
	   {	
		   for(int j=0;j<widthBackgd;j++)
			{
				t=i*widthBackgd+j;
				bufOut[(t*3)]=bufBackgd[(t*3)];
				bufOut[(t*3)+1]=bufBackgd[(t*3)+1];
				bufOut[(t*3)+2]=bufBackgd[(t*3)+2];
			}
	   }
	}
	retForegd = TGALoad( fnameForegd, &bufForegd, &widthForegd, &heightForegd, &bppForegd);
	
	
   if ( ! retForegd )
   {
	   	printf("CompoundTGA:: Failed to load  %s  TGA image file !\n", fnameForegd);
		_tsprintf_s(ErrorMsg,MAX_PATH,__TEXT("Failed to load  %s   TGA image file !\n"), fnameForegd);
		OutputDebugString(ErrorMsg); 
	    OutputDebugString(_T("\r\n"));
		
      return NULL;
   }

  
	bmifhOut->biSize=40L;
	bmifhOut->biWidth=widthBackgd;
	bmifhOut->biHeight =heightBackgd;
	bmifhOut->biPlanes=(unsigned short)1;
	bmifhOut->biBitCount =(unsigned short)24;
	bmifhOut->biCompression = BI_RGB;
	int   nBytesPerLineBackgd=((widthBackgd*bmifhOut->biBitCount+31)&~31)/8;  
	bmifhOut->biSizeImage =0;//widthBackgd * heightBackgd * 3;//(bmifhOut->biWidth*bmifhOut->biBitCount+31)/32*4*bmifhOut->biHeight; //nBytesPerLineBackgd*heightBackgd; ;//(((widthBackgd+3)>>2)<<2)*heightBackgd; ;
	bmifhOut->biXPelsPerMeter = 0;
	bmifhOut->biYPelsPerMeter = 0;
	bmifhOut->biClrUsed = 0;

	bmifhOut->biClrImportant = 0;
	 
	//   ����ϳ�ͼ������ƫ����   			
	UINT  nColors=bmifhOut->biClrUsed   ?   bmifhOut->biClrUsed   :   1<<bmifhOut->biBitCount;   
	if (  nColors > 256   )   
          nColors=0;   
   //   ͨ��alphaֵ�ϲ�����ͼ�������ֵ   //   
  //   ���������24λ���ɫͼ��������ȵ�ͼ�����������Դ�����   
	  int m,n;
	  int count1=0;
	  int count2=0;
	  float alpha1=0;
	  float alpha2=0;
	  int umax=0;
	  int ux=0,uy=0,um=0;
	  if((bppForegd==32)&&(bppBackgd==24))
	  {	
		  for(int y=heightForegd-1; y>=0; y--)   
		  {   
				  for   ( int x=0; x<widthForegd; x++ )   
				  {   
						m=(y*widthForegd+x)*4;
						n=((top+y)*widthBackgd+x+left)*3;
						
						if(bufForegd[m+3]==255)
						{
							if(findTransparentPixel(210,30,40,bufForegd[m],bufForegd[m+1],bufForegd[m+2])==0)
							{
								bufOut[n]=(int)((bufBackgd[n]*(1-Alpha)+(bufForegd[m])*Alpha));
								bufOut[n+1]=(int)((bufBackgd[n+1]*(1-Alpha)+(bufForegd[m+1]*Alpha)));
								bufOut[n+2]=(int)((bufBackgd[n+2]*(1-Alpha)+(bufForegd[m+2]*Alpha)));
								count1++;
							}
						}
						if(bufForegd[m+3]!=255)
						{	
							//printf("f4[%d][%d]=%d,",y,x,bufForegd[m+3]);
							alpha1=(float)bufForegd[m+3]/255.0;
							alpha2=1-alpha1;
							bufOut[n]=(int)(bufBackgd[n]*alpha2 + bufForegd[m]*alpha1);
							bufOut[n+1]=(int)(bufBackgd[n+1]*alpha2 + bufForegd[m+1]*alpha1);
							bufOut[n+2]=(int)(bufBackgd[n+2]*alpha2 + bufForegd[m+2]*alpha1);
				
							count2++;

						}
				  }   
				  
		  }  
		  printf("count1=%d,count2=%d\n",count1,count2);
	  }
	  else if((bppForegd==32)&&(bppBackgd==32))
	  {
		
		  for(int y=heightForegd-1; y>=0; y--)   
		  { 
				  for   (   int   x=0;   x<widthForegd;   x++   )   
				  {   
						m=(y*widthForegd+x)*4;
						n=((top+y)*widthBackgd+x+left);//n=((heightBackgd-top-heightForegd+y)*widthBackgd+x+left);
						/*if(bufForegd[m+3]>umax)
						{		umax=bufForegd[m+3];
								ux=x;uy=y;um=m;
						}*/
						if(bufForegd[m+3]==255)
						{
							if(findTransparentPixel(210,30,40,bufForegd[m],bufForegd[m+1],bufForegd[m+2])==0)
							{
								bufOut[n*3]=(int)((bufBackgd[n*4]*(1-Alpha)+(bufForegd[m])*Alpha));
								bufOut[n*3+1]=(int)((bufBackgd[n*4+1]*(1-Alpha)+(bufForegd[m+1]*Alpha)));
								bufOut[n*3+2]=(int)((bufBackgd[n*4+2]*(1-Alpha)+(bufForegd[m+2]*Alpha)));
								count1++;
							}
						}
						if(bufForegd[m+3]!=255)
						{	
							//printf("f4[%d][%d]=%d,",y,x,bufForegd[m+3]);
							alpha1=(float)bufForegd[m+3]/255.0;
							alpha2=1-alpha1;
							bufOut[n*3]=(int)(bufBackgd[n*4]*alpha2 + bufForegd[m]*alpha1);
							bufOut[n*3+1]=(int)(bufBackgd[n*4+1]*alpha2 + bufForegd[m+1]*alpha1);
							bufOut[n*3+2]=(int)(bufBackgd[n*4+2]*alpha2 + bufForegd[m+2]*alpha1);
							count2++;

						}
				  }   
				  
		  }  printf("alpha1=%.1f,alpha2=%0.1f,count1=%d,count2=%d\n",Alpha,1-Alpha,count1,count2);
		 printf("(32x32,heightBackgd=%d,widthBackgd=%d),(heightForegd=%d, widthForegd=%d),(left=%d,top=%d,)\n",heightBackgd,widthBackgd,heightForegd,widthForegd,left,top);
		printf("umax=%d,[%d,%d]=(%d,%d,%d,%d)\n",umax,ux,uy,bufForegd[um],bufForegd[um+1],bufForegd[um+2],bufForegd[um+3]);		 
	  }
			
	  if((bppForegd==24)&&(bppBackgd==24))
	  {
		  for   (   int y=heightForegd-1;   y>=0; y--  )   
		  {   
				  for   (   int x=0;   x<widthForegd;   x++  )   
				  {   
					  m=(y*widthForegd+x)*3;
					  n=((heightBackgd-top-heightForegd+y)*widthBackgd+x+left)*3;	

					  bufOut[n]=(unsigned char)((bufBackgd[n]*(255-Alpha)+bufForegd[m]*(Alpha))/255.0);
					  bufOut[n+1]=(unsigned char)((bufBackgd[n+1]*(255-Alpha)+bufForegd[m+1]*(Alpha))/255.0);
					  bufOut[n+2]=(unsigned char)((bufBackgd[n+2]*(255-Alpha)+bufForegd[m+2]*(Alpha))/255.0);
				  }   
		  }  
	  }
	  int nrow=heightBackgd;
	  int ncol=widthBackgd;
	  IMAGE_STRUCT *rgb=NULL;
	  imageBMP *bmp=NULL;
	   rgb=new IMAGE_STRUCT();
	   rgb->bytes_per_pixel=3;
	   rgb->x_size=ncol;
	   rgb->y_size=nrow;
		rgb->pixel=(PIXEL*)calloc(nrow*ncol,sizeof(PIXEL));
		PIXEL *ppix=rgb->pixel;
		for(int i=0;i<nrow*ncol;i++)
		{	
			(ppix+i)->red=bufOut[i*3];
			(ppix+i)->green=bufOut[i*3+1];
			(ppix+i)->blue=bufOut[i*3+2];
		}
	//	bmp->Write_BMP(rgb,"G:\\testStudy\\test5\\apple3-QinQinGLES2T������ĺ�-2011-5-19\\bin\\cathyCompound.bmp");
		
		if(rgb->pixel) 
		{
			free(rgb->pixel);
			rgb->pixel=NULL;
		}
		if(rgb)	
		{
			delete rgb;
			rgb=NULL;
		}

		  //   ��buttonͼƬ����Ļ�� 
       /* SetDIBitsToDevice(hDC,left,top,cx,cy,0,0,0,cy,lpvBufFairy, 
                (LPBITMAPINFO)m_hDIBFairy,DIB_RGB_COLORS); */
		
		if(bufBackgd)	
		{
				free(bufBackgd); bufBackgd=NULL;
		 }
		if(bufForegd)
		{
			free(bufForegd); bufForegd=NULL;
		}
          return   bufOut;   
    
  } 

/*
 * [IN]:int   left;bitblt orininal coordinate of foreground image on x coordinate of background image in x axis
 * [IN] int   top; bitblt orininal coordinate of foreground image on y coordinate of background image in y axis
 * [IN] TCHAR *fnameBackgd - input background image
 * [IN] TCHAR *fnameForegd - input foreground image
 * [IN] int Alpha
* [IN] int nXDest : bitblt orininal coordinate of compound  image on x coordinate of desination DC in x axis
* [IN] int nYDest : bitblt orininal coordinate of compound  image on y coordinate of desination DC in y axis
//* [IN] int Alpha
//* [IN] int Alpha
 */

void DisplayCompoundIMG( HDC hDC,int   left,int   top, TCHAR *fnameBackgd,TCHAR *fnameForegd,float Alpha,int nXDest,int nYDest)
{ 
	HBITMAP hBackgroundDIB;//hDIBitmap; 
	BYTE *pBackgrounData=NULL; //;lpBitmapBits; 
 //Ϊ��Ļ�����豸������

//	 //HDC hDC=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);

	BITMAPINFO *lpbmi=(BITMAPINFO*)malloc(sizeof(BITMAPINFO));
	lpbmi->bmiColors[0].rgbBlue = 0;
	lpbmi->bmiColors[0].rgbGreen = 0;
	lpbmi->bmiColors[0]. rgbRed= 0;
	lpbmi->bmiColors[0]. rgbReserved= 0;

	unsigned char *bufCompound= CompoundTGA(left, top, fnameBackgd,fnameForegd,&(lpbmi->bmiHeader),Alpha);
	printf("After CompoundTGA\n");
//	ZeroMemory(&DriectBmp,sizeof(BITMAPINFO)); 
	/*lpbmi->bmiHeader.biSize=sizeof(BITMAPINFOHEADER); 
	lpbmi->bmiHeader.biWidth = 410; 
	lpbmi->bmiHeader.biHeight = 65; 
	lpbmi->bmiHeader.biPlanes = 1; 
	lpbmi->bmiHeader.biBitCount = 24; */

	int ncol=lpbmi->bmiHeader.biWidth;
	int nrow=lpbmi->bmiHeader.biHeight;
	

  //Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	printf("ncol=%d,nrow=%d\n",ncol,nrow);
	HDC hMemDC = CreateCompatibleDC(hDC); 
	// ���� hDIBitmap, dcDirect, �� lpBitmapBits ֮��Ĺ�ϵ. 
	// ͨ�� DC ������ dcDirect ��д��ͼ������,  
	//�� lpBitmapBits ��ȡ���ڴ�����, �����ڴ� Copy ����, ֱ��д���Դ�. 
	hBackgroundDIB = CreateDIBSection(hMemDC, (BITMAPINFO *)lpbmi,  DIB_RGB_COLORS, (void **)&pBackgrounData, NULL, 0); 
	//if(NULL != hBackgroundDIB) 
	//{ 
	//	HGDIOBJ OldBitmap = SelectObject(dcDirect, hBackgroundDIB); 
	//	BitBlt(dcDirect,0,0,800,480,dcBuf,0,0,SRCCOPY); 
	//	//CopyMemory(gpbFrameAddress,lpBitmapBits,800 * 480 * 2); 
	//	SelectObject(dcDirect,OldBitmap); 
	//} 
	
	if(!hBackgroundDIB)
	{
		OutputDebugString(TEXT("SlipCalendarsRight CreateDIBSection false\r\n"));
		printf("Failed to CreateDIBSection\n");
		return;
	}
	printf("After CreateDIBSection\n ");
	int t1,t2;
	int pad=(((ncol+3)>>2)<<2)-ncol;
	printf("pad=%d\n",pad);
	for(int i=0;i<nrow;i++)
	{	
		t1=i*(ncol*3+pad);
		t2=(nrow - 1 - i)*(ncol*3);
		for(int j=0;j<ncol;j++)
		{
			pBackgrounData[t1+3 * j]=bufCompound[t2+3 * j+2];
			pBackgrounData[t1+3* j+1]=bufCompound[t2+3 * j+1];	
			pBackgrounData[t1+3* j+2]=bufCompound[t2+3 * j];
	
			
		}
	}

	printf("After copy memory\n");
	if(bufCompound) 
	{
		free(bufCompound); 
		bufCompound=NULL;
	}
	///file.ReadHuge(lpBits,dwBitlen);   
	SelectObject(hMemDC,hBackgroundDIB);
	//pDC-> StretchBlt(0,0,bmp.bmWidth,bmp.bmHeight,&dcMem,0,0, bmp.bmWidth,bmp.bmHeight,SRCCOPY); 
	//RECT rc;
	/*GetClientRect(GetDlgItem(hwnd,IDC_BITMAPFRAME),&rc);
	InflateRect(&rc,-3,-3);*/
	
	
	//ԭ����ͼ
	// BitBlt(hDC,rc.left,rc.top,rc.right,rc.bottom,hMemDC,0,0  ,SRCCOPY);
	
	//������ͼ 
	//StretchBlt(hdc,rc.left ,rc.top,rc.right,rc.bottom,hdcMEM,0,0,rc.right,rc.bottom,SRCCOPY);
	printf("Before Bitblt\n");
	BitBlt(hDC, nXDest,nYDest,ncol,nrow,hMemDC,0,0  ,SRCCOPY);
	printf("After Bitblt,nXDest=%d,nYDest=%d\n",nXDest,nYDest);
/*void * hBackgroundDIBFile=CreateFile(L"\\Windows\\bmp2.bmp",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	int iBackgroundFileSize=GetFileSize(hBackgroundDIBFile,NULL);
	ReadFile(hBackgroundDIBFile,pBackgrounData,iBackgroundFileSize,NULL,NULL);
	SetDIBitsToDevice(hDC,0,0,240,268,0,0,0,268,pBackgrounData,lpbmi,DIB_RGB_COLORS);*/


	
	DeleteObject(hBackgroundDIB);
	DeleteDC(hMemDC);
	if(lpbmi)
	{		free(lpbmi);
			lpbmi=NULL;
	}
}


//��ConpoundTGA����Ϊ24λ��ͼƬ
int Save24BMP_CompoundTGA(char *fnameOutBMP, int   left,int   top, TCHAR *fnameBackgdTGA,TCHAR *fnameForegdTGA,int Alpha )
{
    BITMAPINFO *lpbmi=(BITMAPINFO*)malloc(sizeof(BITMAPINFO));
    unsigned char *bufCompound= CompoundTGA(left, top, fnameBackgdTGA,fnameForegdTGA,&(lpbmi->bmiHeader),Alpha);
	if(bufCompound==NULL)
	{
		printf("Compound TGA failed \n");
		return -1;
	}
	int ncol=lpbmi->bmiHeader.biWidth;
	int nrow=lpbmi->bmiHeader.biHeight;
	if(lpbmi)
	{
		free(lpbmi);
		lpbmi=NULL;
	}
	IMAGE_STRUCT *bmp=NULL;
	imageBMP *imgBMP=new imageBMP(nrow,ncol);
	bmp=imgBMP->image_struct;
	bmp->bytes_per_pixel=3;
	PIXEL *ptr=bmp->pixel;
    int numOfPixel;
	numOfPixel=bmp->y_size*bmp->x_size;
	int t;
   for(int i=0;i<nrow;i++)
   {	for(int j=0;j<ncol;j++)
		{	
			t=i*ncol+j;
			ptr->red=bufCompound[t*3];
			ptr->green=bufCompound[t*3+1];
			ptr->blue=bufCompound[t*3+2];
			ptr++;
		}
   }
   if(bufCompound!=NULL)
   {
		free(bufCompound);
		bufCompound=NULL;
   
   }
	
	imgBMP->Write_BMP(bmp,fnameOutBMP);
	if(imgBMP)
	{	
		delete imgBMP;
		imgBMP=NULL;
	}	
    
 return 1;
}

int _TGA2BMP(char *OutBMP, char *InTGA)
{
	int ret;
   unsigned char *bufTGA;   
	   
	int widthTGA, heightTGA, bppTGA;
 

    ret = GDI_TGALoad( InTGA, &bufTGA, &widthTGA, &heightTGA, &bppTGA);
	//ret= LoadTGA1(const char *fname, RGBA_PIXEL  **pixels, int *ncol, int *nrow, int *Depth)
	TCHAR ErrorMsg[MAX_PATH];
    if ( ! ret )
	{   
   		printf("CompoundTGA:: Failed to load  %s  TGA image file !\n", InTGA);
		_tsprintf_s(ErrorMsg,MAX_PATH,__TEXT("Failed to load  %s   TGA image file !\n"), InTGA);
		OutputDebugString(ErrorMsg); 
	    OutputDebugString(_T("\r\n"));
	//	OutputDebugString(TEXT("SlipCalendarsRight CreateDIBSection false\r\n"));
      return -1;
   }
	IMAGE_STRUCT *bmp=NULL;

	imageBMP *imgBMP=new imageBMP(heightTGA,widthTGA);
	bmp=imgBMP->image_struct;
	bmp->bytes_per_pixel=3;
	PIXEL *ptr=bmp->pixel;
	int numOfPixel=heightTGA*widthTGA;
	for(int i=0;i<numOfPixel;i++)
	{
			ptr->red=(bufTGA[i*3]); 
			ptr->green=(bufTGA[i*3+1]); 
			ptr->blue=(bufTGA[i*3+2]); 
			//printf("(r=%d,c=%d),",ptr->red,_CHAR2INT(bufTGA[i*3]));
			ptr++;
	}
	unsigned char t=255,t1=3;
	printf("CHAR256=%d,char3=%d,CHAR256=%d,char3=%d \n",(int)t,(int)t1,t,t1);
	if(bufTGA!=NULL)
   {
		free(bufTGA);
		bufTGA=NULL;
   
   }
	imgBMP->Write_BMP(bmp,OutBMP);
	if(imgBMP)
	{	delete imgBMP;
		imgBMP=NULL;
	}	
	return 1;
}


int _TGA2BMP1(char *OutBMP, char *InTGA)
{
	
   
	   
	int widthTGA, heightTGA, bppTGA;
	RGBA_PIXEL  *rgba;

   // ret = GDI_TGALoad( InTGA, &bufTGA, &widthTGA, &heigcolshtTGA, &bppTGA);
    OutputDebugString(TEXT("Before Load TAG image success\r\n"));
	rgba= LoadTGA1(InTGA, &widthTGA, &heightTGA,  &bppTGA);
	TCHAR ErrorMsg[MAX_PATH];
    if ( rgba==NULL )
	{   
   		printf("CompoundTGA:: Failed to load  %s  TGA image file !\n", InTGA);
		_tsprintf_s(ErrorMsg,MAX_PATH,__TEXT("Failed to load  %s   TGA image file !\n"), InTGA);
		OutputDebugString(ErrorMsg); 
	    OutputDebugString(_T("\r\n"));
	//	OutputDebugString(TEXT("SlipCalendarsRight CreateDIBSection false\r\n"));
		return 0;
   }
	OutputDebugString(TEXT("Load TAG image success\r\n"));
	IMAGE_STRUCT *bmp=NULL;

	imageBMP *imgBMP=new imageBMP(heightTGA,widthTGA);
	bmp=imgBMP->image_struct;
	bmp->bytes_per_pixel=3;
	PIXEL *ptr=bmp->pixel;
	int numOfPixel=heightTGA*widthTGA;
	for(int i=0;i<numOfPixel;i++)
	{
			ptr->red=rgba[i].r; //(bufTGA[i*3]); 
			ptr->green=rgba[i].g; //(bufTGA[i*3+1]); 
			ptr->blue=rgba[i].b; //bufTGA[i*3+2]); 
			//printf("(r=%d,c=%d),",ptr->red,_CHAR2INT(bufTGA[i*3]));
			ptr++;
	}
	unsigned char t=255,t1=3;
	printf("CHAR256=%d,char3=%d,CHAR256=%d,char3=%d \n",(int)t,(int)t1,t,t1);
	if(rgba!=NULL)
   {
		free(rgba);
		rgba=NULL;
   
   }
	imgBMP->Write_BMP(bmp,OutBMP);
	if(imgBMP)
	{	delete imgBMP;
		imgBMP=NULL;
	}	
	return 1;
}


int _TGA2TGA1(char *OutTGA, char *InTGA)
{
	int widthTGA, heightTGA, bppTGA;
	RGBA_PIXEL  *rgba;
	
   // ret = GDI_TGALoad( InTGA, &bufTGA, &widthTGA, &heigcolshtTGA, &bppTGA);
  OutputDebugString(TEXT("Before Load TAG image success\r\n"));
	rgba= LoadTGA1(InTGA, &widthTGA, &heightTGA,  &bppTGA);
	TCHAR ErrorMsg[MAX_PATH];
    if ( rgba==NULL )
	{   
   		printf("CompoundTGA:: Failed to load  %s  TGA image file !\n", InTGA);
		_tsprintf_s(ErrorMsg,MAX_PATH,__TEXT("Failed to load  %s   TGA image file !\n"), InTGA);
		OutputDebugString(ErrorMsg); 
	    OutputDebugString(_T("\r\n"));
	//	OutputDebugString(TEXT("SlipCalendarsRight CreateDIBSection false\r\n"));
		return 0;
   }
	writeTGA32(OutTGA,rgba,heightTGA,widthTGA);

if(rgba!=NULL)
   {
		free(rgba);
		rgba=NULL;
   
   }
return 1;
}

//TGAwrite (const char *Filename, unsigned char **Buffer, int *Width, int *Height, int *Depth);


int _TGA2TGA2(TCHAR *OutTGA, TCHAR *InTGA)
{
	int ret;
   unsigned char *bufTGA;   
	   
	int widthTGA, heightTGA, bppTGA;
 
//int TGALoad(const char *Filename, unsigned char **Buffer, int *Width, int *Height, int *Depth)
    ret =TGALoad( InTGA, &bufTGA, &widthTGA, &heightTGA, &bppTGA);
	//ret= LoadTGA1(const char *fname, RGBA_PIXEL  **pixels, int *ncol, int *nrow, int *Depth)
	TCHAR ErrorMsg[MAX_PATH];
    if ( ! ret )
	{   
   		printf("CompoundTGA:: Failed to load  %s  TGA image file !\n", InTGA);
		_tsprintf_s(ErrorMsg,MAX_PATH,__TEXT("Failed to load  %s   TGA image file !\n"), InTGA);
		OutputDebugString(ErrorMsg); 
	    OutputDebugString(_T("\r\n"));
	//	OutputDebugString(TEXT("SlipCalendarsRight CreateDIBSection false\r\n"));
      return -1;
   }
	printf("bppTGA=%d\n",bppTGA);
	TGAwrite (OutTGA, bufTGA, widthTGA, heightTGA, bppTGA);
	
	if(bufTGA!=NULL)
   {
		free(bufTGA);
		bufTGA=NULL;
   
   }
	
	return 1;

}




#define WIDTHBYTES(w,b) ((((w)*(b)+31)/32)*4)



BOOL OnScreenSave( WCHAR* lpszFileName, HDC saveDC, RECT rc ) 
{ 
    BITMAPFILEHEADER bf = { 0 }; 
    BITMAPINFOHEADER bi = { 0 }; 
    HDC hMemDC; 
    HBITMAP hBitmap; 
    LPBYTE lpBits; 

    bi.biSize = sizeof( BITMAPINFOHEADER ); 
    bi.biBitCount = 24; 
    bi.biWidth = rc.right - rc.left; 
    bi.biHeight = rc.bottom - rc.top; 
    bi.biPlanes = 1; 
    bi.biCompression = BI_RGB; 
    bi.biSizeImage = WIDTHBYTES( bi.biWidth, bi.biBitCount ) * bi.biHeight; 

    HDC hdc = GetDC( NULL ); 
    if( !hdc ) 
        return FALSE; 
    hBitmap = CreateDIBSection( hdc, ( CONST BITMAPINFO * )&bi, DIB_RGB_COLORS, 
                        ( VOID ** )&lpBits, NULL, 0 ); 
    if( !hBitmap ){ 
        ReleaseDC( NULL, hdc ); 
        return FALSE; 
    } 

    hMemDC = CreateCompatibleDC( hdc ); 
    HBITMAP hOldBmp = ( HBITMAP )SelectObject( hMemDC, hBitmap ); 
    BitBlt( hMemDC, 0, 0, bi.biWidth, bi.biHeight, saveDC, 0, 0, SRCCOPY ); 
    SelectObject( hMemDC, hOldBmp ); 
    ReleaseDC( NULL, hdc ); 
    DeleteDC( hMemDC ); 

    bf.bfType = 0x4d42; 
    bf.bfSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) 
                + bi.biSizeImage; 
    bf.bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ); 

    DWORD dwWrite; 
    HANDLE fh = CreateFile( lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
                        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL ); 
    WriteFile( fh, ( LPBYTE )&bf, sizeof( BITMAPFILEHEADER ), &dwWrite, NULL ); 
    WriteFile( fh, ( LPBYTE )&bi, sizeof( BITMAPINFOHEADER ), &dwWrite, NULL ); 
    WriteFile( fh, ( LPBYTE )lpBits, bi.biSizeImage, &dwWrite, NULL ); 


    DeleteObject( hBitmap ); 
    return TRUE; 
}


 void QGetActiveWindow(TCHAR *filename,HWND hWnd)
{ 
	DWORD dwRest = GetTickCount();
	HDC hScrDC, hMemDC;         
	int width, height;
	//����ָ��DIB��������ָ��
	BYTE *lpBitmapBits = NULL;
	//������ĻDC
	//hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	hScrDC = GetDC(hWnd);
	RECT rc;
	//GetWindowRect(hWnd,&rc);
	GetClientRect(hWnd,&rc);
	//��ȡ��Ļ����
	width =rc.right - rc.left ;   //GetDeviceCaps(hScrDC, HORZRES);
	height =rc.bottom - rc.top ;   //GetDeviceCaps(hScrDC, VERTRES);
	//UartTrans.UART
	//printf("��Ļwidth = %d, height = %d. \n",width,height);

	//�����ڴ�DC
	hMemDC = CreateCompatibleDC(hScrDC);
	//��ʼ��λͼ�ṹ������Ϣ
	BITMAPINFO bitmapInfo; 
	ZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = width ;
	bitmapInfo.bmiHeader.biHeight = height ;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 24;//32;   //����Ϊ32λɫ���λͼ
	//����λͼ
	HBITMAP directBmp = CreateDIBSection(hMemDC, (BITMAPINFO*)&bitmapInfo, 
	DIB_RGB_COLORS, (void **)&lpBitmapBits, NULL, 0);
	HGDIOBJ previousObject = SelectObject(hMemDC, directBmp);
	//����ԴDC���ڴ�DC��
	BitBlt(hMemDC,0 , 0 , width ,height , hScrDC, 0 , 0 , SRCCOPY);
	//��ʼ��λͼ�ļ�ͷ�ṹ
	BITMAPFILEHEADER bmBITMAPFILEHEADER;
	ZeroMemory(&bmBITMAPFILEHEADER, sizeof(BITMAPFILEHEADER));
	bmBITMAPFILEHEADER.bfType = 0x4d42; //bmp 
	bmBITMAPFILEHEADER.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmBITMAPFILEHEADER.bfSize = bmBITMAPFILEHEADER.bfOffBits + ((width*height)*3); ///
	//׼��д�ļ�
	FILE *mStream = NULL;
	if((mStream = _wfopen(filename, L"wb")))
	{ 
		//дλͼͷ
		fwrite(&bmBITMAPFILEHEADER, sizeof(BITMAPFILEHEADER), 1, mStream);
		//дλͼ������Ϣ
		fwrite(&(bitmapInfo.bmiHeader), sizeof(BITMAPINFOHEADER), 1, mStream);
		//дRGB����
		fwrite(lpBitmapBits, 3*width*height, 1, mStream);
		fclose(mStream);
	}
	//��Դ�ͷ�
	SelectObject(hMemDC,previousObject);
	DeleteObject(directBmp);
	DeleteObject(hMemDC);
	DeleteObject(hScrDC);
}

/**
 *Function :ץȡ���ڱ���ΪͼƬ
 *1st Param [OUT]: filename��ͼƬ·��
 *2nd Param [IN]: hScrDC������DC
 *3rd Param[IN]: nXSrc - Grasp Ҫ����ľ������� x����ʼ����
 *4th Param [IN]: nYSrc - Grasp Ҫ����ľ������� y����ʼ����
 *5th Param [IN]: nWidth - Grasp Ҫ����ľ���������
 *6th Param [IN]: nHeight - Grasp Ҫ����ľ�������߶�
 *7th Param [OUT]:����ָ��DIB��������ָ��
 return : ����DIBλͼ���
 **/
//Grasp Ҫ����ľ������� from nXSrc,int nYSrc,size : int nWidth, int nHeight
 HBITMAP  GraspRectWindow(TCHAR *filename, HDC hScrDC,int nXSrc,int nYSrc,int nWidth, int nHeight,BYTE **lpBitmapBits)  //void GraspRectWindow(TCHAR *filename,HWND hWnd,int nXSrc,int nYSrc,int nWidth, int nHeight)
{ 
		HDC hMemDC;   

		//����ָ��DIB��������ָ��
		//BYTE *lpBitmapBits = NULL;   //RGB������Ϣ

		//RECT rc;

		//��ô��ڿͻ������δ�С
		//GetClientRect(hWnd,&rc);


		//���������ڴ�DC
		hMemDC = CreateCompatibleDC(hScrDC);

		//��ʼ��λͼ�ṹ������Ϣ
		BITMAPINFO bitmapInfo; 
		ZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));

		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biWidth = nWidth ;
		bitmapInfo.bmiHeader.biHeight = nHeight ;
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 24;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;

		//����λͼ
		HBITMAP directBmp = CreateDIBSection(hMemDC, (BITMAPINFO*)&bitmapInfo, 
		DIB_RGB_COLORS, (void **)lpBitmapBits, NULL, 0);                         //������ָ���λͼ��� ��û��ͼƬ����
		//ѡ��λͼ���豸
		HGDIOBJ previousObject = SelectObject(hMemDC, directBmp);

		//���ƴ����ڴ�DC�����ڻ���DC
		BitBlt(hMemDC,0 , 0 , nWidth ,nHeight , hScrDC, nXSrc , nYSrc , SRCCOPY);           //����Ļ�Ķ������Ƶ��ڴ�DC�������ų���ͼƬ

		if(filename!=NULL)
		{
			//��ʼ��λͼ�ļ�ͷ�ṹ

			BITMAPFILEHEADER  bmBITMAPFILEHEADER;//������һ���ļ�ͷ
			ZeroMemory(&bmBITMAPFILEHEADER, sizeof(BITMAPFILEHEADER));//��ʼ���ļ�ͷ

			bmBITMAPFILEHEADER.bfType = 0x4d42; //
			bmBITMAPFILEHEADER.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
			bmBITMAPFILEHEADER.bfSize = bmBITMAPFILEHEADER.bfOffBits + ((nWidth*nHeight)*3); ///

			if(filename!=NULL)
			{
				//׼��д�ļ�
				FILE *mStream = NULL;

				if((mStream = _wfopen(filename, L"wb"))) //�����ļ�
				{ 
					//дλͼ�ļ�ͷ
					fwrite(&bmBITMAPFILEHEADER, sizeof(BITMAPFILEHEADER), 1, mStream);
					//дλͼͷ��Ϣ
					fwrite(&(bitmapInfo.bmiHeader), sizeof(BITMAPINFOHEADER), 1, mStream);
					//дλͼRGB����
					fwrite(*lpBitmapBits, WIDTHBYTES( nWidth, bitmapInfo.bmiHeader.biBitCount ) *nHeight, 1, mStream);
					fclose(mStream);
				}
			}
			else
			{
				printf("Not Required OUTPUT !\n");
			}
		}

		
		//��Դ�ͷ�
		SelectObject(hMemDC,previousObject);
		//DeleteObject(directBmp);
		DeleteObject(hMemDC);
	
		return directBmp;
}


/**
 * Copy Foreground picture to screen under consideration of background image
 */
void CopyImgToScreen(HDC hDC,TCHAR *fnameForegd, int left,int top, float Alpha,TCHAR *FilePath)
{    

   unsigned char *bufBackgd=NULL,  /*Store background image pixel*/
	    *bufForegd=NULL,
	    *bufOut=NULL;  /*output image pixel buffer*/   
  // int bppForegd;	   
	int widthBackgd, heightBackgd, bppBackgd;
	int widthForegd, heightForegd, bppForegd; 

 
	int	retForegd = TGALoad( fnameForegd, &bufForegd, &widthForegd, &heightForegd, &bppForegd);
	TCHAR ErrorMsg[MAX_PATH];
	if ( ! retForegd )
	{
   		printf("CompoundTGA:: Failed to load  %s  TGA image file !\n", fnameForegd);
		_tsprintf_s(ErrorMsg,MAX_PATH,__TEXT("Failed to load  %s   TGA image file !\n"), fnameForegd);
		OutputDebugString(ErrorMsg); 
		OutputDebugString(_T("\r\n"));
		exit(-1);	
	  
	}
	TCHAR tmpPath[MAX_PATH];
	_tcscpy_s(tmpPath,MAX_PATH,FilePath);
	_tcscat_s(tmpPath,MAX_PATH,L"bin\\");	
	_tcscat_s(tmpPath,MAX_PATH,L"cathyGraspRectWindow.bmp");
   
	//HBITMAP hCompoundDIB=GraspRectWindow(L"G:\\testStudy\\test5\\apple3-QinQinGLES2T������ĺ�-2011-5-19\\bin\\cathyGraspRectWindow.bmp",hDC,left, top,widthForegd, heightForegd,&bufBackgd);
 HBITMAP hCompoundDIB=GraspRectWindow(tmpPath,hDC,left, top,widthForegd, heightForegd,&bufBackgd);

	  int m,n,k;
	
	  float alpha1=0;
	  float alpha2=0;
	

	widthBackgd=widthForegd; 
	heightBackgd=heightForegd;
	widthBackgd=((widthBackgd+3)>>2)<<2;
	bufOut= (unsigned char *) malloc(3 * (widthBackgd) * (heightBackgd));
	if(bppForegd==32)
	{	
		  for(int y=heightForegd-1; y>=0; y--)   
		  {   
				  for   (   int   x=0;   x<20;   x++   )   
				  {   
						m=(y*widthForegd+x)*4;
						n=((y)*widthBackgd+x)*3;
						k= ((heightForegd-1-y)*widthBackgd+x)*3;		
    					alpha1=(float)bufForegd[m+3]/255.0;
						alpha2=1-alpha1;
						bufOut[k]=(int)(bufBackgd[n]*alpha2 + bufForegd[m]*alpha1);
						bufOut[k+1]=(int)(bufBackgd[n+1]*alpha2 + bufForegd[m+1]*alpha1);
						bufOut[k+2]=(int)(bufBackgd[n+2]*alpha2 + bufForegd[m+2]*alpha1);
						
				  }   
				for   (   int   x=20;   x<418;   x++   )   
				  {   
						m=(y*widthForegd+x)*4;
						n=((y)*widthBackgd+x)*3;
						k= ((heightForegd-1-y)*widthBackgd+x)*3;		
    					alpha1=(float)bufForegd[m+3]/255.0;
						alpha2=1-alpha1;
						bufBackgd[n]=220;
						bufBackgd[n+1]=220;
						bufBackgd[n+2]=220;
						bufOut[k]=(int)(bufBackgd[n]*alpha2 + bufForegd[m]*alpha1);
						bufOut[k+1]=(int)(bufBackgd[n+1]*alpha2 + bufForegd[m+1]*alpha1);
						bufOut[k+2]=(int)(bufBackgd[n+2]*alpha2 + bufForegd[m+2]*alpha1);
						
				  }   
				 for   (   int   x=418;   x<widthForegd;   x++   )   
				  {   
						m=(y*widthForegd+x)*4;
						n=((y)*widthBackgd+x)*3;
						k= ((heightForegd-1-y)*widthBackgd+x)*3;		
    					alpha1=(float)bufForegd[m+3]/255.0;
						alpha2=1-alpha1;
						bufOut[k]=(int)(bufBackgd[n]*alpha2 + bufForegd[m]*alpha1);
						bufOut[k+1]=(int)(bufBackgd[n+1]*alpha2 + bufForegd[m+1]*alpha1);
						bufOut[k+2]=(int)(bufBackgd[n+2]*alpha2 + bufForegd[m+2]*alpha1);
						
				  }   
		  } 
	}
	//printf("(32x32,heightBackgd=%d,widthBackgd=%d),(heightForegd=%d, widthForegd=%d),(left=%d,top=%d,)\n",heightBackgd,widthBackgd,heightForegd,widthForegd,left,top);
	else if(bppForegd==24)
	{
		  for   (   int   y=heightForegd-1;   y>=0   ;y--   )   
		  {   
				  for   (   int   x=0;   x<widthForegd;   x++   )   
				  {   
					  m=(y*widthForegd+x)*3;
					  n=((heightBackgd-heightForegd+y)*widthBackgd+x)*3;	
					  k= ((heightForegd-1-y)*widthBackgd+x)*3;
					  bufOut[k]=(bufBackgd[n]*(1-Alpha)+bufForegd[m]*Alpha);
					  bufOut[k+1]=(bufBackgd[n+1]*(1-Alpha)+bufForegd[m+1]*Alpha);
					  bufOut[k+2]=(bufBackgd[n+2]*(1-Alpha)+bufForegd[m+2]*Alpha);
				  }   
		  }  
	}
	else
	{
		_tsprintf_s(ErrorMsg,MAX_PATH,__TEXT("Not 24 bits or 32 bits Image Format suppport !!!\n"), fnameForegd);
		OutputDebugString(ErrorMsg); 
		OutputDebugString(_T("\r\n"));
		exit(-1);	
	}
	int nrow=heightBackgd;
	int ncol=widthBackgd;
	IMAGE_STRUCT *rgb=NULL;
	imageBMP *bmp=NULL;
	rgb=new IMAGE_STRUCT();
	rgb->bytes_per_pixel=3;
	rgb->x_size=ncol;
	rgb->y_size=nrow;
	rgb->pixel=(PIXEL*)calloc(nrow*ncol,sizeof(PIXEL));
	PIXEL *ppix=rgb->pixel;
	for(int i=0;i<nrow*ncol;i++)
	{	
		(ppix+i)->red=bufOut[i*3+2];
		(ppix+i)->green=bufOut[i*3+1];
		(ppix+i)->blue=bufOut[i*3];
	}
	//bmp->Write_BMP(rgb,"G:\\testStudy\\test5\\apple3-QinQinGLES2T������ĺ�-2011-5-19\\bin\\cathyCompound2.bmp");

	if(rgb->pixel) {free(rgb->pixel);rgb->pixel=NULL;}
	if(rgb)	{delete rgb; rgb=NULL;}

	if(bufForegd)
	{
		free(bufForegd); bufForegd=NULL;
	}

	
	int t1,t2;
	int pad=(((ncol+3)>>2)<<2)-ncol;
	printf("pad=%d\n",pad);
	for(int i=0;i<nrow;i++)
	{	
		t1=i*(ncol*3+pad);
		t2=(nrow - 1 - i)*(ncol*3);
		for(int j=0;j<ncol;j++)
		{
			bufBackgd[t1+3 * j]=bufOut[t2+3 * j];
			bufBackgd[t1+3* j+1]=bufOut[t2+3 * j+1];	
			bufBackgd[t1+3* j+2]=bufOut[t2+3 * j+2];

			
		}
	}
	
	if(bufOut) {free(bufOut); bufOut=NULL;}
	
	HDC hMemDC = CreateCompatibleDC(hDC); 
	SelectObject(hMemDC,hCompoundDIB);
	
	BitBlt(hDC, left,top,ncol,nrow,hMemDC,0,0  ,SRCCOPY);
	//printf("After Bitblt,nXDest=%d,nYDest=%d\n",left,top);       
	    
  } 






/****************************************************************/
/* Function :���ݴ��ڵ�DesDC
/* 1st Param [OUT]: DesDC��Ŀ��DC
/* 2nd Param [IN]: hScrDC������DC
/* 3rd Param[IN]: nXSrc - Grasp Ҫ����ľ������� x����ʼ����
/* 4th Param [IN]: nYSrc - Grasp Ҫ����ľ������� y����ʼ����
/* 5th Param [IN]: nWidth - Grasp Ҫ����ľ���������
/* 6th Param [IN]: nHeight - Grasp Ҫ����ľ�������߶�
/* 7th Param [OUT]:����ָ��DIB��������ָ��
/* return : ����DIBλͼ���
 ******************************************************************/
//Grasp Ҫ����ľ������� from nXSrc,int nYSrc,size : int nWidth, int nHeight
 HBITMAP  BackUpScreen(HDC DesDC,HDC hScrDC,int nXSrc,int nYSrc,int nWidth, int nHeight,BYTE **lpBitmapBits)  
{ 
	
		//��ʼ��λͼ�ṹ������Ϣ
		BITMAPINFO bitmapInfo; 
		ZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));

		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biWidth = nWidth ;
		bitmapInfo.bmiHeader.biHeight = nHeight ;
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 24;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;

		//����λͼ
		                
		HBITMAP directBmp = CreateDIBSection(DesDC, (BITMAPINFO*)&bitmapInfo, 
			DIB_RGB_COLORS, (void **)lpBitmapBits, NULL, 0); 
		//ѡ��λͼ���豸
	
		HGDIOBJ previousObject = SelectObject(DesDC, directBmp);

		//���ƴ����ڴ�DC�����ڻ���DC
	  	BitBlt(DesDC,0 , 0 , nWidth ,nHeight , hScrDC, nXSrc , nYSrc , SRCCOPY);   
		
		
		////��ʼ��λͼ�ļ�ͷ�ṹ

		//BITMAPFILEHEADER  bmBITMAPFILEHEADER;//������һ���ļ�ͷ
		//ZeroMemory(&bmBITMAPFILEHEADER, sizeof(BITMAPFILEHEADER));//��ʼ���ļ�ͷ

		//bmBITMAPFILEHEADER.bfType = 0x4d42; //
		//bmBITMAPFILEHEADER.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		//bmBITMAPFILEHEADER.bfSize = bmBITMAPFILEHEADER.bfOffBits + ((nWidth*nHeight)*3); 

		//
		//	//׼��д�ļ�
		//FILE *mStream = NULL;

		//if((mStream = _wfopen(L"G:\\testStudy\\test5\\apple3-QinQinGLES2T������ĺ�-2011-5-19\\bin\\cathy3.bmp", L"wb"))) //�����ļ�
		//{ 
		//	//дλͼ�ļ�ͷ
		//	fwrite(&bmBITMAPFILEHEADER, sizeof(BITMAPFILEHEADER), 1, mStream);
		//	//дλͼͷ��Ϣ
		//	fwrite(&(bitmapInfo.bmiHeader), sizeof(BITMAPINFOHEADER), 1, mStream);
		//	//дλͼRGB����
		//	fwrite(*lpBitmapBits, WIDTHBYTES( nWidth, bitmapInfo.bmiHeader.biBitCount ) *nHeight, 1, mStream);
		//	fclose(mStream);
		//}
			
		return directBmp;
}
