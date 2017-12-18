#include "stdafx.h"

#include "TradingInfo.h"
//#include "PlatFormService.hpp"
//#include "Module-Misc2/globalfunc.h"
//#include "ISvr/IPlatformSingleSvr.h"
//#include "ISvr/PlatformSvrMgr.h"
//#include "Module-Misc2/GlobalConfigMgr.h"
//#include "Module-Misc2/globalDefines.h"
//#include "ReqOnRtnTrade.h"
#include "Tools_Ansi.h"
#include "Tools_Win32.h"
//#include "easytime.h"
//#include "FileOpr.h"
//#include "myxml.h"


Ceasymutex CTradingInfo::m_mutex;


//********************************************************************************************
//  ��ȡxml��Ϣ����
//
//  ��xml����ȡ������������ȡƷ�ֲ�������Լ�����������Ѳ�������֤�����
//  <Instruments>
//      <colname>InstrumentID,InvestorRange,BrokerID,InvestorID,OpenRatioByMoney,OpenRatioByVolume,CloseRatioByMoney,CloseRatioByVolume,CloseTodayRatioByMoney,CloseTodayRatioByVolume,StrikeRatioByMoney,StrikeRatioByVolume</colname>
//      <lines>
//          <line>...</line>
//          <line>...</line>
//      </lines>
//  </Instruments>
//
//����ʹ��malloc�ķ��������ڴ棬�����й����ڴ棬������stl������
//ԭ������debugģʽ�£������Լ������1400�����ң���stl���ʱ5-6��
//*********************************************************************************


//��ȡ�����������ơ������Ƽ�����xml�ļ����ƣ�������������������tradingparam��product��instrument��commissionrate��marginrate
string CTradingInfo::GetDataTypeName(eDataStructType datatype)
{
    switch(datatype)
    {
        case eTradingParams:    return string("tradingparam");
        case eProductInfos:     return string("product");
        case eInstrumentInfos:  return string("instrument");
        case eCommissionRates:  return string("commissionrate");
        case eMarginRates:      return string("marginrate");
    }
    return string("");
}

//��ȡxml�ļ����ƣ���·��
string CTradingInfo::GetXMLFileName(eDataStructType datatype)
{
    return CTools_Win32::GetMainPath()+GetDataTypeName(datatype)+string(".xml");
}

//��ȡ[SvrGrpIDxxx-Accountyyy]������
string CTradingInfo::GetAccountNodeName(int SvrGrpID,const string& strAccount)
{
    return string("SvrGrpID")+CTools_Ansi::GetStringFromInteger(SvrGrpID)+string("-Account")+strAccount;
}


//Loadʱ���ȼ��xml�Ƿ�Ϸ���1. ������ǵ�ǰ�����գ�����������ļ�; 2. ���û��<SvrGrpIDxxx-Accountyyy>�ڣ��򴴽�֮
void CTradingInfo::CheckAfterLogin_Internal(CMyXML& myxml)
{
    string AccountNodeName=GetAccountNodeName(m_SvrGrpID,m_strAccount);
    string xmlfilename=GetXMLFileName(m_datatype);

    //��齻�����Ƿ���ȷ������ȷ�����֮
    if(myxml.m_bvalid&&myxml.GetNodeContent2(string(""),"root","tradingday")!=m_strTradingDay)
    {
        unlink(xmlfilename.c_str());
        myxml.m_bvalid=false;
    }

    if(!myxml.m_bvalid)
    {
        //������´�xml��û���򴴽��յ�
        myxml=CMyXML(xmlfilename.c_str(),true,true);
        myxml.SaveXML();
    }

    //��齻���������Ƿ���ڣ������������
    string tmpstr;
    if(!myxml.GetNodeContent(tmpstr,"root","tradingday"))
        myxml.ModifyOrAppendNode_save(CMyXML(string("tradingday"),m_strTradingDay),"root");

    //���<SvrGrpIDxxx-Accountyyy>���Ƿ���ڣ��������򴴽�֮
    if(!myxml.GetNode(false,NULL,"root",AccountNodeName.c_str()))
        myxml.ModifyOrAppendNode_save(CMyXML(AccountNodeName,string("")),"root");
}



CTradingInfo::CTradingInfo(eDataStructType datatype,int SvrGrpID,const string& strAccount,const string& strTradingDay)
:   m_datatype(datatype)
    ,m_SvrGrpID(SvrGrpID)
    ,m_strAccount(strAccount)
    ,m_strTradingDay(strTradingDay)
    ,m_bValid(false)
    ,m_ColCnt(0)
    ,m_LineCnt(0)
    ,m_pLineContent(NULL)
    ,m_psubposs(NULL)
    ,m_psublens(NULL)
{
}

CTradingInfo::~CTradingInfo()
{
    for(int i=0;i<m_LineCnt;i++) if(m_pLineContent[i]!=NULL) { free(m_pLineContent[i]);  m_pLineContent[i]=NULL; }
    if(m_pLineContent) { free(m_pLineContent);  m_pLineContent=NULL; }
    if(m_psubposs) { free(m_psubposs);  m_psubposs=NULL; }
    if(m_psublens) { free(m_psublens);  m_psublens=NULL; }
}

