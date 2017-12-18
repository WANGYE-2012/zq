/*
    ����ά���ײ��б������ݣ������������ɽ����ֲ���ϸ���ֲ֡���ϳֲ���ϸ
    ע�⣺����Ķ���ֻ����PlatformDataMgr��ʹ�ã����ﲻ�ܼ���
    ���е����ݽṹ���ж���Instrument��FTID��UpdateSeq�⼸��

    ע��
    1. ������Ԫ��ʱ����newһ��DataStruType��m_all��m_all2������new�����Ľṹ��
    2. ɾ��Ԫ��ʱ����Ҫ��DataStruTypeɾ��

    added by l. 20120313
*/


#pragma once

#include <windows.h>
#include <vector>
#include <map>
#include <set>
#include <string>
using std::string;
using std::map;
using std::vector;
using std::set;
using std::make_pair;

#ifdef WIN32
#pragma managed(push,off)
#endif 


template <typename KeyType,typename DataStruType>
class CPlatformDataSetList
{
public:
    //bUpdateSeq2FTID:�Ƿ�Ҫά��UpdateSeq��FTID�Ķ�Ӧ��ϵ���ɽ��ͱ���Ҫά���������������
    CPlatformDataSetList(volatile long& baseFTID,volatile long& baseUpdateSeq,bool bUpdateSeq2FTID=false)
    :   m_bUpdateSeq2FTID(bUpdateSeq2FTID),
        m_baseFTID(baseFTID),
        m_baseUpdateSeq(baseUpdateSeq),
        m_lastUpdateSeq(baseUpdateSeq)
    {
        m_bNeedKey2QryRlt=false;

        m_strQryInstrument.clear();
        m_QryRlt.clear();
        m_mapKey2QryRlt.clear();
        m_InstrumentsInQryRlt.clear();

        m_all.clear();
        m_all2.clear();
        m_allIndex.clear();
        m_allIndex2.clear();
        m_allFTID.clear();
    };

    ~CPlatformDataSetList(void)
    {
        Clear();
    };

    void Clear(void)
    {
        m_strQryInstrument.clear();
        m_QryRlt.clear();
        m_mapKey2QryRlt.clear();
        m_InstrumentsInQryRlt.clear();

        Clear2();
    }

    void Clear2(void)
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
        m_allFTID.clear();

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

    //bNeedKey2QryRlt:��ѯ�������ʱ���Ƿ���Ҫά��m_mapKey2QryRlt. ���ڳֲ֣���Ҫά�������ںϲ����ճֲֺ���ʷ�ֲ֣������Ĳ���
    void SetbNeedKey2QryRlt(bool bNeedKey2QryRlt)
    {
        m_bNeedKey2QryRlt=bNeedKey2QryRlt;
    }

    //��ѯǰ��׼������������Ҫ��ѯ�ĺ�Լ���ƣ������ѯ���������
    void WillQry_Internal(const string& strQryInstrument)
    {
        m_strQryInstrument=strQryInstrument;
        m_QryRlt.clear();
        m_mapKey2QryRlt.clear();
        m_InstrumentsInQryRlt.clear();
    };

