// NationalInstPromptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArbitrageDelegation.h"
#include "ArbitrageDelegationDlg.h"

#include <io.h> 
#include "Tools.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable : 4800)

CArbitrageDelegationDlg* CArbitrageDelegationDlg::m_pMainDlg = NULL;
bool CArbitrageDelegationDlg::m_bRevoke = false;
bool CArbitrageDelegationDlg::m_bTrigger = false;


#define TIMER_ISNTSUBED 100

int CArbitrageDelegationDlg::PluginsAPICallBack(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId)
{
    switch(nCmdId)
    {
    case CMDID_CONSOLE_LOGON_RSP:
        {
            //��¼����
            if(nErrorCode==SUCCESS)
            {
                m_pMainDlg->m_bLogon=true;

                
                PlusinAPI* pAPI=PlusinAPI::GetInstance();
                if(pAPI)
                {
                    //��ѯ�˺�
                    pAPI->Send(CMDID_ACCOUNT_CURRENT_ACCOUNT_QUERY,NULL,0);

                    //���������ļ�����ĺ�Լ
                    m_pMainDlg->SubscribeInstrument();
                }
            }
            else 
                m_pMainDlg->m_bLogon=false;

            if(m_pMainDlg->m_pLog)
            {
                char buf[256];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"PlusinAPI ��¼%s\n",m_pMainDlg->m_bLogon?"�ɹ�":"ʧ��");

                m_pMainDlg->m_pLog->Log(buf);
            }

            break;
        }
    case CMDID_CONSOLE_CLOSE:
        {
            ::PostMessage(m_pMainDlg->GetSafeHwnd(), WM_CLOSE, 0, NULL);
            break;
        }
    //��ѯ�˺ŷ���
    case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
        {            
            if(nErrorCode==SUCCESS&&nDataLen==(int)sizeof(CommonAccountIDType))
            {
                CommonAccountIDType& pAccount=*(CommonAccountIDType*)pData;
                if(pAccount[sizeof(CommonAccountIDType)-1]!=0)
                    pAccount[sizeof(CommonAccountIDType)-1]=0;
                m_pMainDlg->m_szAccount=string(pAccount); 

                if(m_pMainDlg->m_pLog)
                {
                    char buf[256];
                    memset(buf,0,sizeof(buf));
                    sprintf(buf,"��ѯ�˺ŷ��� Account=%s\n",m_pMainDlg->m_szAccount.c_str());

                    m_pMainDlg->m_pLog->Log(buf);
                }

                //��ѯ������
                PlusinAPI* pAPI=PlusinAPI::GetInstance();
                if(pAPI)
                    pAPI->Send(CMDID_EXCHANGE_LIST_QUERY,NULL,0);
            }
            break;
        } 
    //��ѯ����������
    case CMDID_EXCHANGE_LIST_RSP:
        {            
            if(nErrorCode==SUCCESS)
            {
                char* pExchangeData = (char*)pData;

                int nCount = nDataLen/sizeof(CommonExchangeIDType);

                char ExchangeID[sizeof(CommonExchangeIDType)];
                set<string> setExchangeID;
                for(int i=0;i<nCount;i++)
                {
                    memset(ExchangeID,0,sizeof(CommonExchangeIDType));
                    memcpy(ExchangeID,pExchangeData+i*sizeof(CommonExchangeIDType),sizeof(CommonExchangeIDType));

                    //TRACE("ExchangeID = %s\n",ExchangeID);
                    setExchangeID.insert((string)ExchangeID);
                } 


                //��ѯ�ض��������ĺ�Լ�б���Ϣ
                PlusinAPI* pAPI=PlusinAPI::GetInstance();
                if(pAPI)
                {
                    for (set<string>::iterator it=setExchangeID.begin();it!=setExchangeID.end();it++)
                        pAPI->Send(CMDID_INSTRUMENT_LIST_QUERY,(char*)(*it).c_str(),sizeof(CommonExchangeIDType));
                }
            }
            break;
        } 
    //��ѯ�ض��������ĺ�Լ�б���Ϣ����
    case CMDID_INSTRUMENT_LIST_RSP:
        {            
            if(nErrorCode==SUCCESS)
            {
                char* pInstrumentData = (char*)pData;

                int nCount = nDataLen/sizeof(InstrumentIDType);

                char InstrumentID[sizeof(InstrumentIDType)];
                for(int i=0;i<nCount;i++)
                {
                    memset(InstrumentID,0,sizeof(InstrumentIDType));
                    memcpy(InstrumentID,pInstrumentData+i*sizeof(InstrumentIDType),sizeof(InstrumentIDType));

                    //TRACE("InstrumentID = %s\n",InstrumentID);
                    m_pMainDlg->m_setInstrumentID.insert((string)InstrumentID);
                } 
            }
            break;
        } 
    //�µ��ɹ�����
    case CMDID_ORDER_INSERT_ORDER_RSP:         
        {
            PlatformStru_InputOrder *pInputOrder = (PlatformStru_InputOrder*)pData;
            if(pInputOrder==NULL)
                break;

            //�����µ���Ϣ
            TRACE("6-lock\n");
            m_pMainDlg->m_OrderMutex.lock();

            int nParamID = nRequestId;
            map<int,set<string>>::iterator it = m_pMainDlg->m_mapParamID2OrderRef.find(nParamID);
            if(it == m_pMainDlg->m_mapParamID2OrderRef.end())
            {            
                set<string> setOrderRef;
                setOrderRef.insert(pInputOrder->OrderRef);
                m_pMainDlg->m_mapParamID2OrderRef.insert(make_pair(nParamID,setOrderRef));
            }
            else
            {
                set<string>& setOrderRef = it->second;
                setOrderRef.insert(pInputOrder->OrderRef);
            }

            m_pMainDlg->m_OrderMutex.unlock();
            TRACE("6-unlock\n");


            if(m_pMainDlg->m_pLog)
            {
                char buf[512];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"�µ��ɹ����� RequestId=%d,ParamID=%d,Instrument=%s,OrderRef=%s,CombOffsetFlag=%s,Direction=%s,LimitPrice=%.6f,VolumeTotalOriginal=%d\n",
                    nRequestId,nParamID,pInputOrder->InstrumentID,pInputOrder->OrderRef,pInputOrder->CombOffsetFlag,
                    pInputOrder->Direction==THOST_FTDC_D_Buy?"��":"��",pInputOrder->LimitPrice,pInputOrder->VolumeTotalOriginal);

                m_pMainDlg->m_pLog->Log(buf);
            }

            break;
        }
    case  CMDID_ORDER_RSP_ORDER_INSERT1://�µ�ʧ�ܷ���
        {
            PlatformStru_RspOrderInsert1* pRspOrderInsert1 = (PlatformStru_RspOrderInsert1*)pData;
            if(pRspOrderInsert1==NULL)
                break;

            if(m_pMainDlg->m_pLog)
            {
                char buf[512];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"�µ�ʧ��1���� ParamID=%d,Instrument=%s,OrderRef=%s,LimitPrice=%.6f\n",
                    nRequestId,pRspOrderInsert1->InputOrderInsertField.InstrumentID,pRspOrderInsert1->InputOrderInsertField.OrderRef,
                    pRspOrderInsert1->InputOrderInsertField.LimitPrice);

                m_pMainDlg->m_pLog->Log(buf);
            }

            break;
        }
    case  CMDID_ORDER_RSP_ORDER_INSERT2://�µ�ʧ�ܷ���
        {
            PlatformStru_RspOrderInsert2* pRspOrderInsert2 = (PlatformStru_RspOrderInsert2*)pData;
            if(pRspOrderInsert2==NULL)
                break;

            if(m_pMainDlg->m_pLog)
            {
                char buf[512];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"�µ�ʧ��2���� ParamID=%d,Instrument=%s,OrderRef=%s,LimitPrice=%.6f\n",
                    nRequestId,pRspOrderInsert2->OrderInsertField.InstrumentID,pRspOrderInsert2->OrderInsertField.OrderRef,
                    pRspOrderInsert2->OrderInsertField.LimitPrice);

                m_pMainDlg->m_pLog->Log(buf);
            }

            break;
        }
    case CMDID_ORDER_CANCEL_ORDER_RSP://����������Ӧ
        {
            PlatformStru_InputOrderAction* pInputOrderAction = (PlatformStru_InputOrderAction*)pData;   
            if(pInputOrderAction==NULL)
                break;

            if(m_pMainDlg->m_pLog)
            {
                char buf[512];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"����������Ӧ���� ParamID=%d,Instrument=%s,OrderRef=%s,LimitPrice=%.6f\n",
                    nRequestId,pInputOrderAction->Thost.InstrumentID,pInputOrderAction->Thost.OrderRef,pInputOrderAction->Thost.LimitPrice);

                m_pMainDlg->m_pLog->Log(buf);
            }

            break;     
        }      
    case CMDID_ORDER_RSP_ORDER_ACTION1:		//����ʧ�ܷ���
    case CMDID_ORDER_RSP_ORDER_ACTION2:		//����ʧ�ܷ���
        {
            if(m_pMainDlg->m_pLog)
            {
                char buf[512];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"����ʧ�ܷ���\n");

                m_pMainDlg->m_pLog->Log(buf);
            }
        }
        break;             
    //���ص���ί�е�����
    case CMDID_ALL_ORDER_INFO_RSP:
        {
            PlatformStru_OrderInfo* pOrderData = (PlatformStru_OrderInfo*)pData;
            if(pOrderData==NULL)
                break;

            //������ϸ����
            if(m_pMainDlg->GetDetailDlgOpened())
                m_pMainDlg->m_pDetailDlg->Update(pOrderData);

            TRACE("7-lock\n");
            m_pMainDlg->m_OrderMutex.lock();

            int nParamID = nRequestId;
            map<int,set<string>>::iterator it = m_pMainDlg->m_mapParamID2OrderRef.find(nParamID);
            if(it == m_pMainDlg->m_mapParamID2OrderRef.end())
            {            
                set<string> setOrderRef;
                setOrderRef.insert(pOrderData->OrderRef);
                m_pMainDlg->m_mapParamID2OrderRef.insert(make_pair(nParamID,setOrderRef));
            }
            else
            {
                set<string>& setOrderRef = it->second;
                setOrderRef.insert(pOrderData->OrderRef);
            }

            m_pMainDlg->m_OrderMutex.unlock();
            TRACE("7-unlock\n");

            break;
        }
    //����״̬�ر�
    case CMDID_ORDER_ORDER_STATUS_CHANGE:
        {
            PlatformStru_OrderInfo* pOrderData = (PlatformStru_OrderInfo*)pData;
            if(pOrderData==NULL)
                break;

            //������ϸ����
            if(m_pMainDlg->GetDetailDlgOpened())
                m_pMainDlg->m_pDetailDlg->Update(pOrderData);

            TRACE("8-lock\n");
            m_pMainDlg->m_OrderMutex.lock();
            OrderKey key(pOrderData->InvestorID,pOrderData->InstrumentID,pOrderData->FrontID,pOrderData->SessionID,pOrderData->OrderRef);                
            m_pMainDlg->m_mapOrderRef2OrderKey.insert(make_pair(pOrderData->OrderRef,key));
            m_pMainDlg->m_OrderMutex.unlock();
            TRACE("8-unlock\n");

            break;
        }
    //�����������
    case CMDID_QUOTE_SNAPSHOT_RSP:
        {
            if(nErrorCode==SUCCESS&&nDataLen==sizeof(PlatformStru_DepthMarketData))
            {                
                PlatformStru_DepthMarketData* pMarketData=(PlatformStru_DepthMarketData*)pData;
                m_pMainDlg->UpdateInst2Price(pMarketData->InstrumentID,pMarketData->BidPrice1,pMarketData->AskPrice1);

                //���²���UI
                if(m_pMainDlg->GetSetupDlgOpened())
                    m_pMainDlg->m_pOptDlg->Update(pMarketData);     
            }

            break;
        }
    //���ĺ�Լ���鷵��
    case CMDID_QUOTE_SUBSCRIBE_RSP:
        {
            if(nErrorCode == 1)
                TRACE("���ĵĺ�Լ������������");
            if(nErrorCode == 999)
                TRACE("δ֪����");

            if(m_pMainDlg->m_pLog)
            {
                char buf[512];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"���ĺ�Լ���鷵��(%s nErrorCode=%d)\n",(nErrorCode==SUCCESS)?"�ɹ�":"ʧ��",nErrorCode);

                m_pMainDlg->m_pLog->Log(buf);
            }

            break;
        }
    case CMDID_QUOTE_SEND_QUOTE:
        {
            //���յ���������
            if(nErrorCode==SUCCESS&&nDataLen==sizeof(PlatformStru_DepthMarketData))
            {                
                PlatformStru_DepthMarketData* pMarketData=(PlatformStru_DepthMarketData*)pData;
                m_pMainDlg->UpdateInst2Price(pMarketData->InstrumentID,pMarketData->BidPrice1,pMarketData->AskPrice1);

                //���²���UI
                if(m_pMainDlg->GetSetupDlgOpened())
                    m_pMainDlg->m_pOptDlg->Update(pMarketData);                             
                else
                {
                    //m_pMainDlg->m_Inst2SubedMutex.lock();
                    if(m_pMainDlg->m_mapInstrument2Subscribed.find((string)(pMarketData->InstrumentID)) == m_pMainDlg->m_mapInstrument2Subscribed.end())
                        m_pMainDlg->m_mapInstrument2Subscribed.insert(make_pair((string)(pMarketData->InstrumentID),false)); 
                    else if(m_pMainDlg->m_mapInstrument2Subscribed.find((string)(pMarketData->InstrumentID))!=m_pMainDlg->m_mapInstrument2Subscribed.end())
                        m_pMainDlg->m_mapInstrument2Subscribed[pMarketData->InstrumentID] = true;
                    //m_pMainDlg->m_Inst2SubedMutex.unlock();

                    TRACE("9-lock\n");
                    m_pMainDlg->m_DataMutex.lock();

                    CParamData* pData = NULL;
                    map<int,CParamData*>::iterator it=m_pMainDlg->m_mapIndex2Data.begin();
                    while(it!=m_pMainDlg->m_mapIndex2Data.end())
                    {
                        pData = it->second;

                        string szInstNameUpper1(pData->Order_Instrument1);
                        //transform(szInstNameUpper1.begin(), szInstNameUpper1.end(), szInstNameUpper1.begin(), (int (*)(int))toupper);      

                        string szInstNameUpper2(pData->Order_Instrument2);
                        //transform(szInstNameUpper2.begin(), szInstNameUpper2.end(), szInstNameUpper2.begin(), (int (*)(int))toupper); 

                        if( m_pMainDlg->bInstsArrived(szInstNameUpper1,szInstNameUpper2) && !pData->Param_TriggerFlag &&
                            (0==strcmp(pMarketData->InstrumentID,szInstNameUpper1.c_str())||0==strcmp(pMarketData->InstrumentID,szInstNameUpper2.c_str())) )
                        {
                            if(m_pMainDlg->EnterAlgorithm(pData))
                            {
                                //����  
                                m_pMainDlg->m_mapIndex2DataTriggered.insert(make_pair(pData->Param_ID,pData)); 
                                m_pMainDlg->m_mapIndex2Data.erase(it++);

                                if(!m_pMainDlg->m_bTrigger)
                                    m_pMainDlg->m_listCtrl.DeleteItem(m_pMainDlg->GetListIndex(pData->Param_ID));
                                else
                                    m_pMainDlg->m_listCtrl.SetItemText(m_pMainDlg->GetListIndex(pData->Param_ID),1,"����"); 
                            }
                            else
                                ++it;
                        } 
                        else
                            ++it;
                    }

                    m_pMainDlg->m_DataMutex.unlock();
                    TRACE("9-unlock\n");
                }
            }
            break;
        }    
    }
    return 0;
}