//��xml�м������ݣ����зŵ�m_pLineContent�У�m_psubposs��m_psublens��¼ÿ��ÿ���ֶε�λ�úͳ���
void CTradingInfo::Load()
{
    //��xml��ȡ���������ݡ�
    //���û�������Ϊ: [root]-[SvrGrpIDxxx-Accountyyy]-[NodeName]
    string DataTypeName=GetDataTypeName(m_datatype);
    string AccountNodeName=GetAccountNodeName(m_SvrGrpID,m_strAccount);

    CMyXML myxml(GetXMLFileName(m_datatype).c_str(),true,false);
    CheckAfterLogin_Internal(myxml);

    CMyXML* pnodexml=myxml.GetNode(false,NULL,"root",AccountNodeName.c_str(),DataTypeName.c_str());
    if(!pnodexml) return;

    //�������ͷ����
    m_vecColName=CTools_Ansi::SplitString(pnodexml->GetNodeContent2(string(""),pnodexml->m_name.c_str(),"colname").c_str(),',');

    multimap<string,CMyXML>::const_iterator itlines=pnodexml->m_mapsubnode.find(string("lines"));
    if(itlines==pnodexml->m_mapsubnode.end()) return;
    if(itlines->second.m_mapsubnode.size()==0) return;

    //����
    m_LineCnt=itlines->second.m_mapsubnode.size();

    //�ֶ�����
    m_ColCnt=m_vecColName.size();

    //��ԭʼ�ı�ָ�����飬ÿ��ÿ���ֶε�λ�úͳ�������
    m_pLineContent=(char**)malloc(sizeof(char*)*m_LineCnt);
    m_psubposs=(int*)malloc(sizeof(int)*m_LineCnt*m_ColCnt);
    m_psublens=(int*)malloc(sizeof(int)*m_LineCnt*m_ColCnt);
    memset(m_pLineContent,0,sizeof(char*)*m_LineCnt);
    memset(m_psubposs,0,sizeof(int)*m_LineCnt*m_ColCnt);
    memset(m_psublens,0,sizeof(int)*m_LineCnt*m_ColCnt);

    //�����ÿһ�����ݵ�ÿ���ֶε�λ�úͳ��ȣ�����subposs��sublens��
    multimap<string,CMyXML>::const_iterator it;
    int line=0;
    for(it=itlines->second.m_mapsubnode.begin();it!=itlines->second.m_mapsubnode.end();it++,line++)
    {
        if(it->second.m_content.empty()) break;

        //ȡһ�е�ԭʼ����
        m_pLineContent[line]=(char*)malloc(it->second.m_content.size()+1);
        memcpy(m_pLineContent[line],it->second.m_content.c_str(),it->second.m_content.size());
        m_pLineContent[line][it->second.m_content.size()]=0;

        int rltcolcnt;
        if(!CTools_Ansi::SplitString(m_psubposs+m_ColCnt*line,m_psublens+m_ColCnt*line,rltcolcnt,m_ColCnt,m_pLineContent[line],','))
            break;
        if(rltcolcnt!=m_ColCnt) break;
    }

    if(it!=itlines->second.m_mapsubnode.end())
    {
        //���ݸ�ʽ����������ʧ��
        for(int i=0;i<m_LineCnt;i++) if(m_pLineContent[i]!=NULL) { free(m_pLineContent[i]);  m_pLineContent[i]=NULL; }
        if(m_pLineContent) { free(m_pLineContent);  m_pLineContent=NULL; }
        if(m_psubposs) { free(m_psubposs);  m_psubposs=NULL; }
        if(m_psublens) { free(m_psublens);  m_psublens=NULL; }
        m_ColCnt=0;
        m_LineCnt=0;
    }
    else
    {
        //�����Ƿ�Ϸ�
        m_bValid = m_LineCnt>0&&m_ColCnt>0;
    }
}


//********************************************************************************************
//  ����xml��Ϣ����
//  ʹ��CTradingInfo::m_mutex
//  ������Ϊ: [root]-[SvrGrpIDxxx-Accountyyy]-[NodeName]
//  ��������<colname>��
//  ���ݷ���<lines>
//  bEraseOldInfo��ʾ�Ƿ�ɾ��ԭ�����ݡ�Ʒ�֡���Լ�����ײ���ʱһ���Ա��棬����ɾ��ԭ�����ݡ��������յ�һ����һ��������ɾ��ԭ������(�ڵ�¼�����֤���Ѿ�����������ɾ����)��
//  ���磺
//      <Instruments>
//          <colname>InstrumentID,InvestorRange,BrokerID,InvestorID,OpenRatioByMoney,OpenRatioByVolume,CloseRatioByMoney,CloseRatioByVolume,CloseTodayRatioByMoney,CloseTodayRatioByVolume,StrikeRatioByMoney,StrikeRatioByVolume</colname>
//          <lines>
//              <line>...</line>
//              <line>...</line>
//          </lines>
//      </Instruments>
//********************************************************************************************
CSaveTradingInfo::CSaveTradingInfo(CTradingInfo::eDataStructType datatype,int SvrGrpID,const string& strAccount)
:   m_datatype(datatype)
    ,m_SvrGrpID(SvrGrpID)
    ,m_strAccount(strAccount)
    ,m_plinecontent(NULL)
    ,m_pcolname(NULL)
    ,m_linebuflen(0)
    ,m_colnamelen(0)
    ,m_pxml(NULL)
    ,m_pnodexml(NULL)
    ,m_pcolnamexml(NULL)
    ,m_plinesxml(NULL)
{
}
CSaveTradingInfo::~CSaveTradingInfo()
{
    if(m_plinecontent) { free(m_plinecontent);  m_plinecontent=NULL; }
    if(m_pcolname) { free(m_pcolname);  m_pcolname=NULL; }
    if(m_pxml) {  delete(m_pxml);  m_pxml=NULL; }
}
void CSaveTradingInfo::OpenXML4Save(bool bEraseOldInfo)
{
    m_plinecontent=(char*)malloc(m_linebufsize);
    m_pcolname=(char*)malloc(m_linebufsize);
    if(!m_plinecontent||!m_pcolname) return;

    string DataTypeName=CTradingInfo::GetDataTypeName(m_datatype);
    string AccountNodeName=CTradingInfo::GetAccountNodeName(m_SvrGrpID,m_strAccount);
    string xmlfilename=CTradingInfo::GetXMLFileName(m_datatype);
    m_pxml=new CMyXML(xmlfilename.c_str(),true,false);
    if(!m_pxml) return;
 
    CMyXML* paccountnode=m_pxml->GetNode(true,NULL,"root",AccountNodeName.c_str());
    if(!paccountnode) return;

    //���ײ�����Ʒ�֡���Լ��Ҫ��ɾ����������
    if(bEraseOldInfo)
    {
        paccountnode->m_mapsubnode.clear();
    }

    m_pnodexml=paccountnode->GetNode(true,NULL,NULL,DataTypeName.c_str());
    if(!m_pnodexml) return;

    m_pcolnamexml=m_pnodexml->GetNode(true,NULL,NULL,"colname");
    m_plinesxml=m_pnodexml->GetNode(true,NULL,NULL,"lines");
}