    //��ѯ�������
    void QryRltReach_Internal(const DataStruType& DataInfo,const int ErrID,const bool bIsLast)
    {
        if(ErrID==0)
        {
            //�Ƚ���ѯ����ŵ�m_QryRlt�У���ȫ����������ͳһ����
            string strInstrument(DataInfo.InstrumentID);
            m_QryRlt.push_back(DataInfo);

            if(m_bNeedKey2QryRlt)
            {
                //���ڳֲ����ݣ���Ҫά��m_mapKey2QryRlt
                KeyType key(DataInfo);
                if(m_mapKey2QryRlt.find(key)==m_mapKey2QryRlt.end())
                    m_mapKey2QryRlt.insert(make_pair(key,(int)m_QryRlt.size()-1));
            }

            if(m_InstrumentsInQryRlt.find(strInstrument)==m_InstrumentsInQryRlt.end())
                m_InstrumentsInQryRlt.insert(strInstrument);
        }

        if(bIsLast)
        {
            //ȫ�������ˣ�ͳһ�����ݴ���m_QryRlt�еĲ�ѯ���
            map<KeyType,DataStruType*>::iterator it_all;
            string strInstrument;
            int i,count;


            //ֻ����ָ����Լ�����ݡ�Ϊ���Ч�ʣ��ֱ���Բ��ָ��º�ȫ�����µ�������
            if(!m_strQryInstrument.empty())
            {
                map<string,set<KeyType>>::iterator it;
                map<string,set<long>>::iterator it2;
                map<long,KeyType>::iterator it_FTID;
                set<long> setFTID;
                long FTID;

                //��m_QryRlt�е����ݲ������µ�m_all��
                setFTID.clear();
                count=m_QryRlt.size();
                for(i=0;i<count;i++)
                {
                    UpdateRecord_Internal(m_QryRlt[i]);
                    setFTID.insert(m_QryRlt[i].FTID);
                }

                //ɾ��m_all�в��ٴ��ڵ�����(m_QryRlt��û�е�����)
                //ע�⣬��ʱsetFTID�е���������ЧFTIDֵ�����ھ���ԭ�е������Ƿ���Ҫɾ��
                vector<long> InvalidFTIDs;
                it2=m_allIndex2.find(m_strQryInstrument);
                if(it2!=m_allIndex2.end()&&!it2->second.empty())
                {
                    set<long>& oldsetFTID=it2->second;
                    set<long>::iterator itold=oldsetFTID.begin();
                    set<long>::iterator itnew=setFTID.begin();

                    for(itold=oldsetFTID.begin();itold!=oldsetFTID.end();itold++)
                    {
                        FTID=(*itold);
                        while(itnew!=setFTID.end()&&(*itnew)<FTID) itnew++;
                        if(itnew==setFTID.end()||(*itnew)!=FTID)
                            InvalidFTIDs.push_back(FTID);
                    }
                }
                //����InvalidFTIDs�б�������Ч��FTID��ɾ��֮
                for(int i=0;i<(int)InvalidFTIDs.size();i++)
                    DeleteRecord3_Internal(InvalidFTIDs[i]);
            }
            else
            {
                //��������

                //���ԭ����¼
                Clear2();

                //����ԭ����¼
				count=m_QryRlt.size();
                for(i=0;i<count;i++)
                {
                    UpdateRecord_Internal(m_QryRlt[i]);
                }
            }
        }
    };

    //�����ݵ���
    void NewReach_Internal(DataStruType& DataInfo)
    {
        UpdateRecord_Internal(DataInfo);
    };



    //ɾ�����ݣ�����ָ����Key
    void DeleteRecord2_Internal(const KeyType& KeyInfo)
    {
        map<KeyType,DataStruType*>::iterator it_all;
        it_all=m_all.find(KeyInfo);
        if(it_all==m_all.end()||it_all->second==NULL)
            return;

        DataStruType tmpData=*(it_all->second);
        DeleteRecord_Internal(tmpData);
    };

