//���Ƶ��б���ࡣ
//��������ί�е���δ�ɽ������ɽ���¼���ֲ֡��ֲ���ϸ����ϳֲֵ�ģ��
//�б������ݼ�һһ��Ӧ��<KeyType,DataStruType>
//ÿ�����ݼ��о�����InstrumentID��FTID��UpdateSeq�����ֶ�
//FTID���ڱ�ʶΨһ��key
//UpdateSeq���ڱ�ʾ�����Ƿ��б仯
//����˺ŵ����ݿ��Է���һ�������ʱFTIDӦ�ñ�֤��ͳһ�ģ������޷�ȡָ���е�����


#pragma once

#include "ExtListCtrl.h"
#include "..\\Module-Misc2\\GlobalConfigMgr.h"
#include <string>
#include <set>
#include <map>
#include <vector>
using std::string;
using std::set;
using std::map;
using std::vector;

#include "../Module-Misc2/SimpleWriteLog.h"
#include <assert.h>

#if 1
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

    //����fieldID����ID��ӳ��
    void SetCfg(unsigned long textcol,map<int, long>& fieldID2ColID)
    {
        m_TextCol=textcol;
        m_FieldID2ColID=fieldID2ColID;
    };

    //���UI���ڴ�����
    void Clear(void)
    {
        DeleteAllItems();

        EnterCriticalSection(&m_CS);

        Clear2_Internal();

        LeaveCriticalSection(&m_CS);
    };

    //����ڴ�����
    void Clear2_Internal(void)
    {
        //�ȼ�¼����ָ��
        vector<DataStruType*> vecpData;
        vecpData.clear();
        map<KeyType,DataStruType*>::iterator it;
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

    //ˢ����������
    void UpdateAllItems(vector<DataStruType>& vecData)
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


    //ˢ�º�Լ��Ӧ���
    //setFTID����ɾ��������Ҫ���setFTID��vecValue����һ�£����ڸ�������
    void UpdateInstrumentItems2(
        const string& strAccount,
        const string& strInstrument,
        set<long>& setFTID,
        vector<DataStruType>& vecValue)
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
            map<string,map<string,set<long>>>::iterator it_allIndex2=m_allIndex2.find(strAccount);
            if(it_allIndex2!=m_allIndex2.end())
            {
                map<string,set<long>>::iterator it=it_allIndex2->second.find(strInstrument);
                if(it!=it_allIndex2->second.end()&&
                    !it->second.empty()&&
                    it->second.size()!=setFTID.size())
                {
                    set<long>::iterator itnew=setFTID.begin();
                    vector<long> EraseFTIDs;

                    //����Ҫɾ������
                    for(set<long>::iterator itold=it->second.begin();itold!=it->second.end();itold++)
                    {
                        while(itnew!=setFTID.end()&&(*itnew)<(*itold)) itnew++;
                        if(itnew==setFTID.end()||(*itnew)!=(*itold))
                            EraseFTIDs.push_back((*itold));
                    }
                    //ɾ�������ڵ���
                    for(int i=0;i<(int)EraseFTIDs.size();i++)
                    {
                        //itnew��û�ж�Ӧ��Ŀ����Ҫɾ��
                        DeleteLineByFTID_Internal(EraseFTIDs[i]);
                        DeleteRecord3_Internal(EraseFTIDs[i]);
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

    //ˢ��ָ�������ɾ������
    void UpdateInstrumentItems3(vector<DataStruType>& vecValue)
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

    //ˢ��ָ����
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
        map<KeyType, DataStruType*>::iterator it;

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
        string strAccount=string(value.Account);
        string strInstrument=string(value.InstrumentID);
        long FTID=value.FTID;

        bNeedResort=false;

        map<long, DataStruType*>::iterator it_oldData2=m_all2.find(FTID);
        if(it_oldData2!=m_all2.end()&&
            it_oldData2->second!=NULL&&
            it_oldData2->second->UpdateSeq==value.UpdateSeq)
            return;                                             //����û�иı�

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

        assert(it_oldData2!=m_all2.end());
        assert(rowid<LineNum);

        //��ȡ����������
        int sortcol=GetSortCol();

        bool brlt;
        bool bUpdate=false;
        vector<wxExtListItem*> UpdateItems;

        //���и���
        for(map<int,long>::iterator it_col=m_FieldID2ColID.begin();it_col!=m_FieldID2ColID.end();it_col++)
        {
            if(it_col->second<0||it_col->second>=ColNum) continue;

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
        map<long, DataStruType*>::iterator it=m_all2.find(FTID);

        if(it!=m_all2.end()&&it->second!=NULL)
        {
            value=*(it->second);
            brlt=true;
        }
        LeaveCriticalSection(&m_CS);
        return brlt;
    }

private:
    //ɾ������
    void DeleteRecord2_Internal(const KeyType& KeyInfo)
    {
        map<KeyType,DataStruType*>::iterator it_all=m_all.find(KeyInfo);
        if(it_all!=m_all.end()&&it_all->second!=NULL)
            DeleteRecord_Internal(*(it_all->second));
    };

    //ɾ��Account-FTID��Ӧ������
    void DeleteRecord3_Internal(long FTID)
    {
        map<long,DataStruType*>::iterator it_all2=m_all2.find(FTID);
        if(it_all2!=m_all2.end()&&it_all2->second!=NULL)
            DeleteRecord_Internal(*(it_all2->second));
    };


    //ɾ������
    void DeleteRecord_Internal(const DataStruType& DataInfo)
    {
        KeyType key(DataInfo);
        string strAccount(key.Account);
        string strInstrument(key.InstrumentID);
        long FTID=DataInfo.FTID;
        DataStruType *pData=NULL;

        //ɾ��m_allIndex���������
        map<string,map<string,set<KeyType>>>::iterator it_allIndex=m_allIndex.find(strAccount);
        if(it_allIndex!=m_allIndex.end())
        {
            map<string,set<KeyType>>::iterator it=it_allIndex->second.find(strInstrument);
            if(it!=it_allIndex->second.end()&&it->second.find(key)!=it->second.end())
                it->second.erase(key);
        }

        //ɾ��m_allIndex2���������
        map<string,map<string,set<long>>>::iterator it_allIndex2=m_allIndex2.find(strAccount);
        if(it_allIndex2!=m_allIndex2.end())
        {
            map<string,set<long>>::iterator it=it_allIndex2->second.find(strInstrument);
            if(it!=it_allIndex2->second.end()&&it->second.find(FTID)!=it->second.end())
                it->second.erase(FTID);
        }

        //ɾ��m_all���������
        map<KeyType,DataStruType*>::iterator it_all=m_all.find(key);
        if(it_all!=m_all.end()/*&&it_all->second!=NULL*/)
        {
            pData=it_all->second;
            m_all.erase(it_all);
        }

        //ɾ��m_all2���������
        map<long,DataStruType*>::iterator it_all2=m_all2.find(FTID);
        if(it_all2!=m_all2.end())
            m_all2.erase(it_all2);

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
        KeyType key(DataInfo);
        string strAccount(key.Account);
        string strInstrument(key.InstrumentID);
        long FTID=DataInfo.FTID;

        map<long,DataStruType*>::iterator it_all2=m_all2.find(FTID);
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
            m_all.insert(make_pair(key,pvalue));

            //����m_all2
            m_all2.insert(make_pair(FTID,pvalue));

            //����m_allIndex
            map<string,map<string,set<KeyType>>>::iterator it_allIndex=m_allIndex.find(strAccount);
            if(it_allIndex==m_allIndex.end())
            {
                m_allIndex.insert(make_pair(strAccount,map<string,set<KeyType>>()));
                it_allIndex=m_allIndex.find(strAccount);
            }
            map<string,set<KeyType>>::iterator it2_allIndex=it_allIndex->second.find(strInstrument);
            if(it2_allIndex==it_allIndex->second.end())
            {
                set<KeyType> setKey;
                setKey.insert(key);
                it_allIndex->second.insert(make_pair(strInstrument,setKey));
            }
            else it2_allIndex->second.insert(key);

            //����m_allIndex2
            map<string,map<string,set<long>>>::iterator it_allIndex2=m_allIndex2.find(strAccount);
            if(it_allIndex2==m_allIndex2.end())
            {
                m_allIndex2.insert(make_pair(strAccount,map<string,set<long>>()));
                it_allIndex2=m_allIndex2.find(strAccount);
            }
            map<string,set<long>>::iterator it2_allIndex2=it_allIndex2->second.find(strInstrument);
            if(it2_allIndex2==it_allIndex2->second.end())
            {
                set<long> setFTID;
                setFTID.insert(FTID);
                it_allIndex2->second.insert(make_pair(strInstrument,setFTID));
            }
            else it2_allIndex2->second.insert(FTID);
        }
    }


private:

    CRITICAL_SECTION                            m_CS;

    map<string,map<string,set<KeyType>>>        m_allIndex;     //map<Account, map<Instrument,set<Key>>>  Account��(InstrumentID��set of key)��ӳ��
    map<string,map<string,set<long>>>           m_allIndex2;    //map<Account, map<Instrument,set<FTID>>> Account��(InstrumentID��set of key)��ӳ��
	map<long, DataStruType*>                    m_all2;		    //map<FTID,pData> FTID��Value��ӳ��
	map<KeyType, DataStruType*>                 m_all;		    //map<Key,pData> Key��Value��ӳ��

    UpdateListItemCB                            m_UpdateCB;
    unsigned long                               m_TextCol;      //ͨ�õ�������ɫ
    map<int, long>                              m_FieldID2ColID;//�е��ֶ�ID��ӳ��

    zqWriteLog*                                 m_pWriteLog;

};