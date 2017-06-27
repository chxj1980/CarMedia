/**  
* @file MQ.c  
* @brief  
* @author Donhao  
* @date 2010-4-17 12:57:43  
* @version  
* <pre><b>All rights reserved.</b></pre>  
* <pre><b>modification:</b></pre>  
* <pre>Write modifications here.</pre>  
*/  
#include "MQ.h"   
  
#define MQ_INITIALIZE_FLAG 0x1122   
/**  ������
 * ͬ�������������¼������裺
 *
 * ����ź����� 
 * ��ȡ���� 
 * �ж϶����Ƿ�Ϊ�գ� 
 * ����Ϊ�գ����ȡ read_ops �������ݣ��� read_ops �� 1�����ж� read_pos �Ƿ�Խ�磻 
 * ���ͷ����� 
 */
/**  
* @brief MQ_Read  
*  
* Detailed description.  
* @param[in] pMQ Ҫ������Ϣ����  
* @param[in] pOutBuf Ҫ�����Ϣ�Ļ�����  
* @param[in/out] pSize ��Ϊ�������ʱΪ����Ļ�������Ϣ��С����Ϊ�������Ϊ��������Ϣ��С  
* @return BOOL  
*/  
BOOL MQ_Read(MQ_Struct* pMQ, UINT8* pOutBuf, UINT32* pSize)   
{   
    BOOL rt = FALSE;   
  
    if ((NULL == pMQ) || (NULL == pOutBuf) || (NULL == pSize)   
        || (0 == pMQ->msgNum) || (MQ_INITIALIZE_FLAG != pMQ->initialized))   
    {   
        rt = FALSE;   
    }   
    else  
    {   
        Mutex_Lock(pMQ->mutex);   
  
        pMQ->front = (pMQ->front + 1) % pMQ->maxMsgNum;   
  
        --(pMQ->msgNum);   
  /* check if write_pos if overflow. */

        if (pMQ->pMQMsg[pMQ->front].msgSize > *pSize)   
        {   
            rt = FALSE;   
        }   
        else  
        {   
            memcpy(pOutBuf, pMQ->pMQMsg[pMQ->front].pMsg, pMQ->pMQMsg[pMQ->front].msgSize);   
            *pSize = pMQ->pMQMsg[pMQ->front].msgSize;   
  
            rt = TRUE;   
        }   
  /* release lock*/

        Mutex_Unlock(pMQ->mutex);   
    }   
  
    return rt;   
}   

  
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
BOOL MQ_Get(MQ_Struct* pMQ, UINT32 index, UINT8* pOutBuf, UINT32* pSize)   
{   
    BOOL rt = FALSE;   
  
    if ((NULL == pMQ) || (NULL == pOutBuf) || (NULL == pSize)   
        || (index >= pMQ->msgNum) || (MQ_INITIALIZE_FLAG != pMQ->initialized))   
    {   
        rt = FALSE;   
    }   
    else  
    {   
        Mutex_Lock(pMQ->mutex);   
  
        if (pMQ->pMQMsg[(pMQ->front + 1 + index) % pMQ->maxMsgNum].msgSize < *pSize)   
        {   
            rt = FALSE;   
        }   
        else  
        {   
            memcpy(pOutBuf, pMQ->pMQMsg[(pMQ->front + 1 + index) % pMQ->maxMsgNum].pMsg,   
                       pMQ->pMQMsg[(pMQ->front + 1 + index) % pMQ->maxMsgNum].msgSize);   
            *pSize = pMQ->pMQMsg[(pMQ->front + 1 + index) % pMQ->maxMsgNum].msgSize;   
  
            rt = TRUE;   
        }   
  
        Mutex_Unlock(pMQ->mutex);   
    }   
  
    return rt;   
}   
/**д����
 * �����������ͼ���ܣ�д������Ҫ�������¼�����
 * - ��ȡ����
 * �ж϶����Ƿ������� 
 * ��û����������д�� write_pos ������ write_pos �� 1�����ж� write_pos �Ƿ�Խ�磻 
 * �ͷ����������ź����� 1�� 
 */
/**
 *�������ζ���д���ݣ�����Ҫ������� �����ѱ�ռ�ã������ȴ�״̬�������һ��ȥ�жϻ��ζ����Ƿ�������
 *�����ˣ����ͷ��������أ�������δ����������д�� write_pos λ�ã�write_pos �� 1���ͷ��������ź����� 1��
 *��ʾ��д��һ�����ݣ� 
*/
MQ_Write_Result_Enum MQ_Write(MQ_Struct* pMQ, const UINT8* pMsg, UINT32 size)   
{   
    BOOL rt = FALSE;   
  
    if ((NULL == pMQ) || (NULL == pMsg) || (size > pMQ->maxMsgSize)   
        || (MQ_INITIALIZE_FLAG != pMQ->initialized))   
    {   
        rt = MQ_Write_Result_Error;   
    }   
    else  
    {   
        Mutex_Lock(pMQ->mutex);   
  
        /** @brief ��Ϣ����δ��,�������,����Ok */  
        pMQ->rear = (pMQ->rear + 1) % pMQ->maxMsgNum;   
  
        memcpy(pMQ->pMQMsg[pMQ->rear].pMsg, pMsg, size);   
        pMQ->pMQMsg[pMQ->rear].msgSize = size;   
 /**< �ж϶����Ƿ�Ϊ���ĵķ����ǿ� (write_pos + 1)% QUEUE_SIZE == read_pos �Ƿ����>*/
        if ((pMQ->maxMsgNum - 1) ==  pMQ->msgNum)   
        {   
            pMQ->front = (pMQ->front + 1) % pMQ->maxMsgNum;   
  
            rt = MQ_Write_Result_Full;   
        }   
        else  
        {   
            ++(pMQ->msgNum);   
  
            rt = MQ_Write_Result_Ok;   
        }   
  
        Mutex_Unlock(pMQ->mutex);   
    }   
  
    return rt;   
}   
  
