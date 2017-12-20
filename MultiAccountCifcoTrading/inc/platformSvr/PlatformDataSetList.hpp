/*
    ����ά���ײ��б������ݣ������������ɽ����ֲ���ϸ���ֲ֡���ϳֲ�
    ע�⣺����Ķ���ֻ����PlatformDataMgr��ʹ�ã����ﲻ�ܼ���
    ���е����ݽṹ���ж���Instrument��FTID��UpdateSeq�⼸��

    ע��
    1. ������Ԫ��ʱ����newһ��DataStruType��m_all��m_all2������new�����Ľṹ��
    2. ɾ��Ԫ��ʱ����Ҫ��DataStruTypeɾ��

    added by l. 20120313
*/


#pragma once

#include <vector>
#include <map>
#include <list>
#include <set>
#include <string>
#include "../inc/ISvr/BusinessData.hpp"


static long m_baseFTID=1;                                    //FTID��׼
static long m_baseUpdateSeq=1;                               //UpdateSeq��׼
template <typename KeyType,typename DataStruType>
class CPlatformDataSetList
{
public:
    CPlatformDataSetList()
    {
        m_bNeedsetKeyOfQryRlt=false;

        m_strQryInstrument.clear();
        m_QryRlt.clear();
        m_mapKey2VecOrdOfQryRlt.clear();
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
        m_mapKey2VecOrdOfQryRlt.clear();
        m_InstrumentsInQryRlt.clear();

        Clear2();
    }

    void Clear2(void)
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

    //bNeedsetKeyOfQryRlt:��ѯ�������ʱ���Ƿ���Ҫ����mapQryRlt. ���ڳֲ֣���Ҫ���������ںϲ����ճֲֺ���ʷ�ֲ�
    void SetbNeedsetKeyOfQryRlt(bool bNeedsetKeyOfQryRlt)
    {
        m_bNeedsetKeyOfQryRlt=bNeedsetKeyOfQryRlt;
    }

    //��ղ�ѯ��map
    void WillQry_Internal(std::string& strQryInstrument)
    {
        m_strQryInstrument=strQryInstrument;
        m_QryRlt.clear();
        m_mapKey2VecOrdOfQryRlt.clear();
        m_InstrumentsInQryRlt.clear();
    };

