#pragma once

#include <string>
#include <queue>
#include <vector>
#include <map>
#include "UserApiStruct.h"
#include "easymutex.h"

class CForceCloseInfoData
{
public:
    static CForceCloseInfoData* GetInstance();
    static void DestroyInstance();

private:
    CForceCloseInfoData(void);
    virtual ~CForceCloseInfoData(void);

public:
    
    // LockObject()��UnloadObject()��������������ɶԵ���
    void LockObject();
    void UnlockObject();

    //ͨ����Լ����ͷ�������سֲ���Ϣ
    bool GetPositionFieldByInstrumentIDandDirect(std::string strInstrumentID,std::string nDirect);
    SequencialPositionField curPositionField;
    //ͨ����Լ������������������Ϣ
    bool GetDepthMarketDataFieldByInstrumentID(std::string strInstrumentID);
    DepthMarketDataField curDepthMarketDataField;
    

    std::queue<SequencialPositionField>  queuePositionInfo;
    std::vector<SequencialPositionField> vecPositionInfo;
    std::queue<SequencialOrderField>     queueOrderInfo;//�ҵ���Ϣ
    std::vector<DepthMarketDataField>    vecDepthMarketDataInfo;//�ֶ�ǿƽʱ�õ�������ò���
    std::queue<RiskSyncAccountField>     queueFundInfo;
    
private:
    static CForceCloseInfoData* m_pInstance;
    Ceasymutex m_oMutex;

};

