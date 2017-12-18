// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <windows.h>
#include "TcpLayer.h"
#include "FileOpr.h"
#include "Tools_Ansi.h"
#include "CommonPkg.h"
#include "TcpClientWorker.h"
#include <windows.h>
#include "psapi.h"

#include <string>
using namespace std;



#ifdef _DEBUG
	#pragma comment(lib, "ToolsD.lib")
#else
	#pragma comment(lib, "Tools.lib")
#endif

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")


#pragma unmanaged


//�ڲ�����
bool CTcpLayer::m_bStop=false;
Ceasymutex CTcpLayer::m_mutex;
void* CTcpLayer::m_pTcpClient=NULL;
char* CTcpLayer::m_pRcvBuf=NULL;
int CTcpLayer::m_RcvBufLen=0;
char* CTcpLayer::m_pSndBuf=NULL;
int CTcpLayer::m_SndBufLen=0;
time_t CTcpLayer::m_ReceivePkgTime=0;


void CTcpLayer::Init()
{
	m_mutex.lock();

	createtcpclient();

	m_mutex.unlock();
}

void CTcpLayer::Release()
{
	m_mutex.lock();
	CFileOpr::writelocallog("TcpLayer.log","Release��m_pTcpClient=%x ",
							m_pTcpClient);

	if(m_pTcpClient)
	{
		delete m_pTcpClient;
		m_pTcpClient=NULL;
	}
	m_mutex.unlock();
}

//ϵͳ�˳�ǰ����Stop��ֹͣ�ַ����ٹرո�ģ�鴰�ڡ�
void CTcpLayer::Stop()
{
	m_mutex.lock();
	m_bStop=true;
	m_mutex.unlock();
}


//��tcp�õ����ݺ󣬵��ô˻ص��������д���
void CTcpLayer::RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if(m_bStop) return;

	m_mutex.lock();
	if(!m_pRcvBuf) m_pRcvBuf=(char*)malloc(TcpRecvBufSize);
	if(!m_pRcvBuf||m_RcvBufLen+DataLen>TcpRecvBufSize)
	{
		CFileOpr::writelocallog("TcpLayer.log","RecvDataCBFunc���������㣬m_pRcvBuf=%x m_RcvBufLen=%d DataLen=%d TcpRecvBufSize=%d localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
								m_pRcvBuf,m_RcvBufLen,DataLen,TcpRecvBufSize,LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	}
	else
	{
		memcpy(m_pRcvBuf+m_RcvBufLen,pData,DataLen);
		m_RcvBufLen+=DataLen;
	}
	m_mutex.unlock();


    //������������յ�������
    ParseRecvPackageInWindowThread();
}

//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
void CTcpLayer::DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","DisconnCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	m_mutex.lock();
	m_mutex.unlock();
}

//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
void CTcpLayer::StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","StartConnCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	m_mutex.lock();
	m_mutex.unlock();
}

//���ӳɹ��󣬵��ô˻ص��������д���
void CTcpLayer::ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","StartConnCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	
	char sendData[256]=""; /*= "01580101 00000001 2af90000 00280000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00006164 6d696e20 20202020 20202020 20202020 20203132 33343520 20202020 20202020 20202020 2020";
	int dataLen = strlen(loginReq);*/
	m_mutex.lock();
    int dataLen = getloginReqData(sendData);
	Senddata( sendData, dataLen );
	memset(sendData,0,sizeof(sendData));
	dataLen = getHQReqData(sendData);
    Senddata( sendData, dataLen );
	m_mutex.unlock();
}

//����ʧ�ܺ󣬵��ô˻ص��������д���
void CTcpLayer::ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","ConnFailCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	m_mutex.lock();
	m_mutex.unlock();
}

//���ڵ��õĻص�������һ��ÿ������һ�Σ��������ڼ��������ʱ��
void CTcpLayer::OnTimeCBFunc(void)
{
	m_mutex.lock();
	time_t cursecs=time(NULL);
	if(cursecs-m_ReceivePkgTime>=10)
	{}

	//������δ���͵�����
	if(!m_bStop)
		senddata_internal();


	m_mutex.unlock();
}

