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

#include <string.h>

#ifdef WIN32
#pragma managed(push,off)
#endif 


#define PREFIX_VARIABLEPACKAGE		0xa973b72c					//Package��ʼ��־
#define TAIL_VARIABLEPACKAGE		0x5a7b9c3d					//Package������־




class CVariablePackage  
{
public:
	//���涨��VariablePackage��ͷ�ṹ
	struct HeadStru
	{
		HeadStru()
		{
			clear();
		};
		void clear(void)
		{
			memset(this,0,sizeof(*this));
		};

		unsigned int	Prefix;										//ǰ����־�����ڱ�־һ��package�Ŀ�ʼ
		unsigned int	CmdID;                                      //������
		int				DataLen1;					                //���ݿ�1���ȣ�ָ���ݵĳ��ȡ�ע���������ݵĴ�С�ǰ�DataLen�������㲹����ʹ֮Ϊ4�ֽڱ߽硣
		int				DataLen2;					                //���ݿ�2���ȣ�ָ���ݵĳ��ȡ�ע���������ݵĴ�С�ǰ�DataLen�������㲹����ʹ֮Ϊ4�ֽڱ߽硣
		unsigned int	dwCRC;						                //���ݵ������ͣ����ݰ���PackageHead��Data��Tail
		unsigned int	dwSeq;						                //ͬһ��Package�����к�

		unsigned int	UserData1;
		unsigned int	UserData2;
		unsigned int	UserData3;
		unsigned int	UserData4;
	};

public:	
	/**************************************************************************************************//**
	*   @brief				CVariablePackage        
	*   @param[in] CacheSize	���ֽ���
	*   @param[in] Mode			����ģʽ:
	*   - 0 ���̶߳�,���߳�д��������
	*   - 3 ���̶߳�,���߳�д������
	*******************************************************************************************************/
	CVariablePackage(int CacheSize, int Mode=3);	
	virtual ~CVariablePackage();

	//��һ�����ݴ��뻺�棬����
	bool    SaveDataTo(HeadStru& PackageHead, const void* pData1, const void*pData2=NULL);		
	bool	SaveDataTo2(unsigned int CmdID,const void* pData1,int DataLen1,const void* pData2=NULL,int DataLen2=0,unsigned int UserData1=0,unsigned int UserData2=0,unsigned int UserData3=0,unsigned int UserData4=0);

    //ȡ��ǰ�������ݳ��ȣ�ע�⣬�����ݵĳ��ȣ������������ĳ���
    int     GetCurPackageDataLen(void);                                 
    //���ַ�������ȡ����ǰ���ݿ飬��ɾ�������DataSize����������ʧ��.���pData==NULL��ֻȡ��ͷ
	bool	PeekFrom(HeadStru& PackageHead,void* pData1,int DataSize1,void* pData2=NULL,int DataSize2=0);
    //���ַ�������ȡ����ǰ���ݿ飬��ɾ�������DataSize����������ʧ��.���pData==NULL��ֻȡ��ͷ
    HeadStru PeekFrom(void* pData1,int DataSize1,void* pData2=NULL,int DataSize2=0);
    //�ӻ�����ȡ����ǰ���ݿ飬Ȼ��ɾ��
	bool	GetFrom(HeadStru& PackageHead,void* pData1,int DataSize1,void* pData2=NULL,int DataSize2=0); 
    //���ַ�������ȡ����ǰ���ݿ飬Ȼ��ɾ�������DataSize����������ʧ��.���pData==NULL��ֻȡ��ͷ
    HeadStru GetFrom(void* pData1,int DataSize1,void* pData2=NULL,int DataSize2=0);
    //ɾ��һ�����ݿ�
	void	DeleteCurPackage(void);										
    //��ջ��������ɶ�ȡ�̵߳���
	void	Empty(void);												
	int		GetCacheSize(void);

	//������������������
	bool	IsEmpty(void);
    void    GetPosition(int& GetOff,int& SetOff);

public:
	 


private:
	char*			m_pCache;													//�ڴ滺��ʱ��ָ��
	int				m_CacheSize;												//Cache�ĳߴ磬�ֽ�����������4��������
	int				m_GetOff;													//���ػ�����ȡ���ݵĵ�ǰ�ֽ�λ�ã������Cacheͷ
	int				m_SetOff;													//���ػ���������ݵĵ�ǰ�ֽ�λ�ã������Cacheͷ
	int				m_Mode;														//����ģʽ,�μ����캯���Ĳ���˵��
	char			*m_pSem;													//��д������
	unsigned int	m_Seq;

	int				ReadData(int Off,char* pData,int ReadLen) const;			//��ָ��λ�ö�һ�����ݣ����ǻ��δ洢��iOff�����λ��
	int				WriteData(int Off, const char* pData,int WriteLen);			//��ָ��λ��дһ�����ݣ����ǻ��δ洢��iOff�����λ��
	void			IgnoreInvalidPackage(void);									//������Ч������

																				//����������������������ʵ�����ǣ�Ŀ���ǽ�ֹ��������͸�ֵ
	CVariablePackage(const CVariablePackage&);									//Ϊ�˴���������
	CVariablePackage& operator= (const CVariablePackage&);						//��ֵ������

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_VARIABLEPACKAGE_H__C9312ED9_E58A_4C11_8758_3EDE51E2D61A__INCLUDED_)