//���ڼ��صĺ궨�壬ȡ�������е�ĳһ���ֶ�ֵ���ŵ�m_pLineData��
#define min2(x,y) (x<=y?x:y)
#define CopyItem_String(colname,col) \
    {\
        for(int line=0;line<m_LineCnt;line++)\
            memcpy(m_pLineData[line].##colname,m_pLineContent[line]+m_psubposs[m_ColCnt*line+col],min2(sizeof(m_pLineData[line].##colname)-1,m_psublens[m_ColCnt*line+col]));\
    }
#define CopyItem_Integer(colname,col)\
    {\
        for(int line=0;line<m_LineCnt;line++)\
            m_pLineData[line].##colname=CTools_Ansi::GetIntegerFromString(m_pLineContent[line]+m_psubposs[m_ColCnt*line+col]);\
    }
#define CopyItem_char(colname,col)\
    {\
        for(int line=0;line<m_LineCnt;line++)\
            m_pLineData[line].##colname=(char)CTools_Ansi::GetIntegerFromString(m_pLineContent[line]+m_psubposs[m_ColCnt*line+col]);\
    }
#define CopyItem_Double(colname,col)\
    {\
        for(int line=0;line<m_LineCnt;line++)\
        {\
            if(m_psublens[m_ColCnt*line+col]==16)\
            {\
                m_pLineData[line].##colname=CTools_Ansi::GetDoubleFromHexString(m_pLineContent[line]+m_psubposs[m_ColCnt*line+col],m_psublens[m_ColCnt*line+col]);\
            }\
        }\
    }

//���ڱ���ĺ궨�壺��(�������Info��)content������ӵ�m_plinecontent�ַ������档����/�ߴ�Ϊm_linebuflen/m_linebufsize�����bgetcolname=true����ͬʱ��ȡ�����ַ������ߴ�Ϊm_colnamelen/m_linebufsize
#define JoinContent(colname,content) \
        {\
            int len=strlen(content);\
            if(m_linebuflen+len+1<m_linebufsize)\
            {\
                memcpy(m_plinecontent+m_linebuflen,content,len);\
                m_linebuflen+=len;\
                m_plinecontent[m_linebuflen++]=',';\
                m_plinecontent[m_linebuflen]=0;\
            }\
            if(bgetcolname)\
            {\
                len=strlen(colname);\
                if(m_colnamelen+len+1<m_linebufsize)\
                {\
                    memcpy(m_pcolname+m_colnamelen,colname,len);\
                    m_colnamelen+=len;\
                    m_pcolname[m_colnamelen++]=',';\
                    m_pcolname[m_colnamelen]=0;\
                }\
            }\
        }
#define JoinContent_String(colname) JoinContent(#colname,Info.##colname)        
#define JoinContent_Integer(colname) { string strcontent=CTools_Ansi::GetStringFromInteger((int)Info.##colname);  const char* pcontent=strcontent.c_str();  JoinContent(#colname,pcontent) }
#define JoinContent_Double(colname) { string strcontent=CTools_Ansi::GetHexStringFromDouble((double)Info.##colname);  const char* pcontent=strcontent.c_str();  JoinContent(#colname,pcontent) }


//Ʒ����Ϣ
CTradingInfo_PlatformStru_ProductInfo::CTradingInfo_PlatformStru_ProductInfo(int SvrGrpID,const string& strAccount,const string& strTradingDay)
:   CTradingInfo(eProductInfos,SvrGrpID,strAccount,strTradingDay)
    ,CSaveTradingInfo(eProductInfos,SvrGrpID,strAccount)
    ,m_pLineData(NULL)
{
}
CTradingInfo_PlatformStru_ProductInfo::~CTradingInfo_PlatformStru_ProductInfo()
{
    if(m_pLineData) { free(m_pLineData);  m_pLineData=NULL; }
}
void CTradingInfo_PlatformStru_ProductInfo::Load(void)
{
    CeasymutexGuard guard(m_mutex);

    CTradingInfo::Load();
    m_pLineData=(PlatformStru_ProductInfo*)malloc(sizeof(PlatformStru_ProductInfo)*m_LineCnt);
    memset(m_pLineData,0,sizeof(PlatformStru_ProductInfo)*m_LineCnt);

    //��ÿһ�У���ÿ����Լԭʼ������ȡ����ֵ���ŵ���Ӧ��Լ��Ϣ��ȥ
    for(int i=0;i<(int)m_vecColName.size();i++)
    {
        if(strcmp(m_vecColName[i].c_str(),"ProductID")==0) CopyItem_String(ProductID,i)
        else if(strcmp(m_vecColName[i].c_str(),"ProductName")==0) CopyItem_String(ProductName,i)
        else if(strcmp(m_vecColName[i].c_str(),"ExchangeID")==0) CopyItem_String(ExchangeID,i)
        else if(strcmp(m_vecColName[i].c_str(),"ProductClass")==0) CopyItem_char(ProductClass,i)
        else if(strcmp(m_vecColName[i].c_str(),"VolumeMultiple")==0) CopyItem_Integer(VolumeMultiple,i)
        else if(strcmp(m_vecColName[i].c_str(),"PriceTick")==0) CopyItem_Double(PriceTick,i)
        else if(strcmp(m_vecColName[i].c_str(),"MaxMarketOrderVolume")==0) CopyItem_Integer(MaxMarketOrderVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"MinMarketOrderVolume")==0) CopyItem_Integer(MinMarketOrderVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"MaxLimitOrderVolume")==0) CopyItem_Integer(MaxLimitOrderVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"MinLimitOrderVolume")==0) CopyItem_Integer(MinLimitOrderVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"PositionType")==0) CopyItem_char(PositionType,i)
        else if(strcmp(m_vecColName[i].c_str(),"PositionDateType")==0) CopyItem_char(PositionDateType,i)
        else if(strcmp(m_vecColName[i].c_str(),"CloseDealType")==0) CopyItem_char(CloseDealType,i)
        else if(strcmp(m_vecColName[i].c_str(),"TradeCurrencyID")==0) CopyItem_String(TradeCurrencyID,i)
        else if(strcmp(m_vecColName[i].c_str(),"MortgageFundUseRange")==0) CopyItem_char(MortgageFundUseRange,i)
        else if(strcmp(m_vecColName[i].c_str(),"ExchangeProductID")==0) CopyItem_String(ExchangeProductID,i)
        else if(strcmp(m_vecColName[i].c_str(),"UnderlyingMultiple")==0) CopyItem_Double(UnderlyingMultiple,i)
        else if(strcmp(m_vecColName[i].c_str(),"ProductShortName")==0) CopyItem_String(ProductShortName,i)
    }
}
void CTradingInfo_PlatformStru_ProductInfo::SaveXML(const vector<PlatformStru_ProductInfo>& Infos)
{
    CeasymutexGuard guard(m_mutex);

    OpenXML4Save(true);

    if(!m_plinecontent||!m_pcolname||!m_pxml||!m_pnodexml||!m_pcolnamexml||!m_plinesxml) return;
    memset(m_pcolname,0,m_linebufsize);
    m_colnamelen=0;
    bool bgetcolname=true;
    for(vector<PlatformStru_ProductInfo>::const_iterator it=Infos.begin();it!=Infos.end();it++)
    {
        const PlatformStru_ProductInfo& Info=*it;
        memset(m_plinecontent,0,m_linebufsize);
        m_linebuflen=0;

        JoinContent_String(ProductID)
        JoinContent_String(ProductName)
        JoinContent_String(ExchangeID)
        JoinContent_Integer(ProductClass)
        JoinContent_Integer(VolumeMultiple)
        JoinContent_Double(PriceTick)
        JoinContent_Integer(MaxMarketOrderVolume)
        JoinContent_Integer(MinMarketOrderVolume)
        JoinContent_Integer(MaxLimitOrderVolume)
        JoinContent_Integer(MinLimitOrderVolume)
        JoinContent_Integer(PositionType)
        JoinContent_Integer(PositionDateType)
        JoinContent_Integer(CloseDealType)
        JoinContent_String(TradeCurrencyID)
        JoinContent_Integer(MortgageFundUseRange)
        JoinContent_String(ExchangeProductID)
        JoinContent_Double(UnderlyingMultiple)
        JoinContent_String(ProductShortName)

        if(m_linebuflen>0) m_plinecontent[--m_linebuflen]=0;
        multimap<string,CMyXML>::iterator itnew=m_plinesxml->m_mapsubnode.insert(make_pair(string("line"),CMyXML(string("line"),string(""))));
        itnew->second.m_content=m_plinecontent;

        if(bgetcolname) bgetcolname=false;
    }
    if(m_colnamelen>0) m_pcolname[--m_colnamelen]=0;
    m_pcolnamexml->m_content=string(m_pcolname);

    if(m_pxml) m_pxml->SaveXML();
}

