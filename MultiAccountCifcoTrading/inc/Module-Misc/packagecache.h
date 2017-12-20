//*******************************************************************************************
//	���ã�	���岻�������Ļ�����
//	������	1. �������ڽ����������ݿ黺��
//			2. ���ݽṹΪ��Package = [ PackageHead + Data + Tail ]
//			3. Dataռ�õĿռ䰴4�ֽ����㲹��
//			4. ��ѭ�Ƚ��ȳ���ԭ��
//			5. ��Ҫ�ⲿ�ӿڣ�
//				SaveDataTo()	����һ�����ݴ��뻺��
//				GetDataFrom()	���ӻ�����ȡ����ǰ���ݿ�
//				GetCurDataLen()	��ȡ��ǰ���ݿ�ĳ���
//				IsEmpty()		�������Ƿ�Ϊ��
//			6. ����ӿڣ�
//				SetCacheParam()	�����û�������������ļ����棬�����ļ��������ߴ磻�����ڴ滺�棬�������ߴ�
//			7. m_bEmptyID
//				����Ҫ����Ϊ���水���λ�����������Ҫͨ��m_bEmptyID�ж��Ƿ�Ϊ��
//			8. ���̰߳�ȫ��
//				�������ݺͶ�ȡ���ݵĲ������Էֱ���������߳�������ܶ���߳�ͬʱ��ȡ��ͬʱ���档
//			9. ��������ָ�룺GetOff��SetOff��GetOff����SetOff��ʾΪ�գ�GetOff����׷SetOff�������ܳ�������SetOff����׷��GetOff
//
//	history 
//	20100119	lzq		�������ļ�
//
//*******************************************************************************************

#pragma once

#pragma warning(disable : 4311)
#pragma warning(disable : 4312)
#pragma warning(disable : 4996)


//#include "toolpublic.h"

#include "MutexLock.h"

#define PREFIX_PACKAGECACHE		0xa973b71c					//Package��ʼ��־
#define TAIL_PACKAGECACHE		0x5a7b9c2d					//Package������־




//���涨��Package��ͷ�ṹ
typedef struct
{
	DWORD	Prefix;						                    //ǰ����־�����ڱ�־һ��package�Ŀ�ʼ
	int		CmdID;						                    //Package������ID��������CMDID_xxxx
	int		PlatformID;					                    //����ƽ̨ID��������PLATFORM_xxxx
	int		ExchangeID;					                    //������ID��������EXCHANGE_xxxx

	int		DataLen;					                    //���ݳ��ȣ�ָ���ݵĳ��ȡ�ע���������ݵĴ�С�ǰ�DataLen�������㲹����ʹ֮Ϊ4�ֽڱ߽硣
	DWORD	dwCRC;						                    //���ݵ������ͣ����ݰ���PackageHead��Data��Tail
	DWORD	dwSeq;						                    //ͬһ��Package�����к�
}PackageHeadStru;



//	������package������
class CPackageCache
{
public:	
	/**************************************************************************************************//**
	*   @brief				CPackageCache��Ĺ��캯��        
	*   @param[in] CacheSize	���ֽ���
	*   @param[in] Mode			����ģʽ:
	*   - 0 ���̶߳�,���߳�д
	*   - 1 ���̶߳�,���߳�д
	*   - 2 ���̶߳�,���߳�д
	*   - 3������ ���̶߳�,���߳�д
	*******************************************************************************************************/
	CPackageCache(int CacheSize, int Mode=1);	

	~CPackageCache();


																				//���溯���ɱ����̵߳���
	bool			SaveDataTo(PackageHeadStru* pPackageHead, const void* pData,int* pOriSetOff=NULL,int* pNewSetOff=NULL);		//��һ�����ݴ��뻺��
	bool			SaveDataTo(int CmdID,int PlatformID,int ExchangeID,DWORD dwSeq,int DataLen, const void* pData,int* pOriSetOff=NULL,int* pNewSetOff=NULL);	//��һ�����ݴ��뻺��

