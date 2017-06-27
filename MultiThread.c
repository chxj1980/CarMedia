/**  
* @file MultiThread.c  
* @brief ʵ���˿�ƽ̨���̡߳����⡢�źŹ���.  
* @author Donhao  
* @date 2010-3-16 12:51:42  
* @version  
* <pre><b>All rights reserved.</b></pre>  
* <pre><b>modification:</b></pre>  
* <pre>Write modifications here.</pre>  
*/  

////�����漸������£�ͬ������Ҫ�ģ� 
//�� �����������������ڵ��߳̿��Ժ���һ��ͬ��������ע�⣬ͬ������Ӧ����һ�����̳�ʼ�����ڵڶ��γ�ʼ��ʱ����ͬ������������Ϊ����״̬�� 
//�� ͬ����Ψһ��֤�������ݳ־õİ취�� 
//�� һ�����̿���ӳ��һ���ļ���ͨ��һ���߳̽���������޸����֮�󣬸��߳��ͷ��ļ������ָ��ļ������ļ������Ĺ����У��κγ����е��κ� �߳���Ҫ����ʱ����������ֱ�������� 
//���㴴��һ���߳�ʱ����ʵ�Ǹ��߳���һ��ѭ����������������ֻ����һ�εġ������ʹ�����һ�����⣬
//���Ǹ���ѭ����Ҫ�ҵ����ʵ������˳��Ǹ���ѭ������ô����ô��ʵ�������أ���Windows�������ǲ����¼��ķ�ʽ��
//��Ȼ�����Բ��������ķ�ʽ���������Ƚ��ܲ����¼��ķ�ʽ��֪ͨ���߳����к����˳���������ʵ��ԭ����������
//���Ǹ���ѭ���ﲻ�ϵ�ʹ��WaitForSingleObject����������¼��Ƿ����㣬���������˳��̣߳�������ͼ������С�
//�����߳������������ĺ���ʱ������Ҫ���˳��߳�ʱ����Ҫ������״̬��ɷ�����״̬������ʹ��һ���߳�ȥ��������
//���ݣ�ͬʱʹ��������SOCKETʱ����ôҪ�ȹر�SOCKET���ٷ����¼��źţ��ſ����˳��̵߳ġ�
//���������ʾ��ô��ʹ���¼���֪ͨ�߳��˳�����



#include "MultiThread.h"   
  #ifdef UNICODE 
#define CreateMutex  CreateMutexW 
#else 
#define CreateMutex  CreateMutexA 
#endif // !UNICODE 