bool CArbitrageDelegationDlg::EnterAlgorithm(CParamData* pDataParam)
{
    CParamData* pData = pDataParam;
    if(pData == NULL)
        return false;    

    double dPriceOff;                       //�۲���������ǰ ��Լ1��2�ķ����������һ�ۼ۸�ļ۲����ֵ    
    double dPrice1,dPrice2;                 //�۲���������ǰ ��Լ1��2�ķ����������һ�ۼ۸�    
    double dOrderPrice1,dOrderPrice2;       //�۲����������� Ҫ�±�����ί�м۸�
    double dDeviatePrice1,dDeviatePrice2;   //�۲����������� Ҫ��ί�м۸�Ƚϵ�ƫ����

    bool bMayOrder = false;
    if (0==pData->Order_Direction1 && 1==pData->Order_Direction2)//1�� 2��
    {
        dPrice1 = GetPrice(pData->Order_Instrument1,PRICE_TYPE_ASK);//��һ��
        dOrderPrice1 = dPrice1+pData->Order_DelegatePrice1;
        dDeviatePrice1 = pData->Order_UpperPrice1-pData->Param_DeviatePrice;


        dPrice2 = GetPrice(pData->Order_Instrument2,PRICE_TYPE_BID);//��һ��
        dOrderPrice2 = dPrice2-pData->Order_DelegatePrice2;
        dDeviatePrice2 = pData->Order_LowestPrice2+pData->Param_DeviatePrice;

        dPriceOff = fabs(dPrice1-dPrice2);   
        
        if(0==pData->Param_OffsetID && 0==pData->Param_ArbitrageType)//���� �۲���С����
        {            
            if(CTools::compare_double(dPriceOff,pData->Param_TriggerPrice)>=0) //���ڵ���
            {                
                if(CTools::compare_double(dOrderPrice2,dDeviatePrice2)>=0 && CTools::compare_double(dDeviatePrice1,dOrderPrice1)>=0)
                    bMayOrder = true;
            }
        }
        else if(0==pData->Param_OffsetID && 1==pData->Param_ArbitrageType)//���� �۲���������
        {            
            if(CTools::compare_double(pData->Param_TriggerPrice,dPriceOff)>=0) //С�ڵ���
            {
                if(CTools::compare_double(dOrderPrice2,dDeviatePrice2)>=0 && CTools::compare_double(dDeviatePrice1,dOrderPrice1)>=0)
                    bMayOrder = true;
            }
        }
        else if(((1==pData->Param_OffsetID)||(2==pData->Param_OffsetID)) && 0==pData->Param_ArbitrageType)//ƽ��(��) �۲���С����
        {       
            if(CTools::compare_double(pData->Param_TriggerPrice,dPriceOff)>=0) //С�ڵ���
            {
                if(CTools::compare_double(dOrderPrice2,dDeviatePrice2)>=0 && CTools::compare_double(dDeviatePrice1,dOrderPrice1)>=0)
                    bMayOrder = true;
            }
        }
        else if(((1==pData->Param_OffsetID)||(2==pData->Param_OffsetID)) && 1==pData->Param_ArbitrageType)//ƽ��(��) �۲���������
        {
            if(CTools::compare_double(dPriceOff,pData->Param_TriggerPrice)>=0) //���ڵ���
            {
                if(CTools::compare_double(dOrderPrice2,dDeviatePrice2)>=0 && CTools::compare_double(dDeviatePrice1,dOrderPrice1)>=0)
                    bMayOrder = true;
            }
        }    
    }
    else if (1==pData->Order_Direction1 && 0==pData->Order_Direction2)//1�� 2��
    {
        dPrice1 = GetPrice(pData->Order_Instrument1,PRICE_TYPE_BID);//��һ��
        dOrderPrice1 = dPrice1-pData->Order_DelegatePrice1;
        dDeviatePrice1 = pData->Order_LowestPrice1+pData->Param_DeviatePrice;

        dPrice2 = GetPrice(pData->Order_Instrument2,PRICE_TYPE_ASK);//��һ��
        dOrderPrice2 = dPrice2+pData->Order_DelegatePrice2;
        dDeviatePrice2 = pData->Order_UpperPrice2-pData->Param_DeviatePrice;

        dPriceOff = fabs(dPrice1-dPrice2); 

        if(0==pData->Param_OffsetID && 0==pData->Param_ArbitrageType)//���� �۲���С����
        {            
            if(CTools::compare_double(dPriceOff,pData->Param_TriggerPrice)>=0) //���ڵ���
            {                
                if(CTools::compare_double(dDeviatePrice2,dOrderPrice2)>=0 && CTools::compare_double(dOrderPrice1,dDeviatePrice1)>=0)
                    bMayOrder = true;
            }
        }
        else if(0==pData->Param_OffsetID && 1==pData->Param_ArbitrageType)//���� �۲���������
        {            
            if(CTools::compare_double(pData->Param_TriggerPrice,dPriceOff)>=0) //С�ڵ���
            {
                if(CTools::compare_double(dDeviatePrice2,dOrderPrice2)>=0 && CTools::compare_double(dOrderPrice1,dDeviatePrice1)>=0)
                    bMayOrder = true;
            }
        }
        else if(((1==pData->Param_OffsetID)||(2==pData->Param_OffsetID)) && 0==pData->Param_ArbitrageType)//ƽ��(��) �۲���С����
        {       
            if(CTools::compare_double(pData->Param_TriggerPrice,dPriceOff)>=0) //С�ڵ���
            {
                if(CTools::compare_double(dDeviatePrice2,dOrderPrice2)>=0 && CTools::compare_double(dOrderPrice1,dDeviatePrice1)>=0)
                    bMayOrder = true;
            }
        }
        else if(((1==pData->Param_OffsetID)||(2==pData->Param_OffsetID)) && 1==pData->Param_ArbitrageType)//ƽ��(��) �۲���������
        {
            if(CTools::compare_double(dPriceOff,pData->Param_TriggerPrice)>=0) //���ڵ���
            {
                if(CTools::compare_double(dDeviatePrice2,dOrderPrice2)>=0 && CTools::compare_double(dOrderPrice1,dDeviatePrice1)>=0)
                    bMayOrder = true;
            }
        }    
    }

    if(bMayOrder)
    {
        pData->Order_Price1 = dOrderPrice1;
        pData->Order_Price2 = dOrderPrice2;
        pData->Param_TriggerFlag = true; 

        SendInsertOrder(pData);   

        if(m_pLog)
        {
            char buf[512];
            memset(buf,0,sizeof(buf));
            sprintf(buf,"�������� ParamID=%d,OffsetID=%d,ArbitrageType=%d,TriggerPrice=%.6f,DeviatePrice=%.6f,Instrument1=%s,Instrument2=%s,Direction1=%s,Direction2=%s,Count1=%d,Count1=%d,Price1=%.6f,Price2=%.6f\n",
                        pData->Param_ID,pData->Param_OffsetID,pData->Param_ArbitrageType,pData->Param_TriggerPrice,pData->Param_DeviatePrice,
                        pData->Order_Instrument1.c_str(),pData->Order_Instrument2.c_str(),                
                        pData->Order_Direction1==0?"��":"��",pData->Order_Direction2==0?"��":"��",
                        pData->Order_Count1,pData->Order_Count2,
                        pData->Order_Price1,pData->Order_Price2);

            m_pLog->Log(buf);
        }
    }

    return bMayOrder;

}

