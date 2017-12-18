#include "StdAfx.h"
#include "Module-Misc2/GlobalFunc.h"
#include "MovePositionMgr.h"

MovePositionMgr::MovePositionMgr(void)
{
	ZeroMemory(&m_stMovePosition, sizeof(m_stMovePosition));

	m_stMovePosition.nSourcePriceLimit.xmlset(2);		//ԭʼ��Լ�޼ۼ۸�0,�м� 1,���¼� 2������
	m_stMovePosition.nTargetPriceLimit.xmlset(2);		//Ŀ���Լ�޼ۼ۸�
	m_stMovePosition.nSourcePriceOffset.xmlset(0);		//ԭʼ��Լ�����۲�
	m_stMovePosition.nTargetPriceOffset.xmlset(0);		//Ŀ���Լ�����۲�

	m_stMovePosition.nOperateOrder.xmlset(0);			//����˳��
	m_stMovePosition.nOutTimeSec.xmlset(10);			//��ʱʱ��
	m_stMovePosition.nRetryTimes.xmlset(5);			//���Դ���

	m_stMovePosition.nSourceNotDealAction.xmlset(2);	//ԭʼ��Լδ�ɽ���������0,�޶��� 1,�����¼۸����ĵ� 2,�������۸����ĵ� 3,���м۸ĵ� 4,����
	m_stMovePosition.nTargetNotDealAction.xmlset(2);	//Ŀ���Լδ�ɽ�������
	m_stMovePosition.nSourceNotDealPriceOffset.xmlset(1);	//ԭʼ��Լδ�ɽ��������۲�
	m_stMovePosition.nTargetNotDealPriceOffset.xmlset(1);	//Ŀ���Լδ�ɽ��������۲�
}

MovePositionMgr::~MovePositionMgr(void)
{
}

bool MovePositionMgr::InitCfg( TiXmlElement *root, bool bFromUserCfg )
{
    if(root==NULL)
        return false;

    TiXmlElement* pMovePosition = root->FirstChildElement("MovePosition");
	if ( NULL == pMovePosition )
	{
		return false;
	}

	TiXmlElement* pSourcePriceLimit = pMovePosition->FirstChildElement("SourcePriceLimit");
	if (pSourcePriceLimit )
	{
    	if ( NULL != pSourcePriceLimit->GetText())
	    {
		    m_stMovePosition.nSourcePriceLimit.xmlset(atoi( pSourcePriceLimit->GetText()), bFromUserCfg);
    	}
	}

	TiXmlElement* pTargetPriceLimit = pMovePosition->FirstChildElement("TargetPriceLimit");
	if (pTargetPriceLimit )
	{
    	if ( NULL != pTargetPriceLimit->GetText())
	    {
		    m_stMovePosition.nTargetPriceLimit.xmlset(atoi( pTargetPriceLimit->GetText()), bFromUserCfg);
	    }
	}

	TiXmlElement* pSourcePriceOffset = pMovePosition->FirstChildElement("SourcePriceOffset");
	if (pSourcePriceOffset )
	{
    	if ( NULL != pSourcePriceOffset->GetText())
	    {
		    m_stMovePosition.nSourcePriceOffset.xmlset(atoi( pSourcePriceOffset->GetText()), bFromUserCfg);
	    }
	}

	TiXmlElement* pTargetPriceOffset = pMovePosition->FirstChildElement("TargetPriceOffset");
	if ( pTargetPriceOffset )
	{
	    if ( NULL != pTargetPriceOffset->GetText())
	    {
		    m_stMovePosition.nTargetPriceOffset.xmlset(atoi( pTargetPriceOffset->GetText()), bFromUserCfg);
	    }
	}

	TiXmlElement* pOperateOrder = pMovePosition->FirstChildElement("OperateOrder");
	if ( pOperateOrder )
	{
    	if ( NULL != pOperateOrder->GetText())
	    {
		    m_stMovePosition.nOperateOrder.xmlset(atoi( pOperateOrder->GetText()), bFromUserCfg);
	    }
	}

	TiXmlElement* pOutTimeSec = pMovePosition->FirstChildElement("OutTimeSec");
	if ( pOutTimeSec )
	{
	    if ( NULL != pOutTimeSec->GetText())
	    {
		    m_stMovePosition.nOutTimeSec.xmlset(atoi( pOutTimeSec->GetText()), bFromUserCfg);
	    }
	}

	TiXmlElement* pRetryTimes = pMovePosition->FirstChildElement("RetryTimes");
	if ( pRetryTimes )
	{
    	if ( NULL != pRetryTimes->GetText())
	    {
		    m_stMovePosition.nRetryTimes.xmlset(atoi( pRetryTimes->GetText()), bFromUserCfg);
	    }
	}

	TiXmlElement* pSourceNotDealAction = pMovePosition->FirstChildElement("SourceNotDealAction");
	if ( pSourceNotDealAction )
	{
    	if ( NULL != pSourceNotDealAction->GetText())
	    {
		    m_stMovePosition.nSourceNotDealAction.xmlset(atoi( pSourceNotDealAction->GetText()), bFromUserCfg);
	    }
	}

	TiXmlElement* pTargetNotDealAction = pMovePosition->FirstChildElement("TargetNotDealAction");
	if ( pTargetNotDealAction )
	{
    	if ( NULL != pTargetNotDealAction->GetText())
	    {
		    m_stMovePosition.nTargetNotDealAction.xmlset(atoi( pTargetNotDealAction->GetText()), bFromUserCfg);
	    }
	}

	TiXmlElement* pSourceNotDealPriceOffset = pMovePosition->FirstChildElement("SourceNotDealPriceOffset");
	if ( pSourceNotDealPriceOffset )
	{
	    if ( NULL != pSourceNotDealPriceOffset->GetText())
	    {
		    m_stMovePosition.nSourceNotDealPriceOffset.xmlset(atoi( pSourceNotDealPriceOffset->GetText()), bFromUserCfg);
	    }
	}

	TiXmlElement* pTargetNotDealPriceOffset = pMovePosition->FirstChildElement("TargetNotDealPriceOffset");
	if ( pTargetNotDealPriceOffset )
	{
    	if ( NULL != pTargetNotDealPriceOffset->GetText())
	    {
		    m_stMovePosition.nTargetNotDealPriceOffset.xmlset(atoi( pTargetNotDealPriceOffset->GetText()), bFromUserCfg);
	    }
	}

	return true;
}

