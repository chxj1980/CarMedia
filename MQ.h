/**  
* @file MQ.h  
* @brief ʵ������Ϣ���еķ�װ  
* @author Donhao  
* @date 2010-4-17 12:57:57  
* @version  
* <pre><b>All rights reserved.</b></pre>  
* <pre><b>modification:</b></pre>  
* <pre>Write modifications here.</pre>  
*/  
#ifndef _MQ_H   
#define _MQ_H   
//#include "MultiThread.h"  
/** @brief д��Ϣ���з���ֵ */  
typedef enum  
{   
    MQ_Write_Result_Ok    = 0x11, /**< ����δ��   */  
    MQ_Write_Result_Error = 0x22, /**< д����ʧ��   */  
    MQ_Write_Result_Full  = 0x33  /**< �������� */  
}MQ_Write_Result_Enum;   
  
typedef struct  
{   
    unsigned long msgSize;   
    unsigned char* pMsg;   
} MQ_Msg_Struct;   
  
typedef struct  
{   
    unsigned long initialized; /**< �Ƿ��ʼ����         */  
    unsigned long maxMsgNum;   /**< MQ�������Ϣ����     */  
    unsigned long maxMsgSize;  /**< ÿ����Ϣ������ֽ��� */  
    int front;                 /**< ͷָ��               */  
    int rear;                  /**< βָ��               */  
    unsigned long msgNum;      /**< ��Ϣ��������Ϣ����   */  
    MUTEX mutex;               /**< ������Ϣ���еĻ����� */  
    MQ_Msg_Struct* pMQMsg;     /**< ָ���һ����Ϣ    */  
} MQ_Struct;   
  
/**  
* @brief MQ_Init  
*  
* Detailed description.  
* @param[in] pMQ Ҫ��ʼ������Ϣ����  
* @param[in] maxMsgNum ��Ϣ���д�С  
* @param[in] maxMsgSize ÿ����Ϣ�Ĵ�С  
* @return BOOL  
*/  
BOOL MQ_Init(MQ_Struct* pMQ, unsigned long maxMsgNum, unsigned long maxMsgSize);   
  
/**  
* @brief MQ_Get_Msg_Num  
*  
* Detailed description.  
* @param[in] pMQ Ҫ�����Ϣ��������Ϣ����  
* @return int ��ʧ���򷵻�-1,���򷵻���Ϣ����  
*/  
int MQ_Get_Msg_Num(const MQ_Struct* pMQ);   
  
/**  
* @brief MQ_Write  
*  
* Detailed description.  
* @param[in] pMQ Ҫд����Ϣ����  
* @param[in] pMsg Ҫд����Ϣ  
* @param[in] size Ҫд����Ϣ��С  
* @return MQ_Write_Result_Enum  
*/  
MQ_Write_Result_Enum MQ_Write(MQ_Struct* pMQ, const unsigned char* pMsg, unsigned long size);   
  
/**  
* @brief MQ_Read  
*  
* Detailed description.  
* @param[in] pMQ Ҫ������Ϣ����  
* @param[in] pOutBuf Ҫ�����Ϣ�Ļ�����  
* @param[in/out] pSize ��Ϊ�������ʱΪ����Ļ�������Ϣ��С����Ϊ�������Ϊ��������Ϣ��С  
* @return BOOL  
*/  
BOOL MQ_Read(MQ_Struct* pMQ, unsigned char* pOutBuf, unsigned long* pSize);   
  
/**  
* @brief MQ_Get  
*  
* ����ȡ���ݣ���������.  
* @param[in] pMQ  Ҫ������Ϣ����
* @param[in] index  
* @param[out] pOutBuf  Ҫ�����Ϣ�Ļ�����
* @param[out] pSize  �������Ϊ��������Ϣ��С
* @return BOOL  
*/  
BOOL MQ_Get(MQ_Struct* pMQ, unsigned long index, unsigned char* pOutBuf, unsigned long* pSize);   
  
/**  
* @brief MQ_Clear  
*  
* Detailed description.  
* @param[in] pMQ Ҫ��յ���Ϣ����  
* @return BOOL  
*/  
BOOL MQ_Clear(MQ_Struct* pMQ);   
  
#endif  


//��������CSDN���ͣ�ת�������������http://blog.csdn.net/donhao/archive/2010/09/04/5862922.aspx#