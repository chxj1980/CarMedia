/*****************************************************************************************
* TxtEdit��:�ı��Ű棬������ʱ�ļ�
******************************************************************************************/
#include "stdafx.h"
#include "TxtEdit.h"

#define  MAXLEN  68   //����ÿ��������ַ�����󳤶�Ϊ80
#define  LEN  m_szReadPara.size()   //����LENΪÿ�ζ�ȡ�ļ�һ���ַ����ĳ���

DWORD t1,t2;

// ���캯��,���ļ�
TxtEdit::TxtEdit(TCHAR* txtname)
{	
	File.open(txtname, ios::in);//"\\Storage Card\\txt\\te.txt"
	FileOut.open("\\Storage Card\\out.txt", ios::in|ios::out|ios::trunc);//�ļ�������ʱ����
	if (!File)   //�ж��ļ��ܷ��
	{
		cout << "File can not be open." << endl;
		exit(-1);
	}
	if (!FileOut)   //�ж��ļ��ܷ��
	{
		cout << "FileOut can not be open." << endl;
		exit(-1);
	}
	shortCh1 = FALSE;
	shortCh2 = FALSE;
	LineNum = 0;//�Ű���������
	PageNum = 0; //�Ű�����ҳ��

	//����ļ���С
	std::streampos begpos=File.tellg();
	File.clear();
	File.seekg(0,ios::end);
	std::streampos endpos=File.tellg();
	Txtsize = endpos - begpos;	
	File.clear();
	File.seekg(0,ios::beg);
}

//�����������ر��ļ�

TxtEdit::~TxtEdit()
{
	File.close();   
	FileOut.close();
	//DeleteFile(L"\\Storage Card\\out.txt"); //ɾ����ʱ�Ű����ļ�
}
//��ȡ�ļ����ݣ����ζ�ȡ

void TxtEdit::Read()
{
	char chCh;
	while (!File.eof())   //��ʼ��ȡ�ļ�����
	{
		m_szReadPara = "";   //��ʼ���ַ���
	
		if (getline(File,m_szReadPara))
		{  
			if ("" == m_szReadPara)  //û��ȡ�����ݣ����ж�ѭ��
				break;

			WordProcess(m_szReadPara);  //�������ִ�����
		}
		else
		{
			//cout<<"GETLINE FAILED"<<endl; 
		}      
	}

}
//�Զ�ȡ��ÿ�ν������ִ��������Ű�����

void TxtEdit::WordProcess(string strstr)
{
	int len = strstr.length();

	if (len <= MAXLEN)  //����ö�����80���ַ���ֱ�����
	{
		FileOut<<strstr.substr(0, len)<<endl;  //zh<<endl
		LineNum++;
		return;
	}
	else
	{
		string str4=shortString(strstr);
		WordProcess(str4);
	}						
}
string TxtEdit::shortString(string str)
{
	int leng=str.length();
    
	int i=0;
	
	while (i<MAXLEN)
	{
		char ichar = str.at(i);
		if (ichar <= -1 )
		{
			if (i==MAXLEN-3)
			{
				i += 2;
				if (str.at(MAXLEN-1)<0)
				{
					shortCh2 = TRUE;
				}
				else
				{
					shortCh2 = FALSE;
				}
				break; //�ѵ�ĩβ������ѭ��
			}
			else
			{
				shortCh2 = FALSE;
			}
			i+=2;

		}
		else
		{
			if (i==MAXLEN-2)
			{
				i+=1;
				if (str.at(i)<0)
				{
					shortCh2 = TRUE;
				}
				else
				{
					shortCh2 = FALSE;
				}
			}
 			   i++;
		}
	}
	
	if(shortCh2)//�ǰ������
	{
		FileOut<<str.substr(0, MAXLEN-1)<<' '<<endl;  //zh<<endl
		LineNum++;
	
		int n=leng-(MAXLEN-1);
		return str.substr(MAXLEN-1,n);
	}
	else
	{
		FileOut<<str.substr(0, MAXLEN)<<endl;  //zh
		LineNum++;
		return str.substr(MAXLEN,leng-MAXLEN);
	}
}

