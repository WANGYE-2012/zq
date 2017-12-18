// FixedPackage.h: interface for the CFixedPackage class.
//
//	���ã�	���嶨�����Ļ�����
//	������	1. �������ڽ��������ݿ黺��
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

#if !defined(AFX_FIXEDPACKAGE_H__0343E8B0_EE88_48E2_9247_C821BCF36900__INCLUDED_)
#define AFX_FIXEDPACKAGE_H__0343E8B0_EE88_48E2_9247_C821BCF36900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable : 4311)
#pragma warning(disable : 4312)
#pragma warning(disable : 4996)


#ifdef WIN32
#pragma managed(push,off)
#endif 

class CFixedPackage  
{
public:	
	/**************************************************************************************************//**
	*   @brief				CBlockCache��Ĺ��캯��        
	*   @param[in] BlockSize	ÿ�����ݿ���ֽ���
	*   @param[in] BlockCount	���ݿ���ܸ���
	*   @param[in] Mode			����ģʽ:
	*   - 0 ���̶߳�,���߳�д
	*   - 3 ���̶߳�,���߳�д
	*******************************************************************************************************/
	CFixedPackage(int BlockSize, int BlockCount=10000, int Mode=3);	
	virtual ~CFixedPackage();

	//��һ�����ݴ��뻺�棬����
	bool			SaveDataTo(void *pBlockData);

	//�ӻ�����ȡ���ݣ�����				
	int				GetBlockCount(void);										//ȡ��ǰ���ݿ�ĸ���
	bool			PeekFrom(void *pBlockData);									//�ӱ��ػ����ļ���ȡ����ǰ���ݿ�
	bool			GetFrom(void *pBlockData);									//�ӻ�����ȡ����ǰ���ݿ�
	void			DeleteCurBlock(void);										//ɾ��һ�����ݿ�
	void			Empty(void);												//��ջ��������ɶ�ȡ�̵߳���

	//���漸������������
	bool			IsEmpty(void);												//���ػ��������Ƿ�Ϊ��
    void            GetPosition(int& GetOrd,int& SetOrd);
	int				GetBlockSize(void);											//ȡ��ĳߴ�

private:
	char			*m_pData;													//�ڴ滺��ʱ��ָ��
	int				m_BlockSize;
	int				m_BlockCount;
	int				m_GetOrd;													//���ػ�����ȡ���ݵĵ�ǰBlock�ı�ţ�������ļ�ͷ��ע�⣬�����ֽڱ��
	int				m_SetOrd;													//���ػ���������ݵĵ�ǰBlock�ı�ţ�������ļ�ͷ��ע�⣬�����ֽڱ��
	int				m_Mode;														//����ģʽ,�μ����캯���Ĳ���˵��
	char			*m_pSem;													//��д������


	bool			ReadDate(int Ord,char*pData);								//��ָ��λ�ö�һ�����ݣ����ǻ��δ洢��iOff�����λ��
	bool			WriteData(int Ord,char*pData);								//��ָ��λ��дһ�����ݣ����ǻ��δ洢��iOff�����λ��

																				//����������������������ʵ�����ǣ�Ŀ���ǽ�ֹ��������͸�ֵ
	CFixedPackage(const CFixedPackage&);										//Ϊ�˴���������
	CFixedPackage& operator= (const CFixedPackage&);							//��ֵ������


};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_FIXEDPACKAGE_H__0343E8B0_EE88_48E2_9247_C821BCF36900__INCLUDED_)