//��Լ��Ϣ
CTradingInfo_PlatformStru_InstrumentInfo::CTradingInfo_PlatformStru_InstrumentInfo(int SvrGrpID,const string& strAccount,const string& strTradingDay)
:   CTradingInfo(eInstrumentInfos,SvrGrpID,strAccount,strTradingDay)
    ,CSaveTradingInfo(eInstrumentInfos,SvrGrpID,strAccount)
    ,m_pLineData(NULL)
{
}
CTradingInfo_PlatformStru_InstrumentInfo::~CTradingInfo_PlatformStru_InstrumentInfo()
{
    if(m_pLineData) { free(m_pLineData);  m_pLineData=NULL; }
}
void CTradingInfo_PlatformStru_InstrumentInfo::Load(void)
{
    CeasymutexGuard guard(m_mutex);

    CTradingInfo::Load();
    m_pLineData=(PlatformStru_InstrumentInfo*)malloc(sizeof(PlatformStru_InstrumentInfo)*m_LineCnt);
    memset(m_pLineData,0,sizeof(PlatformStru_InstrumentInfo)*m_LineCnt);

    //��ÿһ�У���ÿ����Լԭʼ������ȡ����ֵ���ŵ���Ӧ��Լ��Ϣ��ȥ
    for(int i=0;i<(int)m_vecColName.size();i++)
    {
        if(strcmp(m_vecColName[i].c_str(),"InstrumentID")==0) CopyItem_String(InstrumentID,i)
        else if(strcmp(m_vecColName[i].c_str(),"ExchangeID")==0) CopyItem_String(ExchangeID,i)
        else if(strcmp(m_vecColName[i].c_str(),"InstrumentName")==0) CopyItem_String(InstrumentName,i)
        else if(strcmp(m_vecColName[i].c_str(),"ExchangeInstID")==0) CopyItem_String(ExchangeInstID,i)
        else if(strcmp(m_vecColName[i].c_str(),"ProductID")==0) CopyItem_String(ProductID,i)

        else if(strcmp(m_vecColName[i].c_str(),"ProductClass")==0) CopyItem_char(ProductClass,i)
        else if(strcmp(m_vecColName[i].c_str(),"DeliveryYear")==0) CopyItem_Integer(DeliveryYear,i)
        else if(strcmp(m_vecColName[i].c_str(),"DeliveryMonth")==0) CopyItem_Integer(DeliveryMonth,i)
        else if(strcmp(m_vecColName[i].c_str(),"MaxMarketOrderVolume")==0) CopyItem_Integer(MaxMarketOrderVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"MinMarketOrderVolume")==0) CopyItem_Integer(MinMarketOrderVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"MaxLimitOrderVolume")==0) CopyItem_Integer(MaxLimitOrderVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"MinLimitOrderVolume")==0) CopyItem_Integer(MinLimitOrderVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"VolumeMultiple")==0) CopyItem_Integer(VolumeMultiple,i)

        else if(strcmp(m_vecColName[i].c_str(),"PriceTick")==0) CopyItem_Double(PriceTick,i)

        else if(strcmp(m_vecColName[i].c_str(),"CreateDate")==0) CopyItem_String(CreateDate,i)
        else if(strcmp(m_vecColName[i].c_str(),"OpenDate")==0) CopyItem_String(OpenDate,i)
        else if(strcmp(m_vecColName[i].c_str(),"ExpireDate")==0) CopyItem_String(ExpireDate,i)
        else if(strcmp(m_vecColName[i].c_str(),"StartDelivDate")==0) CopyItem_String(StartDelivDate,i)
        else if(strcmp(m_vecColName[i].c_str(),"EndDelivDate")==0) CopyItem_String(EndDelivDate,i)

        else if(strcmp(m_vecColName[i].c_str(),"InstLifePhase")==0) CopyItem_char(InstLifePhase,i)
        else if(strcmp(m_vecColName[i].c_str(),"IsTrading")==0) CopyItem_Integer(IsTrading,i)
        else if(strcmp(m_vecColName[i].c_str(),"PositionType")==0) CopyItem_char(PositionType,i)
        else if(strcmp(m_vecColName[i].c_str(),"PositionDateType")==0) CopyItem_char(PositionDateType,i)
        else if(strcmp(m_vecColName[i].c_str(),"LongMarginRatio")==0) CopyItem_Double(LongMarginRatio,i)
        else if(strcmp(m_vecColName[i].c_str(),"ShortMarginRatio")==0) CopyItem_Double(ShortMarginRatio,i)
        else if(strcmp(m_vecColName[i].c_str(),"MaxMarginSideAlgorithm")==0) CopyItem_char(MaxMarginSideAlgorithm,i)

        else if(strcmp(m_vecColName[i].c_str(),"UnderlyingInstrID")==0) CopyItem_String(UnderlyingInstrID,i)
        else if(strcmp(m_vecColName[i].c_str(),"StrikePrice")==0) CopyItem_Double(StrikePrice,i)
        else if(strcmp(m_vecColName[i].c_str(),"OptionsType")==0) CopyItem_char(OptionsType,i)
        else if(strcmp(m_vecColName[i].c_str(),"UnderlyingMultiple")==0) CopyItem_Double(UnderlyingMultiple,i)
        else if(strcmp(m_vecColName[i].c_str(),"CombinationType")==0) CopyItem_char(CombinationType,i)

        else if(strcmp(m_vecColName[i].c_str(),"CombMarginDiscountMode")==0) CopyItem_Integer(CombMarginDiscountMode,i)
        else if(strcmp(m_vecColName[i].c_str(),"UnwindPriorities")==0) CopyItem_Integer(UnwindPriorities,i)
        else if(strcmp(m_vecColName[i].c_str(),"PriceForMarginOfTodayPosition")==0) CopyItem_Integer(PriceForMarginOfTodayPosition,i)
        else if(strcmp(m_vecColName[i].c_str(),"CloseTodayInstructionSupport")==0) CopyItem_Integer(CloseTodayInstructionSupport,i)
        else if(strcmp(m_vecColName[i].c_str(),"CloseInstructionSupport")==0) CopyItem_Integer(CloseInstructionSupport,i)
    }
}
void CTradingInfo_PlatformStru_InstrumentInfo::SaveXML(const vector<PlatformStru_InstrumentInfo>& Infos)
{
    CeasymutexGuard guard(m_mutex);

    OpenXML4Save(true);

    if(!m_plinecontent||!m_pcolname||!m_pxml||!m_pnodexml||!m_pcolnamexml||!m_plinesxml) return;
    memset(m_pcolname,0,m_linebufsize);
    m_colnamelen=0;
    bool bgetcolname=true;
    for(vector<PlatformStru_InstrumentInfo>::const_iterator it=Infos.begin();it!=Infos.end();it++)
    {
        const PlatformStru_InstrumentInfo& Info=*it;
        memset(m_plinecontent,0,m_linebufsize);
        m_linebuflen=0;

        JoinContent_String(InstrumentID)
        JoinContent_String(ExchangeID)
        JoinContent_String(InstrumentName)
        JoinContent_String(ExchangeInstID)
        JoinContent_String(ProductID)
        
        JoinContent_Integer(ProductClass)
        JoinContent_Integer(DeliveryYear)
        JoinContent_Integer(DeliveryMonth)
        JoinContent_Integer(MaxMarketOrderVolume)
        JoinContent_Integer(MinMarketOrderVolume)
        JoinContent_Integer(MaxLimitOrderVolume)
        JoinContent_Integer(MinLimitOrderVolume)
        JoinContent_Integer(VolumeMultiple)

        JoinContent_Double(PriceTick)

        JoinContent_String(CreateDate)
        JoinContent_String(OpenDate)
        JoinContent_String(ExpireDate)
        JoinContent_String(StartDelivDate)
        JoinContent_String(EndDelivDate)

        JoinContent_Integer(InstLifePhase)
        JoinContent_Integer(IsTrading)
        JoinContent_Integer(PositionType)
        JoinContent_Integer(PositionDateType)
        JoinContent_Double(LongMarginRatio)
        JoinContent_Double(ShortMarginRatio)
        JoinContent_Integer(MaxMarginSideAlgorithm)

        JoinContent_String(UnderlyingInstrID)
        JoinContent_Double(StrikePrice)
        JoinContent_Integer(OptionsType)
        JoinContent_Double(UnderlyingMultiple)
        JoinContent_Integer(CombinationType)

        JoinContent_Integer(CombMarginDiscountMode)
        JoinContent_Integer(UnwindPriorities)
        JoinContent_Integer(PriceForMarginOfTodayPosition)
        JoinContent_Integer(CloseTodayInstructionSupport)
        JoinContent_Integer(CloseInstructionSupport)

        if(m_linebuflen>0) m_plinecontent[--m_linebuflen]=0;
        multimap<string,CMyXML>::iterator itnew=m_plinesxml->m_mapsubnode.insert(make_pair(string("line"),CMyXML(string("line"),string(""))));
        itnew->second.m_content=m_plinecontent;

        if(bgetcolname) bgetcolname=false;
    }
    if(m_colnamelen>0) m_pcolname[--m_colnamelen]=0;
    m_pcolnamexml->m_content=string(m_pcolname);

    if(m_pxml) m_pxml->SaveXML();
}