																				//���溯���ɶ�ȡ�̵߳���				
    int             GetCurPackageDataLen(void);                                 //ȡ��ǰ�������ݳ��ȣ�ע�⣬�����ݵĳ��ȣ������������ĳ���
	bool			PeekFrom(PackageHeadStru* pPackageHead,void* pData,int DataSize,int* pGetOff=NULL);//���ַ�������ȡ����ǰ���ݿ飬��ɾ�������DataSize����������ʧ��.���pData==NULL��ֻȡ��ͷ
    bool            PeekFrom(int&CmdID,int&PlatformID,int& ExchangeID,DWORD& dwSeq,int& DataLen,void* pData,int DataSize,int* pGetOff=NULL);//���ַ�������ȡ����ǰ���ݿ飬��ɾ�������DataSize����������ʧ��.���pData==NULL��ֻȡ��ͷ
	bool			GetFrom(PackageHeadStru* pPackageHead,void* pData,int DataSize,int* pOriGetOff=NULL,int* pNewGetOff=NULL); //�ӻ�����ȡ����ǰ���ݿ�
	void			DeleteCurPackage(int* pOriGetOff=NULL,int* pNewGetOff=NULL);										//ɾ��һ�����ݿ�
	bool			IsEmpty(void);												//���ػ��������Ƿ�Ϊ��
	void			Empty(void);												//��ջ��������ɶ�ȡ�̵߳���
    void            GetPosition(int& GetOff,int& SetOff);

public:
	 
	/**************************************************************************************************//**
    *   @brief		��ָ�����ݶ��룬���벿����0��Ȼ�������Package��β   
    *   @param[in]	pData ָ�������� 
	*   @param[in]	offset	����ƫ��
	*   @param[in]	AlignedBytes ��ָ���ֽ������룬Ŀǰֻ֧��4�ֽڶ���
    *   @return		�ܹ�����ֽ���������TAIL_PACKAGECACHE��ǵĳ��ȣ�sizeof(DWORD)��
    *******************************************************************************************************/
	static inline unsigned int FillPackageTail(void * pData, unsigned int offset, unsigned int AlignedBytes=4)
	{
		unsigned int dataAlignedLen = (offset+3)&(~3);
		unsigned int filledLen=dataAlignedLen-offset;
		if(filledLen>0)
			memset(((char*)pData)+offset, 0, filledLen);

		*(DWORD*)(((char*)pData)+dataAlignedLen) = TAIL_PACKAGECACHE;

		return (filledLen+sizeof(DWORD));
	};



private:
	char*			m_pCache;													//�ڴ滺��ʱ��ָ��
	int				m_CacheSize;												//Cache�ĳߴ磬�ֽ�����������4��������
	int				m_GetOff;													//���ػ�����ȡ���ݵĵ�ǰ�ֽ�λ�ã������Cacheͷ
	int				m_SetOff;													//���ػ���������ݵĵ�ǰ�ֽ�λ�ã������Cacheͷ

	void			SetCacheParam(int CacheSize);								//���û���Ĳ������������ߴ�
	int				ReadData(int Off,char* pData,int ReadLen) const;					//��ָ��λ�ö�һ�����ݣ����ǻ��δ洢��iOff�����λ��
	int				WriteData(int Off, const char* pData,int WriteLen);				//��ָ��λ��дһ�����ݣ����ǻ��δ洢��iOff�����λ��
	int				m_Mode;														//����ģʽ,�μ����캯���Ĳ���˵��
	MutexLock       *m_pReadMutexLock;											//��������
	MutexLock       *m_pWriteMutexLock;											//д������

	//�˺�����ǰ��public����
	void			IgnoreInvalidPackage(void);									//������Ч������

																				//����������������������ʵ�����ǣ�Ŀ���ǽ�ֹ��������͸�ֵ
	CPackageCache(const CPackageCache&);										//Ϊ�˴���������
	CPackageCache& operator= (const CPackageCache&);							//��ֵ������

};



