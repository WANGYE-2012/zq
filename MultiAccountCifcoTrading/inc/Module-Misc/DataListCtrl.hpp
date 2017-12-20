//���Ƶ��б���ࡣ
//��������ί�е���δ�ɽ������ɽ���¼���ֲ֡��ֲ���ϸ����ϳֲֵ�ģ��
//�б������ݼ�һһ��Ӧ��<KeyType,DataStruType>
//ÿ�����ݼ��о�����InstrumentID��FTID��UpdateSeq�����ֶ�
//FTID���ڱ�ʶΨһ��key
//UpdateSeq���ڱ�ʾ�����Ƿ��б仯


#pragma once

#include "ExtListCtrl.h"
#include "GlobalConfigMgr.h"
#include <string>
#include <set>
#include <map>
#include <vector>

#include "SimpleWriteLog.h"

#if 0
#define LOG_INFO_DataListCtrl(fmt, ...) 
#else
#define LOG_INFO_DataListCtrl(fmt, ...) \
    do{\
        char logbuf[256],logbuf2[256];\
        logbuf[0]=1;  logbuf2[0]=1;\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CDataListCtrl", LOGLEVEL_DEBUGINFO, "[%d]:"fmt, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif


class wxExtListHeaderWindow;


//�ص����������ڶԱ��¾����ݡ�
//���ĳ�������ͬ���������ݸ��µ������ݣ�������Item�����غ���н�����¡�����ֵ��ʾ�����Ƿ��б仯
//pNewData��pOldData����ָ��һ�����ݵ�ָ��
//FieldID��ʾҪ���µ��������ID
//bnewline��ʾ�ǲ������У���������У������Ѿ������ˣ�ֱ������Item
typedef	bool	(*UpdateListItemCB)(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);

template <typename KeyType,typename DataStruType>
class CDataListCtrl : public wxExtListCtrl
{
public:
    CDataListCtrl(  UpdateListItemCB updateCB,
                    wxWindow *parent,
                    wxWindowID winid = wxID_ANY,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxLCEXT_REPORT,
                    const wxValidator& validator = wxDefaultValidator,
                    const wxString &name = wxEmptyString):
    wxExtListCtrl(parent, winid, pos, size, style, validator, name)
    {
        m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"DataListCtrl.log");

        m_UpdateCB=updateCB;
        m_TextCol=0;
	    InitializeCriticalSection(&m_CS);

        m_all.clear();
        m_all2.clear();
        m_allIndex.clear();
        m_allIndex2.clear();
    };

    ~CDataListCtrl()
    {
        Clear();

        DeleteCriticalSection(&m_CS);
        if(m_pWriteLog)
        {
            delete m_pWriteLog;
            m_pWriteLog=NULL;
        }
    };

    void Clear2_Internal(void)
    {
        //�ȼ�¼����ָ��
        std::vector<DataStruType*> vecpData;
        vecpData.clear();
        std::map<KeyType,DataStruType*>::iterator it;
        for(it=m_all.begin();it!=m_all.end();it++)
            vecpData.push_back(it->second);

        //�������map
        m_all.clear();
        m_all2.clear();
        m_allIndex.clear();
        m_allIndex2.clear();

        //�ͷ�����ָ������
        int count=vecpData.size();
        for(int i=0;i<count;i++)
        {
            if(vecpData[i]!=NULL)
            {
                delete vecpData[i];
                vecpData[i]=NULL;
            }
        }
    }

    void SetCfg(unsigned long textcol,std::map<int, long>& fieldID2ColID)
    {
        m_TextCol=textcol;
        m_FieldID2ColID=fieldID2ColID;
    };

    void Clear(void)
    {
        DeleteAllItems();

        EnterCriticalSection(&m_CS);

        Clear2_Internal();

        LeaveCriticalSection(&m_CS);
    };

    void UpdateAllItems(std::vector<DataStruType>& vecData)
    {
        bool bNeedResort,tmpbNeedResort;

        wxGetTopLevelParent(this)->SetCursor(*wxHOURGLASS_CURSOR);
        Clear();

        Freeze();

        EnterCriticalSection(&m_CS);
        bNeedResort=false;

        int count=vecData.size();
        for(int i=0;i<count;i++)
        {
            UpdateOneItem_Internal(vecData[i],tmpbNeedResort, false);
            if(!bNeedResort&&tmpbNeedResort)
                bNeedResort=true;
        }

        if(GetSortCol()!=-1)
        {
            //�б��С�������������ʱ�����Զ���������Ӱ���ٶ�
            if(GetItemCount()>GlobalConfigManager::GetMaxItemCountForAutoResort_ListCtrl())
                DisableSortCol();
            else if(bNeedResort) 
                ReSortItems();
        }

        LeaveCriticalSection(&m_CS);

        Thaw();
        wxGetTopLevelParent(this)->SetCursor(*wxSTANDARD_CURSOR);
    };


    //setFTID����ɾ��������Ҫ���setFTID��vecValue����һ�£����ڸ�������
    void UpdateInstrumentItems2(std::string strInstrument,
                                std::set<long>& setFTID,
                                std::vector<DataStruType>& vecValue)
    {
        bool bNeedResort,tmpbNeedResort;
        int count=setFTID.size();

        if(count!=vecValue.size())
            return;
        
        EnterCriticalSection(&m_CS);

        bNeedResort=false;

        //���и���
        for(int i=0;i<count;i++)
        {
            UpdateOneItem_Internal(vecValue[i],tmpbNeedResort,true);

            if(!bNeedResort&&tmpbNeedResort)
                bNeedResort=true;
        }

        //�����û��Ҫɾ������
        static bool bgo1=true;
        if(bgo1)
        {
            typename std::map<long, DataStruType>::iterator it_oldData2;
            std::map<std::string,std::set<long>>::iterator it2;
            long FTID;

            it2=m_allIndex2.find(strInstrument);
            if(it2!=m_allIndex2.end()&&
                !it2->second.empty()&&
                it2->second.size()!=setFTID.size())
            {
                std::set<long>::iterator itold=it2->second.begin();
                std::set<long>::iterator itnew=setFTID.begin();

                //����Ҫɾ������
                while(itold!=it2->second.end())
                {
                    FTID=(*itold);
                    itold++;

                    while(itnew!=setFTID.end()&&(*itnew)<FTID) itnew++;
                    if(itnew==setFTID.end()||(*itnew)!=FTID)
                    {
                        //itnew��û�ж�Ӧ��Ŀ����Ҫɾ��
                        DeleteLineByFTID_Internal(FTID);
                        DeleteRecord3_Internal(FTID);

                        if(m_allIndex2.find(strInstrument)==m_allIndex2.end())
                            break;
                    }
                }
            }

        }


        static bool bgo2=true;
        if(GetSortCol()!=-1&&bgo2)
        {
            //�б��С�������������ʱ�����Զ���������Ӱ���ٶ�
            if(GetItemCount()>GlobalConfigManager::GetMaxItemCountForAutoResort_ListCtrl())
                DisableSortCol();
            else if(bNeedResort) 
                ReSortItems();
        }

        LeaveCriticalSection(&m_CS);

    };

    //����ɾ������
    void UpdateInstrumentItems3(std::string strInstrument,
                                std::vector<DataStruType>& vecValue)
    {
        bool bNeedResort,tmpbNeedResort;
        int count=vecValue.size();

        EnterCriticalSection(&m_CS);

        bNeedResort=false;

        //���и���
        for(int i=0;i<count;i++)
        {
            UpdateOneItem_Internal(vecValue[i],tmpbNeedResort,true);

            if(!bNeedResort&&tmpbNeedResort)
                bNeedResort=true;
        }

        static bool bgo2=true;
        if(GetSortCol()!=-1&&bgo2)
        {
            //�б��С�������������ʱ�����Զ���������Ӱ���ٶ�
            if(GetItemCount()>GlobalConfigManager::GetMaxItemCountForAutoResort_ListCtrl())
                DisableSortCol();
            else if(bNeedResort) 
                ReSortItems();
        }

        LeaveCriticalSection(&m_CS);

    };

    void UpdateOneItem(DataStruType& value)
    {
        bool bNeedResort;

        EnterCriticalSection(&m_CS);

        UpdateOneItem_Internal(value, bNeedResort,true);

        if(GetSortCol()!=-1)
        {
            //�б��С�������������ʱ�����Զ���������Ӱ���ٶ�
            if(GetItemCount()>GlobalConfigManager::GetMaxItemCountForAutoResort_ListCtrl())
                DisableSortCol();
            else if(bNeedResort) 
                ReSortItems();
        }

        LeaveCriticalSection(&m_CS);
    }

    // ɾ��һ�м�¼
    void DeleteOneItem(const KeyType& key)
    {
        std::map<KeyType, DataStruType*>::iterator it;

        EnterCriticalSection(&m_CS);

        it=m_all.find(key);
        if(it!=m_all.end()&&it->second!=NULL) 
        {
            long FTID=it->second->FTID;

            DeleteLineByFTID_Internal(FTID);

            DeleteRecord3_Internal(FTID);
        }

        LeaveCriticalSection(&m_CS);
    }



    void UpdateOneItem_Internal(const DataStruType& value,bool& bNeedResort, const bool bRefreshUI)
    {
        bool bnewline=false;
        int LineNum,ColNum;
        long FTID=value.FTID;

        bNeedResort=false;

        std::map<long, DataStruType*>::iterator it_oldData2;

        it_oldData2=m_all2.find(FTID);
        if(it_oldData2!=m_all2.end()&&
            it_oldData2->second!=NULL&&
            it_oldData2->second->UpdateSeq==value.UpdateSeq)
            return;                                             //����û�иı�

        std::string strInstrument=std::string(value.InstrumentID);
        int rowid;
        LineNum=GetItemCount();
        ColNum=GetColumnCount();

        if(ColNum<=0) 
            return;

        if(it_oldData2==m_all2.end())
        {
            //��������
            bnewline=true;
            UpdateRecord_Internal(value);

            //��Ϊ���ǲ��뵽���һ�У�����Ӱ��ԭ�м�¼���������SetItem����UI���ɣ����ﲻ�ø���
            rowid=InsertItem2(LineNum,wxString(""),FTID,true);
            LineNum++;

            it_oldData2=m_all2.find(FTID);
        }
        else
        {
            rowid=FindItem2(FTID);
        }

        if(it_oldData2==m_all2.end())
            return;                                             //���ǲ����ܵ�
        if(rowid>=LineNum)
            return;                                             //���������Ҳ�ǲ����ܵ�

        //��ȡ����������
        int sortcol=GetSortCol();

        std::map<int, long>::iterator it_col;
        bool brlt;
        bool bUpdate=false;
        std::vector<wxExtListItem*> UpdateItems;

        //���и���
        for(it_col=m_FieldID2ColID.begin();it_col!=m_FieldID2ColID.end();it_col++)
        {
            if(it_col->second<0||
                it_col->second>=ColNum) continue;

            wxExtListItem *pItem=new wxExtListItem;
            brlt=m_UpdateCB(&value,it_oldData2->second,m_TextCol,it_col->first,it_col->second,rowid,bnewline,*pItem);

            if(brlt)
            {
                if(!bUpdate) bUpdate=true;

                if(it_col->second==0)
                {
                    //ע�⣺�޸ĵ�һ�е�Ԫ��ʱ���п��ܻ�ĵ�����ص�UserData������������б��������ItemData��Mask
                    pItem->HoldItemData();
                }

                UpdateItems.push_back(pItem);

                if(it_col->second==sortcol&&sortcol>=0&&sortcol<ColNum&&!bNeedResort)
                    bNeedResort=true;
            }
            else 
            {
                delete pItem;
            }
        }

        if(UpdateItems.size()>0)
        {
            SetItems(UpdateItems,bRefreshUI);

            LOG_INFO_DataListCtrl("UpdateOneItem_Internal: SetItems, UpdateItems.size()=%d FTID=%u bRefreshUI=%d LineNum=%d",UpdateItems.size(),FTID,bRefreshUI,LineNum);

            int i,isize=UpdateItems.size();
            for(i=0;i<isize;i++)
            {
                delete UpdateItems[i];
            }
            UpdateItems.clear();
        }

        if(!bnewline)
        {
            if(bUpdate) *(it_oldData2->second)=value;
            else it_oldData2->second->UpdateSeq=value.UpdateSeq;
        }
    };
    

    //ɾ��FTIDָ������
    void DeleteLineByFTID_Internal(long FTID)
    {
        int row=FindItem2(FTID);
        if(row>=0)
            DeleteItem(row);
    };

    //ȡָ���е�����
    bool GetValueByRow(int rowid, DataStruType& value)
    {
        bool brlt=false;
        EnterCriticalSection(&m_CS);
        long FTID=GetItemData(rowid);
        std::map<long, DataStruType*>::iterator it2;

        if((it2=m_all2.find(FTID))!=m_all2.end()&&it2->second!=NULL)
        {
            value=*(it2->second);
            brlt=true;
        }
        LeaveCriticalSection(&m_CS);
        return brlt;
    }

private:
    //ɾ������
    void DeleteRecord2_Internal(const KeyType& KeyInfo)
    {
        std::map<KeyType,DataStruType*>::iterator it_all;
        it_all=m_all.find(KeyInfo);
        if(it_all==m_all.end()||it_all->second==NULL)
            return;

        DataStruType tmpData=*(it_all->second);
        DeleteRecord_Internal(tmpData);
    };

    //ɾ������
    void DeleteRecord3_Internal(const long FTID)
    {
        std::map<long,DataStruType*>::iterator it_all2;
        it_all2=m_all2.find(FTID);
        if(it_all2==m_all2.end()||it_all2->second==NULL)
            return;

        DataStruType tmpData=*(it_all2->second);
        DeleteRecord_Internal(tmpData);
    };


    //ɾ������
    void DeleteRecord_Internal(const DataStruType& DataInfo)
    {
        long FTID=DataInfo.FTID;
        KeyType tmpkey(DataInfo);
        std::string strInstrument(tmpkey.InstrumentID);
        DataStruType *pData=NULL;

        //ɾ��m_allIndex���������
        std::map<std::string,std::set<KeyType>>::iterator it;
        it=m_allIndex.find(strInstrument);
        if(it!=m_allIndex.end()&&it->second.find(tmpkey)!=it->second.end())
        {
            it->second.erase(tmpkey);
            if(it->second.empty())
                m_allIndex.erase(it);
        }

        //ɾ��m_allIndex2���������
        std::map<std::string,std::set<long>>::iterator it2;
        it2=m_allIndex2.find(strInstrument);
        if(it2!=m_allIndex2.end()&&it2->second.find(FTID)!=it2->second.end())
        {
            it2->second.erase(FTID);
            if(it2->second.empty())
                m_allIndex2.erase(it2);
        }

        //ɾ��m_all���������
        std::map<KeyType,DataStruType*>::iterator it_all;
        it_all=m_all.find(tmpkey);
        if(it_all!=m_all.end()&&it_all->second!=NULL)
        {
            pData=it_all->second;
            m_all.erase(it_all);
        }

        //ɾ��m_all2���������
        std::map<long,DataStruType*>::iterator it_all2;
        it_all2=m_all2.find(FTID);
        if(it_all2!=m_all2.end()&&it_all2->second!=NULL)
        {
            if(!pData)
                pData=it_all2->second;
            m_all2.erase(it_all2);
        }

        //ɾ����������
        if(pData)
        {
            delete pData;
            pData=NULL;
        }
    };


    ///��������һ���¼�¼
    void UpdateRecord_Internal(const DataStruType& DataInfo)
    {
        std::map<long,DataStruType*>::iterator it_all2;
        KeyType tmpkey(DataInfo);
        std::string strInstrument(tmpkey.InstrumentID);
        long FTID;

        FTID=DataInfo.FTID;
        it_all2=m_all2.find(FTID);
        if(it_all2!=m_all2.end()&&it_all2->second!=NULL)
        {
            //����ԭ������
            *(it_all2->second)=DataInfo;
        }
        else
        {
            //����������
        
            //newһ����Ԫ��
            DataStruType *pvalue=new DataStruType(DataInfo);
            DataStruType& NewData=(*pvalue);

            //����m_all
            m_all.insert(make_pair(tmpkey,pvalue));

            //����m_all2
            m_all2.insert(make_pair(FTID,pvalue));

            //����m_allIndex
            std::map<std::string,std::set<KeyType>>::iterator it;
            it=m_allIndex.find(strInstrument);
            if(it==m_allIndex.end())
            {
                std::set<KeyType> setKey;
                setKey.insert(tmpkey);
                m_allIndex.insert(make_pair(strInstrument,setKey));
            }
            else it->second.insert(tmpkey);

            //����m_allIndex2
            std::map<std::string,std::set<long>>::iterator it2;
            it2=m_allIndex2.find(strInstrument);
            if(it2==m_allIndex2.end())
            {
                std::set<long> setFTID;
                setFTID.insert(FTID);
                m_allIndex2.insert(make_pair(strInstrument,setFTID));
            }
            else it2->second.insert(FTID);
        }
    }


private:

    CRITICAL_SECTION m_CS;

    std::map<std::string, std::set<KeyType>> m_allIndex;        //InstrumentID��set of key��ӳ��
    std::map<std::string, std::set<long>> m_allIndex2;          //InstrumentID��set of key��ӳ��
	std::map<KeyType, DataStruType*> m_all;		                //Key��Value�Ķ�ӦMap
	std::map<long, DataStruType*> m_all2;		                //Key��Value�Ķ�ӦMap

    UpdateListItemCB m_UpdateCB;
    unsigned long m_TextCol;                                    //ͨ�õ�������ɫ
    std::map<int, long> m_FieldID2ColID;                        //�е��ֶ�ID��ӳ��

    zqWriteLog*           m_pWriteLog;

};