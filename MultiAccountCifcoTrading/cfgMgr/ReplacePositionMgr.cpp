#include "StdAfx.h"
#include "Module-Misc/GLobalFunc.h"
#include "ReplacePositionMgr.h"

ReplacePositionMgr::ReplacePositionMgr(void)
{
	ZeroMemory(&m_stReplacePosition, sizeof(m_stReplacePosition));

	m_stReplacePosition.nClosePositionPriceLimit1.xmlset(2);		//��λ1ƽ���޼�
	m_stReplacePosition.nClosePositionPriceOffset1.xmlset(0);		//��λ1ƽ�ֻ����۲�
	m_stReplacePosition.nOpenPositionPriceLimit1.xmlset(2);		//��λ1�����޼�
	m_stReplacePosition.nOpenPositionPriceOffset1.xmlset(0);		//��λ1���ֻ����۲�

	m_stReplacePosition.nClosePositionPriceLimit2.xmlset(2);		//��λ2ƽ���޼�
	m_stReplacePosition.nClosePositionPriceOffset2.xmlset(0);		//��λ2ƽ�ֻ����۲�
	m_stReplacePosition.nOpenPositionPriceLimit2.xmlset(2);		//��λ2�����޼�
	m_stReplacePosition.nOpenPositionPriceOffset2.xmlset(0);		//��λ2���ֻ����۲�

	m_stReplacePosition.nOperateOrder.xmlset(0);					//����˳��
	m_stReplacePosition.nOutTimeSec.xmlset(10);					//��ʱʱ��
	m_stReplacePosition.nRetryTimes.xmlset(5);					//���Դ���

	m_stReplacePosition.nOpenPositionNotDealAction1.xmlset(2);	//��λ1����δ�ɽ�������
	m_stReplacePosition.nOpenPositionNotDealPriceOffset1.xmlset(1);	//��λ1����δ�ɽ��������۲�
	m_stReplacePosition.nClosePositionNotDealAction1.xmlset(2);	//��λ1ƽ��δ�ɽ�������
	m_stReplacePosition.nClosePositionNotDealPriceOffset1.xmlset(1);	//��λ1ƽ��δ�ɽ��������۲�

	m_stReplacePosition.nOpenPositionNotDealAction2.xmlset(2);	//��λ2����δ�ɽ�������
	m_stReplacePosition.nOpenPositionNotDealPriceOffset2.xmlset(1);	//��λ2����δ�ɽ��������۲�
	m_stReplacePosition.nClosePositionNotDealAction2.xmlset(2);	//��λ2ƽ��δ�ɽ�������
	m_stReplacePosition.nClosePositionNotDealPriceOffset2.xmlset(1);	//��λ2ƽ��δ�ɽ��������۲�
}

ReplacePositionMgr::~ReplacePositionMgr(void)
{
}

