// StreamCache.h: interface for the CMemoryCache class.
//
//	���ã�	�����ַ���������
//	������	1. �������ڽ��ַ�������
//			2. ��ѭ�Ƚ��ȳ���ԭ��
//			3. ��Ҫ�ⲿ�ӿڣ�
//				SaveDataTo()	����һ�����ݴ��뻺��
//				GetDataFrom()	���ӻ�����ȡ����ǰ���ݿ�
//				GetCurDataLen()	��ȡ��ǰ���ݿ�ĳ���
//				IsEmpty()		�������Ƿ�Ϊ��
//			4. m_bEmptyID
//				����Ҫ����Ϊ���水���λ�����������Ҫͨ��m_bEmptyID�ж��Ƿ�Ϊ��
//			5. �̰߳�ȫ��
//				�������ݺͶ�ȡ���ݵĲ����̰߳�ȫ�����Զ���߳�ͬʱ��ȡ��ͬʱ���档
//			6. ��������ָ�룺GetOff��SetOff��GetOff����SetOff��ʾΪ�գ�GetOff����׷SetOff�������ܳ�������SetOff����׷��GetOff
//			7. ע�⣬�������������ʱ�����̲߳���ȫ�ġ����߳�Ҫע�Ᵽ�� 
//
//	history 
//	20100119	lzq		�������ļ�
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREAMCACHE_H__BD4088B5_55BB_414B_99FD_99A903F1A9E3__INCLUDED_)
#define AFX_STREAMCACHE_H__BD4088B5_55BB_414B_99FD_99A903F1A9E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable : 4311)
#pragma warning(disable : 4312)
#pragma warning(disable : 4996)

#ifdef WIN32
#pragma managed(push,off)
#endif 



class CMemoryCache  
{
public:
	/**************************************************************************************************//**
	*   @brief				CMemoryCache��Ĺ��캯��        
	*   @param[in] CacheSize	��������С
	*   @param[in] Mode			����ģʽ:
	*   - 0 ���̶߳�,���߳�д
	*   - 3 ���̶߳�,���߳�д
	*******************************************************************************************************/
	CMemoryCache(int CacheSize,int Mode=3);
	virtual ~CMemoryCache();


	//��һ�����ݴ��뻺�棬����
	bool			SaveDataTo(void *pData,int DataLen);

	//�ӻ�����ȡ���ݣ�����				
	int				GetLen(void);												//ȡ��ǰ���������ݵĳ���
	int				GetPos_SubStr(const char* pSubStr);							//ȡ�Ӵ���λ�ã��Ӵ���0��β
	bool			PeekFrom(void *pData,int PeekLen);							//�ӱ��ػ�����ȡ��ָ�����ȵ����ݣ���ɾ��
	bool			GetFrom(void *pData,int GetLen);							//�ӻ�����ȡ��ָ�����ȵ����ݣ����ӻ�����ɾ��
	void			DeleteData(int DeleteLen);									//ɾ��ָ�����ȵ�����
	void			Empty(void);												//��ջ��������ɶ�ȡ�̵߳���

	//���漸������������
	bool			IsEmpty(void);												//���ػ��������Ƿ�Ϊ��
    void            GetPosition(int& GetOrd,int& SetOrd);
	int				GetCacheSize(void);											//ȡCache�ĳߴ�

private:
	char			*m_pData;													//�ڴ滺��ʱ��ָ��
	int				m_CacheSize;
	int				m_GetOff;													//���ػ�����ȡ���ݵĵ�ǰBlock�ı�ţ�������ļ�ͷ��ע�⣬�����ֽڱ��
	int				m_SetOff;													//���ػ���������ݵĵ�ǰBlock�ı�ţ�������ļ�ͷ��ע�⣬�����ֽڱ��
	int				m_Mode;														//����ģʽ,�μ����캯���Ĳ���˵��
	char			*m_pSem;													//��д������


	int				GetLen_Internal(void);										//ȡ��ǰ���������ݵĳ��ȣ��ڲ�����
	int				ReadDate_Internal(int iOff,void*pData,int ReadLen);			//��ָ��λ�ö�һ�����ݣ����ǻ��δ洢��iOff�����λ��
	int				WriteData_Internal(int iOff,void*pData,int WriteLen);		//��ָ��λ��дһ�����ݣ����ǻ��δ洢��iOff�����λ��

																				//����������������������ʵ�����ǣ�Ŀ���ǽ�ֹ��������͸�ֵ
	CMemoryCache(const CMemoryCache&);											//Ϊ�˴���������
	CMemoryCache& operator= (const CMemoryCache&);								//��ֵ������

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_STREAMCACHE_H__BD4088B5_55BB_414B_99FD_99A903F1A9E3__INCLUDED_)