    //ɾ�����ݣ�����ָ����FTID
    void DeleteRecord3_Internal(const long FTID)
    {
        map<long,DataStruType*>::iterator it_all2;
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
        string strInstrument(tmpkey.InstrumentID);
        DataStruType *pData=NULL;

        //ɾ��m_allFTID���������
        set<long>::iterator it_FTID;
        it_FTID=m_allFTID.find(FTID);
        if(it_FTID!=m_allFTID.end())
            m_allFTID.erase(it_FTID);

        //ɾ��m_allIndex���������
        map<string,set<KeyType>>::iterator it;
        it=m_allIndex.find(strInstrument);
        if(it!=m_allIndex.end()&&it->second.find(tmpkey)!=it->second.end())
        {
            it->second.erase(tmpkey);
            if(it->second.empty())
                m_allIndex.erase(it);
        }

        //ɾ��m_allIndex2���������
        map<string,set<long>>::iterator it2;
        it2=m_allIndex2.find(strInstrument);
        if(it2!=m_allIndex2.end()&&it2->second.find(FTID)!=it2->second.end())
        {
            it2->second.erase(FTID);
            if(it2->second.empty())
            {
                //�����Լ��Ӧ�ļ�¼��ɾ�����ˣ�����ԼҲɾ��
                m_allIndex2.erase(it2);
            }
        }

        //ɾ��m_all���������
        map<KeyType,DataStruType*>::iterator it_all;
        it_all=m_all.find(tmpkey);
        if(it_all!=m_all.end()&&it_all->second!=NULL)
        {
            pData=it_all->second;
            m_all.erase(it_all);
        }

        //ɾ��m_all2���������
        map<long,DataStruType*>::iterator it_all2;
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

            //ɾ�����ݺ󣬵���m_baseUpdateSeq����ʾ�����б��
            //++m_baseUpdateSeq;
            m_lastUpdateSeq=InterlockedIncrement(&m_baseUpdateSeq);
            if(m_bUpdateSeq2FTID) m_UpdateSeq2FTID[m_lastUpdateSeq]=FTID;
        }
    };


    ///��������һ���¼�¼
    void UpdateRecord_Internal(DataStruType& DataInfo)
    {
        map<KeyType,DataStruType*>::iterator it_all;
        KeyType tmpkey(DataInfo);
        string strInstrument(tmpkey.InstrumentID);
        long FTID;

        it_all=m_all.find(tmpkey);
        if(it_all!=m_all.end()&&it_all->second!=NULL)
        {
            //����ԭ������

            //����ԭ�е�FTID
            FTID=it_all->second->FTID;
            DataInfo.FTID=FTID;
            //DataInfo.UpdateSeq=m_baseUpdateSeq++;
            DataInfo.UpdateSeq=InterlockedIncrement(&m_baseUpdateSeq);
            m_lastUpdateSeq=DataInfo.UpdateSeq;
            if(m_bUpdateSeq2FTID) m_UpdateSeq2FTID[m_lastUpdateSeq]=FTID;

            //��������
            *(it_all->second)=DataInfo;
        }
        else
        {
            //����������
        
            //�����µ�FTID
            FTID=InterlockedIncrement(&m_baseFTID);
            DataInfo.FTID=FTID;
            DataInfo.UpdateSeq=InterlockedIncrement(&m_baseUpdateSeq);
            m_lastUpdateSeq=DataInfo.UpdateSeq;
            if(m_bUpdateSeq2FTID) m_UpdateSeq2FTID[m_lastUpdateSeq]=FTID;

            //newһ����Ԫ��
            DataStruType *pvalue=new DataStruType(DataInfo);

            //����m_all
            m_all.insert(make_pair(tmpkey,pvalue));

            //����m_all2
            m_all2.insert(make_pair(FTID,pvalue));

            //����m_allIndex
            map<string,set<KeyType>>::iterator it;
            it=m_allIndex.find(strInstrument);
            if(it==m_allIndex.end())
            {
                set<KeyType> setKey;
                setKey.insert(tmpkey);
                m_allIndex.insert(make_pair(strInstrument,setKey));
            }
            else it->second.insert(tmpkey);

            //����m_allIndex2
            map<string,set<long>>::iterator it2;
            it2=m_allIndex2.find(strInstrument);
            if(it2==m_allIndex2.end())
            {
                set<long> setFTID;
                setFTID.insert(FTID);
                m_allIndex2.insert(make_pair(strInstrument,setFTID));
            }
            else it2->second.insert(FTID);

            //����m_allFTID
            set<long>::iterator it_FTID;
            it_FTID=m_allFTID.find(FTID);
            if(it_FTID==m_allFTID.end())
                m_allFTID.insert(FTID);

        }
    }

    ///��ȡָ��Key��Value�����سɹ����
    ///�ɹ�ʱ���ص����ڲ�ָ�룬С��ʹ��
    bool GetValue_Internal(DataStruType*& poutData,const KeyType& Key)
    {
        map<KeyType,DataStruType*>::iterator it=m_all.find(Key);
        if(it!=m_all.end()&&it->second!=NULL) 
        {
            poutData=it->second;
            return true;
        }
        else
        {
            poutData=NULL;
            return false;
        }
    };

    ///��ȡָ��FTID��Value�����سɹ����
    ///�ɹ�ʱ���ص����ڲ�ָ�룬��Ҫ�޸�֮��С��ʹ��
    bool GetValue2_Internal(DataStruType*& poutData,long FTID)
    {
        map<long,DataStruType*>::iterator it2;
        it2=m_all2.find(FTID);
        if(it2!=m_all2.end()&&it2->second!=NULL) 
        {
            poutData=it2->second;
            return true;
        }
        else
        {
            poutData=NULL;
            return false;
        }
    };

    ///��ȡָ��FTID��Value�����سɹ����
    bool GetValue3_Internal(DataStruType& outData,long FTID)
    {
        map<long,DataStruType*>::iterator it2;
        it2=m_all2.find(FTID);
        if(it2!=m_all2.end()&&it2->second!=NULL) 
        {
            outData=*(it2->second);
            return true;
        }
        else
        {
            return false;
        }
    };

    ///��ȡָ��Key��Value�����سɹ����
    bool GetValue4_Internal(DataStruType& outData,const KeyType& Key)
    {
        map<KeyType,DataStruType*>::iterator it=m_all.find(Key);
        if(it!=m_all.end()&&it->second!=NULL) 
        {
            outData=*(it->second);
            return true;
        }
        else return false;
    };

    ///��ȡm_QryRlt��ָ��Key��Value��ָ�룬���سɹ����
    ///�ɹ�ʱ���ص����ڲ�ָ�룬��Ҫ�޸�֮��С��ʹ��
    ///�����ֲ����ݼ�ʹ��
    bool GetpValueInQryRlt_Internal(DataStruType*& poutData,const KeyType& Key)
    {
        poutData=NULL;

        if(!m_bNeedKey2QryRlt) 
            return false;

        map<KeyType,int>::iterator it;
        
        it=m_mapKey2QryRlt.find(Key);
        if(it==m_mapKey2QryRlt.end())
            return false;

        int count=m_QryRlt.size();

        if(it->second>=count) 
            return false;

        KeyType tmpkey(m_QryRlt[it->second]);
        if(tmpkey!=Key)
            return false;

        poutData=&(m_QryRlt[it->second]);

        return true;
    };

    ///��ȡ��Լ��ص�KeySet�����سɹ����
    ///�ɹ�ʱ���ص����ڲ�ָ�룬��Ҫ�޸�֮��С��ʹ��
    bool GetKeySetOfInstrument(set<KeyType>*& poutKeySet,const string& strInstrument)
    {
        map<string,set<KeyType>>::iterator it;
        it=m_allIndex.find(strInstrument);
        if(it!=m_allIndex.end()&&!it->second.empty())
        {
            poutKeySet=&(it->second);
            return true;
        }
        else 
        {
            poutKeySet=NULL;
            return false;
        }
    }

    ///��ȡ��Լ��ص�FTIDSet�����سɹ����
    ///�ɹ�ʱ���ص����ڲ�ָ�룬��Ҫ�޸�֮��С��ʹ��
    bool GetFTIDSetOfInstrument(set<long>*& poutFTIDSet,const string& strInstrument)
    {
        map<string,set<long>>::iterator it2;
        it2=m_allIndex2.find(strInstrument);
        if(it2!=m_allIndex2.end()&&!it2->second.empty())
        {
            poutFTIDSet=&(it2->second);
            return true;
        }
        else 
        {
            poutFTIDSet=NULL;
            return false;
        }
    }

    //��ȡȫ��FTID������Լ���ࡣͬһ����Լ��FTID�������
    void GetFTIDsByInstrument(map<string,vector<long>>& outFTIDs)
    {
        outFTIDs.clear();
        map<string,set<long>>::const_iterator it;
        for(it=m_allIndex2.begin();it!=m_allIndex2.end();it++)
        {
            if(!it->second.empty())
            {
                outFTIDs[it->first]=vector<long>();
                map<string,vector<long>>::iterator it2=outFTIDs.find(it->first);
                
                for(set<long>::const_iterator it3=it->second.begin();it3!=it->second.end();it3++)
                    it2->second.push_back(*it3);
            }
        }
    }


    ///��ȡȫ��FTID
    void GetAllFTID_Internal(set<long>& outDatas)
    {
        outDatas=m_allFTID;
    }

    //��ȡȫ��FTID
    void GetAllFTID_Internal(vector<long>& outFTIDs)
    {
        outFTIDs.clear();
        for(set<long>::const_iterator it=m_allFTID.begin();it!=m_allFTID.end();it++)
            outFTIDs.push_back(*it);
    }

    ///��ȡȫ�����ݣ�������������������������������
    int GetAll2_Internal(vector<DataStruType>& outDatas)
    {
        outDatas.clear();

        map<long,DataStruType*>::iterator it;
        
        for(it=m_all2.begin();it!=m_all2.end();it++)
        {
            if(it->second!=NULL)
                outDatas.push_back(*(it->second));
        }
        return outDatas.size();
    };
    ///��ȡ���ڵ���MinFTID��ȫ�����ݣ������������������������������С�m_bUpdateSeq2FTIDΪtrueʱ��Ч
    int GetAll_GE_UpdateSeq_Internal(long MinUpdateSeq,vector<DataStruType>& outDatas)
    {
        outDatas.clear();

        if(!m_bUpdateSeq2FTID) return 0;

        set<long> FTIDs;
        for(map<long,long>::const_iterator it=m_UpdateSeq2FTID.lower_bound(MinUpdateSeq);it!=m_UpdateSeq2FTID.end();it++)
        {
            if(FTIDs.find(it->second)==FTIDs.end())
                FTIDs.insert(it->second);
        }
        for(set<long>::const_iterator it=FTIDs.begin();it!=FTIDs.end();it++)
        {
            map<long,DataStruType*>::const_iterator it2=m_all2.find(*it);
            if(it2!=m_all2.end()&&it2->second!=NULL)
            {
                outDatas.push_back(*(it2->second));
            }
        }

        return outDatas.size();
    };

    ///��ȡ��Լ��ص�ȫ�����ݣ������������������������ݷ���������
    int GetAllAboutInstrument2_Internal(const string strInstrument,vector<DataStruType>& outDatas)
    {
        map<string,set<long>>::iterator it2;

        outDatas.clear();

        it2=m_allIndex2.find(strInstrument);
        if(it2!=m_allIndex2.end()&&!it2->second.empty())
        {
            set<long>::iterator itFTID;
            long FTID;
            map<long,DataStruType*>::iterator itData;
            for(itFTID=it2->second.begin();itFTID!=it2->second.end();itFTID++)
            {
                FTID=*itFTID;
                itData=m_all2.find(FTID);
                if(itData!=m_all2.end()&&itData->second!=NULL)
                    outDatas.push_back(*(itData->second));
            }
        }
        return outDatas.size();
    };


    ///��ȡ��Լ��ص�ȫ�����ݣ������������������������ݷ���map��
    int GetAllAboutInstrument_Internal(const string strInstrument,map<KeyType,DataStruType>& outDatas)
    {
        map<string,set<KeyType>>::iterator itKeyset;
        map<KeyType,DataStruType>::iterator it;

        itKeyset=m_allIndex.find(strInstrument);
        outDatas.clear();
        if(itKeyset!=m_allIndex.end()&&!itKeyset->second.empty())
        {
            set<KeyType>::iterator itKey;
            for(itKey=itKeyset->second.begin();itKey!=itKeyset->second.end();itKey++)
            {
                it=m_all.find(*itKey);
                if(it!=m_all.end())
                    outDatas.insert(make_pair(it->first,it->second));
            }
        }
        return outDatas.size();
    };

    ///��ȡ���ڽ��в�ѯ�ĺ�Լ����
    void GetQryInstrument(string& outData)
    {
        outData=m_strQryInstrument;
    }

    ///��ȡ��ѯ����ĺ�Լ����
    void GetInstrumentSetOfQryRlt(set<string>& outData)
    {
        outData=m_InstrumentsInQryRlt;
    }

    ///����������ţ���ʾ���������и���
    long NewUpdateSeq(long FTID)
    {
        //long NewSeq=m_baseUpdateSeq++;
        long NewSeq=InterlockedIncrement(&m_baseUpdateSeq);
        m_lastUpdateSeq=NewSeq;
        if(m_bUpdateSeq2FTID) m_UpdateSeq2FTID[m_lastUpdateSeq]=FTID;
        return NewSeq;
    }

    ///��ȡ��ǰ�ĸ�����ţ������ж����������Ƿ��и���
    long GetLastUpdateSeq(void)
    {
        //return m_baseUpdateSeq;
        return m_lastUpdateSeq;
    }

    ///�ж��Ƿ���ָ����Լ���������
    bool HaveDataOfInstrumentID(const string& strInstrument)
    {
        map<string,set<KeyType>>::iterator it;
        it=m_allIndex.find(strInstrument);
        return (it!=m_allIndex.end()&&!it->second.empty()) ? true : false;
    }
    ///�ж�ָ��key���������Ƿ����
    bool IsExist(const KeyType& Key)
    {
        map<KeyType,DataStruType*>::iterator it=m_all.find(Key);
        if(it!=m_all.end()&&it->second!=NULL) return true;
        else return false;
    };

    ///��ȡ���ݼ��к�ԼID�ļ���
    void GetAllInstrumentsID(set<string>& outData)
    {
        outData.clear();
        for(map<string,set<long>>::iterator it=m_allIndex2.begin();it!=m_allIndex2.end();it++)
        {
            if(outData.find(it->first)==outData.end())
                outData.insert(it->first);
        }
    }
    ///�ж��Ƿ�Ϊ��
    bool empty(void)
    {
        return m_all.empty();
    }
    ///ȡ����key
    void GetAllKeys(vector<KeyType>& outData)
    {
        outData.clear();
        for(map<KeyType,DataStruType*>::const_iterator it=m_all.begin();it!=m_all.end();it++)
            outData.push_back(it->first);
    }



    //----------���������ݼ�----------
    string                          m_strQryInstrument;     //��ѯ��Ӧ�ĺ�Լ���룬Ϊ��ʱ��ʾ��ѯȫ��
    vector<DataStruType>            m_QryRlt;               //��ѯ�Ľ��
    map<KeyType,int>                m_mapKey2QryRlt;        //��ѯ�����Key to vector order ��map
    set<string>                     m_InstrumentsInQryRlt;  //��ѯ����еĺ�Լ�б�

    map<KeyType,DataStruType*>      m_all;                  //Key��Ӧ��ȫ����¼
    map<long,DataStruType*>         m_all2;                 //FTID��Ӧ��ȫ����¼
    map<string,set<KeyType>>        m_allIndex;             //��Լ��Ӧ���������ϡ�
    map<string,set<long>>           m_allIndex2;            //��Լ��Ӧ��FTID�ļ��ϡ�
    set<long>                       m_allFTID;              //ȫ��FTID����
    map<long,long>                  m_UpdateSeq2FTID;       //UpdateSeq��Ӧ��FTID
    bool                            m_bUpdateSeq2FTID;      //�Ƿ�Ҫά��UpdateSeq��FTID�Ķ�Ӧ��ϵ���ɽ��ͱ���Ҫά���������������

private:

    bool m_bNeedKey2QryRlt;                                 //��ѯ�������ʱ���Ƿ���Ҫά��m_mapKey2QryRlt. ���ڳֲ֣���Ҫά�������ںϲ����ճֲֺ���ʷ�ֲ֣���ϳֲ���ϸҲ��Ҫ�������Ĳ���
    volatile long& m_baseFTID;                              //FTID��׼ֵ��ÿ��ʹ�ú����
    volatile long& m_baseUpdateSeq;                         //UpdateSeq��׼ֵ��ÿ��ʹ�ú����
    long m_lastUpdateSeq;                                   //���һ���޸ĵ�UpdateSeqֵ����������ʾ������û�и���
};

#ifdef WIN32
#pragma managed(pop)
#endif 
