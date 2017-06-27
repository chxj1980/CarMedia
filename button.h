#ifndef _BUTTON_H_
#define _BUTTON_H_
 
HBITMAP CopyScreenToBitmap(int &nWidth,int &nHeight);
int Save16BitmapToFile(HBITMAP hBitmap,LPCTSTR lpFileName);//���������ñ���Ϊ16λ��ͼƬ
int Save24BitmapToFile(HBITMAP hBitmap,LPCTSTR lpFileName);//���������ñ���Ϊ24λ��ͼƬ
int Save32BitmapToFile(HBITMAP hBitmap,LPCTSTR lpFileName);//���������ñ���Ϊ32λ����ɫ���ͼƬ
//void copyToScreen( );
//void copyToScreen( HWND hwnd,int   left,int   top, const char *fnameBackgd,const char *fnameForegd,int Alpha);
//void DisplayCompoundIMG( HWND hwnd,int   left,int   top, TCHAR *fnameBackgd,TCHAR *fnameForegd,float Alpha,int nXDest,int nYDest);
void DisplayCompoundIMG( HDC hDC,int   left,int   top, TCHAR *fnameBackgd,TCHAR *fnameForegd,float Alpha,int nXDest,int nYDest);

//��ConpoundTGA����Ϊ24λ��ͼƬ
int Save24BMP_CompoundTGA(char *fnameOutBMP, int   left,int   top, char *fnameBackgdTGA,char *fnameForegdTGA,float Alpha );
int _TGA2BMP(char *OutBMP, char *InTGA);
int _TGA2BMP1(char *OutBMP, char *InTGA);
int _TGA2TGA1(char *OutTGA, char *InTGA);
int _TGA2TGA2(char *OutTGA, char *InTGA);

//BOOL QGetActiveWindow( WCHAR* lpszFileName, HDC saveDC, RECT rc ) ;
void QGetActiveWindow(TCHAR  *filename,HWND hWnd);

//void GraspRectWindow(TCHAR *filename,HWND hWnd,int nXSrc,int nYSrc,int nWidth, int nHeight);
 
HBITMAP GraspRectWindow(TCHAR *filename,HDC hScrDC,int nXSrc,int nYSrc,int nWidth, int nHeight,BYTE **lpBitmapBits);
void CopyImgToScreen(HDC hDC,TCHAR *fnameForegd, int left,int top, float Alpha,TCHAR *FilePath);
HBITMAP  BackUpScreen(HDC DesDC,HDC hScrDC,int nXSrc,int nYSrc,int nWidth, int nHeight,BYTE **lpBitmapBits);

#endif // _BUTTON_H_