//����������Ϣ
CTradingInfo_PlatformStru_InstrumentCommissionRate::CTradingInfo_PlatformStru_InstrumentCommissionRate(int SvrGrpID,const string& strAccount,const string& strTradingDay)
:   CTradingInfo(eCommissionRates,SvrGrpID,strAccount,strTradingDay)
    ,CSaveTradingInfo(eCommissionRates,SvrGrpID,strAccount)
    ,m_pLineData(NULL)
{
}
CTradingInfo_PlatformStru_InstrumentCommissionRate::~CTradingInfo_PlatformStru_InstrumentCommissionRate()
{
    if(m_pLineData) { free(m_pLineData);  m_pLineData=NULL; }
}
void CTradingInfo_PlatformStru_InstrumentCommissionRate::Load(void)
{
    CeasymutexGuard guard(m_mutex);

    CTradingInfo::Load();
    m_pLineData=(PlatformStru_InstrumentCommissionRate*)malloc(sizeof(PlatformStru_InstrumentCommissionRate)*m_LineCnt);
    memset(m_pLineData,0,sizeof(PlatformStru_InstrumentCommissionRate)*m_LineCnt);

    //��ÿһ�У���ÿ����Լԭʼ������ȡ����ֵ���ŵ���Ӧ��Լ��Ϣ��ȥ
    for(int i=0;i<(int)m_vecColName.size();i++)
    {
        if(strcmp(m_vecColName[i].c_str(),"InstrumentID")==0) CopyItem_String(InstrumentID,i)
        else if(strcmp(m_vecColName[i].c_str(),"InvestorRange")==0) CopyItem_char(InvestorRange,i)
        else if(strcmp(m_vecColName[i].c_str(),"BrokerID")==0) CopyItem_String(BrokerID,i)
        else if(strcmp(m_vecColName[i].c_str(),"InvestorID")==0) CopyItem_String(InvestorID,i)
        else if(strcmp(m_vecColName[i].c_str(),"OpenRatioByMoney")==0) CopyItem_Double(OpenRatioByMoney,i)
        else if(strcmp(m_vecColName[i].c_str(),"OpenRatioByVolume")==0) CopyItem_Double(OpenRatioByVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"CloseRatioByMoney")==0) CopyItem_Double(CloseRatioByMoney,i)
        else if(strcmp(m_vecColName[i].c_str(),"CloseRatioByVolume")==0) CopyItem_Double(CloseRatioByVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"CloseTodayRatioByMoney")==0) CopyItem_Double(CloseTodayRatioByMoney,i)
        else if(strcmp(m_vecColName[i].c_str(),"CloseTodayRatioByVolume")==0) CopyItem_Double(CloseTodayRatioByVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"StrikeRatioByMoney")==0) CopyItem_Double(StrikeRatioByMoney,i)
        else if(strcmp(m_vecColName[i].c_str(),"StrikeRatioByVolume")==0) CopyItem_Double(StrikeRatioByVolume,i)
    }
}
void CTradingInfo_PlatformStru_InstrumentCommissionRate::SaveXML(const vector<PlatformStru_InstrumentCommissionRate>& Infos)
{
    CeasymutexGuard guard(m_mutex);

    OpenXML4Save(true);

    if(!m_plinecontent||!m_pcolname||!m_pxml||!m_pnodexml||!m_pcolnamexml||!m_plinesxml) return;
    memset(m_pcolname,0,m_linebufsize);
    m_colnamelen=0;
    bool bgetcolname=true;
    for(vector<PlatformStru_InstrumentCommissionRate>::const_iterator it=Infos.begin();it!=Infos.end();it++)
    {
        const PlatformStru_InstrumentCommissionRate& Info=*it;
        memset(m_plinecontent,0,m_linebufsize);
        m_linebuflen=0;

        memset(m_plinecontent,0,m_linebufsize);
        m_linebuflen=0;

        JoinContent_String(InstrumentID)
        JoinContent_Integer(InvestorRange)
        JoinContent_String(BrokerID)
        JoinContent_String(InvestorID)
        JoinContent_Double(OpenRatioByMoney)
        JoinContent_Double(OpenRatioByVolume)
        JoinContent_Double(CloseRatioByMoney)
        JoinContent_Double(CloseRatioByVolume)
        JoinContent_Double(CloseTodayRatioByMoney)
        JoinContent_Double(CloseTodayRatioByVolume)
        JoinContent_Double(StrikeRatioByMoney)
        JoinContent_Double(StrikeRatioByVolume)

        if(m_linebuflen>0) m_plinecontent[--m_linebuflen]=0;
        multimap<string,CMyXML>::iterator itnew=m_plinesxml->m_mapsubnode.insert(make_pair(string("line"),CMyXML(string("line"),string(""))));
        itnew->second.m_content=m_plinecontent;

        if(bgetcolname) bgetcolname=false;
    }

    if(m_colnamelen>0) m_pcolname[--m_colnamelen]=0;
    m_pcolnamexml->m_content=string(m_pcolname);

    if(m_pxml) m_pxml->SaveXML();
}