void CArbitrageDelegationDlg::SendInsertOrder(CParamData* pData)
{
    //����1  
    PlatformStru_InputOrder OrderInsert1;
    memset(&OrderInsert1,0,sizeof(OrderInsert1));

    string szInstNameUpper1(pData->Order_Instrument1);
    //transform(szInstNameUpper1.begin(), szInstNameUpper1.end(), szInstNameUpper1.begin(), (int (*)(int))toupper);

    strncpy_s(OrderInsert1.InvestorID,sizeof(OrderInsert1.InvestorID),m_szAccount.c_str(),sizeof(OrderInsert1.InvestorID)-1);
    strncpy_s(OrderInsert1.InstrumentID,sizeof(OrderInsert1.InstrumentID),szInstNameUpper1.c_str(),sizeof(OrderInsert1.InstrumentID)-1);
    OrderInsert1.OrderPriceType='2';								        //�м� THOST_FTDC_OPT_AnyPrice='1' �޼� THOST_FTDC_OPT_LimitPrice='2';
    OrderInsert1.LimitPrice = pData->Order_Price1;                          //�µ�ί�м۸�;
    OrderInsert1.Direction=pData->Order_Direction1==0?'0':'1';			    //��������0-'0'=��1-'1'=��
    if(pData->Param_OffsetID==0) OrderInsert1.CombOffsetFlag[0]='0';	    //��ƽ��־��0-'0'=����
    else if(pData->Param_OffsetID==1) OrderInsert1.CombOffsetFlag[0]='1';   //��ƽ��־��1-'1'=ƽ��
    else OrderInsert1.CombOffsetFlag[0]='3';						        //��ƽ��־��2-'3'=ƽ��
    OrderInsert1.CombHedgeFlag[0]='1';							            //Ͷ���ױ���'1'=Ͷ����'3'=�ױ�
    OrderInsert1.VolumeTotalOriginal=pData->Order_Count1;		            //����
    OrderInsert1.TimeCondition='3';								            //��Ч�����ͣ��м۵���Ӧ��������ɣ�������='1'; �޼۵���Ӧ��������Ч='3'
    OrderInsert1.VolumeCondition='1';							            //�ɽ������ͣ�'1'��ʾ�κ�����
    OrderInsert1.MinVolume=1;
    OrderInsert1.ContingentCondition='1';						            //��������
    OrderInsert1.ForceCloseReason='0';							            //ǿƽԭ��'0'��ǿƽ
    OrderInsert1.OrderClass='N';									        //�±���
    strncpy_s(OrderInsert1.strAccount,sizeof(OrderInsert1.strAccount),m_szAccount.c_str(),sizeof(OrderInsert1.strAccount)-1);

    //����2
    PlatformStru_InputOrder OrderInsert2;
    memset(&OrderInsert2,0,sizeof(OrderInsert2));

    string szInstNameUpper2(pData->Order_Instrument2);
    //transform(szInstNameUpper2.begin(), szInstNameUpper2.end(), szInstNameUpper2.begin(), (int (*)(int))toupper); 

    strncpy_s(OrderInsert2.InvestorID,sizeof(OrderInsert2.InvestorID),m_szAccount.c_str(),sizeof(OrderInsert2.InvestorID)-1);
    strncpy_s(OrderInsert2.InstrumentID,sizeof(OrderInsert2.InstrumentID),szInstNameUpper2.c_str(),sizeof(OrderInsert2.InstrumentID)-1);
    OrderInsert2.OrderPriceType='2';								        //�м� THOST_FTDC_OPT_AnyPrice='1' �޼� THOST_FTDC_OPT_LimitPrice='2';
    OrderInsert2.LimitPrice = pData->Order_Price2;                          //�µ�ί�м۸�;
    OrderInsert2.Direction=pData->Order_Direction2==0?'0':'1';			    //��������0-'0'=��1-'1'=��
    if(pData->Param_OffsetID==0) OrderInsert2.CombOffsetFlag[0]='0';	    //��ƽ��־��0-'0'=����
    else if(pData->Param_OffsetID==1) OrderInsert2.CombOffsetFlag[0]='1';   //��ƽ��־��1-'1'=ƽ��
    else OrderInsert2.CombOffsetFlag[0]='3';						        //��ƽ��־��2-'3'=ƽ��
    OrderInsert2.CombHedgeFlag[0]='1';							            //Ͷ���ױ���'1'=Ͷ����'3'=�ױ�
    OrderInsert2.VolumeTotalOriginal=pData->Order_Count2;		            //����
    OrderInsert2.TimeCondition='3';								            //��Ч�����ͣ��м۵���Ӧ��������ɣ�������='1'; �޼۵���Ӧ��������Ч='3'
    OrderInsert2.VolumeCondition='1';							            //�ɽ������ͣ�'1'��ʾ�κ�����
    OrderInsert2.MinVolume=1;
    OrderInsert2.ContingentCondition='1';						            //��������
    OrderInsert2.ForceCloseReason='0';							            //ǿƽԭ��'0'��ǿƽ
    OrderInsert2.OrderClass='N';									        //�±���
    strncpy_s(OrderInsert2.strAccount,sizeof(OrderInsert2.strAccount),m_szAccount.c_str(),sizeof(OrderInsert2.strAccount)-1);


    PlusinAPI* pAPI=PlusinAPI::GetInstance();
    if(pAPI) 
    {
        pAPI->Send(CMDID_ORDER_INSERT_ORDER,&OrderInsert1,sizeof(OrderInsert1),pData->Param_ID);
        pAPI->Send(CMDID_ORDER_INSERT_ORDER,&OrderInsert2,sizeof(OrderInsert2),pData->Param_ID);
    }
}


void CArbitrageDelegationDlg::UpdateInst2Price(string InstrumentID, double BidPrice1, double AskPrice1)
{
    TRACE("10-lock\n");
    m_Inst2PriceMutex.lock();
    //transform(InstrumentID.begin(), InstrumentID.end(), InstrumentID.begin(), (int (*)(int))toupper);
    map<string,Stru_Inst2Price>::iterator it = m_mapInst2Price.find(InstrumentID);
    if(it!=m_mapInst2Price.end())
    {
        it->second.AskPrice1 = AskPrice1;
        it->second.BidPrice1 = BidPrice1;
    }
    else
    {
        Stru_Inst2Price inst2Price;
        inst2Price.AskPrice1 = AskPrice1;
        inst2Price.BidPrice1 = BidPrice1;

        m_mapInst2Price.insert(make_pair(InstrumentID,inst2Price));
    }
    m_Inst2PriceMutex.unlock();
    TRACE("10-unlock\n");
}

double CArbitrageDelegationDlg::GetPrice(string InstrumentID,PRICE_TYPE nType)
{
    TRACE("11-lock\n");
    m_Inst2PriceMutex.lock(); 

    double dPrice = 0.0f;
   
    //transform(InstrumentID.begin(), InstrumentID.end(), InstrumentID.begin(), (int (*)(int))toupper);  
    map<string,Stru_Inst2Price>::iterator it = m_mapInst2Price.find(InstrumentID);
    if(it!=m_mapInst2Price.end())
    {
        if(nType == PRICE_TYPE_ASK)
            dPrice = it->second.AskPrice1;
        else if(nType == PRICE_TYPE_BID)
            dPrice = it->second.BidPrice1;
    }
    m_Inst2PriceMutex.unlock();
    TRACE("11-unlock\n");

    return dPrice;
}

int  CArbitrageDelegationDlg::GetListIndex(int nParamID)
{
    for (int i=0;i<m_listCtrl.GetItemCount();i++)
    {
        CString str = m_listCtrl.GetItemText(i,0);
        if(_ttoi(str) == nParamID)
            return i;
    }

    return -1;
}


bool CArbitrageDelegationDlg::bInstsArrived(string szInst1,string szInst2)
{
    //transform(szInst1.begin(), szInst1.end(), szInst1.begin(), (int (*)(int))toupper);
    //transform(szInst2.begin(), szInst2.end(), szInst2.begin(), (int (*)(int))toupper);

    return (m_mapInst2Price.find(szInst1)!=m_mapInst2Price.end() && m_mapInst2Price.find(szInst2)!=m_mapInst2Price.end());
}

// CArbitrageDelegationDlg dialog

CArbitrageDelegationDlg::CArbitrageDelegationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArbitrageDelegationDlg::IDD, pParent)
    , m_bSetupDlgOpened(0)
    , m_bDetailDlgOpened(0)
    , m_nNextArbitrageID(0)
    , m_pLog(NULL)
    , m_pOptDlg(NULL)
    , m_pDetailDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pMainDlg = this;
    m_mapIndex2DataTriggered.clear();
    m_mapIndex2DataRevoked.clear();
    m_mapIndex2Data.clear();
    m_mapParamID2OrderRef.clear();
    m_mapOrderRef2OrderKey.clear();

    if(m_pLog==NULL)
    {
        //��ȡ����·��
        char PathFileName[512];
        memset(PathFileName,0,sizeof(PathFileName));
        CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(LogFileName_ArbitrageDelegation));
        strcat(PathFileName,LogFileName_ArbitrageDelegation);    

        m_pLog = new CLogFile(PathFileName);
    }
}

CArbitrageDelegationDlg::~CArbitrageDelegationDlg()
{    
    m_pMainDlg = NULL;

    //�ͷſռ�
    TRACE("1-lock\n");
    m_DataMutex.lock();
    CParamData* pData = NULL;
    map<int,CParamData*>::iterator it_trigger = m_mapIndex2DataTriggered.begin();
    for (;it_trigger!=m_mapIndex2DataTriggered.end();it_trigger++)
    {
        pData = it_trigger->second;
        if(pData)
        {
            delete pData;
            pData = NULL;
        }
    }

    map<int,CParamData*>::iterator it_revoke = m_mapIndex2DataRevoked.begin();
    for (;it_revoke!=m_mapIndex2DataRevoked.end();it_revoke++)
    {       
        pData = it_revoke->second;
        if(pData)
        {
            delete pData;
            pData = NULL;
        }
    }

    map<int,CParamData*>::iterator it = m_mapIndex2Data.begin();
    for (;it!=m_mapIndex2Data.end();it++)
    {
        pData = it->second;
        if(pData)
        {
            delete pData;
            pData = NULL;
        }
    }
    m_DataMutex.unlock();
    TRACE("1-unlock\n");

    if(g_hWnd_FT!=NULL)
    {
        PlusinAPI* pAPI=PlusinAPI::GetInstance();
        if(pAPI)
        {
            pAPI->Logout();
            pAPI->DestroyInstance();
        }
    }

    if(m_pLog)
    {
        delete m_pLog;
        m_pLog = NULL;
    }
}

void CArbitrageDelegationDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST, m_listCtrl);
    //     DDX_Control(pDX, IDC_BUTTON_APPLY, m_btnApplyCtrl);
}

BEGIN_MESSAGE_MAP(CArbitrageDelegationDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()   
    ON_MESSAGE(WM_USER_SETUPDLG_CLOSE,OnSetupDlgClose) 
    ON_MESSAGE(WM_USER_DETAILDLG_CLOSE,OnDetailDlgClose) 
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_ARBITRAGE, &CArbitrageDelegationDlg::OnBnClickedButtonArbitrage)
    ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CArbitrageDelegationDlg::OnBnClickedButtonModify)
    ON_BN_CLICKED(IDC_BUTTON_REVOKE, &CArbitrageDelegationDlg::OnBnClickedButtonRevoke)
    ON_BN_CLICKED(IDC_BUTTON_DELEGATEDETAIL, &CArbitrageDelegationDlg::OnBnClickedButtonDelegatedetail)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_CHECK_DISPLAYREVOKE, &CArbitrageDelegationDlg::OnBnClickedCheckDisplayrevoke)
    ON_BN_CLICKED(IDC_CHECK_TRIGGERED, &CArbitrageDelegationDlg::OnBnClickedCheckTriggered)
    ON_WM_TIMER()
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CArbitrageDelegationDlg::OnLvnItemchangedList)
END_MESSAGE_MAP()


// CArbitrageDelegationDlg message handlers

BOOL CArbitrageDelegationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    TRACE("Enter initDialog\n");
    m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    //������ 
    int i=0;
    m_listCtrl.InsertColumn( i++, "��ˮ��",     LVCFMT_CENTER,    60 );
    m_listCtrl.InsertColumn( i++, "����״̬",   LVCFMT_CENTER,    80 ); 
    m_listCtrl.InsertColumn( i++, "��ƽ",       LVCFMT_CENTER,    60 ); 
    m_listCtrl.InsertColumn( i++, "��������",   LVCFMT_CENTER,    90 ); 
    m_listCtrl.InsertColumn( i++, "�����۲�",   LVCFMT_CENTER,    80 ); 
    m_listCtrl.InsertColumn( i++, "ƫ����",   LVCFMT_CENTER,    80 ); 
    m_listCtrl.InsertColumn( i++, "��Լ1",      LVCFMT_CENTER,    60 ); 
    m_listCtrl.InsertColumn( i++, "����1",      LVCFMT_CENTER,    60 ); 
    m_listCtrl.InsertColumn( i++, "ί�м۲�1",  LVCFMT_CENTER,    80 ); 
    m_listCtrl.InsertColumn( i++, "ί������1",  LVCFMT_CENTER,    80 ); 
    m_listCtrl.InsertColumn( i++, "��Լ2",      LVCFMT_CENTER,    60 ); 
    m_listCtrl.InsertColumn( i++, "����2",      LVCFMT_CENTER,    60 ); 
    m_listCtrl.InsertColumn( i++, "ί�м۲�2",  LVCFMT_CENTER,    80 ); 
    m_listCtrl.InsertColumn( i++, "ί������2",  LVCFMT_CENTER,    80 ); 


    if(g_hWnd_FT!=NULL)
    {
        PlusinAPI* pAPI=PlusinAPI::GetInstance();
        if(pAPI)
        {
            pAPI->Init( "ArbitrageDelegation",
                        "ArbitrageDelegation",
                        &CArbitrageDelegationDlg::PluginsAPICallBack,
                        g_hWnd_FT,
                        NEED_NO_POS_NO_BIND,
                        GetSafeHwnd());
            pAPI->Logon();
        }
    }

    //��ini����ParamData
    if(ReadData())
        SetDataToListCtrl();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CArbitrageDelegationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CArbitrageDelegationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CArbitrageDelegationDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    int nCount = m_mapIndex2Data.size();
    if(nCount>0)    
    {
        int irlt=MessageBox("����δ�������������������˳��ô��ں󲻻����������\n�Ƿ�����˳�(�����浱ǰ����)��","��ʾ",MB_YESNO);
        if(irlt==IDNO)
            return;
    }  

    if(!m_mapIndex2Data.empty() || !m_mapIndex2DataRevoked.empty() || !m_mapIndex2DataTriggered.empty())
        SaveData();

    KillTimer(TIMER_ISNTSUBED);

    if(g_hWnd_FT!=NULL)
    {
        PlusinAPI* pAPI=PlusinAPI::GetInstance();
        if(pAPI)
            pAPI->Logout();

        PlusinAPI::DestroyInstance();
    }

    CDialog::OnClose();
}

//��ȡini�ļ���������Ϣ,���õ��ؼ���ȥ
bool CArbitrageDelegationDlg::ReadData()
{
    TRACE("Enter ReadData\n");

    char Node[128];
    char Buf[512];
    char PathFileName[512];

    //��ȡ����·��
    memset(PathFileName,0,sizeof(PathFileName));
    CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(IniFileName_ArbitrageDelegation));
    strcat(PathFileName,IniFileName_ArbitrageDelegation);

    /*  �ж��ļ��Ƿ����
        mode ģʽ����5��ģʽ��                               
        0-����ļ��Ƿ����         
        1-����ļ��Ƿ������        
        2-����ļ��Ƿ��д����    
        4-����ļ��Ƿ�ɶ�����     
        6-����ļ��Ƿ�ɶ�/д���� 
    */

    if(-1==_access(PathFileName,0))
        return false;

    CIniFile *pIniFile=new CIniFile(PathFileName);

    int nY,nM,nD;
    CTime time;
    time = CTime::GetCurrentTime();  

    int nRealCount = 0;
    int nCount=pIniFile->GetIniKeyVal("COUNT","Count",0);
    for (int i=0;i<nCount;i++)
    {
        int j=i+1;
        memset(Node,0,sizeof(Node));
        sprintf(Node,"%s_%d",Node_ArbitrageDelegation,j);

        //�ж���Ч�� ���ڵ�����������
        int curYear = time.GetYear();
        int curMonth = time.GetMonth();
        int curDay = time.GetDay();

        pIniFile->GetIniKeyStr(Node,"Param_ValidateDay","",Buf,sizeof(Buf));  
        if(strlen(Buf)>0)
        {
            sscanf(Buf, "%d-%d-%d",&nY,&nM,&nD);
            if(nY<curYear ||
               (nY==curYear && nM<curMonth)||
               (nY==curYear && nM==curMonth && nD<curDay))
               continue;

        }

        //�жϴ���ʱ�� ����֮ǰ���������������Ѿ������ͳ�����Ҳ����
        pIniFile->GetIniKeyStr(Node,"Param_ValidateCreateDay","",Buf,sizeof(Buf));  
        if(strlen(Buf)>0)
        {
            sscanf(Buf, "%d-%d-%d",&nY,&nM,&nD);
            if(nY<curYear ||
              (nY==curYear && nM<curMonth)||
              (nY==curYear && nM==curMonth && nD<curDay))
            {
                bool revokeFlag=pIniFile->GetIniKeyVal(Node,"Param_RevokeFlag",0);
                bool triggerFlag=pIniFile->GetIniKeyVal(Node,"Param_TriggerFlag",0);
                
                if(revokeFlag||triggerFlag)
                    continue;
            }
        }

        CParamData* pData = new CParamData;        

        pData->Param_RevokeFlag=pIniFile->GetIniKeyVal(Node,"Param_RevokeFlag",0);
        pData->Param_TriggerFlag=pIniFile->GetIniKeyVal(Node,"Param_TriggerFlag",0);

        pData->Param_ID=pIniFile->GetIniKeyVal(Node,"Param_ID",0);
        pData->Param_OffsetID=pIniFile->GetIniKeyVal(Node,"Param_OffsetID",0);
        pData->Param_ArbitrageType=pIniFile->GetIniKeyVal(Node,"Param_ArbitrageType",0);
        pData->Param_TriggerPrice=pIniFile->GetIniKeyVal2(Node,"Param_TriggerPrice",0.0f);
        pData->Param_DeviatePrice=pIniFile->GetIniKeyVal2(Node,"Param_DeviatePrice",0.0f);
        pIniFile->GetIniKeyStr(Node,"Param_ValidateDay","",Buf,sizeof(Buf));
        if(strlen(Buf)>0)
        {
            pData->Param_ValidateDayText=Buf;
            sscanf(Buf, "%d-%d-%d",&nY,&nM,&nD);
            pData->Param_ValidateDay = CTime(nY,nM,nD,0,0,0);
        }

        //��Լ1
        pIniFile->GetIniKeyStr(Node,"Order_Instrument1","",Buf,sizeof(Buf));
        pData->Order_Instrument1=Buf;
        pData->Order_Direction1=pIniFile->GetIniKeyVal(Node,"Order_Direction1",1);
        pData->Order_Count1=pIniFile->GetIniKeyVal(Node,"Order_Count1",0);
        pData->Order_Price1=pIniFile->GetIniKeyVal2(Node,"Order_Price1",0.0f);
        pData->Order_DelegatePrice1=pIniFile->GetIniKeyVal2(Node,"Order_DelegatePrice1",0.0f);
        pData->Order_UpperPrice1=pIniFile->GetIniKeyVal2(Node,"Order_UpperPrice1",0.0f);
        pData->Order_LowestPrice1=pIniFile->GetIniKeyVal2(Node,"Order_LowestPrice1",0.0f);

        //��Լ2
        pIniFile->GetIniKeyStr(Node,"Order_Instrument2","",Buf,sizeof(Buf));
        pData->Order_Instrument2=Buf;
        pData->Order_Direction2=pIniFile->GetIniKeyVal(Node,"Order_Direction2",1);
        pData->Order_Count2=pIniFile->GetIniKeyVal(Node,"Order_Count1",0);
        pData->Order_Price2=pIniFile->GetIniKeyVal2(Node,"Order_Price2",0.0f);
        pData->Order_DelegatePrice2=pIniFile->GetIniKeyVal2(Node,"Order_DelegatePrice2",0.0f);
        pData->Order_UpperPrice2=pIniFile->GetIniKeyVal2(Node,"Order_UpperPrice2",0.0f);
        pData->Order_LowestPrice2=pIniFile->GetIniKeyVal2(Node,"Order_LowestPrice2",0.0f);
        
        //OrderKey1
        pIniFile->GetIniKeyStr(Node,"OrderKey_Account1","",Buf,sizeof(Buf));
        pData->OrderKey_Account1=Buf;
        pIniFile->GetIniKeyStr(Node,"OrderKey_Instrument1","",Buf,sizeof(Buf));
        pData->OrderKey_Instrument1=Buf;
        pData->OrderKey_FrontID1=pIniFile->GetIniKeyVal(Node,"OrderKey_FrontID1",0);
        pData->OrderKey_SessionID1=pIniFile->GetIniKeyVal(Node,"OrderKey_SessionID1",0);
        pIniFile->GetIniKeyStr(Node,"OrderKey_OrderRef1","",Buf,sizeof(Buf));
        pData->OrderKey_OrderRef1=Buf;

        //OrderKey2
        pIniFile->GetIniKeyStr(Node,"OrderKey_Account2","",Buf,sizeof(Buf));
        pData->OrderKey_Account2=Buf;
        pIniFile->GetIniKeyStr(Node,"OrderKey_Instrument2","",Buf,sizeof(Buf));
        pData->OrderKey_Instrument2=Buf;
        pData->OrderKey_FrontID2=pIniFile->GetIniKeyVal(Node,"OrderKey_FrontID2",0);
        pData->OrderKey_SessionID2=pIniFile->GetIniKeyVal(Node,"OrderKey_SessionID2",0);
        pIniFile->GetIniKeyStr(Node,"OrderKey_OrderRef2","",Buf,sizeof(Buf));
        pData->OrderKey_OrderRef2=Buf;


        //ȡ���챣������ֵ
        static int iMaxCount = 0;
        iMaxCount = (iMaxCount>=pData->Param_ID)?iMaxCount:pData->Param_ID;
        if(((iMaxCount>>25)&0x0000003F)+BASE_YEAR == time.GetYear() &&
            ((iMaxCount<<7>>27)&0x0000001F) == time.GetMonth() &&
            ((iMaxCount<<12>>27)&0x0000001F) == time.GetDay() )
            m_nNextArbitrageID = (iMaxCount&0x00007FFF);


        //��ʼ��
        if(pData->Param_TriggerFlag)
            m_mapIndex2DataTriggered.insert(make_pair(pData->Param_ID,pData));
        else if(pData->Param_RevokeFlag)
            m_mapIndex2DataRevoked.insert(make_pair(pData->Param_ID,pData));
        else
            m_mapIndex2Data.insert(make_pair(pData->Param_ID,pData));

        if(pData->OrderKey_OrderRef1.length()>0 && pData->OrderKey_OrderRef2.length()>0)
        {
            set<string> setOrderRef;
            setOrderRef.insert(pData->OrderKey_OrderRef1);
            setOrderRef.insert(pData->OrderKey_OrderRef2);
            m_mapParamID2OrderRef.insert(make_pair(pData->Param_ID,setOrderRef));  

            OrderKey key1(pData->OrderKey_Account1.c_str(),pData->OrderKey_Instrument1.c_str(),pData->OrderKey_FrontID1,pData->OrderKey_SessionID1,pData->OrderKey_OrderRef1.c_str());
            OrderKey key2(pData->OrderKey_Account2.c_str(),pData->OrderKey_Instrument2.c_str(),pData->OrderKey_FrontID2,pData->OrderKey_SessionID2,pData->OrderKey_OrderRef2.c_str());
            m_mapOrderRef2OrderKey.insert(make_pair(pData->OrderKey_OrderRef1,key1));
            m_mapOrderRef2OrderKey.insert(make_pair(pData->OrderKey_OrderRef2,key2));
        }
    }

    if(pIniFile)
    {
        delete pIniFile;
        pIniFile=NULL;
    }

    return true;
}