//����tcpclient
void CTcpLayer::createtcpclient(void)
{
	m_mutex.lock();
	if(!m_bStop&&!m_pTcpClient)
	{
		string svrIP("114.255.138.141"),localIP("0");
		DWORD dwsvrIP,dwlocalIP;
		unsigned short svrPort=18611,localPort=0;

		dwsvrIP=CTools_Ansi::ConvertStrIP(svrIP.c_str());
		if(dwsvrIP!=0&&svrPort!=0)
		{
			dwlocalIP=CTools_Ansi::ConvertStrIP(localIP.c_str());

			m_pTcpClient=new CTcpClientWorker(Stru_IPAddress(dwlocalIP,localPort),Stru_IPAddress(dwsvrIP,svrPort),
				 RecvDataCBFunc,
				 DisconnCBFunc,
				 StartConnCBFunc,
				 ConnSuccCBFunc,
				 ConnFailCBFunc,
				 OnTimeCBFunc,
				 SocketBufSize,SocketBufSize);

			if(Debug_TcpLayer||m_pTcpClient)
				CFileOpr::writelocallog("TcpLayer.log","����TcpClient��localaddr=%s(%u):%u svraddr=%s(%u):%u, m_pTcpClient=%x",
                localIP.c_str(),dwlocalIP,localPort,svrIP.c_str(),dwsvrIP,svrPort,m_pTcpClient);
		}
	}
	m_mutex.unlock();
}


//��������
bool CTcpLayer::Senddata(const char* pData,int DataLen)
{
	if(m_bStop) return false;

	bool brlt=false;
	m_mutex.lock();
    brlt=Senddata_withoutlock(pData,DataLen);
	m_mutex.unlock();
	return brlt;
}


//�������ݣ�������
bool CTcpLayer::Senddata_withoutlock(const char* pData,int DataLen)
{
    bool brlt=false;
	if(!m_pSndBuf) m_pSndBuf=(char*)malloc(TcpSendBufSize);
	if(!m_pSndBuf||m_SndBufLen+DataLen>TcpSendBufSize)
	{
		CFileOpr::writelocallog("TcpLayer.log","senddata���������㣬m_pSndBuf=%x m_SndBufLen=%d DataLen=%d TcpSendBufSize=%d m_pTcpClient=%x hSocket=%x ConnStatus=%d",
			m_pSndBuf,m_SndBufLen,DataLen,TcpSendBufSize,m_pTcpClient,
			m_pTcpClient?((CTcpClientWorker*)m_pTcpClient)->getSocket():-1,
			m_pTcpClient?((CTcpClientWorker*)m_pTcpClient)->getConnStatus():0);
		brlt=false;
	}
	else
	{
		memcpy(m_pSndBuf+m_SndBufLen,pData,DataLen);
		m_SndBufLen+=DataLen;
		brlt=true;
		senddata_internal();
	}
    return brlt;
}


//��������
void CTcpLayer::senddata_internal(void)
{
	if(m_pSndBuf&&
		!m_bStop&&m_SndBufLen>0&&
		m_pTcpClient&&
		((CTcpClientWorker*)m_pTcpClient)->getSocket()>0&&
		((CTcpClientWorker*)m_pTcpClient)->getConnStatus()==TcpClientConnStatus_Connected)
	{
		int sentlen=((CTcpClientWorker*)m_pTcpClient)->SendData(m_pSndBuf,m_SndBufLen);
		if(sentlen>0&&sentlen<=m_SndBufLen)
		{
			memmove(m_pSndBuf,m_pSndBuf+sentlen,m_SndBufLen-sentlen);
			m_SndBufLen-=sentlen;
		}
	}
}



