// VariablePackage.h: interface for the CVariablePackage class.
//
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
//			6. m_bEmptyID
//				����Ҫ����Ϊ���水���λ�����������Ҫͨ��m_bEmptyID�ж��Ƿ�Ϊ��
//			7. �̰߳�ȫ��
//				�������ݺͶ�ȡ���ݵĲ����̰߳�ȫ�����Զ���߳�ͬʱ��ȡ��ͬʱ���档
//			8. ��������ָ�룺GetOff��SetOff��GetOff����SetOff��ʾΪ�գ�GetOff����׷SetOff�������ܳ�������SetOff����׷��GetOff
//			9. ע�⣬�������������ʱ�����̲߳���ȫ�ġ����߳�Ҫע�Ᵽ�� 
//
//	history 
//	20100119	lzq		�������ļ�
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARIABLEPACKAGE_H__C9312ED9_E58A_4C11_8758_3EDE51E2D61A__INCLUDED_)
#define AFX_VARIABLEPACKAGE_H__C9312ED9_E58A_4C11_8758_3EDE51E2D61A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable : 4996)


#include <windows.h>


#define PREFIX_VARIABLEPACKAGE		0xa973b72c					//Package��ʼ��־
#define TAIL_VARIABLEPACKAGE		0x5a7b9c3d					//Package������־


//���涨��VariablePackage��ͷ�ṹ
typedef struct
{
	DWORD	Prefix;						                    //ǰ����־�����ڱ�־һ��package�Ŀ�ʼ
	int		DataLen;					                    //���ݳ��ȣ�ָ���ݵĳ��ȡ�ע���������ݵĴ�С�ǰ�DataLen�������㲹����ʹ֮Ϊ4�ֽڱ߽硣
	DWORD	dwCRC;						                    //���ݵ������ͣ����ݰ���PackageHead��Data��Tail
	DWORD	dwSeq;						                    //ͬһ��Package�����к�

	DWORD	UserData1;
	DWORD	UserData2;
	DWORD	UserData3;
	DWORD	UserData4;
}VariablePackageHeadStru;


class CVariablePackage  
{

public:	
	/**************************************************************************************************//**
	*   @brief				CVariablePackage        
	*   @param[in] CacheSize	���ֽ���
	*   @param[in] Mode			����ģʽ:
	*   - 0 ���̶߳�,���߳�д
	*   - 3 ���̶߳�,���߳�д
	*******************************************************************************************************/
	CVariablePackage(int CacheSize, int Mode=3);	
	virtual ~CVariablePackage();

	//��һ�����ݴ��뻺�棬����
	bool			SaveDataTo(VariablePackageHeadStru& PackageHead, const void* pData);		
	bool			SaveDataTo(const void* pData,int DataLen,DWORD UserData1=0,DWORD UserData2=0,DWORD UserData3=0,DWORD UserData4=0);

	//�ӻ�����ȡ���ݣ�����				
    int             GetCurPackageDataLen(void);                                 //ȡ��ǰ�������ݳ��ȣ�ע�⣬�����ݵĳ��ȣ������������ĳ���
	bool			PeekFrom(VariablePackageHeadStru& PackageHead,void* pData,int DataSize);//���ַ�������ȡ����ǰ���ݿ飬��ɾ�������DataSize����������ʧ��.���pData==NULL��ֻȡ��ͷ
    bool            PeekFrom(void* pData,int DataSize,DWORD* pUserData1=NULL,DWORD* pUserData2=NULL,DWORD* pUserData3=NULL,DWORD* pUserData4=NULL);//���ַ�������ȡ����ǰ���ݿ飬��ɾ�������DataSize����������ʧ��.���pData==NULL��ֻȡ��ͷ
	bool			GetFrom(VariablePackageHeadStru& PackageHead,void* pData,int DataSize); //�ӻ�����ȡ����ǰ���ݿ飬Ȼ��ɾ��
    bool            GetFrom(void* pData,int DataSize,DWORD* pUserData1=NULL,DWORD* pUserData2=NULL,DWORD* pUserData3=NULL,DWORD* pUserData4=NULL);//���ַ�������ȡ����ǰ���ݿ飬Ȼ��ɾ�������DataSize����������ʧ��.���pData==NULL��ֻȡ��ͷ
	void			DeleteCurPackage(void);										//ɾ��һ�����ݿ�
	void			Empty(void);												//��ջ��������ɶ�ȡ�̵߳���
	int				GetCacheSize(void);

	//������������������
	bool			IsEmpty(void);
    void            GetPosition(int& GetOff,int& SetOff);

public:
	 


private:
	char*			m_pCache;													//�ڴ滺��ʱ��ָ��
	int				m_CacheSize;												//Cache�ĳߴ磬�ֽ�����������4��������
	int				m_GetOff;													//���ػ�����ȡ���ݵĵ�ǰ�ֽ�λ�ã������Cacheͷ
	int				m_SetOff;													//���ػ���������ݵĵ�ǰ�ֽ�λ�ã������Cacheͷ
	int				m_Mode;														//����ģʽ,�μ����캯���Ĳ���˵��
	char			*m_pSem;													//��д������
	DWORD			m_Seq;

	int				ReadData(int Off,char* pData,int ReadLen) const;			//��ָ��λ�ö�һ�����ݣ����ǻ��δ洢��iOff�����λ��
	int				WriteData(int Off, const char* pData,int WriteLen);			//��ָ��λ��дһ�����ݣ����ǻ��δ洢��iOff�����λ��
	void			IgnoreInvalidPackage(void);									//������Ч������

																				//����������������������ʵ�����ǣ�Ŀ���ǽ�ֹ��������͸�ֵ
	CVariablePackage(const CVariablePackage&);									//Ϊ�˴���������
	CVariablePackage& operator= (const CVariablePackage&);						//��ֵ������

};

#endif // !defined(AFX_VARIABLEPACKAGE_H__C9312ED9_E58A_4C11_8758_3EDE51E2D61A__INCLUDED_)