//����������Ϣ��ini�ļ�
void CArbitrageDelegationDlg::SaveData(void)
{
    TRACE("Enter SaveData\n");

    char Node[40];
    char PathFileName[512];

    //��ȡ����·��
    memset(PathFileName,0,sizeof(PathFileName));
    CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(IniFileName_ArbitrageDelegation));
    strcat(PathFileName,IniFileName_ArbitrageDelegation);
    //��ɾ��
    DeleteFile(PathFileName);
    //�󴴽�
    CIniFile *pIniFile=new CIniFile(PathFileName);

    int nCount=0,j=0;
    CParamData* pData = NULL;

    //�Ѵ�����
    nCount= m_mapIndex2DataTriggered.size(); 
    for (map<int,CParamData*>::iterator it_triggered=m_mapIndex2DataTriggered.begin();it_triggered!=m_mapIndex2DataTriggered.end();it_triggered++)
    {
        j++;
        memset(Node,0,sizeof(Node));
        sprintf(Node,"%s_%d",Node_ArbitrageDelegation,j);

        pData = it_triggered->second;

        pIniFile->UpdateKeyVal(Node,"Param_RevokeFlag",(int)pData->Param_RevokeFlag);
        pIniFile->UpdateKeyVal(Node,"Param_TriggerFlag",(int)pData->Param_TriggerFlag);

        pIniFile->UpdateKeyVal(Node,"Param_ID",(int)pData->Param_ID);
        pIniFile->UpdateKeyVal(Node,"Param_OffsetID",(int)pData->Param_OffsetID);
        pIniFile->UpdateKeyVal(Node,"Param_ArbitrageType",(int)pData->Param_ArbitrageType);
        pIniFile->UpdateKeyVal2(Node,"Param_TriggerPrice",pData->Param_TriggerPrice);
        pIniFile->UpdateKeyVal2(Node,"Param_DeviatePrice",pData->Param_DeviatePrice);
        pIniFile->UpdateKeyStr(Node,"Param_ValidateDay",pData->Param_ValidateDay.Format("%Y-%m-%d"));
        pIniFile->UpdateKeyStr(Node,"Param_ValidateCreateDay",pData->Param_ValidateCreateDay.Format("%Y-%m-%d"));

        //��Լ1
        pIniFile->UpdateKeyStr(Node,"Order_Instrument1",pData->Order_Instrument1.c_str());
        pIniFile->UpdateKeyVal(Node,"Order_Direction1",(int)pData->Order_Direction1);
        pIniFile->UpdateKeyVal(Node,"Order_Count1",pData->Order_Count1);
        pIniFile->UpdateKeyVal2(Node,"Order_Price1",pData->Order_Price1);
        pIniFile->UpdateKeyVal2(Node,"Order_DelegatePrice1",pData->Order_DelegatePrice1);
        pIniFile->UpdateKeyVal2(Node,"Order_UpperPrice1",pData->Order_UpperPrice1);
        pIniFile->UpdateKeyVal2(Node,"Order_LowestPrice1",pData->Order_LowestPrice1);

        //��Լ2
        pIniFile->UpdateKeyStr(Node,"Order_Instrument2",pData->Order_Instrument2.c_str());
        pIniFile->UpdateKeyVal(Node,"Order_Direction2",(int)pData->Order_Direction2);
        pIniFile->UpdateKeyVal(Node,"Order_Count2",pData->Order_Count2);
        pIniFile->UpdateKeyVal2(Node,"Order_Price2",pData->Order_Price2);
        pIniFile->UpdateKeyVal2(Node,"Order_DelegatePrice2",pData->Order_DelegatePrice2);
        pIniFile->UpdateKeyVal2(Node,"Order_UpperPrice2",pData->Order_UpperPrice2);
        pIniFile->UpdateKeyVal2(Node,"Order_LowestPrice2",pData->Order_LowestPrice2);


        string ref1,ref2;
        if(GetOrderRefByParamID(pData->Param_ID,ref1,ref2))
        {
            OrderKey key1,key2;
            if(GetOrderKeyByOrderRef(ref1,key1))
            {
                //OrderKey1
                pIniFile->UpdateKeyStr(Node,"OrderKey_Account1",key1.Account);
                pIniFile->UpdateKeyStr(Node,"OrderKey_Instrument1",key1.InstrumentID);
                pIniFile->UpdateKeyVal(Node,"OrderKey_FrontID1",key1.FrontID);
                pIniFile->UpdateKeyVal(Node,"OrderKey_SessionID1",key1.SessionID);
                pIniFile->UpdateKeyStr(Node,"OrderKey_OrderRef1",key1.OrderRef);
            }
            if(GetOrderKeyByOrderRef(ref2,key2))
            {
                //OrderKey2
                pIniFile->UpdateKeyStr(Node,"OrderKey_Account2",key2.Account);
                pIniFile->UpdateKeyStr(Node,"OrderKey_Instrument2",key2.InstrumentID);
                pIniFile->UpdateKeyVal(Node,"OrderKey_FrontID2",key2.FrontID);
                pIniFile->UpdateKeyVal(Node,"OrderKey_SessionID2",key2.SessionID);
                pIniFile->UpdateKeyStr(Node,"OrderKey_OrderRef2",key2.OrderRef);
            }
        }        
    }

    //�ѳ�����
    nCount += m_mapIndex2DataRevoked.size(); 
    for (map<int,CParamData*>::iterator it_revoked=m_mapIndex2DataRevoked.begin();it_revoked!=m_mapIndex2DataRevoked.end();it_revoked++)
    {
        j++;
        memset(Node,0,sizeof(Node));
        sprintf(Node,"%s_%d",Node_ArbitrageDelegation,j);

        pData = it_revoked->second;

        pIniFile->UpdateKeyVal(Node,"Param_RevokeFlag",(int)pData->Param_RevokeFlag);
        pIniFile->UpdateKeyVal(Node,"Param_TriggerFlag",(int)pData->Param_TriggerFlag);

        pIniFile->UpdateKeyVal(Node,"Param_ID",(int)pData->Param_ID);
        pIniFile->UpdateKeyVal(Node,"Param_OffsetID",(int)pData->Param_OffsetID);
        pIniFile->UpdateKeyVal(Node,"Param_ArbitrageType",(int)pData->Param_ArbitrageType);
        pIniFile->UpdateKeyVal2(Node,"Param_TriggerPrice",pData->Param_TriggerPrice);
        pIniFile->UpdateKeyVal2(Node,"Param_DeviatePrice",pData->Param_DeviatePrice);
        pIniFile->UpdateKeyStr(Node,"Param_ValidateDay",pData->Param_ValidateDay.Format("%Y-%m-%d"));
        pIniFile->UpdateKeyStr(Node,"Param_ValidateCreateDay",pData->Param_ValidateCreateDay.Format("%Y-%m-%d"));

        //��Լ1
        pIniFile->UpdateKeyStr(Node,"Order_Instrument1",pData->Order_Instrument1.c_str());
        pIniFile->UpdateKeyVal(Node,"Order_Direction1",(int)pData->Order_Direction1);
        pIniFile->UpdateKeyVal(Node,"Order_Count1",pData->Order_Count1);
        pIniFile->UpdateKeyVal2(Node,"Order_Price1",pData->Order_Price1);
        pIniFile->UpdateKeyVal2(Node,"Order_DelegatePrice1",pData->Order_DelegatePrice1);
        pIniFile->UpdateKeyVal2(Node,"Order_UpperPrice1",pData->Order_UpperPrice1);
        pIniFile->UpdateKeyVal2(Node,"Order_LowestPrice1",pData->Order_LowestPrice1);

        //��Լ2
        pIniFile->UpdateKeyStr(Node,"Order_Instrument2",pData->Order_Instrument2.c_str());
        pIniFile->UpdateKeyVal(Node,"Order_Direction2",(int)pData->Order_Direction2);
        pIniFile->UpdateKeyVal(Node,"Order_Count2",pData->Order_Count2);
        pIniFile->UpdateKeyVal2(Node,"Order_Price2",pData->Order_Price2);
        pIniFile->UpdateKeyVal2(Node,"Order_DelegatePrice2",pData->Order_DelegatePrice2);
        pIniFile->UpdateKeyVal2(Node,"Order_UpperPrice2",pData->Order_UpperPrice2);
        pIniFile->UpdateKeyVal2(Node,"Order_LowestPrice2",pData->Order_LowestPrice2);


        string ref1,ref2;
        if(GetOrderRefByParamID(pData->Param_ID,ref1,ref2))
        {
            OrderKey key1,key2;
            if(GetOrderKeyByOrderRef(ref1,key1))
            {
                //OrderKey1
                pIniFile->UpdateKeyStr(Node,"OrderKey_Account1",key1.Account);
                pIniFile->UpdateKeyStr(Node,"OrderKey_Instrument1",key1.InstrumentID);
                pIniFile->UpdateKeyVal(Node,"OrderKey_FrontID1",key1.FrontID);
                pIniFile->UpdateKeyVal(Node,"OrderKey_SessionID1",key1.SessionID);
                pIniFile->UpdateKeyStr(Node,"OrderKey_OrderRef1",key1.OrderRef);
            }
            if(GetOrderKeyByOrderRef(ref2,key2))
            {
                //OrderKey2
                pIniFile->UpdateKeyStr(Node,"OrderKey_Account2",key2.Account);
                pIniFile->UpdateKeyStr(Node,"OrderKey_Instrument2",key2.InstrumentID);
                pIniFile->UpdateKeyVal(Node,"OrderKey_FrontID2",key2.FrontID);
                pIniFile->UpdateKeyVal(Node,"OrderKey_SessionID2",key2.SessionID);
                pIniFile->UpdateKeyStr(Node,"OrderKey_OrderRef2",key2.OrderRef);
            }
        }        
    }

    //δ�����ͳ�����
    nCount += m_mapIndex2Data.size(); 
    pIniFile->UpdateKeyVal("COUNT","Count",nCount);

    for (map<int,CParamData*>::iterator it=m_mapIndex2Data.begin();it!=m_mapIndex2Data.end();it++)
    {
        j++;
        memset(Node,0,sizeof(Node));
        sprintf(Node,"%s_%d",Node_ArbitrageDelegation,j);

        pData = it->second;
        
        pIniFile->UpdateKeyVal(Node,"Param_RevokeFlag",(int)pData->Param_RevokeFlag);
        pIniFile->UpdateKeyVal(Node,"Param_TriggerFlag",(int)pData->Param_TriggerFlag);

        pIniFile->UpdateKeyVal(Node,"Param_ID",(int)pData->Param_ID);
        pIniFile->UpdateKeyVal(Node,"Param_OffsetID",(int)pData->Param_OffsetID);
        pIniFile->UpdateKeyVal(Node,"Param_ArbitrageType",(int)pData->Param_ArbitrageType);
        pIniFile->UpdateKeyVal2(Node,"Param_TriggerPrice",pData->Param_TriggerPrice);
        pIniFile->UpdateKeyVal2(Node,"Param_DeviatePrice",pData->Param_DeviatePrice);
        pIniFile->UpdateKeyStr(Node,"Param_ValidateDay",pData->Param_ValidateDay.Format("%Y-%m-%d"));
        pIniFile->UpdateKeyStr(Node,"Param_ValidateCreateDay",pData->Param_ValidateCreateDay.Format("%Y-%m-%d"));

        //��Լ1
        pIniFile->UpdateKeyStr(Node,"Order_Instrument1",pData->Order_Instrument1.c_str());
        pIniFile->UpdateKeyVal(Node,"Order_Direction1",(int)pData->Order_Direction1);
        pIniFile->UpdateKeyVal(Node,"Order_Count1",pData->Order_Count1);
        pIniFile->UpdateKeyVal2(Node,"Order_Price1",pData->Order_Price1);
        pIniFile->UpdateKeyVal2(Node,"Order_DelegatePrice1",pData->Order_DelegatePrice1);
        pIniFile->UpdateKeyVal2(Node,"Order_UpperPrice1",pData->Order_UpperPrice1);
        pIniFile->UpdateKeyVal2(Node,"Order_LowestPrice1",pData->Order_LowestPrice1);

        //��Լ2
        pIniFile->UpdateKeyStr(Node,"Order_Instrument2",pData->Order_Instrument2.c_str());
        pIniFile->UpdateKeyVal(Node,"Order_Direction2",(int)pData->Order_Direction2);
        pIniFile->UpdateKeyVal(Node,"Order_Count2",pData->Order_Count2);
        pIniFile->UpdateKeyVal2(Node,"Order_Price2",pData->Order_Price2);
        pIniFile->UpdateKeyVal2(Node,"Order_DelegatePrice2",pData->Order_DelegatePrice2);
        pIniFile->UpdateKeyVal2(Node,"Order_UpperPrice2",pData->Order_UpperPrice2);
        pIniFile->UpdateKeyVal2(Node,"Order_LowestPrice2",pData->Order_LowestPrice2);


        string ref1,ref2;
        if(GetOrderRefByParamID(pData->Param_ID,ref1,ref2))
        {
            OrderKey key1,key2;
            if(GetOrderKeyByOrderRef(ref1,key1))
            {
                //OrderKey1
                pIniFile->UpdateKeyStr(Node,"OrderKey_Account1",key1.Account);
                pIniFile->UpdateKeyStr(Node,"OrderKey_Instrument1",key1.InstrumentID);
                pIniFile->UpdateKeyVal(Node,"OrderKey_FrontID1",key1.FrontID);
                pIniFile->UpdateKeyVal(Node,"OrderKey_SessionID1",key1.SessionID);
                pIniFile->UpdateKeyStr(Node,"OrderKey_OrderRef1",key1.OrderRef);
            }
            if(GetOrderKeyByOrderRef(ref2,key2))
            {
                //OrderKey2
                pIniFile->UpdateKeyStr(Node,"OrderKey_Account2",key2.Account);
                pIniFile->UpdateKeyStr(Node,"OrderKey_Instrument2",key2.InstrumentID);
                pIniFile->UpdateKeyVal(Node,"OrderKey_FrontID2",key2.FrontID);
                pIniFile->UpdateKeyVal(Node,"OrderKey_SessionID2",key2.SessionID);
                pIniFile->UpdateKeyStr(Node,"OrderKey_OrderRef2",key2.OrderRef);
            }
        }        
    }

    if(pIniFile)
    {
        delete pIniFile;
        pIniFile=NULL;
    }
}

