#include "ReadLyric.h"
//#include "tchar.h"
#include "fstream"
using namespace std;
//#ifdef _WINDOWS_ 
//	#undef _WINDOWS_ 
//		#include "afxcmn.h" 
//#endif

int     LineCount = 0;     //������
LYRIC stLyric[1000];      //ʱ����͸��

//��ȡ���Ѹ���зֳɾ�
bool SplitLyric(CString path)
{
	//��ʼ���洢����
	  for (int i = 0;i<1000;i++)
	  {
		   for(int j = 0;j<1000;j++)
		   {
				stLyric[i].Lyric [j] = 0;
				stLyric[i].Time = 0;
		   }
	  }
	   LineCount = 0;  //��ʼ��������


	 path.Replace (L".mp3",L".lrc");      //���·��
	 path.Replace (L".MP3",L".lrc");      //·����Сд

	 ifstream file(path,ios::in);
	 if (!file)
	 {
		  path.Replace (L".lrc",L".LRC");    //�����ʺ�׺��д����
		  file.open (path,ios::in);
		  if(!file)
		  {
			//MessageBox(L"���û�з���");
			  file.close ();
			  return false;    //û�и��
		  }
	  
	 }
	 char pbuf[1000];
	 while (file.getline (pbuf,1000))
	 {
		  DWORD dwNum = MultiByteToWideChar(CP_ACP,0,pbuf,-1,NULL,0);
		  TCHAR *pTmp;
		  pTmp = new TCHAR[dwNum];
		  MultiByteToWideChar(CP_ACP,0,pbuf,-1,pTmp,dwNum);
		  CString  Lyric;
		  Lyric = pTmp;
		  int pos = 0;
		  int ppos = 0;
		  LONGLONG tmpTime = 0; 
		  CString strLyric,bstrLyric,strTime,bstrTime;
		  pos = Lyric.Find (L"]");

		  //�ж���ʱ���
		  strTime = Lyric.Left (pos+1);
		  strTime.Remove (L'[');
		  strTime.Remove (L']');  //���ʱ���ʽ 03:25.68
		  ppos = strTime.Find (L'0');

		  if (pos != -1 || ppos != -1) //�ж��Ƿ��� ���о�����ʱ�����û��
		  {

			  strLyric = Lyric.Right (Lyric.GetLength ()-pos-1);  //��ø��
			  
			  
			  //pcʱ��ת��
			  char fenbuf[100];
			  char miaobuf[100];
			  char haombuf[100];
			  CString fen, //��
				  miao,   //��
				  haom;   //����
			  fen = strTime.Mid (0,2);
			  miao = strTime.Mid (3,2);
			  haom = strTime.Mid (6,strTime.GetLength ()-6);
			  for(int i = 0;i<fen.GetLength ()+1;i++)
			  {
				fenbuf[i]= fen.GetAt (i);
			  }
			  for(int i = 0;i<miao.GetLength ()+1;i++)
			  {
				miaobuf[i]= miao.GetAt (i);
			  }
			  for(int i = 0;i<haom.GetLength ()+1;i++)
			  {
				haombuf[i]= haom.GetAt (i);
			  }
		  
			  //atoi wince��pc��ͬ
			  tmpTime = (atoi(fenbuf)*60 + atoi(miaobuf)); //ת������      //*1000 + atoi(haombuf); //ʱ��ת�� ����
			  //��һ�еĴ���
			  if (tmpTime == 0)
			  {
				tmpTime = 1;  //һ��ʱ����
			  }
	   
			  //�в�����ʱ���ǩ����
			  int bpos =0;
			  bpos = strLyric.Find (L"]");
			   if (bpos != -1)
			   {
					bstrLyric = strLyric; //����ʱ���ǩ�ĸ��
					strLyric = strLyric.Right (strLyric.GetLength ()-bpos-1);
				
			   }
			   //�洢
			   for (int i = 0;i<strLyric.GetLength ()+1;i++)
			   {
					stLyric[LineCount].Lyric[i]  = strLyric.GetAt (i);
			   }
				stLyric[LineCount].Time = tmpTime;   

				LineCount++;
		    
				if (bpos != -1)
				{
					 //bstrLyric = bstrLyric.Right (bstrLyric.GetLength ()-bpos-1); 
					 bstrTime = bstrLyric.Left (bpos+1);
					 bstrTime.Remove (L'[');
					 bstrTime.Remove (L']');  //���ʱ���ʽ 03:25.68
					 char fenbuf[100];
					 char miaobuf[100];
					 char haombuf[100];
					 CString fen,miao,haom;
					 fen = bstrTime.Mid (0,2);
					 miao = bstrTime.Mid (3,2);
					 haom = bstrTime.Mid (6,bstrTime.GetLength ()-6);
					 for(int i = 0;i<fen.GetLength ()+1;i++)
					 {
					   fenbuf[i]= fen.GetAt (i);
					 }
					 for(int i = 0;i<miao.GetLength ()+1;i++)
					 {
						miaobuf[i]= miao.GetAt (i);
					 }
					 for(int i = 0;i<haom.GetLength ()+1;i++)
					 {
						haombuf[i]= haom.GetAt (i);
					 }
				  
					 //atoi wince��pc��ͬ
					 tmpTime = (atoi(fenbuf)*60 + atoi(miaobuf));  //��                        //*1000 + atoi(haombuf);

					 bstrLyric = bstrLyric.Right (bstrLyric.GetLength ()-bpos-1); 
					 for (int i = 0;i<bstrLyric.GetLength ()+1;i++)
					 {
						stLyric[LineCount].Lyric [i] = bstrLyric.GetAt (i);
					 }
					  stLyric[LineCount].Time  = tmpTime;
					  LineCount++;
				}//end if (bpos != -1)
		   

			}//end �ж��Ƿ��� ���о�����ʱ�����û��
		delete pTmp;
		}// end while (file.getline (pbuf,1000))
		file.close ();
		return true;
}