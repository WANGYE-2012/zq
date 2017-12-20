#pragma once
//�������
#include <string>
#include <vector>

enum eCommandType
{
	CommandType_ReInit,   //����Ϊ��
	CommandType_StartUser,//����ΪUserName
	CommandType_EndUser, //����ΪUserName
	CommandType_Order     //����ΪsOrderAction
};
enum eOrderType
{
	OrderType_OrderInput,
	OrderType_OrderAction,
	OrderType_Random,
};
enum eOpenCloseType
{
	OpenCloseType_Open,
	OpenCloseType_Close,
	OpenCloseType_Random,
};

struct sOrderAction
{
	char mUserName[30];
	int  mnIndex;
	int  mnSocket;
	eOrderType mnOrder; 
	char mProduct[30];
	eOpenCloseType  mnOpenClose;
};

struct sOrderControl
{
	eOrderType mnOrder; 
	char mProduct[30];
	eOpenCloseType  mnOpenClose;
	int mnDuring;//�µ���ʱ��s
	int mnTotal; //�µ��ܱ���
	bool mbUserSingle; //�Ƿ��Ƿ�ÿ���û�
};
struct sUserAndSocket
{
	sUserAndSocket()
	{
		mnSocket = mnIndex = mnNumOrder = mnLastOrderNum = 0;
	}
	sUserAndSocket(std::string nsName,
		int nSocket,
		int nIndex):msTraderName(nsName),mnSocket(nSocket),mnIndex(nIndex){mnNumOrder = 0;mnLastOrderNum = 0;}

	void SetCanOrderNum(int nNumOrder)
	{
		mnNumOrder = nNumOrder;
		mnLastOrderNum = nNumOrder;
	}
	std::string msTraderName;
	int mnSocket;
	int mnIndex;
	int mnNumOrder;
	int mnLastOrderNum;

};
struct sCommand
{
	eCommandType meCommandType;
	std::vector<sUserAndSocket> msDestionTrader;
	sOrderControl mOrderControl;

	void AddUserAndSocket(const sUserAndSocket & nUserAndSocket)
	{
		msDestionTrader.push_back(nUserAndSocket);
	}
	void Clear()
	{
		msDestionTrader.clear();
	}
};
class CCommandManager
{
public:
	CCommandManager()
	{
		mbRuning = false;
		mnTotal = 0;
		mnRemindSecond = 0;
		mnHasSend = 0;
	}
	void SetCurrentCommand(const sCommand & nCommand)
	{
		mCommand = nCommand;
		if(mCommand.mOrderControl.mbUserSingle)
		{
			mnTotal = mCommand.mOrderControl.mnTotal * mCommand.msDestionTrader.size();
			//
			for(int i = 0; i < mCommand.msDestionTrader.size();i++)
			{
				mCommand.msDestionTrader[i].SetCanOrderNum(mCommand.mOrderControl.mnTotal);
			}
		}
		else
		{
			mnTotal = mCommand.mOrderControl.mnTotal;
			//�������ÿ���û������¶��ٱ�
			int nLast = mnTotal;
			for(int i = 0; i < mCommand.msDestionTrader.size();i++)
			{
				if(i == mCommand.msDestionTrader.size()-1)
					mCommand.msDestionTrader[i].SetCanOrderNum(nLast);
				else
				{
					mCommand.msDestionTrader[i].SetCanOrderNum(System::Random::Random(System::Guid::NewGuid().GetHashCode()).Next(0,nLast/2));
					nLast -= mCommand.msDestionTrader[i].mnNumOrder;
				}			
			}
		}

		mnHasSend = 0;
		mnRemindSecond = mCommand.mOrderControl.mnDuring;
	}
	void GetCurrentCommand(sCommand & nCommand)
	{
		nCommand = mCommand;
	}
	void Start()
	{
		mbRuning = true;
	}
	void End()
	{
		mbRuning = false;	

	}
	bool IsRuning(){return mbRuning;};

	int JustifySingleUserCanOrder(int i,std::vector<sOrderAction> & nAction);
	//ÿs����һ�εĻ�ȡ��������Ľӿ�
	bool FetchCustomOrderAction(std::vector<sOrderAction> & nAction);

	void GetDesc(std::string & nsDesc)
	{
		nsDesc.clear();
		char lsTemp[1024] = {0};
		sprintf(lsTemp,"��ǰ���� �ܱ����� %d,�ѷ��ͱ����� %d ,��ʱ���� %ds ʣ��ʱ����%ds,Ŀ���û�����%d",mnTotal,mnHasSend,mCommand.mOrderControl.mnDuring,mnRemindSecond,mCommand.msDestionTrader.size());
		nsDesc = lsTemp;
	}
private:
	bool mbRuning;
	sCommand mCommand;
	int mnTotal;        //�ܹ����ٱ�
	int mnHasSend;      //�ѷ������ٱ�
	int mnRemindSecond; //ʣ������

};