void CArbitrageDelegationDlg::SetDataToListCtrl(bool bDevoke/* = false*/,bool bTrigger/* = false*/)
{    
    TRACE("Enter SetDataToListCtrl(bDevoke = %s,bTrigger = %s)\n",bDevoke?"true":"false",bTrigger?"true":"false");

    m_listCtrl.DeleteAllItems();

    CParamData* pData = NULL;
    for(map<int,CParamData*>::iterator it=m_mapIndex2Data.begin();it!=m_mapIndex2Data.end();it++)
    {
        pData = it->second; 

        int i=1;
        int nCount = m_listCtrl.GetItemCount();
        m_listCtrl.InsertItem(nCount,CTools::StringFromInt(pData->Param_ID).c_str());                    //��ˮ��
        m_listCtrl.SetItemText(nCount,i++,pData->Param_TriggerFlag?"����":"δ����");                     //����״̬
        m_listCtrl.SetItemText(nCount,i++,pData->Param_OffsetID?"ƽ��":"����");                          //��ƽ
        m_listCtrl.SetItemText(nCount,i++,pData->Param_ArbitrageType?"�۲���������":"�۲���С����");     //��������
        m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pData->Param_TriggerPrice).c_str());  //�����۲�
        m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pData->Param_DeviatePrice).c_str());  //ƫ����
        m_listCtrl.SetItemText(nCount,i++,pData->Order_Instrument1.c_str());                             //��Լ1
        m_listCtrl.SetItemText(nCount,i++,pData->Order_Direction1?"��":"��");                            //����1
        m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pData->Order_DelegatePrice1).c_str());//ί�м۲�1
        m_listCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pData->Order_Count1).c_str());           //ί������1
        m_listCtrl.SetItemText(nCount,i++,pData->Order_Instrument2.c_str());                             //��Լ2
        m_listCtrl.SetItemText(nCount,i++,pData->Order_Direction2?"��":"��");                            //����2
        m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pData->Order_DelegatePrice2).c_str());//ί�м۲�2
        m_listCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pData->Order_Count2).c_str());           //ί������2
    }

    if(bDevoke)
    {
        for(map<int,CParamData*>::iterator it=m_mapIndex2DataRevoked.begin();it!=m_mapIndex2DataRevoked.end();it++)
        {
            pData = it->second; 

            int i=1;
            int nCount = m_listCtrl.GetItemCount();
            m_listCtrl.InsertItem(nCount,CTools::StringFromInt(pData->Param_ID).c_str());                    //��ˮ��
            m_listCtrl.SetItemText(nCount,i++,pData->Param_RevokeFlag?"�ѳ���":"δ����");                    //����״̬
            m_listCtrl.SetItemText(nCount,i++,pData->Param_OffsetID?"ƽ��":"����");                          //��ƽ
            m_listCtrl.SetItemText(nCount,i++,pData->Param_ArbitrageType?"�۲���������":"�۲���С����");     //��������
            m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pData->Param_TriggerPrice).c_str());  //�����۲�
            m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pData->Param_DeviatePrice).c_str());  //ƫ����
            m_listCtrl.SetItemText(nCount,i++,pData->Order_Instrument1.c_str());                             //��Լ1
            m_listCtrl.SetItemText(nCount,i++,pData->Order_Direction1?"��":"��");                            //����1
            m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pData->Order_DelegatePrice1).c_str());//ί�м۲�1
            m_listCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pData->Order_Count1).c_str());           //ί������1
            m_listCtrl.SetItemText(nCount,i++,pData->Order_Instrument2.c_str());                             //��Լ2
            m_listCtrl.SetItemText(nCount,i++,pData->Order_Direction2?"��":"��");                            //����2
            m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pData->Order_DelegatePrice2).c_str());//ί�м۲�2
            m_listCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pData->Order_Count2).c_str());           //ί������2
        }
    }

    if(bTrigger)
    {
        for(map<int,CParamData*>::iterator it=m_mapIndex2DataTriggered.begin();it!=m_mapIndex2DataTriggered.end();it++)
        {
            pData = it->second; 

            int i=1;
            int nCount = m_listCtrl.GetItemCount();
            m_listCtrl.InsertItem(nCount,CTools::StringFromInt(pData->Param_ID).c_str());                    //��ˮ��
            m_listCtrl.SetItemText(nCount,i++,pData->Param_TriggerFlag?"����":"δ����");                     //����״̬
            m_listCtrl.SetItemText(nCount,i++,pData->Param_OffsetID?"ƽ��":"����");                          //��ƽ
            m_listCtrl.SetItemText(nCount,i++,pData->Param_ArbitrageType?"�۲���������":"�۲���С����");     //��������
            m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pData->Param_TriggerPrice).c_str());  //�����۲�
            m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pData->Param_DeviatePrice).c_str());  //ƫ����
            m_listCtrl.SetItemText(nCount,i++,pData->Order_Instrument1.c_str());                             //��Լ1
            m_listCtrl.SetItemText(nCount,i++,pData->Order_Direction1?"��":"��");                            //����1
            m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pData->Order_DelegatePrice1).c_str());//ί�м۲�1
            m_listCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pData->Order_Count1).c_str());           //ί������1
            m_listCtrl.SetItemText(nCount,i++,pData->Order_Instrument2.c_str());                             //��Լ2
            m_listCtrl.SetItemText(nCount,i++,pData->Order_Direction2?"��":"��");                            //����2
            m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pData->Order_DelegatePrice2).c_str());//ί�м۲�2
            m_listCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pData->Order_Count2).c_str());           //ί������2
        }
    }
}