bool ReplacePositionMgr::InitCfg( TiXmlElement *root, bool bFromUserCfg )
{
    if(root==NULL)
        return false;

    TiXmlElement* pReplacePosition = root->FirstChildElement("ReplacePosition");
	if ( NULL == pReplacePosition )
	{
		return false;
	}

	TiXmlElement* pElement = NULL;
	pElement = pReplacePosition->FirstChildElement("ClosePositionPriceLimit1");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nClosePositionPriceLimit1.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("ClosePositionPriceOffset1");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nClosePositionPriceOffset1.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("OpenPositionPriceLimit1");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nOpenPositionPriceLimit1.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("OpenPositionPriceOffset1");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nOpenPositionPriceOffset1.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("ClosePositionPriceLimit2");
	if ( pElement ) 
    {
	    if ( NULL == pElement->GetText()) 
        {
	        m_stReplacePosition.nClosePositionPriceLimit2.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("ClosePositionPriceOffset2");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nClosePositionPriceOffset2.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("OpenPositionPriceLimit2");
	if ( pElement ) 
    {
	    if ( NULL == pElement->GetText()) 
        {
	        m_stReplacePosition.nOpenPositionPriceLimit2.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("OpenPositionPriceOffset2");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nOpenPositionPriceOffset2.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("OperateOrder");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nOperateOrder.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("OutTimeSec");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nOutTimeSec.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("RetryTimes");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nRetryTimes.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("OpenPositionNotDealAction1");
	if ( pElement ) 
    {
	    if ( NULL == pElement->GetText()) 
        {
	        m_stReplacePosition.nOpenPositionNotDealAction1.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("OpenPositionNotDealPriceOffset1");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nOpenPositionNotDealPriceOffset1.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("ClosePositionNotDealAction1");
	if ( pElement ) 
    {
	    if ( NULL == pElement->GetText()) 
        {
	        m_stReplacePosition.nClosePositionNotDealAction1.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("ClosePositionNotDealPriceOffset1");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nClosePositionNotDealPriceOffset1.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("OpenPositionNotDealAction2");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nOpenPositionNotDealAction2.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("OpenPositionNotDealPriceOffset2");
	if ( pElement ) 
    {
	    if ( NULL == pElement->GetText()) 
        {
	        m_stReplacePosition.nOpenPositionNotDealPriceOffset2.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("ClosePositionNotDealAction2");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nClosePositionNotDealAction2.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	pElement = pReplacePosition->FirstChildElement("ClosePositionNotDealPriceOffset2");
	if ( pElement ) 
    {
	    if ( pElement->GetText()) 
        {
	        m_stReplacePosition.nClosePositionNotDealPriceOffset2.xmlset(atoi( pElement->GetText()), bFromUserCfg);
        }
    }

	return true;
}

bool ReplacePositionMgr::SaveCfg( TiXmlElement *root )
{
	TiXmlElement *pReplacePosition = root->FirstChildElement("ReplacePosition");
	if(pReplacePosition)
		root->RemoveChild(pReplacePosition);

	pReplacePosition = new TiXmlElement("ReplacePosition");

	TiXmlElement *pChildItem = NULL;
	std::string strContent;
	TiXmlText *pContent = NULL;

	if ( m_stReplacePosition.nClosePositionPriceLimit1.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ClosePositionPriceLimit1");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nClosePositionPriceLimit1.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nClosePositionPriceOffset1.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ClosePositionPriceOffset1");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nClosePositionPriceOffset1.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nOpenPositionPriceLimit1.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OpenPositionPriceLimit1");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nOpenPositionPriceLimit1.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nOpenPositionPriceOffset1.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OpenPositionPriceOffset1");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nOpenPositionPriceOffset1.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);

	}

	if ( m_stReplacePosition.nClosePositionPriceLimit2.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ClosePositionPriceLimit2");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nClosePositionPriceLimit2.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nClosePositionPriceOffset2.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ClosePositionPriceOffset2");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nClosePositionPriceOffset2.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nOpenPositionPriceLimit2.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OpenPositionPriceLimit2");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nOpenPositionPriceLimit2.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nOpenPositionPriceOffset2.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OpenPositionPriceOffset2");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nOpenPositionPriceOffset2.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nOperateOrder.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OperateOrder");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nOperateOrder.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nOutTimeSec.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OutTimeSec");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nOutTimeSec.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nRetryTimes.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("RetryTimes");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nRetryTimes.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nOpenPositionNotDealAction1.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OpenPositionNotDealAction1");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nOpenPositionNotDealAction1.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nOpenPositionNotDealPriceOffset1.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OpenPositionNotDealPriceOffset1");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nOpenPositionNotDealPriceOffset1.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nClosePositionNotDealAction1.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ClosePositionNotDealAction1");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nClosePositionNotDealAction1.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nClosePositionNotDealPriceOffset1.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ClosePositionNotDealPriceOffset1");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nClosePositionNotDealPriceOffset1.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nOpenPositionNotDealAction2.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OpenPositionNotDealAction2");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nOpenPositionNotDealAction2.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nOpenPositionNotDealPriceOffset2.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OpenPositionNotDealPriceOffset2");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nOpenPositionNotDealPriceOffset2.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nClosePositionNotDealAction2.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ClosePositionNotDealAction2");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nClosePositionNotDealAction2.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stReplacePosition.nClosePositionNotDealPriceOffset2.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ClosePositionNotDealPriceOffset2");
		pReplacePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stReplacePosition.nClosePositionNotDealPriceOffset2.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( NULL != pReplacePosition->FirstAttribute() || NULL != pReplacePosition->FirstChild())
	{
		root->LinkEndChild(pReplacePosition);
	}
	else
	{
		delete pReplacePosition;
		pReplacePosition = NULL;
	}

	return true;
}

void ReplacePositionMgr::ClearCfg()
{
	ZeroMemory(&m_stReplacePosition, sizeof(m_stReplacePosition));
}

REPLACEPOSITION ReplacePositionMgr::GetReplacePositionCfg()
{
	return m_stReplacePosition;
}

void ReplacePositionMgr::SetReplacePositionCfg( const REPLACEPOSITION& replacePosition )
{
	m_stReplacePosition = replacePosition;
}