/**  
* @brief MQ_Init  
*  
* Detailed description.  
* @param[in] pMQ Ҫ��ʼ������Ϣ����  
* @param[in] maxMsgNum ��Ϣ���д�С  
* @param[in] maxMsgSize ÿ����Ϣ�Ĵ�С  
* @return BOOL  
*/  
/**  
*���г�ʼ��
*��ʼ����Ҫ�����������
*
*Ϊ���ζ��з����ڴ棻 
*��ʼ������������ pthread_mutex_init ��ɣ� 
*��ʼ���ź������� sem_init ��ɡ� 
*/ 
BOOL MQ_Init(MQ_Struct* pMQ, UINT32 maxMsgNum, UINT32 maxMsgSize)   
{   
    BOOL rt  = FALSE;   
    UINT32 i = 0;   
    UINT32 j = 0;   
  
    /** @brief ��������Ϣ������ÿ����Ϣ������ֽ���Ϊ0,�򷵻ش��� */  
  
    if ((NULL == pMQ) || (0 == maxMsgNum) || (0 == maxMsgSize))   
    {   
        return FALSE;   
    }   
  
    if (MQ_INITIALIZE_FLAG == pMQ->initialized)   
    {   
        return TRUE;   
    }   
  
    pMQ->pMQMsg     = NULL;   
  
    pMQ->msgNum     = 0;   
    pMQ->front      = 0;   
    pMQ->rear       = 0;   
    pMQ->maxMsgNum  = maxMsgNum;   
    pMQ->maxMsgSize = maxMsgSize;   
    pMQ->pMQMsg  = malloc(sizeof(MQ_Msg_Struct) * pMQ->maxMsgNum);   
  
    if (NULL == pMQ)   
    {   
        return FALSE;   
    }   
  
    for (i = 0; i < pMQ->maxMsgNum; ++i)   
    {   
        pMQ->pMQMsg[i].pMsg = NULL;   
        pMQ->pMQMsg[i].pMsg = malloc(pMQ->maxMsgSize);   
  
        /** @brief �����ʼ��ʧ��,���ͷ�֮ǰ�������Դ */  
  
        if (NULL == pMQ)   
        {   
            for (j = i - 1; j >= 0; --j)   
            {   
                free(pMQ->pMQMsg[j].pMsg);   
                pMQ->pMQMsg[j].pMsg = NULL;   
            }   
  
            free(pMQ->pMQMsg);   
  
            pMQ->pMQMsg = NULL;   
            return FALSE;   
        }   
        else  
        {   
            pMQ->pMQMsg[i].msgSize = 0;   
            memset(pMQ->pMQMsg[i].pMsg, 0, pMQ->maxMsgSize);   
        }   
    }   
  
    rt = Mutex_Init(&(pMQ->mutex));   
  
    if (FALSE == rt)   
    {   
        return FALSE;   
    }   
  
    pMQ->initialized = MQ_INITIALIZE_FLAG;   
  
    return TRUE;   
}   

/**��System V�ṩ����Ϣ���еĲ��������У������Ϣ���еķ�����֪������һ�����֣�
 *1. ɾ���Ժ����½�����Ϣ���С�
 *2. type��ֵΪ0��һ��һ����������
 */

/**  
* @brief MQ_Clear  
*  
* Detailed description.  
* @param[in] pMQ Ҫ��յ���Ϣ����  
* @return BOOL  
*/  
BOOL MQ_Clear(MQ_Struct* pMQ)   
{   
    if ((NULL == pMQ) || (MQ_INITIALIZE_FLAG != pMQ->initialized))   
    {   
        return FALSE;   
    }   
  
    Mutex_Lock(pMQ->mutex);   
  
    pMQ->front  = 0;   
    pMQ->rear   = 0;   
    pMQ->msgNum = 0;   
  
    Mutex_Unlock(pMQ->mutex);   
  
    return TRUE;   
}

/**  
* @brief MQ_Get_Msg_Num Ҫ�����Ϣ������Ϣ����  
*  
* Detailed description.  
* @param[in] pMQ Ҫ�����Ϣ������ ��Ϣ����
* @return int ��ʧ���򷵻�-1,���򷵻���Ϣ����  
*/  
int MQ_Get_Msg_Num(const MQ_Struct* pMQ)   
{   
    if ((NULL == pMQ) || (MQ_INITIALIZE_FLAG != pMQ->initialized))   
    {   
        return -1;   
    }   
    else  
    {   
        return pMQ->msgNum;   
    }   
}  


//��������CSDN���ͣ�ת�������������http://blog.csdn.net/donhao/archive/2010/09/04/5862922.aspx#