//�����߳��е��ã����ڽ������ַ��������ݰ���
//���յ����ݺ󣬻������̷߳���WM_COMMAND������߳��ٵ��ô˺������н����ͷַ���
//�����ڴ����߳��д������ݣ���Ƚϰ�ȫ
void CTcpLayer::ParseRecvPackageInWindowThread(void)
{
	if(m_bStop) return;

	m_mutex.lock();

	if(!m_pRcvBuf) m_pRcvBuf=(char*)malloc(TcpRecvBufSize);

	if(m_pRcvBuf)
	{
        //m_pRcvBuf�ǽ��յ������ݻ�����ָ��
        //m_RcvBufLen�ǽ��յ������ݳ���
        int ProcessedLen=0;

		m_ReceivePkgTime=time(NULL);
		//��ʼ����ͷ
		DataHead	rHead;
		memset(&rHead,0,sizeof(rHead));
		g_Mem2Head(&rHead, m_pRcvBuf);
		CFileOpr::writelocallog("TcpLayer.log","ParseRecvPackageInWindowThread��m_usDataType=%u,m_ulDataLength=%u",
							rHead.m_usDataType,rHead.m_ulDataLength);
		if(rHead.m_usDataType == 11001 && rHead.m_rStatus.s.m_cError == 0 && rHead.m_pcReserve[0] == 1)//��¼����
		{
			
			char data[1000];
			memset(&data,0,sizeof(data));
			memcpy(&data, m_pRcvBuf+50, 64);
			CFileOpr::writelocallog("TcpLayer.log","��½�ɹ���%s",data);
		}


		ProcessedLen = _DataHeadLength_ + rHead.m_ulDataLength;
		//ProcessedLen���Ѿ�����������ݳ��ȣ���Ҫ��m_pRcvBuf��ɾ��
		if(ProcessedLen>0&&ProcessedLen<=m_RcvBufLen)
		{
			memmove(m_pRcvBuf,m_pRcvBuf+ProcessedLen,m_RcvBufLen-ProcessedLen);
			m_RcvBufLen-=ProcessedLen;
		}
	}

	m_mutex.unlock();
}
int CTcpLayer::getloginReqData(char* loginReq)
 {
	    long		lCount = 0;
       //����ͷ��Ϣ
		DataHead rHead;
		//��ʼ����ͷ��Ϣ
	    g_InitHead(&rHead);
	//	memcpy(&rHead, &m_rHead, _DataHeadLength_);
        //��ʼд��ƫ����Ϊ��ͷ֮���λ��
		lCount = _DataHeadLength_;

      //�û�������
		lCount += g_String2Mem(loginReq+lCount, "admin", 20);
		lCount += g_String2Mem(loginReq+lCount, "12345", 20);
  
        //������峤�ȼ����ͷ
		rHead.m_ulDataLength = lCount - _DataHeadLength_;
		rHead.m_usDataType = 11001;
        //д���ͷ
		g_Head2Mem(loginReq, &rHead);//����ͷ��Ϣ
		return lCount;
		
 }

int CTcpLayer::getHQReqData(char* pData)
 {
	    long		lCount = 0;
       //����ͷ��Ϣ
		DataHead rHead;
		//��ʼ����ͷ��Ϣ
	    g_InitHead(&rHead);
	//	memcpy(&rHead, &m_rHead, _DataHeadLength_);
        //��ʼд��ƫ����Ϊ��ͷ֮���λ��
		lCount = _DataHeadLength_;
		int index=1;
		int count=12;

		////��ʼλ��
		//lCount += g_Int2Mem(pData+lCount,&index , 2);
		////����λ��
		//lCount += g_Int2Mem(pData+lCount,&index , 2);
		////��ȡ����
		//lCount += g_Int2Mem(pData+lCount,&count , 2);
		//����
		lCount += g_String2Mem(pData+lCount, "0", 1);		
        
        lCount += g_Int2Mem(pData+lCount,&count , 2);
		long data1=3413060;
		long data2=2213051;
		//��������
		lCount += g_Int2Mem(pData+lCount,&data1, 4);
		lCount += g_Int2Mem(pData+lCount,&data2, 4);
	

		//������峤�ȼ����ͷ
		rHead.m_ulDataLength = lCount - _DataHeadLength_;
		rHead.m_usDataType = 30002;
        //д���ͷ
		g_Head2Mem(pData, &rHead);//����ͷ��Ϣ
		return lCount;
		
 }