    //��ѯ�������
    void QryRltReach_Internal(const DataStruType& DataInfo,const int ErrID,const bool bIsLast)
    {
        if(ErrID==0)
        {
            std::string strInstrument(DataInfo.InstrumentID);
            m_QryRlt.push_back(DataInfo);
            if(m_bNeedsetKeyOfQryRlt)
            {
                KeyType key(DataInfo);
                if(m_mapKey2VecOrdOfQryRlt.find(key)==m_mapKey2VecOrdOfQryRlt.end())
                    m_mapKey2VecOrdOfQryRlt.insert(make_pair(key,(int)m_QryRlt.size()-1));
            }

            if(m_InstrumentsInQryRlt.find(strInstrument)==m_InstrumentsInQryRlt.end())
                m_InstrumentsInQryRlt.insert(strInstrument);
        }

        if(bIsLast)
        {
            std::map<KeyType,DataStruType*>::iterator it_all;
            std::string strInstrument;
            int i,count;


            //ֻ����ָ����Լ�����ݡ�Ϊ���Ч�ʣ��ֱ���Բ��ָ��º�ȫ�����µ�������
            if(!m_strQryInstrument.empty())
            {
                std::map<std::string,std::set<KeyType>>::iterator it;
                std::map<std::string,std::set<long>>::iterator it2;
                std::map<long,KeyType>::iterator it_FTID;
                std::set<long> setFTID;
                long FTID;

                //��m_QryRlt�е����ݲ������µ�m_all��
                setFTID.clear();
                count=m_QryRlt.size();
                for(i=0;i<count;i++)
                {
                    UpdateRecord_Internal(m_QryRlt[i]);
                    setFTID.insert(m_QryRlt[i].FTID);
                }

                //ɾ��m_all�в��ٴ��ڵ�����(m_QryRlt��û���е�����)
                //ע�⣬��ʱsetFTID�е���������ЧFTIDֵ�����ھ���ԭ�е������Ƿ���Ҫɾ��
                it2=m_allIndex2.find(m_strQryInstrument);
                if(it2!=m_allIndex2.end()&&!it2->second.empty())
                {
                    std::set<long>& oldmapFTID=it2->second;
                    std::set<long>::iterator itold=oldmapFTID.begin();
                    std::set<long>::iterator itnew=setFTID.begin();

                    while(itold!=oldmapFTID.end())
                    {
                        FTID=(*itold);
                        itold++;

                        while(itnew!=setFTID.end()&&(*itnew)<FTID) itnew++;
                        if(itnew==setFTID.end()||(*itnew)!=FTID)
                        {
                            //itnew��û�ж�Ӧ��Ŀ����Ҫɾ��
                            DeleteRecord3_Internal(FTID);

                            if(m_allIndex2.find(m_strQryInstrument)==m_allIndex2.end())
                                break;
                        }
                    }
                }
            }
            else
            {
                //��������

                //���ԭ����Key���ڣ�����ԭ�е�FTID�����������µģ�ͬʱ����UpdateSeq
                
                //for(i=0;i<count;i++)
                //{
                //    KeyType tmpkey(m_QryRlt[i]);
                //    it_all=m_all.find(tmpkey);
                //    if(it_all!=m_all.end()&&it_all->second!=NULL)
                //        m_QryRlt[i].FTID=it_all->second->FTID;
                //    else 
                //        m_QryRlt[i].FTID=m_baseFTID++;

                //    m_QryRlt[i].UpdateSeq=m_baseUpdateSeq++;
                //}

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
        DataInfo.UpdateSeq=m_baseUpdateSeq++;

        UpdateRecord_Internal(DataInfo);
    };



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

        //ɾ��m_allFTID���������
        std::set<long>::iterator it_FTID;
        it_FTID=m_allFTID.find(FTID);
        if(it_FTID!=m_allFTID.end())
            m_allFTID.erase(it_FTID);

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
    void UpdateRecord_Internal(DataStruType& DataInfo)
    {
        std::map<KeyType,DataStruType*>::iterator it_all;
        KeyType tmpkey(DataInfo);
        std::string strInstrument(tmpkey.InstrumentID);
        long FTID;

        it_all=m_all.find(tmpkey);
        if(it_all!=m_all.end()&&it_all->second!=NULL)
        {
            //����ԭ������

            //����ԭ�е�FTID
            FTID=it_all->second->FTID;
            DataInfo.FTID=FTID;
            DataInfo.UpdateSeq=m_baseUpdateSeq++;

            //��������
            *(it_all->second)=DataInfo;
        }
        else
        {
            //����������
        
            //�����µ�FTID
            FTID=m_baseFTID++;
            DataInfo.FTID=FTID;
            DataInfo.UpdateSeq=m_baseUpdateSeq++;

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

            //����m_allFTID
            std::set<long>::iterator it_FTID;
            it_FTID=m_allFTID.find(FTID);
            if(it_FTID==m_allFTID.end())
                m_allFTID.insert(FTID);

        }
    }

    ///��ȡָ��Key��Value�����سɹ����
    bool GetValue_Internal(DataStruType*& poutData,const KeyType& Key)
    {
        std::map<KeyType,DataStruType*>::iterator it=m_all.find(Key);
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
    bool GetValue2_Internal(DataStruType*& poutData,long FTID)
    {
        std::map<long,DataStruType*>::iterator it2;
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
        std::map<long,DataStruType*>::iterator it2;
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


    ///��ȡm_QryRlt��ָ��Key��Value��ָ�룬���سɹ����
    bool GetpValueInQryRlt_Internal(DataStruType*& poutData,const KeyType& Key)
    {
        poutData=NULL;

        if(!m_bNeedsetKeyOfQryRlt) 
            return false;

        std::map<KeyType,int>::iterator it;
        
        it=m_mapKey2VecOrdOfQryRlt.find(Key);
        if(it==m_mapKey2VecOrdOfQryRlt.end())
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
    bool GetKeySetOfInstrument(std::set<KeyType>*& poutKeySet,std::string& strInstrument)
    {
        std::map<std::string,std::set<KeyType>>::iterator it;
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
    bool GetFTIDSetOfInstrument(std::set<long>*& poutFTIDSet,const std::string& strInstrument)
    {
        std::map<std::string,std::set<long>>::iterator it2;
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


    ///��ȡȫ��FTID
    int GetAllFTID_Internal(std::set<long>& outDatas)
    {
        outDatas=m_allFTID;
    }


    ///��ȡȫ�����ݣ����������������
    int GetAll2_Internal(std::vector<DataStruType>& outDatas)
    {
        outDatas.clear();

        std::map<long,DataStruType*>::iterator it;
        
        for(it=m_all2.begin();it!=m_all2.end();it++)
        {
            if(it->second!=NULL)
                outDatas.push_back(*(it->second));
        }
        return outDatas.size();
    };

    ///��ȡ��Լ��ص�ȫ�����ݣ����������������
    int GetAllAboutInstrument2_Internal(const std::string strInstrument,std::vector<DataStruType>& outDatas)
    {
        std::map<std::string,std::set<long>>::iterator it2;

        outDatas.clear();

        it2=m_allIndex2.find(strInstrument);
        if(it2!=m_allIndex2.end()&&!it2->second.empty())
        {
            std::set<long>::iterator itFTID;
            long FTID;
            std::map<long,DataStruType*>::iterator itData;
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


    ///��ȡ��Լ��ص�ȫ�����ݣ����������������
    int GetAllAboutInstrument_Internal(const std::string strInstrument,std::map<KeyType,DataStruType>& outDatas)
    {
        std::map<std::string,std::set<KeyType>>::iterator itKeyset;
        std::map<KeyType,DataStruType>::iterator it;

        itKeyset=m_allIndex.find(strInstrument);
        outDatas.clear();
        if(itKeyset!=m_allIndex.end()&&!itKeyset->second.empty())
        {
            std::set<KeyType>::iterator itKey;
            for(itKey=itKeyset->second.begin();itKey!=itKeyset->second.end();itKey++)
            {
                it=m_all.find(*itKey);
                if(it!=m_all.end())
                    outDatas.insert(std::make_pair(it->first,it->second));
            }
        }
        return outDatas.size();
    };

    void GetQryInstrument(std::string& outData)
    {
        outData=m_strQryInstrument;
    }

    void GetInstrumentSetOfQryRlt(std::set<std::string>& outData)
    {
        outData=m_InstrumentsInQryRlt;
    }

    long NewUpdateSeq(void)
    {
        long NewSeq=m_baseUpdateSeq++;
        return NewSeq;
    }

    long GetUpdateSeq(void)
    {
        return m_baseUpdateSeq;
    }

    bool HaveDataOfInstrumentID(const std::string& strInstrument)
    {
        std::map<std::string,std::set<KeyType>>::iterator it;
        it=m_allIndex.find(strInstrument);
        return (it!=m_allIndex.end()&&!it->second.empty()) ? true : false;
    }



    //----------���������ݼ�----------
    std::string m_strQryInstrument;                     //��ѯ��Ӧ�ĺ�Լ���룬Ϊ��ʱ��ʾ��ѯȫ��
    std::vector<DataStruType> m_QryRlt;                 //��ѯ�Ľ��
    std::map<KeyType,int> m_mapKey2VecOrdOfQryRlt;      //��ѯ�����Key to vector order ��map
    std::set<std::string> m_InstrumentsInQryRlt;        //��ѯ����еĺ�Լ�б�

    std::map<KeyType,DataStruType*> m_all;              //Key��Ӧ��ȫ����¼
    std::map<long,DataStruType*> m_all2;                //FTID��Ӧ��ȫ����¼
    std::map<std::string,std::set<KeyType>> m_allIndex; //ȫ����¼������, set�ǰ�KeyType�Ķ�������
    std::map<std::string,std::set<long>> m_allIndex2;   //ȫ����¼������, set�ǰ�FTID�Ķ�������
    std::set<long> m_allFTID;                           //ȫ��FTID

private:

    bool m_bNeedsetKeyOfQryRlt;
};