//��֤������Ϣ
CTradingInfo_PlatformStru_InstrumentMarginRate::CTradingInfo_PlatformStru_InstrumentMarginRate(int SvrGrpID,const string& strAccount,const string& strTradingDay)
:   CTradingInfo(eMarginRates,SvrGrpID,strAccount,strTradingDay)
    ,CSaveTradingInfo(eMarginRates,SvrGrpID,strAccount)
    ,m_pLineData(NULL)
{
}
CTradingInfo_PlatformStru_InstrumentMarginRate::~CTradingInfo_PlatformStru_InstrumentMarginRate()
{
    if(m_pLineData) { free(m_pLineData);  m_pLineData=NULL; }
}
void CTradingInfo_PlatformStru_InstrumentMarginRate::Load()
{
    CeasymutexGuard guard(m_mutex);

    CTradingInfo::Load();
    m_pLineData=(PlatformStru_InstrumentMarginRate*)malloc(sizeof(PlatformStru_InstrumentMarginRate)*m_LineCnt);
    memset(m_pLineData,0,sizeof(PlatformStru_InstrumentMarginRate)*m_LineCnt);

    //��ÿһ�У���ÿ����Լԭʼ������ȡ����ֵ���ŵ���Ӧ��Լ��Ϣ��ȥ
    for(int i=0;i<(int)m_vecColName.size();i++)
    {
        if(strcmp(m_vecColName[i].c_str(),"InstrumentID")==0) CopyItem_String(InstrumentID,i)
        else if(strcmp(m_vecColName[i].c_str(),"InvestorRange")==0) CopyItem_char(InvestorRange,i)
        else if(strcmp(m_vecColName[i].c_str(),"BrokerID")==0) CopyItem_String(BrokerID,i)
        else if(strcmp(m_vecColName[i].c_str(),"InvestorID")==0) CopyItem_String(InvestorID,i)
        else if(strcmp(m_vecColName[i].c_str(),"HedgeFlag")==0) CopyItem_char(HedgeFlag,i)
        else if(strcmp(m_vecColName[i].c_str(),"LongMarginRatioByMoney")==0) CopyItem_Double(LongMarginRatioByMoney,i)
        else if(strcmp(m_vecColName[i].c_str(),"LongMarginRatioByVolume")==0) CopyItem_Double(LongMarginRatioByVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"ShortMarginRatioByMoney")==0) CopyItem_Double(ShortMarginRatioByMoney,i)
        else if(strcmp(m_vecColName[i].c_str(),"ShortMarginRatioByVolume")==0) CopyItem_Double(ShortMarginRatioByVolume,i)
        else if(strcmp(m_vecColName[i].c_str(),"IsRelative")==0) CopyItem_Integer(IsRelative,i)
        else if(strcmp(m_vecColName[i].c_str(),"FixedMargin")==0) CopyItem_Double(FixedMargin,i)
        else if(strcmp(m_vecColName[i].c_str(),"MiniMargin")==0) CopyItem_Double(MiniMargin,i)
        else if(strcmp(m_vecColName[i].c_str(),"Royalty")==0) CopyItem_Double(Royalty,i)
        else if(strcmp(m_vecColName[i].c_str(),"ExchFixedMargin")==0) CopyItem_Double(ExchFixedMargin,i)
        else if(strcmp(m_vecColName[i].c_str(),"ExchMiniMargin")==0) CopyItem_Double(ExchMiniMargin,i)
    }
}
void CTradingInfo_PlatformStru_InstrumentMarginRate::SaveXML(const vector<PlatformStru_InstrumentMarginRate>& Infos)
{
    CeasymutexGuard guard(m_mutex);

    OpenXML4Save(false);

    if(!m_plinecontent||!m_pcolname||!m_pxml||!m_pnodexml||!m_pcolnamexml||!m_plinesxml) return;
    memset(m_pcolname,0,m_linebufsize);
    m_colnamelen=0;
    bool bgetcolname=true;

    for(vector<PlatformStru_InstrumentMarginRate>::const_iterator it=Infos.begin();it!=Infos.end();it++)
    {
        const PlatformStru_InstrumentMarginRate& Info=*it;
        memset(m_plinecontent,0,m_linebufsize);
        m_linebuflen=0;

        memset(m_plinecontent,0,m_linebufsize);
        m_linebuflen=0;

        JoinContent_String(InstrumentID)
        JoinContent_Integer(InvestorRange)
        JoinContent_String(BrokerID)
        JoinContent_String(InvestorID)
        JoinContent_Integer(HedgeFlag)
        JoinContent_Double(LongMarginRatioByMoney)
        JoinContent_Double(LongMarginRatioByVolume)
        JoinContent_Double(ShortMarginRatioByMoney)
        JoinContent_Double(ShortMarginRatioByVolume)
        JoinContent_Integer(IsRelative)
        JoinContent_Double(FixedMargin)
        JoinContent_Double(MiniMargin)
        JoinContent_Double(Royalty)
        JoinContent_Double(ExchFixedMargin)
        JoinContent_Double(ExchMiniMargin)

        if(m_linebuflen>0) m_plinecontent[--m_linebuflen]=0;
        multimap<string,CMyXML>::iterator itnew=m_plinesxml->m_mapsubnode.insert(make_pair(string("line"),CMyXML(string("line"),string(""))));
        itnew->second.m_content=m_plinecontent;

        if(bgetcolname) bgetcolname=false;
    }

    if(m_colnamelen>0) m_pcolname[--m_colnamelen]=0;
    m_pcolnamexml->m_content=string(m_pcolname);

    if(m_pxml) m_pxml->SaveXML();
}