long CTcpLayer::g_Head2Mem(char* pcData, DataHead* pHead)
{
    //�����������ν�DataHead�ĸ�����Աд���ڴ�
    //ÿ��д����¼д���ڴ��ƫ�������ں���д��
	long lCount = 0;
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_usHeadID), 2);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_usVersion), 2);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_ulDataID), 4);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_usDataType), 2);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_ulDataLength), 4);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_usEncryptType), 2);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_ulEncryptLength), 4);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_usSignatureType), 2);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_ulSignatureLength), 4);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_ulSrcCode), 4);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_ulDestCode), 4);
    //char���������ֽ���ֱ���ڴ渴�Ƽ���
	memcpy(pcData+lCount, &(pHead->m_rStatus.S.m_cStatus), 1);
	lCount += 1;
    //char���������ֽ���ֱ���ڴ渴�Ƽ���
	memcpy(pcData+lCount, &(pHead->m_pcReserve), 15);
	lCount += 15;
	return lCount;
}
long  g_lDataID = 0;
void CTcpLayer::g_InitHead(DataHead* pHead)
{
    //���γ�ʼ��DataHead������Ա
	pHead->m_usHeadID = 0x0158;
	pHead->m_usVersion = 0x0201;
    //m_ulDataIDΪ���ݰ���Ψһ��ʶ���������ظ�
	pHead->m_ulDataID = g_lDataID++;
    if(LONG_MAX == g_lDataID)
    {
        g_lDataID = 0;
    }
	pHead->m_usDataType = 0;
	pHead->m_ulDataLength = 0;
	pHead->m_usEncryptType = 0;
	pHead->m_ulEncryptLength = 0;
	pHead->m_usSignatureType = 0;
	pHead->m_ulSignatureLength = 0;
	pHead->m_ulSrcCode = 0;
	pHead->m_ulDestCode = 0;
	pHead->m_rStatus.S.m_cStatus = 0;
	memset(pHead->m_pcReserve, 0, 15);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ���ַ���д���ڴ�
/// @param pcBuffer[out]��Ҫд����ڴ��ַ
/// @param str[in]������д���ַ���
/// @param nBuffer[in]��д�볤��
/// @retval д�볤��
///
//------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
long CTcpLayer::g_String2Mem(char* pcBuffer, std::string str, int nBuffer)
{
	int nStr = str.length();
    //����ָ������д�룬�ַ��������Զ���ȡ
	if (nBuffer<nStr)
    {
		nStr = nBuffer;
    }
    //д���ַ���
    int i=0;
	for (i=0; i<nStr; i++)
    {
		pcBuffer[i] = str[i];
    }
    //�ַ����������ָ����������׷���㹻�Ŀո�
	for (; i<nBuffer; i++)
    {
		pcBuffer[i] = ' ';
    }
	return long(nBuffer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ��1������д���ڴ�
/// @param pcMem[out]��Ҫд����ڴ�
/// @param pValue[in]��Ҫд������ָ��
/// @param s[in]��д�볤��
/// @retval д�볤��

//--------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
long CTcpLayer::g_Int2Mem(char* pcMem, void* pValue, int s)
{
    //4��long�ĳ���
	if (s==4)
	{
        //ȡ��Ҫд������
		long* plValue = (long*)pValue;
        //���������ֽ���ת��
		long lTrans = (long)htonl(*plValue);
        //������д���ڴ�
		memcpy(pcMem, &lTrans, 4);
	}
    //ֻҪ����long�;���Ϊ��short��
    //2��short�ĳ���
	else
	{
        //ȡ��Ҫд������
		short* psValue = (short*)pValue;
        //���������ֽ���ת��
		short sTrans = (short)htons(*psValue);
        //������д���ڴ�
		memcpy(pcMem, &sTrans, 2);
	}
	return s;
}

long CTcpLayer::g_Mem2Head(DataHead* pHead, char* pcData)
{
    //�����������ζ�ȡDataHead�ĸ�����Ա
    //ÿ�ζ�ȡ���¼��ȡ�ڴ��ƫ�������ں�����ȡ
	long lCount = 0;
	lCount += g_Mem2Int(&(pHead->m_usHeadID), pcData+lCount, 2);
	lCount += g_Mem2Int(&(pHead->m_usVersion), pcData+lCount, 2);
	lCount += g_Mem2Int(&(pHead->m_ulDataID), pcData+lCount, 4);
	lCount += g_Mem2Int(&(pHead->m_usDataType), pcData+lCount, 2);
	lCount += g_Mem2Int(&(pHead->m_ulDataLength), pcData+lCount, 4);
	lCount += g_Mem2Int(&(pHead->m_usEncryptType), pcData+lCount, 2);
	lCount += g_Mem2Int(&(pHead->m_ulEncryptLength), pcData+lCount, 4);
	lCount += g_Mem2Int(&(pHead->m_usSignatureType), pcData+lCount, 2);
	lCount += g_Mem2Int(&(pHead->m_ulSignatureLength), pcData+lCount, 4);
	lCount += g_Mem2Int(&(pHead->m_ulSrcCode), pcData+lCount, 4);
	lCount += g_Mem2Int(&(pHead->m_ulDestCode), pcData+lCount, 4);
    //char���������ֽ���ֱ���ڴ渴�Ƽ���
	memcpy(&(pHead->m_rStatus.S.m_cStatus), pcData+lCount, 1);
	lCount += 1;
    //char���������ֽ���ֱ���ڴ渴�Ƽ���
	memcpy(&(pHead->m_pcReserve), pcData+lCount, 15);
	lCount += 15;
	return lCount;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ���ڴ��ȡ1������
/// @param pValue[out]����ȡ�Ľ��
/// @param pcMem[in]��Ҫ��ȡ���ڴ��ַ
/// @param s[in]����ȡ����
/// @retval ��ȡ����
//--------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
long CTcpLayer::g_Mem2Int(void* pValue, char* pcMem, int s)
{
    //4��long�ĳ���
	if (s==4)
	{
		long* plValue = (long*)pValue;		
		long lTrans;
        //��long�Ͷ�ȡһ��long���ȵ��ڴ�
		memcpy(&lTrans, pcMem, 4);
        //����ȡ������봫����ָ�����
		*plValue = (long)(htonl(lTrans));
	}
    //ֻҪ����long�;���Ϊ��short��
    //2��short�ĳ���
	else
	{
		short* psValue = (short*)pValue;		
		short sTrans;
        //��short�Ͷ�ȡһ��short���ȵ��ڴ�
		memcpy(&sTrans, pcMem, 2);
        //����ȡ������봫����ָ�����
		*psValue = (short)(htons(sTrans));
	}
	return s;
}

typedef BOOL	(__stdcall *EnumProcessesFunc)(DWORD *lpidProcess,DWORD cb,DWORD *cbNeeded);
typedef DWORD	(__stdcall *GetModuleBaseNameFunc)(HANDLE hProcess,HMODULE hModule,LPTSTR lpBaseName,DWORD nSize);
typedef BOOL	(__stdcall *EnumProcessModulesFunc)(HANDLE hProcess,HMODULE *lphModule,DWORD cb,LPDWORD lpcbNeeded);
HANDLE	m_hPsapi=NULL;													//LoadLibrary("psapi",)�ľ��
HANDLE  m_pEnumProcesses=NULL;
HANDLE			m_pEnumProcessModules=NULL;
HANDLE  m_pGetModuleBaseName=NULL;
 // �ҽ���ID��
//	�ж��Ƿ����ָ��ģ�����ƵĽ���
//	iModuleName��ģ������
//	pProcessID������ģ���Ӧ�Ľ��̵�ID
//	����ֵ��TRUE ���̴��ڣ�FALSE ���̲�����
BOOL CTcpLayer::IsProcessExist(const char *iModuleName,DWORD *pProcessID)
{
	char			ModuleName[256];
	char			iBaseName[256];
	char			BaseName[256];
	int				i,iLen;
	DWORD			ProcessID[256];
	BOOL			bRlt,bExist;
	DWORD			cbNeeded,cbReturned;
	HANDLE			hProcess;
	HMODULE			hModule;
																				//�ȷ����������ģ���ļ�����
	bRlt=CTools_Ansi::GetPureFileName(iModuleName,ModuleName,sizeof(ModuleName));
	if(!bRlt) return FALSE;
	if(!ModuleName[0]) return FALSE;

	m_hPsapi=(HMODULE)LoadLibrary("psapi");
	if(m_hPsapi)
	{
		m_pEnumProcesses=(HANDLE)GetProcAddress((HMODULE)m_hPsapi,"EnumProcesses");   
		m_pEnumProcessModules=(HANDLE)GetProcAddress((HMODULE)m_hPsapi,"EnumProcessModules");   

		m_pGetModuleBaseName=(HANDLE)GetProcAddress((HMODULE)m_hPsapi,"GetModuleBaseNameA");   

	}


	//if(!m_pEnumProcesses) return 0;
	//if(!m_pGetModuleBaseName) return 0;
	//if(!m_pEnumProcessModules) return FALSE;
																				//ö�ٽ���
	memset((char*)ProcessID,0,sizeof(ProcessID));
	bRlt=((EnumProcessesFunc)m_pEnumProcesses)(ProcessID,sizeof(ProcessID),&cbNeeded);
	if(!bRlt) return FALSE;

	bExist=FALSE;
	for(i=0;!bExist&&i<(int)(cbNeeded/sizeof(DWORD));i++)
	{																			//��ȡ���̾��
		hProcess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,ProcessID[i]);
		if(hProcess)
		{																		
			bRlt=((EnumProcessModulesFunc)m_pEnumProcessModules)(hProcess,&hModule,sizeof(hModule),&cbReturned);
			if(bRlt)
			{
				iLen=((GetModuleBaseNameFunc)m_pGetModuleBaseName)(hProcess,hModule,iBaseName,sizeof(iBaseName)-1);
				if(iLen>0)
				{
					memset(BaseName,0,sizeof(BaseName));
					bRlt=CTools_Ansi::GetPureFileName(iBaseName,BaseName,sizeof(BaseName)-1);
					if(stricmp(ModuleName,BaseName)==0) 
					{
						if(pProcessID) *pProcessID=ProcessID[i];
						bExist=TRUE;
					}
				}
			}
			CloseHandle(hProcess);
		}
	}

	return bExist;
}