void CArbitrageDelegationDlg::SubscribeInstrument()
{
    if(g_hWnd_FT!=NULL)
    {
        set<string> setSubInsts;
        PlusinAPI* pAPI=PlusinAPI::GetInstance();
        if(pAPI)
        {
            TRACE("2-lock\n");
            m_DataMutex.lock();
            CParamData* pData = NULL;
            for(map<int,CParamData*>::iterator it=m_mapIndex2Data.begin();it!=m_mapIndex2Data.end();it++)
            {
                pData = it->second;

                string szInst1(pData->Order_Instrument1);
                //transform(szInst1.begin(), szInst1.end(), szInst1.begin(), (int (*)(int))toupper);               

                string szInst2(pData->Order_Instrument2);
                //transform(szInst2.begin(), szInst2.end(), szInst2.begin(), (int (*)(int))toupper); 

                setSubInsts.insert(szInst1);
                setSubInsts.insert(szInst2);                
            }
            m_DataMutex.unlock();
            TRACE("2-unlock\n");

            for (std::set<string>::iterator it=setSubInsts.begin();it!=setSubInsts.end();it++)
            {
                TRACE("SubscribeInstrument inst = %s\n",(*it).c_str());

                //m_pMainDlg->m_Inst2SubedMutex.lock();
                if(m_mapInstrument2Subscribed.find((string)(*it)) == m_mapInstrument2Subscribed.end())
                    m_mapInstrument2Subscribed.insert(make_pair((string)(*it),false)); 
                //m_pMainDlg->m_Inst2SubedMutex.unlock();

                pAPI->Send(CMDID_QUOTE_SUBSCRIBE,(char*)(*it).c_str(),sizeof(InstrumentIDType));
                pAPI->Send(CMDID_QUOTE_SNAPSHOT_QUERY,(char*)(*it).c_str(),sizeof(InstrumentIDType));
            }
        }
    }
}

int CArbitrageDelegationDlg::GetListSelectedIndex()
{
    POSITION pos = m_listCtrl.GetFirstSelectedItemPosition(); 
    return m_listCtrl.GetNextSelectedItem(pos);
}

LRESULT CArbitrageDelegationDlg::OnSetupDlgClose(WPARAM wParam, LPARAM lParam)
{
    SetSetupDlgOpened(0);
    return 0;
}

LRESULT CArbitrageDelegationDlg::OnDetailDlgClose(WPARAM wParam, LPARAM lParam)
{
    SetDetailDlgOpened(0);
    return 0;
}

void CArbitrageDelegationDlg::OnBnClickedButtonArbitrage()
{
    // TODO: Add your control notification handler code here
     SetSetupDlgOpened(1);

     TRACE("3-lock\n");
     m_DataMutex.lock();

     CParamData* pParamData = new CParamData;
 
     COptSetupDlg dlg(pParamData,m_setInstrumentID,ARBITRAGE_ADD);
     m_pOptDlg = &dlg;
     INT_PTR nResponse = dlg.DoModal();
     if (nResponse == IDOK)
     {
         // TODO: Place code here to handle when the dialog is
         //  dismissed with OK 
         CTime time = CTime::GetCurrentTime();
         int nYear = time.GetYear()-BASE_YEAR;
         int nMonth = time.GetMonth();
         int nDay = time.GetDay();

         CString szDay;
         szDay.Format("%d-%d-%d",nYear,nMonth,nDay);

         pParamData->Param_ValidateCreateDay = time;
         pParamData->Param_ID = MAKE_DATEBASE_ID(nYear,nMonth,nDay,++m_nNextArbitrageID);

         if(m_pLog)
         {
             char buf[512];
             memset(buf,0,sizeof(buf));
             sprintf(buf,"������������ ParamID=%d,OffsetID=%d,ArbitrageType=%d,TriggerPrice=%.6f,DeviatePrice=%.6f,Instrument1=%s,Instrument2=%s,Direction1=%s,Direction2=%s,Count1=%d,Count1=%d,Price1=%.6f,Price2=%.6f\n",
                         pParamData->Param_ID,pParamData->Param_OffsetID,pParamData->Param_ArbitrageType,pParamData->Param_TriggerPrice,pParamData->Param_DeviatePrice,
                         pParamData->Order_Instrument1.c_str(),pParamData->Order_Instrument2.c_str(),                
                         pParamData->Order_Direction1==0?"��":"��",pParamData->Order_Direction2==0?"��":"��",
                         pParamData->Order_Count1,pParamData->Order_Count2,
                         pParamData->Order_Price1,pParamData->Order_Price2);

             m_pLog->Log(buf);
         }


         int nCount = m_listCtrl.GetItemCount();
         int i=1;
         m_listCtrl.InsertItem(nCount,CTools::StringFromInt(pParamData->Param_ID).c_str());                    //��ˮ��
         m_listCtrl.SetItemText(nCount,i++,"δ����");                                                          //����״̬
         m_listCtrl.SetItemText(nCount,i++,pParamData->Param_OffsetID?"ƽ��":"����");                          //��ƽ
         m_listCtrl.SetItemText(nCount,i++,pParamData->Param_ArbitrageType==0?"�۲���С����":"�۲���������");     //��������
         m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pParamData->Param_TriggerPrice).c_str());  //�����۲�
         m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pParamData->Param_DeviatePrice).c_str());  //ƫ����
         m_listCtrl.SetItemText(nCount,i++,pParamData->Order_Instrument1.c_str());                             //��Լ1
         m_listCtrl.SetItemText(nCount,i++,pParamData->Order_Direction1?"��":"��");                            //����1
         m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pParamData->Order_DelegatePrice1).c_str());//ί�м۲�1
         m_listCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pParamData->Order_Count1).c_str());        //ί������1
         m_listCtrl.SetItemText(nCount,i++,pParamData->Order_Instrument2.c_str());                             //��Լ2
         m_listCtrl.SetItemText(nCount,i++,pParamData->Order_Direction2?"��":"��");                            //����2
         m_listCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pParamData->Order_DelegatePrice2).c_str());//ί�м۲�2
         m_listCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pParamData->Order_Count2).c_str());        //ί������2 
         m_listCtrl.Update(nCount);

         //��������������Ҫִ��һ�� (ͣ���ڼ�)
         if(bInstsArrived(pParamData->Order_Instrument1,pParamData->Order_Instrument2) && EnterAlgorithm(pParamData))
         {
             //����  
             m_mapIndex2DataTriggered.insert(make_pair(pParamData->Param_ID,pParamData));

             if(m_bTrigger)
                 m_listCtrl.SetItemText(nCount,1,"����");                                                          //����״̬
             else
                m_listCtrl.DeleteItem(nCount);

             m_DataMutex.unlock();         
             TRACE("3-unlock\n");
             return;
         }


         m_mapIndex2Data.insert(make_pair(pParamData->Param_ID,pParamData));

     }
     else if (nResponse == IDCANCEL)
     {
         delete pParamData;
         pParamData = NULL;
     }

     m_DataMutex.unlock();         
     TRACE("3-unlock\n");
}

void CArbitrageDelegationDlg::OnBnClickedButtonModify()
{
    // TODO: Add your control notification handler code here
     int nSelectedIndex = GetListSelectedIndex();
     if(nSelectedIndex<0)
         return;    

     SetSetupDlgOpened(1);

     TRACE("4-lock\n");
     m_DataMutex.lock();

     int nParamID = _ttoi(m_listCtrl.GetItemText(nSelectedIndex,0));
     map<int,CParamData*>::iterator it = m_mapIndex2Data.find(nParamID);
     if(it==m_mapIndex2Data.end())  
         return;

     CParamData* pParamData = it->second;     
     if(pParamData)
     {
         COptSetupDlg dlg(pParamData,m_setInstrumentID,ARBITRAGE_MODIFY);
         m_pOptDlg = &dlg;
         INT_PTR nResponse = dlg.DoModal();
         if (nResponse == IDOK)
         {
             if(m_pLog)
             {
                 char buf[512];
                 memset(buf,0,sizeof(buf));
                 sprintf(buf,"�޸��������� ParamID=%d,TriggerPrice=%.6f,DeviatePrice=%.6f,ValidateDayText=%s,Count1=%d,Count2=%d,DelegatePrice1=%.6f,DelegatePrice2=%.6f\n",
                             pParamData->Param_ID,pParamData->Param_TriggerPrice,pParamData->Param_DeviatePrice,                             
                             pParamData->Param_ValidateDayText.c_str(),pParamData->Order_Count1,pParamData->Order_Count2,
                             pParamData->Order_DelegatePrice1,pParamData->Order_DelegatePrice2);

                 m_pLog->Log(buf);
             }


             //�޸ĵ���������Ҫִ��һ�� (ͣ���ڼ�)
             if(bInstsArrived(pParamData->Order_Instrument1,pParamData->Order_Instrument2) && EnterAlgorithm(pParamData))
             {
                 //����  
                 m_mapIndex2DataTriggered.insert(make_pair(pParamData->Param_ID,pParamData));
                 m_mapIndex2Data.erase(m_mapIndex2Data.find(pParamData->Param_ID));

                 if(m_bTrigger)
                 {
                    m_listCtrl.SetItemText(nSelectedIndex,1,"����"); 
                    m_listCtrl.SetItemText(nSelectedIndex,4,CTools::StringFromDouble(pParamData->Param_TriggerPrice).c_str());  //�����۲�
                    m_listCtrl.SetItemText(nSelectedIndex,5,CTools::StringFromDouble(pParamData->Param_DeviatePrice).c_str());   //ƫ����
                    m_listCtrl.SetItemText(nSelectedIndex,8,CTools::StringFromDouble(pParamData->Order_DelegatePrice1).c_str());//ί�м۲�1
                    m_listCtrl.SetItemText(nSelectedIndex,9,CTools::StringFromInt(pParamData->Order_Count1).c_str());        //ί������1
                    m_listCtrl.SetItemText(nSelectedIndex,12,CTools::StringFromDouble(pParamData->Order_DelegatePrice2).c_str());//ί�м۲�2
                    m_listCtrl.SetItemText(nSelectedIndex,13,CTools::StringFromInt(pParamData->Order_Count2).c_str());        //ί������2
                 }
                 else
                    m_listCtrl.DeleteItem(nSelectedIndex);
             } 
             else
             {
                 m_listCtrl.SetItemText(nSelectedIndex,4,CTools::StringFromDouble(pParamData->Param_TriggerPrice).c_str());  //�����۲�
                 m_listCtrl.SetItemText(nSelectedIndex,5,CTools::StringFromDouble(pParamData->Param_DeviatePrice).c_str());   //ƫ����
                 m_listCtrl.SetItemText(nSelectedIndex,8,CTools::StringFromDouble(pParamData->Order_DelegatePrice1).c_str());//ί�м۲�1
                 m_listCtrl.SetItemText(nSelectedIndex,9,CTools::StringFromInt(pParamData->Order_Count1).c_str());        //ί������1
                 m_listCtrl.SetItemText(nSelectedIndex,12,CTools::StringFromDouble(pParamData->Order_DelegatePrice2).c_str());//ί�м۲�2
                 m_listCtrl.SetItemText(nSelectedIndex,13,CTools::StringFromInt(pParamData->Order_Count2).c_str());        //ί������2
             }

         }
     }

     m_DataMutex.unlock();
     TRACE("4-unlock\n");
}

