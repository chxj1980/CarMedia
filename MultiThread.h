/**  
* @file MultiThread.h  
* @brief ʵ���˿�ƽ̨(Windows��Linux��VxWorks)�̡߳����⡢�ź�  
* @author Donhao  
* @date 2010-3-16 12:49:30  
* @version  
* <pre><b>All rights reserved.</b></pre>  
* <pre><b>modification:</b></pre>  
* <pre>Write modifications here.</pre>  
*/  
  
#ifndef _MULTITHREAD_H   
#define _MULTITHREAD_H   
  
/**  
* @CommonOSHeadFile  
* @brief ��ƽ̨ϵͳͷ�ļ�  
*  
* Windowx��Linux��VxWorks��ͬʹ�õ�ͷ�ļ�.  
* @{  
*/  
#include <string.h>   
#include <stdio.h>   
#include <stdlib.h>   
#include <ctype.h>   
/** @} */  
  
/**  
* @OSHeadFile  
* @brief ����ϵͳ���ͷ�ļ�  
*  
* Windowx��Linux��VxWorks����ʹ�õ�ͷ�ļ�.  
* @{  
*/  
#ifdef WIN32   
#define OS_WINDOWS WIN32   
#include <time.h>   
#include <winsock2.h>   
#include "process.h"   
#pragma comment(lib, "Ws2_32.lib")   
#include <direct.h>   
#include "tlhelp32.h"   
#include "shellapi.h"   
#elif LINUX   
#include <arpa/inet.h>   
#include <dirent.h>   
#include <errno.h>   
#include <fcntl.h>   
#include <math.h>   
#include <netinet/ip_icmp.h>   
#include <netdb.h>   
#include <netinet/in.h>   
#include <netinet/ip.h>   
#include <netinet/tcp.h>   
#include <pthread.h>   
#include <sched.h>   
#include <signal.h>   
#include <string.h>   
#include <stropts.h>   
#include <sys/procfs.h>   
#include <sys/socket.h>   
#include <sys/stat.h>   
#include <sys/time.h>   
#include <sys/types.h>   
#include <sys/vfs.h>   
#include <time.h>   
#include <unistd.h>   
#else   
#include <vxworks.h>   
#include <config.h>   
#include <configAll.h>   
#include <dosFsLib.h>   
#include <errnoLib.h>   
#include <flioctl.h>   
#include <fldrvvxw.h>   
#include <ftpLib.h>   
#include <hostLib.h>   
#include <inetLib.h>   
#include <intLib.h>   
#include <loadLib.h>   
#include <ioLib.h>   
#include <logLib.h>   
#include <msgQLib.h>   
#include <netinet/in.h>   
#include <netinet/ip.h>   
#include <netinet/tcp.h>   
#include <semLib.h>   
#include <signal.h>   
#include <sioLib.h>   
#include <sockLib.h>   
#include <sntp.h>   
#include <sntpcLib.h>   
#include <symLib.h>   
#include <sysLib.h>   
#include <sys/socket.h>   
#include <sys/types.h>   
#include <taskLib.h>   
#include <tickLib.h>   
#include <time.h>   
#include <wdLib.h>   
#endif   
/** @} */  
  
#ifdef WIN32   
typedef unsigned        THREAD;   
typedef HANDLE          MUTEX;   
typedef HANDLE          CONDITION;   
typedef unsigned        THREAD_FUNC_RETURN;   
typedef THREAD_FUNC_RETURN(__stdcall* THREAD_FUNC)(void* args);   
#define THREAD_CALL __stdcall   
#elif LINUX   
typedef pthread_t       THREAD;   
typedef pthread_mutex_t MUTEX;   
typedef pthread_cond_t  CONDITION;   
typedef void*           THREAD_FUNC_RETURN;   
typedef THREAD_FUNC_RETURN(* THREAD_FUNC)(void* args);   
#define THREAD_CALL   
#else   
typedef int             THREAD;   
typedef SEM_ID          MUTEX;   
typedef SEM_ID          CONDITION;   
typedef int             THREAD_FUNC_RETURN;   
typedef FUNCPTR         THREAD_FUNC;   
#define THREAD_CALL   
#endif   
  
/** ��������ṹ��  */  
  
typedef struct  
{   
    int args0;   
    int args1;   
    int args2;   
    int args3;   
    int args4;   
    int args5;   
    int args6;   
    int args7;   
    int args8;   
    int args9;   
}Thread_Arg_Struct;   
  
/**  
* @name Thread  
* @brief ʵ�����̴߳��������١�����߳�ID  
* @{  
*/  
/**  
* @brief Thread_Create  
*  
* @param[out] thread �߳̾��(��ΪNULL������߳̾��)  
* @param[in] myfunc �̺߳���  
* @param[in] threadName �߳���(��VxWorks�õ�)  
* @param[in] stackSize �̶߳�ջ��С(��VxWorks�õ�)  
* @param[in] priority ���ȼ�(��VxWorks�õ�)  
* @param[in] args ����  
* @return BOOL  
*/  
BOOL Thread_Create(THREAD* thread, THREAD_FUNC myfunc, char* threadName, int stackSize, int priority, Thread_Arg_Struct* pArgs);   
  
/**  
* @brief Thread_Self  
*  
* @return THREAD  
*/  
THREAD Thread_Self(void);   
/** @} */  
  
/**  
* @name Mutex  
* @brief ������  
*  
* @{  
*/  
/**  
* @brief Mutex_Init  
*  
* ��ʼ��������.  
* @param[out] mutex  
* @return BOOL  
*/  
BOOL Mutex_Init(MUTEX* pMutex);   
  
/**  
* @brief Mutex_Destroy  
*  
* ���ٻ�����.  
* @param[in] mutex  
* @return BOOL  
*/  
BOOL Mutex_Destroy(MUTEX mutex);   
  
/**  
* @brief Mutex_Lock  
*  
* ��û�������Դ.  
* @param[in] mutex  
* @return BOOL  
*/  
BOOL Mutex_Lock(MUTEX mutex);   
  
/**  
* @brief Mutex_Unlock  
*  
* �ͷŻ�����.  
* @param[in] mutex  
* @return BOOL  
*/  
BOOL Mutex_Unlock(MUTEX mutex);   
/** @} */  
  
  
/**  
* @name Condition  
* @brief �¼�  
* @{  
*/  
  
/**  
* @brief Condition_Init  
*  
* ��ʼ��Condition.  
* @param[out] cond  
* @return BOOL  
*/  
BOOL Condition_Init(CONDITION* pCond);   
  
/**  
* @brief Condition_Destroy  
*  
* ����Condition.  
* @param[in] cond  
* @return BOOL  
*/  
BOOL Condition_Destroy(CONDITION cond);   
  
/**  
* @brief Condition_Wait  
*  
* �ȴ�Condition.  
* @param[in] cond  
* @param[in] mutex  
* @return BOOL  
*/  
BOOL Condition_Wait(CONDITION cond, MUTEX mutex);   
  
/**  
* @brief Condition_Signal  
*  
* ֪ͨCondition.  
* @param[in] cond  
* @return BOOL  
*/  
BOOL Condition_Signal(CONDITION cond);   
/** @} */  
#endif  

//
//��������CSDN���ͣ�ת�������������http://blog.csdn.net/donhao/archive/2010/09/04/5862935.aspx