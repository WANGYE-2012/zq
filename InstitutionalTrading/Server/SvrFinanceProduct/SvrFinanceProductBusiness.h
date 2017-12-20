#pragma once
#include "rwlock.h"
#include "interface_svrfinanceproduct.h"

class CSvrFinanceProductBusiness : public CInterface_SvrFinanceProduct
{

public:
    static CSvrFinanceProductBusiness& getObj(void);
    static void deleteObj();

    CF_ERROR ResponseToAsk(const eAskType& neDataType,const eAskOperationType& neChangeType,const int nllDataID,const char * npDataBuf);

    void SetServerRunMode(eServerRunMode nMode);
    char* GetData(int& nDataLen);
    void  SetData(	std::map<int,FinancialProduct>& mapFinancialProduct,
                    std::map<int,std::vector<int>>& mapProductID2TraderIDsRelation);

    //��Ʋ�Ʒ
    int AddFinancialProduct(const int nUserID, FinancialProduct *pFP, int& nID);
    int DeleteFinancialProduct(const int nUserID, int nID);
    int ModifyFinancialProduct(const int nUserID, FinancialProduct *pFP);    


    //��Ʋ�Ʒ�뽻��Ա�Ĺ�ϵ
    int SaveProductTraderRelation(const int nUserID, int nRelationType, int nID,std::vector<int> vecID);

    //ģ���ӿ�
    void GetFinancialProduct(std::vector<FinancialProduct>& vecFP);
    void GetTraderProRelation(std::vector<TraderProRelation>& vecRela);
    void GetTraderByFinancialProductID(int nFinancialProductID,std::vector<int>& vecTD);


public:
    //�����ݿ��������
    bool LoadDataFromDB();

private:   
    //��ȡ��Ʋ�Ʒ
    bool LoadFinancialProductFromDB();
    //��ȡ��Ʋ�Ʒ�뽻��Ա�Ĺ�ϵ
    bool LoadTraderAndProRelationFromDB();

private:
    CSvrFinanceProductBusiness(void);
    virtual ~CSvrFinanceProductBusiness(void);
    static CSvrFinanceProductBusiness* m_pObj;

private:

    //��������Ʋ�ƷIDΪ��������Ʋ�Ʒ
    std::map<int, FinancialProduct>		m_mapFinancialProduct;
    //��������Ʋ�ƷIDΪ��������Ʋ�Ʒ�ͽ���ԱID��ϵ
    std::map<int, std::vector<int>>		m_mapProductID2TraderIDsRelation;

    /*
           sign      Year        Month        Day                   id   
    32bit  |-1-|------6------|-----5-----|-----5-----|--------------15---------------|  
    */
    int m_nNextFinancialProductID;   

    rwlock_t    m_Lock;

    eServerRunMode m_ServerRunMode;
};
