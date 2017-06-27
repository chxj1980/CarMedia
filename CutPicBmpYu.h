#pragma once

class CutPicBmpYu
{
public:
	CutPicBmpYu(void);
	~CutPicBmpYu(void);
private:
	unsigned char *pBuf;    //read
	unsigned char *pNewBuf; //cut
	unsigned char *rotateBmpBuf;//����ת���λͼ�ռ�ָ��
	unsigned char *rotateBmpBuf1;

	int bmpWidth;
	int bmpHeight;
	int biBitCount;
	int linebyte;
	int newlinebyte;
	BITMAPFILEHEADER Sbfh;
	BITMAPINFOHEADER Sbi;
	BITMAPINFOHEADER bh;
public:
	BOOL ReadBmp(TCHAR *pFileIn);
	BOOL SaveBmp(TCHAR *pFileOut,long Width,long Height);
	BOOL CutBmp(TCHAR *pFileIn,TCHAR *pFileOut,long Width,long Height,BYTE **buffer);
};