//���͹�˾���ײ�����Ϣ
CTradingInfo_PlatformStru_BrokerTradingParams::CTradingInfo_PlatformStru_BrokerTradingParams(int SvrGrpID,const string& strAccount,const string& strTradingDay)
:   CTradingInfo(eTradingParams,SvrGrpID,strAccount,strTradingDay)
    ,CSaveTradingInfo(eTradingParams,SvrGrpID,strAccount)
    ,m_pLineData(NULL)
{
}
CTradingInfo_PlatformStru_BrokerTradingParams::~CTradingInfo_PlatformStru_BrokerTradingParams()
{
    if(m_pLineData) { free(m_pLineData);  m_pLineData=NULL; }
}
void CTradingInfo_PlatformStru_BrokerTradingParams::Load(void)
{
    CeasymutexGuard guard(m_mutex);

    CTradingInfo::Load();
    m_pLineData=(PlatformStru_BrokerTradingParams*)malloc(sizeof(PlatformStru_BrokerTradingParams)*m_LineCnt);
    memset(m_pLineData,0,sizeof(PlatformStru_BrokerTradingParams)*m_LineCnt);

    //��ÿһ�У���ÿ����Լԭʼ������ȡ����ֵ���ŵ���Ӧ��Լ��Ϣ��ȥ
    for(int i=0;i<(int)m_vecColName.size();i++)
    {
        if(strcmp(m_vecColName[i].c_str(),"BrokerID")==0) CopyItem_String(BrokerID,i)
        else if(strcmp(m_vecColName[i].c_str(),"InvestorID")==0) CopyItem_String(InvestorID,i)
        else if(strcmp(m_vecColName[i].c_str(),"MarginPriceType")==0) CopyItem_char(MarginPriceType,i)
        else if(strcmp(m_vecColName[i].c_str(),"Algorithm")==0) CopyItem_char(Algorithm,i)
        else if(strcmp(m_vecColName[i].c_str(),"AvailIncludeCloseProfit")==0) CopyItem_char(AvailIncludeCloseProfit,i)
        else if(strcmp(m_vecColName[i].c_str(),"CurrencyID")==0) CopyItem_String(CurrencyID,i)
        else if(strcmp(m_vecColName[i].c_str(),"OptionRoyaltyPriceType")==0) CopyItem_char(OptionRoyaltyPriceType,i)
    }
}
void CTradingInfo_PlatformStru_BrokerTradingParams::SaveXML(const PlatformStru_BrokerTradingParams& Info)
{
    CeasymutexGuard guard(m_mutex);

    OpenXML4Save(false);

    if(!m_plinecontent||!m_pcolname||!m_pxml||!m_pnodexml||!m_pcolnamexml||!m_plinesxml) return;
    memset(m_pcolname,0,m_linebufsize);
    m_colnamelen=0;
    bool bgetcolname=true;

    memset(m_plinecontent,0,m_linebufsize);
    m_linebuflen=0;

    JoinContent_String(BrokerID)
    JoinContent_String(InvestorID)
    JoinContent_Integer(MarginPriceType)
    JoinContent_Integer(Algorithm)
    JoinContent_Integer(AvailIncludeCloseProfit)
    JoinContent_String(CurrencyID)
    JoinContent_Integer(OptionRoyaltyPriceType)

    if(m_linebuflen>0) m_plinecontent[--m_linebuflen]=0;
    multimap<string,CMyXML>::iterator itnew=m_plinesxml->m_mapsubnode.insert(make_pair(string("line"),CMyXML(string("line"),string(""))));
    itnew->second.m_content=m_plinecontent;

    if(m_colnamelen>0) m_pcolname[--m_colnamelen]=0;
    m_pcolnamexml->m_content=string(m_pcolname);

    if(m_pxml) m_pxml->SaveXML();
}

#undef JoinContent_Double
#undef JoinContent_Integer
#undef JoinContent_String
#undef JoinContent

#undef CopyItem_Double
#undef CopyItem_char
#undef CopyItem_Integer
#undef CopyItem_String
#undef min2















