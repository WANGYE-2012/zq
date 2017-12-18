#pragma once

class CWriteLog;
struct RequestData;
struct Stru_UniPkgHead;
class CThreadSharedData;
class CToolThread;
#define MAX_IOBUFSIZE 655360
class CWorker
{
public:
	CWorker(CThreadSharedData* pThreadSharedData );
	~CWorker(void);

	static DWORD WINAPI ThreadFunc(void* pData);
	void Process();
private:
	void OnIdle();

	void ProcessRequest(const  RequestData& reqData);

	void ProcessLogonRequest(const  RequestData& reqData );
	void ProcessAccountRequest(const  RequestData& reqData );
	void ProcessTradeAccountRequest(const  RequestData& reqData );
	void ProcessTraderRequest(const  RequestData& reqData );
	void ProcessAssetMgmtOrgRequest(const  RequestData& reqData );
	void ProcessFinancialProductRequest(const  RequestData& reqData );
	void ProcessRiskIndModuleRequest(const  RequestData& reqData );
	void ProcessRiskWarningRequest(const  RequestData& reqData );
	void ProcessRiskIndicatorRequest(const  RequestData& reqData );
	void ProcessEvtMsgTemplateRequest(const  RequestData& reqData );
	void ProcessRiskEventRequest(const  RequestData& reqData );
	void ProcessRiskEvtHandlingRequest(const  RequestData& reqData );
	void ProcessPermissionRequest(const  RequestData& reqData );
	void ProcessMessageRequest(const  RequestData& reqData );
	//����������Ϣ
	void ProcessSubscribedData( const Risk2ClientData& reqData );
	//��������
	void ProcessDepositRequest(const RequestData& reqData );
	//����ɽ�
	void ProcessTradeRequest(const RequestData& reqData );
	//������
	void ProcessOrderRequest( const RequestData& reqData );
	//�����ʽ�
	void ProcessFundRequest( const RequestData& reqData );
	//����ֲ�
	void ProcessPositionRequest( const RequestData& reqData );
	//��������
	void ProcessQuotRequest( const RequestData& reqData );
	//�����Լ��Ϣ
	void ProcessInstrumentRequest( const RequestData& reqData );
	//����ǿƽ
	void ProcessForceCloseRequest( const RequestData& reqData );
	//ͳ����Ϣ
	void ProcessStatisticsRequest( const RequestData& reqData );
	//������Ϣ
	void ProcessGatherRequest( const RequestData& reqData );
	void ProcessGatherAccountVec(int nTypeID, int nID, std::vector<TrustTradeAccount>& vec, SGather& sGather);
	

	void SendMessage( std::vector<MsgSendStatus> &vForeAccount, MessageInfo msgInfo );

	void RspResultData( SOCKET hSocket, int nOperateRet, int nCmdID, 
		void* pData, int nLen, int nSeq = 0, 
		int nCurPkgIndex = 0, int nTotalPkg = 1 );

	void OutputUnkownReqLog(const RequestData& reqData);

	bool GetOnlineStatus(int nAccountID);
	bool IsLogon(SOCKET hSocket);
	void ForceCloseClient(int nAccountID);

	void DeleteExpiredWaitForSendAccounts();

	void SendEventMessage( MessageInfo& msgInfo, std::vector<TargetAccount>& vec);

protected:
	//�ְ�����ģ�庯������Ϊ�����������⣬����Ϊ��ͷ�ļ���
	template<typename T>
	void RspResultDataAutoDividePkg( SOCKET hSocket, int nOperateRet, int nCmdID, 
		std::vector<T>& vData, int nSeq = 0 )
	{
		if ( vData.empty())
		{
			RspResultData( hSocket, nOperateRet, nCmdID, NULL, 0, nSeq);
		}
		else
		{
			int pkgmaxrecord=(MAX_IOBUFSIZE-500)/sizeof(T);
			int nTotalPkg = vData.size() / pkgmaxrecord;
			if ( vData.size() % pkgmaxrecord != 0 )
			{
				nTotalPkg += 1;
			}

			for ( int i = 0; i < nTotalPkg; i++ )
			{
				int nRecordNum = pkgmaxrecord;
				if ( i == nTotalPkg - 1 )
				{
					nRecordNum = vData.size() - pkgmaxrecord * i;
				}

				RspResultData( hSocket, nOperateRet, nCmdID, 
					&vData[i*pkgmaxrecord], sizeof(T)*nRecordNum, nSeq, i, nTotalPkg);
			}

		}
	}

private:
	bool GetLowerAssetOrg(int nCurOrgID, std::vector<AssetMgmtOrganization>& vOut);

	void FilterAssetOrg( int nCurOrgID, std::vector<AssetMgmtOrganization>& vAssetOrg, 
		std::vector<AssetMgmtOrganization>& vOut);

	void CalcLowerData(std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetOrgTree,
		int nOwnerID, std::vector<AssetMgmtOrganization>& vOutData);

private:
	CWriteLog* m_pWriteLog;
	CToolThread*	m_hWorkerThread;

	int m_backSysTime;
	bool m_bFirstIdle;
	std::vector<StatisticsEntry> m_StatVec;
	std::vector<TargetAccount>   m_TargetAccountVec;
	CThreadSharedData* m_pThreadSharedData;
};
