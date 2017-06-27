/*****************************************************************************************
* TxtEdit��:�ı��Ű棬������ʱ�ļ�
******************************************************************************************/

#ifndef _TxtEdit_H_
#define _TxtEdit_H_

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class TxtEdit
{
private:
	BOOL shortCh1;
	BOOL shortCh2;
	char m_szFileName[30];  //�ļ���
	/*fstream File;  */
	//ofstream FileOut;
	string m_szReadPara;  //�����洢ÿ�ζ�ȡ�ļ�һ�ε�����

public:
	fstream File;  
	fstream FileOut;
	std::streampos Txtsize;
	int LineNum; //�Ű���������
	int PageNum; //�Ű�����ҳ��

enum TEXTENCODING { ASCII, UNI16_BE, UNI16_LE, UTF_8 };

//���캯��,���ļ�

	TxtEdit(TCHAR* txtname);

//��������,�ر��ļ�

	~TxtEdit();

//��ȡ�ļ����ݣ����ζ�ȡ

	void Read();

//�Զ�ȡ��ÿ�ν������ִ��������Ű�����

	void WordProcess(string strstr);
	string shortString(string str);
};

#endif