void CArbitrageDelegationDlg::OnBnClickedButtonRevoke()
{
    // TODO: Add your control notification handler code here
    int nSelectedIndex = GetListSelectedIndex();
    if(nSelectedIndex<0)
        return;

    int nParamID = _ttoi(m_listCtrl.GetItemText(nSelectedIndex,0));

    //����Ҫ���ԣ����Բ���delete
    TRACE("5-lock\n");
    m_DataMutex.lock();

    map<int,CParamData*>::iterator it = m_mapIndex2Data.find(nParamID);
    if(it!=m_mapIndex2Data.end())
    {
        CParamData* pData = it->second;
        if(pData)
        {
            if(IDCANCEL == ::MessageBox(NULL, "�Ƿ�ȷ�ϳ���?", "����", MB_OKCANCEL|MB_ICONWARNING|MB_SYSTEMMODAL))
            {
                m_DataMutex.unlock();
                TRACE("5-unlock\n");
                return;
            }

            pData->Param_RevokeFlag = true;

            if(m_pLog)
            {
                char buf[512];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"������������ ParamID=%d,OffsetID=%d,ArbitrageType=%d,TriggerPrice=%.6f,DeviatePrice=%.6f,\
                            Instrument1=%s,Instrument2=%s,Direction1=%s,Direction2=%s,\
                            Count1=%d,Count1=%d,Price1=%.6f,Price2=%.6f\n",
                            pData->Param_ID,pData->Param_OffsetID,pData->Param_ArbitrageType,pData->Param_TriggerPrice,pData->Param_DeviatePrice,
                            pData->Order_Instrument1.c_str(),pData->Order_Instrument2.c_str(),                
                            pData->Order_Direction1==0?"��":"��",pData->Order_Direction2==0?"��":"��",
                            pData->Order_Count1,pData->Order_Count2,
                            pData->Order_Price1,pData->Order_Price2);

                m_pLog->Log(buf);
            }
         }

        m_mapIndex2DataRevoked.insert(make_pair(nParamID,pData));
        m_mapIndex2Data.erase(it);
    }

    if(!m_bRevoke)
        m_listCtrl.DeleteItem(nSelectedIndex);
    else
        m_listCtrl.SetItemText(nSelectedIndex,1,"�ѳ���");

    m_DataMutex.unlock();
    TRACE("5-unlock\n");
}

void CArbitrageDelegationDlg::OnBnClickedButtonDelegatedetail()
{
    // TODO: Add your control notification handler code here  
    int nSelectedIndex = GetListSelectedIndex();
    if(nSelectedIndex<0)
        return;

    bool bFind = true;
    int nParamID = _ttoi(m_listCtrl.GetItemText(nSelectedIndex,0));
    map<int,CParamData*>::iterator it = m_mapIndex2Data.find(nParamID);
    if(it==m_mapIndex2Data.end())  
    {  
        it = m_mapIndex2DataRevoked.find(nParamID);
        if(it==m_mapIndex2DataRevoked.end())  
        {
            it = m_mapIndex2DataTriggered.find(nParamID);
            if(it==m_mapIndex2DataTriggered.end())  
                bFind = false;
        }
    }


    CParamData* pParamData = it->second;

    set<OrderKey> setOrderKey;
    if(!GetOrderKeyByParamID(pParamData->Param_ID,setOrderKey))
        bFind = false;

    if(!bFind)
    {
        ::MessageBox(NULL, "û���ҵ���Ӧ�ı�����ϸ", "��ʾ", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
        return;           
    }


    if(setOrderKey.empty())
    {
        CDelegationDetailDlg dlgDetail;
        m_pDetailDlg = &dlgDetail;
        SetDetailDlgOpened(1);

        INT_PTR nResponse = dlgDetail.DoModal();
        if (nResponse == IDOK)
        {
        }
        else if (nResponse == IDCANCEL)
        {
        }
    }
    else
    {
        CDelegationDetailDlg dlgDetail(setOrderKey);
        m_pDetailDlg = &dlgDetail;
        SetDetailDlgOpened(1);

        INT_PTR nResponse = dlgDetail.DoModal();
        if (nResponse == IDOK)
        {
        }
        else if (nResponse == IDCANCEL)
        {
        }
    }

    

}

void CArbitrageDelegationDlg::OnBnClickedCheckDisplayrevoke()
{
    // TODO: Add your control notification handler code here    
    m_bRevoke = !m_bRevoke;
    SetDataToListCtrl(m_bRevoke,m_bTrigger);

    (CButton*)GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(false);
    (CButton*)GetDlgItem(IDC_BUTTON_REVOKE)->EnableWindow(false);
    (CButton*)GetDlgItem(IDC_BUTTON_DELEGATEDETAIL)->EnableWindow(false);
}

void CArbitrageDelegationDlg::OnBnClickedCheckTriggered()
{
    // TODO: Add your control notification handler code here
    m_bTrigger = !m_bTrigger;    
    SetDataToListCtrl(m_bRevoke,m_bTrigger);

    (CButton*)GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(false);
    (CButton*)GetDlgItem(IDC_BUTTON_REVOKE)->EnableWindow(false);
    (CButton*)GetDlgItem(IDC_BUTTON_DELEGATEDETAIL)->EnableWindow(false);
}

void CArbitrageDelegationDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    if(pNMLV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED))
    {
        int nSelectedIndex = pNMLV->iItem;    

        int nParamID = _ttoi(m_listCtrl.GetItemText(nSelectedIndex,0));
        map<int,CParamData*>::iterator it = m_mapIndex2Data.find(nParamID);
        if(it==m_mapIndex2Data.end())  
        {  
            it = m_mapIndex2DataRevoked.find(nParamID);
            if(it==m_mapIndex2DataRevoked.end())  
            {
                it = m_mapIndex2DataTriggered.find(nParamID);
                if(it==m_mapIndex2DataTriggered.end())  
                    return;
            }
        }


        CParamData* pParamData = it->second;

        if(pParamData->Param_RevokeFlag || pParamData->Param_TriggerFlag)
            (CButton*)GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(false);
        else
            (CButton*)GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(true);

        if(!pParamData->Param_RevokeFlag && !pParamData->Param_TriggerFlag)
            (CButton*)GetDlgItem(IDC_BUTTON_REVOKE)->EnableWindow(true);
        else
            (CButton*)GetDlgItem(IDC_BUTTON_REVOKE)->EnableWindow(false);

        if(!pParamData->Param_TriggerFlag)
            (CButton*)GetDlgItem(IDC_BUTTON_DELEGATEDETAIL)->EnableWindow(false);
        else
            (CButton*)GetDlgItem(IDC_BUTTON_DELEGATEDETAIL)->EnableWindow(true);
    }
    else
    {
        (CButton*)GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(false);
        (CButton*)GetDlgItem(IDC_BUTTON_REVOKE)->EnableWindow(false);
        (CButton*)GetDlgItem(IDC_BUTTON_DELEGATEDETAIL)->EnableWindow(false);
    }

    *pResult = 0;
}

bool CArbitrageDelegationDlg::GetOrderRefByParamID(int nParamID,string& orderRef1,string& orderRef2)
{
    //m_OrderMutex.lock();    
    map<int,set<string>>::iterator it = m_mapParamID2OrderRef.find(nParamID);
    if(it!=m_mapParamID2OrderRef.end())
    {
        set<string> setOrderRef = it->second;
        int nIndex = 1;      
        for(set<string>::iterator itset=setOrderRef.begin();itset!=setOrderRef.end();itset++)
        {                  
            if(nIndex++==1)
                orderRef1 = *itset;
            else
                orderRef2 = *itset;
        }
    }        
    //m_OrderMutex.unlock();

    return (orderRef1.length()>0 && orderRef2.length()>0)?true:false;
}

bool CArbitrageDelegationDlg::GetOrderKeyByParamID(int nParamID,set<OrderKey>& setOrderKey)
{
    //m_OrderMutex.lock();
    map<int,set<string>>::iterator it = m_mapParamID2OrderRef.find(nParamID);
    if(it!=m_mapParamID2OrderRef.end())
    {
        set<string> setOrderRef = it->second;
        int nIndex=1;
        for(set<string>::iterator itset=setOrderRef.begin();itset!=setOrderRef.end();itset++)
        {
            map<string,OrderKey>::iterator it = m_mapOrderRef2OrderKey.find(*itset);
            if(it!=m_mapOrderRef2OrderKey.end())
            {
                OrderKey key = it->second;
                setOrderKey.insert(key);
            }
        }
    }
    //m_OrderMutex.unlock();

    return (setOrderKey.size()>0)?true:false;
}

bool CArbitrageDelegationDlg::GetOrderKeyByOrderRef(string szOrderRef,OrderKey& key)
{
    //m_OrderMutex.lock();
    map<string,OrderKey>::iterator it = m_mapOrderRef2OrderKey.find(szOrderRef);
    if(it!=m_mapOrderRef2OrderKey.end())
    {
        key = it->second;
        m_OrderMutex.unlock();
        return true;
    }
    //m_OrderMutex.unlock();

    return false;
}

void CArbitrageDelegationDlg::SetSetupDlgOpened(int nVal)
{
    int lnSimulate = nVal;
    InterlockedExchange((volatile long*)(&m_bSetupDlgOpened),lnSimulate);
}
bool CArbitrageDelegationDlg::GetSetupDlgOpened()
{
    int lnSimulate = 0;
    InterlockedExchange((volatile long*)(&lnSimulate),m_bSetupDlgOpened);
    return lnSimulate != 0;
}

void CArbitrageDelegationDlg::SetDetailDlgOpened(int nVal)
{
    int lnSimulate = nVal;
    InterlockedExchange((volatile long*)(&m_bDetailDlgOpened),lnSimulate);
}
bool CArbitrageDelegationDlg::GetDetailDlgOpened()
{
    int lnSimulate = 0;
    InterlockedExchange((volatile long*)(&lnSimulate),m_bDetailDlgOpened);
    return lnSimulate != 0;
}