bool MovePositionMgr::SaveCfg( TiXmlElement *root )
{
	TiXmlElement *pMovePosition = root->FirstChildElement("MovePosition");
	if(pMovePosition)
		root->RemoveChild(pMovePosition);

	pMovePosition = new TiXmlElement("MovePosition");

	TiXmlElement *pChildItem = NULL;
	std::string strContent;
	TiXmlText *pContent = NULL;
	if ( m_stMovePosition.nSourcePriceLimit.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("SourcePriceLimit");
		pMovePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stMovePosition.nSourcePriceLimit.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stMovePosition.nTargetPriceLimit.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("TargetPriceLimit");
		pMovePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stMovePosition.nTargetPriceLimit.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stMovePosition.nSourcePriceOffset.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("SourcePriceOffset");
		pMovePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stMovePosition.nSourcePriceOffset.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stMovePosition.nTargetPriceOffset.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("TargetPriceOffset");
		pMovePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stMovePosition.nTargetPriceOffset.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stMovePosition.nOperateOrder.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OperateOrder");
		pMovePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stMovePosition.nOperateOrder.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stMovePosition.nOutTimeSec.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OutTimeSec");
		pMovePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stMovePosition.nOutTimeSec.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stMovePosition.nRetryTimes.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("RetryTimes");
		pMovePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stMovePosition.nRetryTimes.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stMovePosition.nSourceNotDealAction.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("SourceNotDealAction");
		pMovePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stMovePosition.nSourceNotDealAction.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stMovePosition.nTargetNotDealAction.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("TargetNotDealAction");
		pMovePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stMovePosition.nTargetNotDealAction.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stMovePosition.nSourceNotDealPriceOffset.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("SourceNotDealPriceOffset");
		pMovePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stMovePosition.nSourceNotDealPriceOffset.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_stMovePosition.nTargetNotDealPriceOffset.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("TargetNotDealPriceOffset");
		pMovePosition->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_stMovePosition.nTargetNotDealPriceOffset.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( NULL != pMovePosition->FirstAttribute() || NULL != pMovePosition->FirstChild())
	{
		root->LinkEndChild(pMovePosition);
	}
	else
	{
		delete pMovePosition;
		pMovePosition = NULL;
	}

	return true;
}

void MovePositionMgr::ClearCfg()
{
	ZeroMemory(&m_stMovePosition, sizeof(m_stMovePosition));
}

MOVEPOSITIONCFG MovePositionMgr::GetMovePositionCfg()
{
	return m_stMovePosition;
}

void MovePositionMgr::SetMovePositionCfg( const MOVEPOSITIONCFG& stMovePosition )
{
	m_stMovePosition = stMovePosition;
}