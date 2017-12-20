#include "stdafx.h"
#include "CommandManager.h"
using namespace System;
int CCommandManager::JustifySingleUserCanOrder(int i,std::vector<sOrderAction> & nAction)
{
	//���������ʣ���ʱ��������ô����ʣ��ı���

	//�Ⱦ������ͼ���
	int nOrder = 0;

	if(mnRemindSecond == 0)
		return mCommand.msDestionTrader[i].mnLastOrderNum;
	//�ȼ���ƽ��Ҫ���ͼ���
	int nAvgOrder = mCommand.msDestionTrader[i].mnLastOrderNum / mnRemindSecond;
	if(mnRemindSecond== 1)
		nOrder = mCommand.msDestionTrader[i].mnLastOrderNum;
	else
	{
		Int32 lRandom = System::Random::Random(System::Guid::NewGuid().GetHashCode()).Next(-1,1);

		////�����0 ���͸���
		if(nAvgOrder == 0)
		{		
			lRandom = System::Random::Random(System::Guid::NewGuid().GetHashCode()).Next(-mnRemindSecond,mCommand.msDestionTrader[i].mnLastOrderNum);
			if(lRandom > 0) 
				lRandom = 1;
			else
				lRandom = 0;
		}
	

		nOrder = nAvgOrder+lRandom;
		if(nOrder < 0)
			nOrder = 0;
	
		if(nOrder > mCommand.msDestionTrader[i].mnLastOrderNum)
			nOrder = mCommand.msDestionTrader[i].mnLastOrderNum;
	}

	for(int j = 0 ; j < nOrder; j++)
	{
		sOrderAction lsAction;
		strcpy(lsAction.mProduct,mCommand.mOrderControl.mProduct);
		strcpy(lsAction.mUserName,mCommand.msDestionTrader[i].msTraderName.c_str());
		lsAction.mnIndex = mCommand.msDestionTrader[i].mnIndex;
		if(mCommand.mOrderControl.mnOrder == OrderType_Random)
		{
			Int32 lRandom = System::Random::Random(System::Guid::NewGuid().GetHashCode()).Next(-2,2);
			lsAction.mnOrder = lRandom < 0 ? OrderType_OrderInput : OrderType_OrderAction;
		}
		else
		{
			lsAction.mnOrder = mCommand.mOrderControl.mnOrder;
		}

		if(mCommand.mOrderControl.mnOpenClose == OpenCloseType_Random)
		{
			Int32 lRandom2 = System::Random::Random(System::Guid::NewGuid().GetHashCode()).Next(-2,2);
			lsAction.mnOpenClose = lRandom2 < 0 ? OpenCloseType_Open : OpenCloseType_Close;
		}
		else
		{
			lsAction.mnOpenClose = mCommand.mOrderControl.mnOpenClose;
		}

		lsAction.mnSocket = mCommand.msDestionTrader[i].mnSocket;

		nAction.push_back(lsAction);
	}


	return nOrder;
}

bool CCommandManager::FetchCustomOrderAction(std::vector<sOrderAction> & nAction)
{
	//���ݵ�ǰ�������� 
	//������ɵ�ǰ���������
	if(mbRuning == false)
		return false;

	//ʣ��������һ
	mnRemindSecond --;
	for(int i = 0; i < mCommand.msDestionTrader.size();i++)
	{
		//��������û����ɼ���
		int nOrder = JustifySingleUserCanOrder(i,nAction);
		mCommand.msDestionTrader[i].mnLastOrderNum -= nOrder;
		mnHasSend += nOrder;
	}

	if(mnHasSend == mnTotal)
	{
		End();
	}

	return true;
}