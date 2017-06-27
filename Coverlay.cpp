#include "Coverlay.h"

DDPIXELFORMAT ddpfOverlayFormats =
{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xF800, 0x07e0, 0x001F, 0};   //51
Coverlay::Coverlay()
{
  lpDD = NULL;
  lpPrimary = NULL;
  lpOverlay = NULL;
}

Coverlay::~Coverlay()
{
	if(lpPrimary)
	{
		lpPrimary->Release();
		lpPrimary = NULL;
	}
	if(lpOverlay)
	{
		lpOverlay->Release();
		lpOverlay = NULL;
	}
  if(lpDD)
  {
	  lpDD->Release();
	  lpDD = NULL;
  }
}

BOOL Coverlay::CreateDdraw(HWND hWnd,int width,int height,TCHAR *bmpPath)
{
	BITMAP bmp;
	HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(bmpPath);
	GetObject(hbmp,sizeof(BITMAP),&bmp);

	HRESULT hr = S_FALSE;
	hr = DirectDrawCreate(NULL,&lpDD,NULL);
	if(FAILED(hr))
		return FALSE;
	hr = lpDD->SetCooperativeLevel(hWnd,DDSCL_NORMAL);  //����ģʽ
	if(FAILED(hr))
		return FALSE;
	overHeight = bmp.bmHeight;
	overWidth = bmp.bmWidth;

	ZeroMemory(&ddcaps,sizeof(ddcaps));
	ddcaps.dwSize = sizeof(ddcaps);
	hr = lpDD->GetCaps(&ddcaps,NULL);
	if(FAILED(hr))
		return FALSE;
    
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	hr = lpDD->CreateSurface(&ddsd,&lpPrimary,NULL);
	if(FAILED(hr))
		return FALSE;

	if (ddcaps.dwAlignSizeSrc != 0)
		overWidth += overWidth % (WORD)ddcaps.dwAlignSizeSrc;

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT|DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY|DDSCAPS_VIDEOMEMORY;
	ddsd.dwWidth = overWidth;//width;
	ddsd.dwHeight = overHeight;//height;
    ddsd.ddpfPixelFormat = ddpfOverlayFormats;

    rSrc->top = 0;
	rSrc->left = 0;
	rSrc->right = overWidth;
	rSrc->bottom = overHeight;

    hr = lpDD->CreateSurface(&ddsd,&lpOverlay,NULL);
	if(FAILED(hr))
		return FALSE;
    //����ͼƬ
	HDC hdc = NULL;
	hr = lpOverlay->GetDC(&hdc);
	if(FAILED(hr))
		return FALSE;
	HDC hdcmem = CreateCompatibleDC(hdc);
	
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdcmem,hbmp);
	BitBlt(hdc,0,0,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
	SelectObject(hdcmem,oldbrush);
	DeleteDC(hdcmem);

	lpOverlay->ReleaseDC(hdc);
	return TRUE;

}
BOOL Coverlay::UpdateOverlay(LPRECT rDes)
{
	HRESULT hr;
	DWORD uStretchFactor1000,uSrcSizeAlign,uDestSizeAlign,g_dwOverlayXPositionAlignment;
	
	uStretchFactor1000 = (ddcaps.dwMinOverlayStretch>1000) ? ddcaps.dwMinOverlayStretch : 1000;
	/*if (ddcaps.dwAlignSizeSrc != 0)
		rSrc->right += rSrc->right% ddcaps.dwAlignSizeSrc;*/   //�ڶ���Ҫ���ε�
	//rSrc->right�Ѿ�У������
    RECT rRdes;
	rDes->right  = (rSrc->right * uStretchFactor1000 + 999) / 1000 + rDes->left;   //Ӧ������ƫ������
	rDes->bottom = rSrc->bottom * uStretchFactor1000 / 1000 + rDes->top;           //

    rRdes.right = (rSrc->right * uStretchFactor1000 + 999) / 1000;
	rRdes.bottom = rSrc->bottom * uStretchFactor1000 / 1000;

     rRdes.left = 0;
	 rRdes.top = 0;

	if (ddcaps.dwAlignSizeDest != 0)
		//rDes->right = (int)((rDes->right + ddcaps.dwAlignSizeDest - 1)/ ddcaps.dwAlignSizeDest) *ddcaps.dwAlignSizeDest ;  //У��һ��
        rRdes.right = (int)((rDes->right + ddcaps.dwAlignSizeDest - 1)/ ddcaps.dwAlignSizeDest) *ddcaps.dwAlignSizeDest ;
	if(ddcaps.dwOverlayCaps&DDOVERLAYCAPS_ALPHASRC)
	{

		memset(&ofx, 0, sizeof(DDOVERLAYFX));
		ofx.dwSize = sizeof(DDOVERLAYFX);

		ofx.dwAlphaConstBitDepth = 8;
		ofx.dwAlphaConst = 50;

		//���ζ����ܸı�
		hr = lpOverlay->UpdateOverlay(rSrc, lpPrimary, &rRdes,DDOVER_SHOW|DDOVER_ALPHACONSTOVERRIDE, &ofx);
        if(FAILED(hr))
			return FALSE;
	}
	else
		return FALSE;
	return TRUE;
}