#ifdef WIN32   
BOOL Thread_Create(THREAD* thread, THREAD_FUNC myfunc, char* threadName, int stackSize, int priority, Thread_Arg_Struct* pargs)   
{   
    HANDLE h             = 0;   
    THREAD tmpThread = 0;   
    BOOL rt          = FALSE;   
  
    if (NULL == thread)   
    {   
        h = (HANDLE)_beginthreadex(NULL, stackSize, myfunc, pargs, 0, &tmpThread);   
    }   
    else  
    {   
        h = (HANDLE)_beginthreadex(NULL, stackSize, myfunc, pargs, 0, thread);   
    }   
  
    if (h > 0)   
    {   
        rt = SetThreadPriority(h, priority);   
  
        if (FALSE == rt)   
        {   
            return FALSE;   
        }   
        else  
        {   
            return TRUE;   
        }   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
 //get the calling thread ID,a owner capability for the current thread 
THREAD Thread_Self(void)   
{   
    THREAD td = GetCurrentThreadId();   
  
    return td;   
}   
  //��ʼ��һ��������
BOOL Mutex_Init(MUTEX* pMutex)   
{   
    if (NULL == pMutex)   
    {   
        return FALSE;   
    }   
  //����һ�������������Ļ���������
	//��������ɹ�ִ�У�������һ������������ľ���������CreateMutex()ִ��ǰ�Ѿ���������ͬ���ֵĻ�������
	//��������������Ѿ����ڻ������ľ�������ҿ���ͨ��GetLastError()�õ��������ERROR_ALREADY_EXIST��
	//CreateMutex() �����ж�ռҪ��ĳ��� (������������ڼ䲻��������ʹ�ô˶˿��豸�ĳ������У�
	//������ͬ����������)������ͬ���������У���ͨ�� GetLastError()�õ�������� ERROR_ALREADY_EXIST��

 //TRUE�������߳�ӵ�л������ �������߳�û���ͷŸö���  �������˭ӵ�� ˭�ͷ�
    //FLASE����ǰû���߳�ӵ������������

    *pMutex = CreateMutex(NULL, FALSE, NULL);   
  
    if (0 == (*pMutex))   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
//�������״̬ 
  
BOOL Mutex_Destroy(MUTEX mutex)   
{   
    int rt = 0;   
  //��CloseHandle���ͷ�Mutexʱ��ֻ���ͷŵ�ǰ���̻�õĸ��������������ɾ����Mutex�ں˶���

    rt = CloseHandle(mutex);   
  
    if (0 == rt)   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   

//��һ������������ �û���������ʹ�߳�˳��ִ�С�������ͨ��ֻ����һ���߳�ִ��һ���ؼ����ֵĴ��룬��ͬ���̡߳�������Ҳ���������������̴߳��롣 
//������������й����ҿ�д���ڴ棬��������Ӧ�ĳ�ʼ�����ú�(�μ�mmap(2))������������ʵ�ֽ��̼���߳�ͬ���� 
//��������ʹ��ǰһ��Ҫ��ʼ���� 
//���̵߳ȴ�һ��������ʱ�����û�������˳���ǲ�ȷ���ġ�

BOOL Mutex_Lock(MUTEX mutex)   
{   
    DWORD rt = 0;   
  
    rt = WaitForSingleObject(mutex, INFINITE);   
   //The process terminated
    if (WAIT_OBJECT_0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
//����
BOOL Mutex_Unlock(MUTEX mutex)   
{   
    int rt = 0;   
//ReleaseMutexֻ���ɴ�����Mutex�Ľ���ִ�У���Ȼ���Ƿ��ش���
  
    rt = ReleaseMutex(mutex);   
  
    if (0 == rt)   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
  
BOOL Condition_Init(CONDITION* pCond)   
{   
    if (NULL == pCond)   
    {   
        return FALSE;   
    }   
 /*  �ú�������һ��Eventͬ������,�����ظö����Handle.�������һ�������Ļ��������¼�����   
	Param1:lpEventAttributes���¼������ԡ�
	Param2:bManualReset�Ǹ�λ��ʽ:ָ�¼��ֶ���λ(TRUE)�������Զ���λ״̬��
	Param3:bInitialState�ǳ�ʼ״̬:��ʼ����״̬�Ƿ������źŵ�״̬��,true,���ź�,false���ź� 
	Param4:lpName���¼������ƣ���������ƣ����Կ���̹����¼�
	*/
    *pCond = CreateEvent(NULL, FALSE, FALSE, NULL);   
  
    if (0 == (*pCond))   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
  
BOOL Condition_Destroy(CONDITION cond)   
{   
    int rt = 0;   
  
    rt = CloseHandle(cond);   
  
    if (0 == rt)   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
  
BOOL Condition_Signal(CONDITION cond)   
{   
    int rt = 0;   
  //�ֱ�EVENT��Ϊ������״̬�ֱ��Ƿ��ź��벻���źš�

    rt = SetEvent(cond);   
  
    if (0 == rt)   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
  
BOOL Condition_Wait(CONDITION cond, MUTEX mutex)   
{   
    DWORD rt = TRUE;   
  
    rt = Mutex_Unlock(mutex);   
  
    if (FALSE == rt)   
    {   
        return FALSE;   
    }   
  
    rt = WaitForSingleObject(cond, INFINITE);   
  
    if (WAIT_OBJECT_0 != rt)   
    {   
        return FALSE;   
    }   
  
    rt = Mutex_Lock(mutex);   
  
    return rt;   
}   
  
#elif LINUX   
BOOL Thread_Create(THREAD* thread, THREAD_FUNC myfunc, char* threadName, int stackSize, int priority, Thread_Arg_Struct* args)   
{   
    THREAD tmpThread = -1;   
    THREAD rt        = -1;   
  
    if (NULL == thread)   
    {   
        rt = pthread_create(&tmpThread, NULL, myfunc, &args);   
    }   
    else  
    {   
        rt = pthread_create(thread, NULL, myfunc, &args);   
    }   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
  
    return rt;   
}   
  
THREAD Thread_Self(void)   
{   
    THREAD rt = pthread_self();   
  
    return rt;   
}   
  
BOOL Mutex_Init(MUTEX* pMutex)   
{   
    int rt = 0;   
  
    if (NULL == pMutex)   
    {   
        return FALSE;   
    }   
  
    rt = pthread_mutex_init(pMutex, NULL);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Mutex_Destroy(MUTEX mutex)   
{   
    int rt = 0;   
  
    rt = pthread_mutex_destroy(&mutex);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Mutex_Lock(MUTEX mutex)   
{   
    int rt = 0;   
  
    rt = pthread_mutex_lock(&mutex);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Mutex_Unlock(MUTEX mutex)   
{   
    int rt = 0;   
  
    rt = pthread_mutex_unlock(&mutex);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Init(CONDITION* pCond)   
{   
    int rt = 0;   
  
    if (NULL == pCond)   
    {   
        return FALSE;   
    }   
  
    rt = pthread_cond_init(pCond, NULL);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Destroy(CONDITION cond)   
{   
    int rt = 0;   
  
    rt = pthread_cond_destroy(&cond);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Wait(CONDITION cond, MUTEX mutex)   
{   
    int rt = 0;   
  
    rt = pthread_cond_wait(&cond, &mutex);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Signal(CONDITION cond)   
{   
    int rt = 0;   
  
    rt = pthread_cond_signal(&cond);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
#else   
BOOL Thread_Create(THREAD* thread, THREAD_FUNC myfunc, char* threadName, int stackSize, int priority, Thread_Arg_Struct * pargs)   
{   
    THREAD tmpThread = 0;   
  
    if (NULL == thread)   
    {   
        tmpThread = taskSpawn(threadName, priority, VX_FP_TASK, stackSize, (FUNCPTR)myfunc, (int)pargs, 1, 2, 3, 4, 5, 6, 7, 8, 9);   
  
        if (ERROR == tmpThread)   
        {   
            return FALSE;   
        }   
        else  
        {   
            return TRUE;   
        }   
    }   
    else  
    {   
        *thread = taskSpawn(threadName, priority, VX_FP_TASK, stackSize, (FUNCPTR)myfunc, (int)pargs, 1, 2, 3, 4, 5, 6, 7, 8, 9);   
  
        if (ERROR == (*thread))   
        {   
            return FALSE;   
        }   
        else  
        {   
            return TRUE;   
        }   
    }   
}   
  
THREAD Thread_Self(void)   
{   
    THREAD td = taskIdSelf();   
  
    return td;   
}   
  
BOOL Mutex_Init(MUTEX* pMutex)   
{   
    if (NULL == pMutex)   
    {   
        return FALSE;   
    }   
  
    *pMutex = semBCreate(SEM_Q_FIFO, SEM_FULL);   
  
    if (NULL == (*pMutex))   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
  
BOOL Mutex_Destroy(MUTEX mutex)   
{   
    STATUS rt = OK;   
  
    rt = semTake(mutex, WAIT_FOREVER);   
  
    if (OK == rt)   
    {   
        semDelete(mutex);   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Mutex_Lock(MUTEX mutex)   
{   
    STATUS rt = OK;   
  
    rt = semTake(mutex, WAIT_FOREVER);   
  
    if (OK == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Mutex_Unlock(MUTEX mutex)   
{   
    STATUS rt = OK;   
  
    rt = semGive(mutex);   
  
    if (OK == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Init(CONDITION* pCond)   
{   
    if (NULL == pCond)   
    {   
        return FALSE;   
    }   
  
    *pCond = semBCreate(SEM_Q_FIFO, SEM_EMPTY);   
  
    if (NULL == (*pCond))   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
  
BOOL Condition_Destroy(CONDITION cond)   
{   
    STATUS rt = OK;   
  
    rt = semTake(cond, WAIT_FOREVER);   
  
    if (OK == rt)   
    {   
        semDelete(cond);   
  
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Wait(CONDITION cond, MUTEX mutex)   
{   
    STATUS rt = OK;   
  
    rt = Mutex_Unlock(mutex);   
  
    if (FALSE == rt)   
    {   
        return FALSE;   
    }   
  
    rt = semTake(cond, WAIT_FOREVER);   
  
    if (OK == rt)   
    {   
        rt =  Mutex_Lock(mutex);   
  
        return rt;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Signal(CONDITION cond)   
{   
    STATUS rt = OK;   
  
    if (NULL == cond)   
    {   
        return FALSE;   
    }   
  
    rt = semGive(cond);   
  
    if (OK == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
#endif  


//��������CSDN���ͣ�ת�������������http://blog.csdn.net/donhao/archive/2010/09/04/5862935.aspx