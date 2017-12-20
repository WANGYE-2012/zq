#include "StdAfx.h"
#include "BankCore.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-FundAccount/FundAccountModule.h"//��ѯ�ڻ��ʽ��˻�
#include "../inc/Module-Misc/TradeInfoDlg.h" //��ѯ�������Ի���
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
CBankCore::CBankCore(void):
m_pDelayTimer(NULL),
m_DetailList(NULL),
m_pSvr(NULL)
{
}

CBankCore::~CBankCore(void)
{
	SAFE_DELETE(m_pDelayTimer);
}
void CBankCore::SubscribeBusinessData(GuiCallBackFunc callback)
{
	//���������ʽ�ת�ڻ��ɹ�Ӧ��
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RspFromBankToFutureByFuture, GID_TRANSFER, callback, false);
	//�����ڻ������ڻ��ʽ�ת����Ӧ��
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RspFromFutureToBankByFuture, GID_TRANSFER, callback, false);
	//�����ڻ������ѯ�������Ӧ��
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RspQueryBankAccountMoneyByFuture, GID_TRANSFER, callback, false);
	//���������ѯת����ˮ��Ӧ
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RspQryTransferSerial, GID_TRANSFER, callback, false);

	//���������ʽ�ת�ڻ��ɹ��ر�
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RtnFromBankToFutureByFuture, GID_TRANSFER, callback, false);
	//�����ڻ������ڻ��ʽ�ת���лر�
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RtnFromFutureToBankByFuture, GID_TRANSFER, callback, false);
	//�����ڻ������ѯ�������ر�
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RtnQueryBankAccountMoneyByFuture, GID_TRANSFER, callback, false);

}
void CBankCore::UnSubscribeBusinessData()
{
	//ȡ�����������ʽ�ת�ڻ��ɹ�Ӧ��
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspFromBankToFutureByFuture, GID_TRANSFER);
	//ȡ�������ڻ������ڻ��ʽ�ת����Ӧ��
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspFromFutureToBankByFuture, GID_TRANSFER);
	//ȡ�������ڻ������ѯ�������Ӧ��
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspQueryBankAccountMoneyByFuture, GID_TRANSFER);
	//ȡ�����������ѯת����ˮ��Ӧ
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspQryTransferSerial, GID_TRANSFER);

	//ȡ�����������ʽ�ת�ڻ��ɹ��ر�
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RtnFromBankToFutureByFuture, GID_TRANSFER);
	//ȡ�������ڻ������ڻ��ʽ�ת���лر�
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RtnFromFutureToBankByFuture, GID_TRANSFER);
	//ȡ�������ڻ������ѯ�������ر�
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RtnQueryBankAccountMoneyByFuture, GID_TRANSFER);
}
bool CBankCore::GetBankInfo(const wxString& BankName,char* BankID,char* BankBranchID,char* BrokerID)
{
	std::map<std::string, PlatformStru_ContractBank> cbi;
	if(m_pSvr&&m_pSvr->GetContractBank(cbi)>0)
	{
		std::map<std::string, PlatformStru_ContractBank>::iterator it=cbi.find(BankName.c_str());
		if(it!=cbi.end())
		{
			strcpy(BankID,it->second.BankID);
			strcpy(BankBranchID,it->second.BankBranchID);
			strcpy(BrokerID,it->second.BrokerID);
			return true;
		}
	}
	return false;
}
wxString CBankCore::GetBankInfo(const char* BankID)
{
	wxString strReturn;
	std::map<std::string, PlatformStru_ContractBank> cbi;
	if(m_pSvr&&m_pSvr->GetContractBank(cbi)>0)
	{
		std::map<std::string, PlatformStru_ContractBank>::iterator it;
		for(it=cbi.begin();it!=cbi.end();++it)
		{
			if(strcmp(BankID,it->second.BankID)==0)
			{
				strReturn=it->second.BankName;
			}
		}
	}
	return strReturn;
}
wxString CBankCore::GetSafeAccout( const wxString strAcc )
{
	wxString str=strAcc;
	size_t digits=str.Length();
	for (size_t i=0;i<digits;++i)
	{
		if(i>4&&i<digits-4)
			str[i]='*';
	}
	return str;
}
void CBankCore::PopupError( CThostFtdcRspInfoField* RspInfoField,const string strmsg )
{
	if(RspInfoField&&RspInfoField->ErrorID!=0)
	{
		wxString strMsg;
		strMsg.Printf(_("%s"),
			RspInfoField->ErrorMsg
			);
		wxMessageBox(strMsg,LOADSTRING_TEXT(strmsg));
	}
}
void CBankCore::OnRtnFutureBank(CThostFtdcRspTransferField* pRawData,const std::string prompt)
{
	if(pRawData->ErrorID == 0)
	{
		DetailListInsertData(pRawData->FutureSerial,
			pRawData->BankAccount,
			pRawData->TradeCode,
			pRawData->TradeAmount,
			pRawData->TradeTime,
			pRawData->ErrorMsg,
			pRawData->BankID);
		wxCommandEvent evt(wxEVT_SUCESS_TRANSFER);	
		GETTOPWINDOW()->AddPendingEvent(evt);
		if(m_pDelayTimer)
			m_pDelayTimer->Start(1000,true);
	}
	//������ʾ��
	CTradeInfoDlg*pBankAcctDlg=TRADEINFODLG(this);
	if(pBankAcctDlg!=NULL) 
	{
		pBankAcctDlg->Hide();
		wxString strMsg;
		if(pRawData->ErrorID == 0)
		{
			strMsg.Printf(LOADFORMATSTRING(BankTransfer_MSG6,"%f"),////�ı��������%f
				pRawData->TradeAmount);
		}
		else
		{
			strMsg.Printf(pRawData->ErrorMsg);
		}
		pBankAcctDlg->ShowTradeMsg(strMsg, -1, LOADSTRING_TEXT(prompt), -1, TRUE);///"�����ʽ�ת�ڻ�"
	}
}
void CBankCore::QueryAccountDetail()
{
	m_TransferDetailIDSet.clear();
	m_DetailList->DeleteAllItems();//ɾ��������
	PlatformStru_QryTransferSerial QryTransferSerial;
	memset(&QryTransferSerial,0x00,sizeof(QryTransferSerial));

	if(m_pSvr) m_pSvr->ReqQryTransferSerial(QryTransferSerial);
}
void CBankCore::QueryBankAccount() 
{
	PlatformStru_ReqQueryAccount ReqQueryAccount;
	memset(&ReqQueryAccount,0x00,sizeof(ReqQueryAccount));
	///ҵ������
	strcpy(ReqQueryAccount.Thost.TradeCode,"204002");
	///���д���
	wxString BankName=GetUIValue("BankName");
	if(BankName.Trim().IsEmpty())
	{
		wxMessageBox(LOADSTRING(BT_ERROR_SELECTBANK),LOADSTRING(USERLOGINDLG_INFORMATION));
		return;
	}
	if(GetBankInfo(BankName,ReqQueryAccount.Thost.BankID,
		ReqQueryAccount.Thost.BankBranchID,
		ReqQueryAccount.Thost.BrokerID))
	{
		///�ڻ��ʽ�����˶Ա�־
		ReqQueryAccount.Thost.SecuPwdFlag=THOST_FTDC_BPWDF_NoCheck;
		ReqQueryAccount.Thost.BankPwdFlag=THOST_FTDC_BPWDF_NoCheck;
		ReqQueryAccount.Thost.VerifyCertNoFlag=THOST_FTDC_YNI_No;
		//�ʽ��˺�
		TThostFtdcAccountIDType data;
		memset(&data,0,sizeof(data));
		if(m_pSvr&&m_pSvr->GetTradingAccountID(data)==0)
			strcpy(ReqQueryAccount.Thost.AccountID,data);
		//�ʽ��˻�����
		wxString strPassword=GetUIValue("AccountPassword");
		if(strPassword.Trim().IsEmpty())
		{
			wxMessageBox(LOADSTRING(BankTransfer_MSG1),LOADSTRING(BankTransfer_MSGERROR),wxOK|wxICON_QUESTION);//"�������ʽ��˻�����!",LOADSTRING(USERLOGINDLG_ERROR)
			return;
		}
		else
		{
			strcpy(ReqQueryAccount.Thost.Password,strPassword);
		}
		//�����˻�����
		strPassword=GetUIValue("BankPassword");
		strcpy(ReqQueryAccount.Thost.BankPassWord,strPassword);
		if(m_pSvr) m_pSvr->ReqQueryBankAccountMoneyByFuture(ReqQueryAccount);
	}


}
void CBankCore::QueryFromBankToFuture(const wxString& strCurrencyID) 
{
	PlatformStru_ReqTransfer ReqTransfer;
	memset(&ReqTransfer,0x00,sizeof(ReqTransfer));
	///ҵ������
	strcpy(ReqTransfer.Thost.TradeCode,"202001");
	///���д���
	wxString BankName=GetUIValue("BankName");
	if(BankName.Trim().IsEmpty())
	{
		wxMessageBox(LOADSTRING(BT_ERROR_SELECTBANK),LOADSTRING(USERLOGINDLG_INFORMATION));
		return;
	}
	if(GetBankInfo(BankName,ReqTransfer.Thost.BankID,
		ReqTransfer.Thost.BankBranchID,
		ReqTransfer.Thost.BrokerID))
	{
		///�ڻ��ʽ�����˶Ա�־
		ReqTransfer.Thost.SecuPwdFlag=THOST_FTDC_BPWDF_NoCheck;
		ReqTransfer.Thost.BankPwdFlag=THOST_FTDC_BPWDF_NoCheck;
		ReqTransfer.Thost.VerifyCertNoFlag=THOST_FTDC_YNI_No;
		//�ʽ��˺�
		TThostFtdcAccountIDType data;
		memset(&data,0,sizeof(data));
		if(m_pSvr&&m_pSvr->GetTradingAccountID(data)==0)
			strcpy(ReqTransfer.Thost.AccountID,data);
		//�ʽ��˻�����
		wxString strPassword;
		strPassword=GetUIValue("AccountPassword");
		if(strPassword.Trim().IsEmpty())
		{
			wxMessageBox(LOADSTRING(BankTransfer_MSG1),LOADSTRING(USERLOGINDLG_INFORMATION));
			return;
		}
		else
		{
			strcpy(ReqTransfer.Thost.Password,strPassword);
		}
		//�����˻�����
		strPassword=GetUIValue("BankPassword");
		strcpy(ReqTransfer.Thost.BankPassWord,strPassword);

		double Amount=atof(GetUIValue("Amount").c_str());
		if(Amount>0)
			ReqTransfer.Thost.TradeAmount=Amount;
		else
		{
			wxMessageBox(LOADSTRING(BT_ERROR_AMOUNT),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
			return;
		}
		strcpy(ReqTransfer.Thost.CurrencyID,strCurrencyID.c_str());
		if(m_pSvr) m_pSvr->ReqFromBankToFutureByFuture(ReqTransfer);
	}
}

void CBankCore::QueryFromFutureToBank(const wxString& strCurrencyID) 
{
	PlatformStru_ReqTransfer ReqTransfer;
	memset(&ReqTransfer,0x00,sizeof(ReqTransfer));
	///ҵ������
	strcpy(ReqTransfer.Thost.TradeCode,"202002");
	///���д���
	wxString BankName=GetUIValue("BankName");
	if(BankName.Trim().IsEmpty())
	{
		wxMessageBox(LOADSTRING(BT_ERROR_SELECTBANK),LOADSTRING(USERLOGINDLG_INFORMATION));
		return;
	}
	if(GetBankInfo(BankName,ReqTransfer.Thost.BankID,
		ReqTransfer.Thost.BankBranchID,
		ReqTransfer.Thost.BrokerID))
	{
		///�ڻ��ʽ�����˶Ա�־
		ReqTransfer.Thost.SecuPwdFlag=THOST_FTDC_BPWDF_NoCheck;
		ReqTransfer.Thost.BankPwdFlag=THOST_FTDC_BPWDF_NoCheck;
		ReqTransfer.Thost.VerifyCertNoFlag=THOST_FTDC_YNI_No;
		//�ʽ��˺�
		TThostFtdcAccountIDType data;
		memset(&data,0,sizeof(data));
		if(m_pSvr&&m_pSvr->GetTradingAccountID(data)==0)
			strcpy(ReqTransfer.Thost.AccountID,data);
		//�ʽ��˻�����
		wxString strPassword;
		strPassword=GetUIValue("AccountPassword");
		if(strPassword.Trim().IsEmpty())
		{
			wxMessageBox(LOADSTRING(BankTransfer_MSG1),LOADSTRING(USERLOGINDLG_INFORMATION));
			return;
		}
		else
		{
			strcpy(ReqTransfer.Thost.Password,strPassword);
		}
		//�����˻�����
		strPassword=GetUIValue("BankPassword");
		strcpy(ReqTransfer.Thost.BankPassWord,strPassword);

		double Amount=atof(GetUIValue("Amount").c_str());
		if(Amount>0)
			ReqTransfer.Thost.TradeAmount=Amount;
		else
		{
			wxMessageBox(LOADSTRING(BT_ERROR_AMOUNT),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
			return;
		}
		strcpy(ReqTransfer.Thost.CurrencyID,strCurrencyID.c_str());
		if(m_pSvr) m_pSvr->ReqFromFutureToBankByFuture(ReqTransfer);
	}
}
/*���з��ش���*/
void CBankCore::ProcessRcvRspTransfer(wxCommandEvent& event)
{
	AbstractBusinessData* pAbstractBusinessData=static_cast<AbstractBusinessData*>(event.GetClientData());
	if(!pAbstractBusinessData)
	{
		wxASSERT(pAbstractBusinessData);
		return;
	}
	CThostFtdcRspInfoField* RspInfoField=NULL;
	switch (pAbstractBusinessData->BID)
	{
	case  BID_RspFromBankToFutureByFuture://�ڻ����������ʽ�ת�ڻ�Ӧ��
		RspInfoField= &(((DataRspFromBankToFutureByFuture*)(pAbstractBusinessData))->RspInfoField);
		PopupError(RspInfoField,"BankTransfer_MSG2");
		break;
	case   BID_RspFromFutureToBankByFuture://�ڻ������ڻ��ʽ�ת����Ӧ��
		RspInfoField= &(((DataRspFromFutureToBankByFuture*)(pAbstractBusinessData))->RspInfoField);
		PopupError(RspInfoField,"BankTransfer_MSG2");
		break;
	case   BID_RspQueryBankAccountMoneyByFuture://�ڻ������ѯ�������Ӧ��
		RspInfoField= &(((DataRspQueryBankAccountMoneyByFuture*)(pAbstractBusinessData))->RspInfoField);
		PopupError(RspInfoField,"BankTransfer_MSG2");
		break;
	case   BID_RspQryTransferSerial://�����ѯת����ˮ��Ӧ
		{
			DataRspQryTransferSerial* pTmp= (DataRspQryTransferSerial*)(pAbstractBusinessData);
			CThostFtdcTransferSerialField* pRawData= &pTmp->TransferSerialField;
			CThostFtdcRspInfoField  *RspInfoField=&pTmp->RspInfoField;
			wxString strMsg;
			strMsg.Printf(_(" %s"),RspInfoField->ErrorMsg);
			if(RspInfoField->ErrorID==0)//û�д���
			{
				DetailListInsertData(pRawData->FutureSerial,
					pRawData->BankAccount,
					pRawData->TradeCode,
					pRawData->TradeAmount,
					pRawData->TradeTime,
					pRawData->ErrorMsg,
					pRawData->BankID);
			}
			else if(RspInfoField->ErrorID == CustomErrorID)
				wxMessageBox(strMsg,LOADSTRING(BankTransfer_MSG4));//"��ѯת����ϸ"
			else
				wxMessageBox(strMsg,LOADSTRING(BankTransfer_MSG3));//"��ѯת����ˮ����ʧ��"
		}           
		break;
	case  BID_RtnFromBankToFutureByFuture://�ڻ����������ʽ�ת�ڻ�Ӧ��
		{
			DataRtnFromBankToFutureByFuture* pTmp= (DataRtnFromBankToFutureByFuture*)(pAbstractBusinessData);
			CThostFtdcRspTransferField* pRawData= &pTmp->RspTransferField;
			OnRtnFutureBank(pRawData,"BankTransfer_MSG7");
		}
		break;
	case   BID_RtnFromFutureToBankByFuture://�ڻ������ڻ��ʽ�ת����Ӧ��
		{
			DataRtnFromFutureToBankByFuture* pTmp= (DataRtnFromFutureToBankByFuture*)(pAbstractBusinessData);
			CThostFtdcRspTransferField* pRawData= &pTmp->RspTransferField;
			OnRtnFutureBank(pRawData,"BankTransfer_MSG5");

		}
		break;
	case BID_RtnQueryBankAccountMoneyByFuture://�ڻ������ѯ�������Ӧ��
		{
			DataRtnQueryBankAccountMoneyByFuture* pTmp= (DataRtnQueryBankAccountMoneyByFuture*)(pAbstractBusinessData);
			CThostFtdcNotifyQueryAccountField* pRawData= &pTmp->NotifyQueryAccountField;
			if(pRawData->ErrorID == 0)
			{
				DetailListInsertData(pRawData->FutureSerial,
					pRawData->BankAccount,
					pRawData->TradeCode,
					pRawData->BankFetchAmount,
					pRawData->TradeTime,
					pRawData->ErrorMsg,
					pRawData->BankID);
			}
			//��ʾ��
			CTradeInfoDlg*pBankAcctDlg=TRADEINFODLG(this);
			if(pBankAcctDlg!=NULL) 
			{
				pBankAcctDlg->Hide();
				wxString info;
				if(pRawData->ErrorID == 0)
				{
					info.reserve(512);
					info.Printf(LOADFORMATSTRING(BankTransfer_BankRsp2,"%s%s"),//�ı��������%s  %s  %s %s
						//"�����˺ţ� %s\n"
						//"���п��ý� %s\n"
						pRawData->BankAccount,
						wxString::Format("%0.2f",pRawData->BankFetchAmount).c_str());
					pBankAcctDlg->ShowTradeMsg(info, -1, LOADSTRING(BankTransfer_BankRsp1Title), -1, TRUE);//wxString("�����˻�����")
				}
				else
				{
					info.Printf(pRawData->ErrorMsg);
					pBankAcctDlg->ShowTradeMsg(info, -1, LOADSTRING(BankTransfer_MSG10), -1, TRUE);///"�����ʽ�ת�ڻ�"
				}
			}
		}
		break;
	default:
		break;
	}
	delete (void*)pAbstractBusinessData;
	pAbstractBusinessData=NULL;
	event.SetClientData(NULL);
}
void CBankCore::UserChoice(int sel)
{
	wxArrayString items=MA_CORE()->GetAccountArray();
	if(sel>=0&&sel<(int)items.size())
	{
		m_pSvr=g_pPlatformMgr->GetPlatformSvr(items[sel].c_str());
	}	
}