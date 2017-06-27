/*****************************************************************************************
* ElecBook��: �ı���ȡ����ҳ�л���ʾ
******************************************************************************************/
#ifndef ELEC_BOOK
#define ELEC_BOOK
#include <commctrl.h> 
#include "atlstr.h"   
#include "TxtEdit.h"
#include "public.h"
#include "ElecBook.h"
#include <algorithm>
#include <vector>
#define LINE_LENGTH 100//84
#define LINAGE 13  //ÿҳ��ʾ������
#define KEYPAGE_SPACE 8  //�ؼ�ҳ���
class ElecBook
{
private:
	BOOL last; //�Ƿ��ļ�β��־
	int Len;  //getline����ַ���
	//HDC hdc ;
	HBITMAP bmp,bmp1;
	HDC hdcBg;   //���б���ͼ
	HDC hdcMemTxt;  //������ϱ��������֣�Ȼ����ʾ
	int index; //ҳ����
	//std::streampos KeyPagepos[1024];  //���ÿҳ��λ��
	vector<std::streampos>EveryPagepos;
	int num;
	std::streampos Prepos;
	BOOL FILE_LAST;
	//�߳�ͬ���ٽ�������
	
	BOOL BUTTON_NEXT;
	BOOL BUTTON_PRE;

public:
	TxtEdit* cText;  //������Ĺ��캯�����ļ�=new TxtEdit()
	std::streampos ExitPos;//�˳��߳�ִ��������ʱ��λ��
	std::streampos ReadPos; //��ȡ��ʾ�ĵ�ǰ��λ��
	float Percent;
	std::streampos Endpos;
	
	ElecBook(HWND hWnd,TCHAR* path);
	~ElecBook();
	void InitBook(HWND hWnd);
	void ButtonDown(HWND hWnd, POINT point);
	void ButtonUp(HWND,  POINT point);
	void MouseMove(HWND hWnd, POINT point);
	void DoTimer(HWND hWnd,  WPARAM wParam);
	LRESULT PaintMain(HWND,UINT,WPARAM,LPARAM);

	void ChangePage(HWND hWnd,BOOL Dir); //��ҳ
	void ReadNextPage(char** nextstr); //����һҳ
	void ReadPrePage(char** prestr);  //��ǰһҳ
	void CreateBgDC(HWND hWnd);     //���������ڴ�
	void TxtDisp(HWND hWnd,TCHAR* str);  //��ʾһҳ����
	void ReleaseVariable(); //ɾ������
	void PrePointer();  //��ָ���Ƶ�ǰһҳ�Ŀ�ʼλ��
	void DispFirstPage(HWND hWnd);
};
#endif