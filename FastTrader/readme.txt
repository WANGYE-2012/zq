2014.11.20
	1. ��Module-Misc2������RuntimeConfigMgr.h/cpp����һЩ����ʱ��������Ϣ
	2. ���ӵ���������ӿڣ�
		CMDID_UI_ColorScheme_QUERY: ��ѯUI��ɫ����������Ϊ��
		CMDID_UI_ColorScheme_RSP: ����UI��ɫ�������ɹ�ʱlpDataָ��һ��Int, 0:ǳ��ɫ������1:���ɫ����; cbData=4

2014.11.12
	1. �ײ�����ѯ�۶��ġ��˶���֪ͨ
		(1)����ѯ��
			///����ѯ��֪ͨ��OptionInstrumentIDsΪ������ȫ����Ȩ��Լ��ѯ��֪ͨ
			int  SubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs);
		(2)�˶�ѯ��
			///�˶�ѯ��֪ͨ��OptionInstrumentIDsΪ�����˶�ȫ����Լ��ѯ��֪ͨ
			int UnSubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs);
		(3)QuotSpi������Ӧ����
			#ifdef CTP060300
				///����ѯ��Ӧ��
				void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
				///�˶�ѯ��Ӧ��
				void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
				///ѯ��֪ͨ
				void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);
			#endif
		(4)Ӧ�𷵻ص�BID�����ݽṹ��
			����ѯ��Ӧ��	: 	BID_RspSubForQuoteRsp, 	DataRspSubForQuoteRsp
			�˶�ѯ��Ӧ��	:	BID_RspUnSubForQuoteRsp,DataRspUnSubForQuoteRsp
			ѯ��֪ͨ		:	BID_RtnForQuoteRsp,		DataRtnForQuoteRsp
	2. ����������
		(1)��platform.xml������������ReceiveForQuote����ʾ�Ƿ�֧�ֽ���ѯ��֪ͨ�Ĺ��ܣ�Ĭ�ϲ�֧��
			<platform>
				<Option>
					<ReceiveForQuote>1</ReceiveForQuote>
				</Option>
			</platform>
		(2)��GlobalConfigManager::m_CanReceiveForQuote��ȡ�����ò���
		

2014.11.6
	1. ����޸ģ�
	   (1)��������У�ִ�С���˲���˳�ʱ��ȡ����˹��ܡ�ʱ����˲�����ڴ��������m_hSendThreadHandle������Ϊm_hOrderVerifyPlugin
	   (2)�������xml�У�������������������һ������Ķ��ʵ�����̣��Լ�����򿪲��ʱ����ǰ��ԼID��Ϊ�����в������룺
			//�Ƿ���Դ򿪶��ʵ����1���ԣ�0�����ԣ�Ĭ��Ϊ0
			<MultiInstance>0</MultiInstance>	
			//����ʱ�Ƿ���Ҫ����ǰ��ԼID��Ϊ������1��Ҫ��0����Ҫ��Ĭ��Ϊ0��
			//����Ҫ����򿪷�ʽΪ�� PluginApp.exe TopWndOfFastTrader CurrentInstrumentID=***
			<NeedCurrentInstrumentID>0</NeedCurrentInstrumentID> 	
	2. �ײ����ӽӿڣ�
			//�ж��Ƿ�����Ȩ������ڻ���Լ
			bool IsUnderlyingInstrumentID(const string& InstrumentID)
	3. FastTrader��RunModeΪ2/3ʱ������Ȩ���Բ���������⴦��ֻ���ڵ�ǰ��Լ����Ȩ������Լʱ���������ڱ��۱��ڵ��Ҽ��˵������˫������Ȩ���Դ���		

2014.11.5
	1. CTools_Ansi����GetFirstAlphasOfChineseStrGBK(),ȡ���ִ���ÿ�����ֵ�����ĸ��ɵĴ������ֱ�����GB2312����
	2. PlatformStru_ProductInfo����ProductShortName����ʾ��Ʒ��ơ�ȱʡ����ProductName������ProductName�������������productshortname.xml����ж���
	3. �ײ��ѯ��Լ������ͨ����Լ�����Ʒ��ɾ��û�к�Լ�Ĳ�Ʒ(ʵ���﷢������������������й��ڵĲ�Ʒ)
    4. ���ӵײ�ӿڣ���ȡȫ����������Ʒ����Ϣ 
		void GetExchangeID_ProductInfos(vector<pair<string,vector<PlatformStru_ProductInfo>>>& outData);
	5. ���ӵ���������ӿڣ���ȡȫ��Ʒ����Ϣ
		CMDID_PRODUCT_ALLINFO_QUERY����ѯȫ��Ʒ�����顣����Ϊ��
		CMDID_PRODUCT_ALLINFO_RSP  ����ѯȫ��Ʒ��������Ӧ���ɹ�ʱlpDataָ��PlatformStru_ProductInfo���飬cbData=n*sizeof(PlatformStru_ProductInfo)
	6. ���ӵ���������ӿ�
		֪ͨ���������ɫ������֪ͨ���������ɫ������lpDataָ��һ��Int, 0:ǳ��ɫ������1:���ɫ����

2014.10.24
	�ײ����Ӷ�ѯ�ۺ�ִ�еĴ���
	1. �ײ��ṩ���½ӿڣ�
		///����ѯ��¼��
		virtual int ReqForQuoteInsert(PlatformStru_InputForQuoteField& InputForQuote, int nRequestID)=0;
		///ִ������¼������
		virtual int ReqExecOrderInsert(PlatformStru_InputExecOrderField& InputExecOrder, int nRequestID)=0;
		///ִ�������������
		virtual int ReqExecOrderAction(PlatformStru_InputExecOrderActionField& InputExecOrderAction, int nRequestID)=0;
		///�����ѯִ������
		virtual int ReqQryExecOrder(PlatformStru_QryExecOrderField& QryExecOrder, int nRequestID)=0;
	2. �ײ㴦�����»�ִ�ͻر���
		///ѯ��¼��������Ӧ
		void OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///ִ������¼��������Ӧ
		void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///ִ���������������Ӧ
		void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///�����ѯִ��������Ӧ
		void OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///ѯ��¼�����ر�
		void OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo);
		///ִ������¼�����ر�
		void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo);
		///ִ�������������ر�
		void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo);
		///ִ������֪ͨ
		void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder);
	3. �ײ��ִ�ͻر����ص�BID�����ݽṹ��
		ѯ��¼��������Ӧ		: 	BID_RspForQuoteInsert, 		DataRspForQuoteInsert
		ִ������¼��������Ӧ	:	BID_RspExecOrderInsert,		DataRspExecOrderInsert
		ִ���������������Ӧ	:	BID_RspExecOrderAction,		DataRspExecOrderAction
		�����ѯִ��������Ӧ	:	BID_RspQryExecOrder,		DataRspQryExecOrder
		ѯ��¼�����ر�		:	BID_ErrRtnForQuoteInsert,	DataErrRtnForQuoteInsert
		ִ������¼�����ر�	:	BID_ErrRtnExecOrderInsert,	DataErrRtnExecOrderInsert
		ִ�������������ر�	:	BID_ErrRtnExecOrderAction,	DataErrRtnExecOrderAction
		ִ������֪ͨ			:	BID_RtnExecOrder,			DataRtnExecOrder
	

2014.9.29
	1. ������Ȩ��Լ�������ᱣ֤��ļ���
	2. ������Ȩ��Լ�ֲ���ϸ�ı�֤����㡣��Ȩ�ֲ���ϸ�ݲ���������Ż�
	3. ������Ȩ��Լ�ֲֵı�֤����㡣��Ȩ�ֲ���ϸ�ݲ���������Ż�
	4. ��Լ�б��У���Ȩ�������ʺͽ��׳ɱ���ѯ����ʱ�����º�Լ��Ϣ����ʾ

2014.9.23
	1. ��ѯ������������Ӧ��ԭ����m_pDataMgr->OnRspQryInstrumentCommissionRate()���ж���Ʒ�������ѻ��Ǻ�Լ�����ѣ��ָ�Ϊ��DataCenter���ж�
	2. ExtListCtrl������RefreshHeader()��������Լ�б��У����޴�ֱ�������л����д�ֱ�������Ĵ��ڣ����ˮƽ�����������ұߣ����ڱ�ͷδˢ�µ����⡣ǿ��ˢ��һ�±�ͷ
	3. �ں�Լ�б�����ʾ��Ȩ����Ȩ�������ʺͱ�֤�����²��ּ���С��֤��

2014.9.22
	1. ��xml����ͳһ��ʽ���浱�ս��ײ�����Ʒ�֡���Լ���������ʡ���֤���ʵ���Ϣ
		1) �ֱ𱣴�Ϊtradingparam.xml��product.xml��instrument.xml��commissionrate.xml��marginrate.xml
		1) ��DataCenter.cpp��BID_RspUserLoginʱ��ȡ�����õ��ײ����ݼ���
		2) ���յ����һ����Լ��Ϣ�����һ��Ʒ����Ϣʱ�������Լ��Ϣ��Ʒ����Ϣ
		3) ���յ����͹�˾���ײ���ʱ������֮
		4����DataCenter����ʱ���������
	2. ԭ��UserLoginCore��PreLoginQuery()�м��ر���Ʒ�ֺͺ�Լ��Ϣɾ������Ϊ��StartLoginQuery()�жϵ�ǰ�ײ��Ƿ���Ʒ�ֺͺ�Լ��Ϣ��û�����ѯ
	3. UserLoginCore�в��ټ���LoadCommissionRateAndMarginRate
	4. UserLoginDlg::QryInstrumentListCallBackFunc()�в��ٱ��浱�պ�Լ��Ϣ

2014.9.19
	1. ������ӿ�PlusinAPI��Debug����Ŀ��Ŀ¼�ŵ�debug/Plugin��
	2. �޸Ļ���/���ص��պ�Լ��Ϣʱdouble����Ĵ���
	3. QryQueue��ѯ���ʵ�˳�������
		1) �������Ѻͱ�֤��ֻ��һ�Ρ��鲻��������
		2) ����InstrumentIDΪ�ղ��������ʡ���֤���ʡ���Ȩ�������ʡ���Ȩ���׳ɱ�����ȡ���ֲֺ�Լ�����ֵ
		3) ��m_bStartQryRate��ʾ�Ƿ��Ѿ���ʼ��ѯ����
		4) �տ�ʼ��ѯ����ʱ������Լ��˳��ŵ�m_InstrumentIDsNeedQry�����С�˳��Ϊ����Ȩ������Լ(RunMode3)/��Ȩ��Լ(3)/�ڻ���Լ(123)/��Ȩ��Լ(12)
		5) ���ĳ����Լ��ѯ�޷��ʣ���ͬƷ�ֵ�������Լ�Ƶ�m_InstrumentIDsNeedQry2���к��棬����ٲ�

2014.9.17
	1. ����ǰ��������ϢtagServerParamCfg��current����ΪCurSvrGrpID����ʾ��ǰ�ķ�������ID
	2. ��¼�ɹ��󣬼��tradingparam.xml�е�tradingday��������ǵ�ǰ�����գ���ɾ�������ļ�
	3. ��ѯ������Ϣȷ�Ͻ����Ӧ�󣬼��tradingparam.xml���Ƿ񻺴���BrokerTradingParams�����û�У�������ѯ

2014.9.17
	1. �޸�CPlatformDataMgr_LoginInfo������ctp��������¼��Ϣ������PlatformStru_LoginInfo�ṹ��
	2. ÿ���״�����ʱ��ѯ���ײ���ReqQryBrokerTradingParams������ȡ��OptionRoyaltyPriceType��������ȨȨ������㡣
	   �鵽�󱣴浽tradingparam.xml�У������Ժ�ֱ�Ӵ��ļ��ж�ȡ���ڲ�ѯ������Ϣȷ����Ӧ�󣬽���ѯָ��ŵ���ѯ������ȥ
	   ����RunMode==2/3�²�ѯ
	3. �����������ݽṹ����ִ�������ѣ���Ȩ��Լ����ReqQryOptionInstrCommRate�õ���������, ��չPlatformStru_InstrumentCommissionRate���������ִ��������
	4. ��ReqQryOptionInstrTradeCost()��ȡͶ������Ȩ��Լ��֤�𲻱䲿�ֺ���С��֤����չPlatformStru_InstrumentMarginRate��������Ȩ���׳ɱ���Ϣ
	5. �ײ��ѯ����ʱ����QryQueue��QryID_ReqQryInstrumentCommissionRateʱ���ڻ����ڻ����ʣ���Ȩ����Ȩ���ʣ���QryID_ReqQryInstrumentMarginRateʱ���ڻ��鱣֤���ʣ���Ȩ�齻�׳ɱ�
	6. �޸ĵ���������ӿ�
        //CMDID_PRODUCT_LIST_QUERY                      : ��ѯָ����������Ʒ���б�����ΪExchangeID���ɹ�ʱ����n*32�ַ������顣��һ���ַ�����ExchangeID�������ǽ��(n-1)��ProductID
        //CMDID_OPTIONUNDERLYINGPRODUCT_LIST_QUERY      : ��ѯָ������������Ȩ�����Ʒ���б�����ΪExchangeID���ɹ�ʱ����n*32�ַ������顣��һ���ַ�����ExchangeID�������ǽ��(n-1)��ProductID
        //CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_QUERY: ��ѯָ�������Ʒ��Լ��Ӧ����Ȩ��Լ������ΪUnderlyingInstrumentID���ɹ�ʱ����n*32�ַ������顣��һ���ַ�����UnderlyingInstrumentID�������ǽ��(n-1)����ȨInstrumentID
        //CMDID_INSTRUMENT_LIST_BYPRODUCTID_QUERY       : ��ѯָ��Ʒ�ֶ�Ӧ�ĺ�Լ������ΪProductID���ɹ�ʱ����n*32�ַ������顣��һ���ַ�����ProductID�������ǽ��(n-1)����ȨInstrumentID

2014.9.15		
	1.  CPlatformDataMgr::m_baseFTID/m_baseUpdateSeq����static����
		1) Ϊ�˼��ݶ��˺Ű汾���Ա����ڶ��˺���ʹ��ͳһ��FTID��UpdateSeq, �������ڲ�ͬ�˺ŵ�FTID��ͬ����UI�����쳣
		2) ÿ����¼���ݵ�FTID�Ĳ���ģ���˶��˺���ʹ��ͬһ��baseFTID��������long�����õ����⡣
		3) UpdateSeq�ڶ��˺�����ܳ���long�����õ����⣬��7fffffff������0������UpdateSeqֻ�����Ƿ���ȵ��жϣ������д���С�ڵ��жϣ�����۷�Ϊ0�������Ӱ��ϵͳ����
	2.  ��¼�̴߳�����Ϣ��threadid.log��
	3.  �汾�Ÿ��µ�1.7.2.28
	4.  �ײ����ӽӿڣ�
		//��ȡ���б�����ڻ���Լ
		virtual int GetAllOptionUnderlyingInstrumentIDs(vector<string>& outUnderlyingInstrumentIDs)=0;
		virtual int GetAllOptionUnderlyingInstrumentIDs(set<string>& outUnderlyingInstrumentIDs)=0;
		virtual bool IsCommissionRateExist(const string& InstrumentID)=0;
		virtual bool IsMarginRateExist(const string& InstrumentID)=0;
		
2014.9.12
	1. CQryQueue�ľ�̬����m_bInQry��ΪCPlatFormService�����Ա��������TradeSpi�п��Է��ʡ���̬�����ڶ��˺Ű汾�£����ڶ���ײ�ʵ��ͬʱ���ʣ�����ֳ�ͻ��ʧȥ����
	2. �Ż�CQryQueue�е�SendQryCmd()�����߼����ύЧ�ʡ�����ڶ��˺�(10���˺ţ��ܹ�700��ֲ���ϸ)�����¿���������
	3. CDataCenter()���������ȵ���Stop()��������˺��£�δ��¼��ֱ���˳������쳣������

2014.9.5
	1. �޸�wxEVT_GRID_INSTRUMENTID_CHANGED��ش���GetInt()ΪEvtParamID��GetExtraLong()Ϊ������GID
	2. ʹT�ͱ��۴����ܴ����۱�ǰ��Լ����¼�
	3. OptionTPriceWrapper�ڸ��ĵ�ǰ��Ȩ��Լ�����Ͻ���Լ�����һ���������͸�TPriceList
	4. �޸�FastTraderSetup-simu-option.iss
		1) [Files]ɾ���ɵ�OptionPriceListT���
		2��[Files]�ų�commonfiles·���µ�layout��Ŀ¼�Լ�layout.xml 
		3) source\FastTrader-simu-option·���¼���ר��layout.xml
	5. ����1.7.2.27��Ȩ�����նˣ����ڲ���T�ͱ���ģ��

2014.9.4
	1. �޸ı���/���ص����Լ��Ϣ��ģ�飬��߼����ٶ�

2014.9.3
	1. ���۱�ı䵱ǰ��Լ����T�ͱ��۴��ڷ�����Ϣ
	2. �ײ�����GetOptionInstrumentIDsByUnderlyingInstrumentID()�����ݱ�����ڻ���Լ��ȡ��Ӧ����Ȩ��Լ
	3. CPlatformDataMgr_Instruments::m_OptionProductIDsByExchangeID����Ϊm_OptionUnderlyingProductIDsByExchangeID
	4. ����������ӿ�����CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_QUERY/RSP������
		#define CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_QUERY	0x3014	//��ѯ������ڻ���Լ��Ӧ����Ȩ��Լ�б�����ΪUnderlying InstrumentID
		#define CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_RSP	0x3015	//��ѯ������ڻ���Լ��Ӧ����Ȩ��Լ�б���Ӧ���ɹ�ʱlpDataָ���ַ������飬ÿ���ַ���64�ֽڣ���һ��Option InstrumentID; cbData����Ʒ������*64
	5. ������Ȩ�ն�(RunMode==2/3)������µ����л��˵����Լ����׷���л��˵������ӡ���ɫ����/ǳɫ�������˵�
	6. ������Ȩ�նˣ��ڡ�������-�����ڡ�����ʾ���й������ڻ���Ȩ�����նˡ�

2014.8.29
	1. ������ȨT�ͱ���ģ��
		1) ����GID_OptionTPrice=8138����־T�ͱ���ģ��
		2) ��TPriceList��Ŀ���뵽FastTrader��sln��
		3) ����TPriceList�ĸ�����COptionTPriceWrapper�������洴��TPriceList����

2014.8.21
	1. ԭ�ײ��ṩ��GetProductID()�ӿڸ���ΪGetProductID_InstrumentIDsByExchangeID()
	2. �޸ĵײ��ṩ��GetProductList()�ӿڣ�ʹ��vector����set
	3. �ײ�����GetOptionProductIDsByExchangeID()�ӿڣ����ڻ�ȡָ������������Ȩ������ڻ�Ʒ��
	4. ����������ӿ�����CMDID_OPTIONUNDERLYINGPRODUCT_LIST_QUERY/RSP������
		#define CMDID_OPTIONUNDERLYINGPRODUCT_LIST_QUERY	0x3012	//��ѯ�ض�����������Ȩ������ڻ�Ʒ���б�����ΪExchangeID
		#define CMDID_OPTIONUNDERLYINGPRODUCT_LIST_RSP	    0x3013	//��ѯ�ض�����������Ȩ������ڻ�Ʒ���б���Ӧ���ɹ�ʱlpDataָ���ַ������飬ÿ���ַ���64�ֽڣ���һ��ProductID; cbData����Ʒ������*64
	5. CPlatformDataMgr_Instruments���Ա����m_allProductID����Ϊm_allInstrumentIDsByProductID

2014.8.20
�ύ���ݣ�
	1. ctp API������6.3.0API��֧����Ȩ(������Ȩ�������ݲ��ܷ�����׼��)
	2. platform.xml������RunMode�ڣ���������ģʽ
	3. �ײ㣺����Ʒ����Ϣ����
		����ʱ��ѯƷ����Ϣ
		����PlatformDataMgr_Instrumnetsģ���й���Ʒ����Ϣ
		�ṩGetProductList()��GetProductInfo()��GetProductInfo2()�ӿڹ�UI���ȡƷ���б��Ʒ����Ϣ
	4. ����������ӿڣ�����CMDID_PRODUCT_LIST_QUERY/RSP��CMDID_PRODUCT_INFO_QUERY/RSP���������ڻ�ȡƷ���б��Ʒ����Ϣ
	5. ���ڱ����Լ��Ϣ�ļ���Instruments.txt��ΪInstruments.xml
	6. ���ڱ���Ʒ����Ϣ��Products.xml��
	7. �汾�Ÿ�Ϊ1.7.2.26


2014.8.15
	1. platformSvr��Ŀ������Ŀ���������Ӻ궨��CTP060300����ʾ�Ƿ�֧��ctp ver 6.3.0(option)����Ϊctp api6.3.0for option�������Ȩ�Ľṹ������ctp6.2.5��û�С�
	2. ctp api 6.3.0 for option ��6.2.5������
		1)ThostFtdcMdApi.h
		`	���Ӷ���/�˶�ѯ�۵ĺ�����
			����ѯ�ۣ�					virtual int SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) = 0;
			�˶�ѯ�ۣ�					virtual int UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) = 0;
			����ѯ��Ӧ��				virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			�˶�ѯ��Ӧ��				virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			ѯ��֪ͨ��					virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {};

		2)ThostFtdcTraderApi.h
			ִ������¼������			virtual int ReqExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, int nRequestID) = 0;
			ִ�������������			virtual int ReqExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, int nRequestID) = 0;
			ѯ��¼������				virtual int ReqForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, int nRequestID) = 0;
			����¼������				virtual int ReqQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, int nRequestID) = 0;
			���۲�������				virtual int ReqQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, int nRequestID) = 0;

			�����ѯ��Ȩ���׳ɱ���		virtual int ReqQryOptionInstrTradeCost(CThostFtdcQryOptionInstrTradeCostField *pQryOptionInstrTradeCost, int nRequestID) = 0;
			�����ѯ��Ȩ��Լ�����ѣ�		virtual int ReqQryOptionInstrCommRate(CThostFtdcQryOptionInstrCommRateField *pQryOptionInstrCommRate, int nRequestID) = 0;
			�����ѯִ�����棺			virtual int ReqQryExecOrder(CThostFtdcQryExecOrderField *pQryExecOrder, int nRequestID) = 0;
			�����ѯѯ�ۣ�				virtual int ReqQryForQuote(CThostFtdcQryForQuoteField *pQryForQuote, int nRequestID) = 0;
			�����ѯ���ۣ�				virtual int ReqQryQuote(CThostFtdcQryQuoteField *pQryQuote, int nRequestID) = 0;
		
			ִ������¼��������Ӧ��		virtual void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			ִ���������������Ӧ��		virtual void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			ѯ��¼��������Ӧ��			virtual void OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			����¼��������Ӧ��			virtual void OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			���۲���������Ӧ��			virtual void OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
		
			�����ѯ��Ȩ���׳ɱ���Ӧ��	virtual void OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			�����ѯ��Ȩ��Լ��������Ӧ��	virtual void OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			�����ѯִ��������Ӧ��		virtual void OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			�����ѯѯ����Ӧ��			virtual void OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			�����ѯ������Ӧ��			virtual void OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
		
			ִ������֪ͨ��				virtual void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) {};
			ִ������¼�����ر���		virtual void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo) {};
			ִ�������������ر���		virtual void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo) {};
			ѯ��¼�����ر���			virtual void OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo) {};
			����֪ͨ��					virtual void OnRtnQuote(CThostFtdcQuoteField *pQuote) {};
			����¼�����ر���			virtual void OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo) {};
			���۲�������ر���			virtual void OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo) {};
			ѯ��֪ͨ��					virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {};

		3)ThostFtdcUserApiDataType.h
			TFtdcProductClassType(��Ʒ��������)��						#define THOST_FTDC_PC_SpotOption '6'				//�ֻ���Ȩ
			TFtdcUnderlyingMultipleType(������Ʒ��������)��				typedef double TThostFtdcUnderlyingMultipleType;	
			TFtdcExchangeSettlementParamIDType(���������������������)��	#define THOST_FTDC_ESPI_OptOutDisCountRate 'a'		///��ֵ��Ȩ��֤���Żݱ���
																		#define THOST_FTDC_ESPI_OptMiniGuarantee 'b'		///��ͱ���ϵ��
			TFtdcFileIDType(�ļ���ʶ����)��								#define THOST_FTDC_FI_OptionStrike 'S'				///��Ȩִ���ļ�
																		#define THOST_FTDC_FI_SettlementPriceComparison 'M'	///����۱ȶ��ļ�
																		#define THOST_FTDC_FI_NonTradePosChange 'B'			///�������ǳֱֲ䶯��ϸ
			TFtdcCSRCOptionsTypeType(��Ȩ��������)��						typedef char TThostFtdcCSRCOptionsTypeType[2];
			TFtdcCSRCStrikePriceType(ִ�м�����)��						typedef double TThostFtdcCSRCStrikePriceType;
			TFtdcCSRCTargetProductIDType(���Ʒ������)��					typedef char TThostFtdcCSRCTargetProductIDType[3];
			TFtdcCSRCTargetInstrIDType(��ĺ�Լ����)��					typedef char TThostFtdcCSRCTargetInstrIDType[31];
					
			ɾ��TFtdcCombineTypeType(�����������)

			TFtdcCusAccountTypeType(�����˻���������)��			ɾ��	THOST_FTDC_CAT_Assetmgr							///�ʹܽ����˻�
																����	#define THOST_FTDC_CAT_AssetmgrFuture '2'		///���ڻ��ʹ�ҵ���µ��ʹܽ����˻�
																		#define THOST_FTDC_CAT_AssetmgrTrustee '3'		///�ۺ����ʹ�ҵ���µ��ڻ��ʹ��й��˻�
																		#define THOST_FTDC_CAT_AssetmgrTransfer '4'		///�ۺ����ʹ�ҵ���µ��ʽ���ת�˻�

			TFtdcDirectionEnType(������������)��							typedef char TThostFtdcDirectionEnType;
			TFtdcOffsetFlagEnType(��ƽ��־����)��						typedef char TThostFtdcOffsetFlagEnType;
			TFtdcHedgeFlagEnType(Ͷ���ױ���־����)��						typedef char TThostFtdcHedgeFlagEnType;
			TFtdcFundIOTypeEnType(�������������)��						typedef char TThostFtdcFundIOTypeEnType;
			TFtdcFundTypeEnType(�ʽ���������)��							typedef char TThostFtdcFundTypeEnType;
			TFtdcFundDirectionEnType(�����������)��					typedef char TThostFtdcFundDirectionEnType;
			TFtdcFundMortDirectionEnType(������Ѻ��������)��				typedef char TThostFtdcFundMortDirectionEnType;
			TFtdcSwapBusinessTypeType(����ҵ����������)��				typedef char TThostFtdcSwapBusinessTypeType[3];
			TFtdcOptionsTypeType(��Ȩ��������)��							typedef char TThostFtdcOptionsTypeType;
			TFtdcStrikeModeType(ִ�з�ʽ����)��							typedef char TThostFtdcStrikeModeType;
			TFtdcStrikeTypeType(ִ����������)��							typedef char TThostFtdcStrikeTypeType;
			TFtdcApplyTypeType(�н�����Ȩ����ִ��������������)��			typedef char TThostFtdcApplyTypeType;
			TFtdcGiveUpDataSourceType(����ִ������������Դ����)��			typedef char TThostFtdcGiveUpDataSourceType;
			TFtdcExecOrderSysIDType(ִ������ϵͳ�������)��				typedef char TThostFtdcExecOrderSysIDType[21];
			TFtdcExecResultType(ִ�н������)��							typedef char TThostFtdcExecResultType;
			TFtdcStrikeSequenceType(ִ���������)��						typedef int TThostFtdcStrikeSequenceType;
			TFtdcStrikeTimeType(ִ��ʱ������)��							typedef char TThostFtdcStrikeTimeType[13];
			TFtdcCombinationTypeType(�����������)��						typedef char TThostFtdcCombinationTypeType;
			TFtdcOptionRoyaltyPriceTypeType(��ȨȨ����۸���������)��		typedef char TThostFtdcOptionRoyaltyPriceTypeType;
			TFtdcBalanceAlgorithmType(Ȩ���㷨����)��					typedef char TThostFtdcBalanceAlgorithmType;
			TFtdcActionTypeType(ִ����������)��							typedef char TThostFtdcActionTypeType;
			TFtdcForQuoteStatusType(ѯ��״̬����)��						typedef char TThostFtdcForQuoteStatusType;
			TFtdcQuotStatusType(����״̬����)��							typedef char TThostFtdcQuotStatusType;
			TFtdcValueMethodType(ȡֵ��ʽ����)��							typedef char TThostFtdcValueMethodType;
			TFtdcExecOrderPositionFlagType(��Ȩ��Ȩ���Ƿ����ڻ�ͷ��ı������)��	typedef char TThostFtdcExecOrderPositionFlagType;
			TFtdcExecOrderCloseFlagType(��Ȩ��Ȩ�����ɵ�ͷ���Ƿ��Զ�ƽ������)��	typedef char TThostFtdcExecOrderCloseFlagType;
			TFtdcProductTypeType(��Ʒ��������)��							typedef char TThostFtdcProductTypeType;
			TFtdcCZCEUploadFileNameType(֣���������ļ�������)��			typedef char TThostFtdcCZCEUploadFileNameType;
			TFtdcDCEUploadFileNameType(�����������ļ�������)��			typedef char TThostFtdcDCEUploadFileNameType;
			TFtdcSHFEUploadFileNameType(�����������ļ�������)��			typedef char TThostFtdcSHFEUploadFileNameType;
			TFtdcCFFEXUploadFileNameType(�н��������ļ�������)��			typedef char TThostFtdcCFFEXUploadFileNameType;

			
		4)ThostFtdcUserApiStruct.h
			struct CThostFtdcProductField					(��Ʒ)��					����	TThostFtdcInstrumentIDType	ExchangeProductID;			///��������Ʒ����
																							TThostFtdcUnderlyingMultipleType	UnderlyingMultiple;	///��Լ������Ʒ����
			struct CThostFtdcInstrumentField				(��Լ)��					����	TThostFtdcInstrumentIDType	UnderlyingInstrID;			///������Ʒ����
																							TThostFtdcPriceType	StrikePrice;						///ִ�м�
																							TThostFtdcOptionsTypeType	OptionsType;				///��Ȩ����
																							TThostFtdcUnderlyingMultipleType	UnderlyingMultiple;	///��Լ������Ʒ����
																							TThostFtdcCombinationTypeType	CombinationType;		///�������
			struct CThostFtdcTradingAccountField			(�ʽ��˻�)��				����	TThostFtdcMoneyType	OptionCloseProfit;					///��Ȩƽ��ӯ��
																							TThostFtdcMoneyType	OptionValue;						///��Ȩ��ֵ
			struct CThostFtdcInvestorPositionField			(Ͷ���ֲ߳�)��			����	TThostFtdcVolumeType	StrikeFrozen;					///ִ�ж���
																							TThostFtdcMoneyType	StrikeFrozenAmount;					///ִ�ж�����
																							TThostFtdcVolumeType	AbandonFrozen;					///����ִ�ж���
																							TThostFtdcMoneyType	OptionValue;						///��Ȩ��ֵ
			struct CThostFtdcSyncingTradingAccountField		(����ͬ���еĽ����˺�)��	����	TThostFtdcMoneyType	OptionCloseProfit;					///��Ȩƽ��ӯ��
																							TThostFtdcMoneyType	OptionValue;						///��Ȩ��ֵ
			struct CThostFtdcSyncingInvestorPositionField	(����ͬ���е�Ͷ���ֲ߳�)������	TThostFtdcVolumeType StrikeFrozen;						///ִ�ж���
																							TThostFtdcMoneyType	StrikeFrozenAmount;					///ִ�ж�����
																							TThostFtdcVolumeType AbandonFrozen;						///����ִ�ж���
																							TThostFtdcMoneyType	OptionValue;						///��Ȩ��ֵ
			struct CThostFtdcQryProductField				(��ѯ��Ʒ)��				����	TThostFtdcProductClassType ProductClass;				///��Ʒ����
			struct CThostFtdcBrokerWithdrawAlgorithmField	(���͹�˾�����ʽ��㷨��)������	TThostFtdcBalanceAlgorithmType BalanceAlgorithm;		///Ȩ���㷨
			struct CThostFtdcBrokerTradingParamsField		(���͹�˾���ײ���)��		����	TThostFtdcOptionRoyaltyPriceTypeType OptionRoyaltyPriceType;///��ȨȨ����۸�����
			
			struct CThostFtdcOptionInstrMiniMarginField		(��ǰ��Ȩ��Լ��С��֤��)��		�����ṹ
			struct CThostFtdcOptionInstrMarginAdjustField	(��ǰ��Ȩ��Լ��֤�����ϵ��)��	�����ṹ
			struct CThostFtdcOptionInstrCommRateField		(��ǰ��Ȩ��Լ�����ѵ���ϸ����)��	�����ṹ
			struct CThostFtdcOptionInstrTradeCostField		(��Ȩ���׳ɱ�)��					�����ṹ
			struct CThostFtdcQryOptionInstrTradeCostField	(��Ȩ���׳ɱ���ѯ)��				�����ṹ
			struct CThostFtdcQryOptionInstrCommRateField	(��Ȩ�������ʲ�ѯ)��				�����ṹ
			struct CThostFtdcIndexPriceField				(��ָ�ֻ�ָ��)��					�����ṹ
			struct CThostFtdcInputExecOrderField			(�����ִ������)��				�����ṹ
			struct CThostFtdcInputExecOrderActionField		(����ִ���������)��				�����ṹ
			struct CThostFtdcExecOrderField					(ִ������)��						�����ṹ
			struct CThostFtdcExecOrderActionField			(ִ���������)��					�����ṹ
			struct CThostFtdcQryExecOrderField				(ִ�������ѯ)��					�����ṹ
			struct CThostFtdcExchangeExecOrderField			(������ִ��������Ϣ)��			�����ṹ
			struct CThostFtdcQryExchangeExecOrderField		(������ִ�������ѯ)��			�����ṹ
			struct CThostFtdcQryExecOrderActionField		(ִ�����������ѯ)��				�����ṹ
			struct CThostFtdcExchangeExecOrderActionField	(������ִ���������)��			�����ṹ
			struct CThostFtdcQryExchangeExecOrderActionField(������ִ�����������ѯ)��		�����ṹ
			struct CThostFtdcErrExecOrderField				(����ִ������)��					�����ṹ
			struct CThostFtdcQryErrExecOrderField			(��ѯ����ִ������)��				�����ṹ
			struct CThostFtdcErrExecOrderActionField		(����ִ���������)��				�����ṹ
			struct CThostFtdcQryErrExecOrderActionField		(��ѯ����ִ���������)��			�����ṹ
			struct CThostFtdcOptionInstrTradingRightField	(Ͷ������Ȩ��Լ����Ȩ��)��		�����ṹ
			struct CThostFtdcQryOptionInstrTradingRightField(��ѯ��Ȩ��Լ����Ȩ��)��			�����ṹ
			struct CThostFtdcInputForQuoteField				(�����ѯ��)��					�����ṹ
			struct CThostFtdcForQuoteField					(ѯ��)��							�����ṹ
			struct CThostFtdcQryForQuoteField				(ѯ�۲�ѯ)��						�����ṹ
			struct CThostFtdcExchangeForQuoteField			(������ѯ����Ϣ)��				�����ṹ
			struct CThostFtdcQryExchangeForQuoteField		(������ѯ�۲�ѯ)��				�����ṹ
			struct CThostFtdcInputQuoteField				(����ı���)��					�����ṹ
			struct CThostFtdcInputQuoteActionField			(���뱨�۲���)��					�����ṹ
			struct CThostFtdcQuoteField						(����)��							�����ṹ
			struct CThostFtdcQuoteActionField				(���۲���)��						�����ṹ
			struct CThostFtdcQryQuoteField					(���۲�ѯ)��						�����ṹ
			struct CThostFtdcExchangeQuoteField				(������������Ϣ)��				�����ṹ
			struct CThostFtdcQryExchangeQuoteField			(���������۲�ѯ)��				�����ṹ
			struct CThostFtdcQryQuoteActionField			(���۲�����ѯ)��					�����ṹ
			struct CThostFtdcExchangeQuoteActionField		(���������۲���)��				�����ṹ
			struct CThostFtdcQryExchangeQuoteActionField	(���������۲�����ѯ)��			�����ṹ
			struct CThostFtdcOptionInstrDeltaField			(��Ȩ��Լdeltaֵ)��				�����ṹ
			struct CThostFtdcForQuoteRspField				(���������̵�ѯ������)��			�����ṹ
			struct CThostFtdcStrikeOffsetField				(��ǰ��Ȩ��Լִ��ƫ��ֵ����ϸ����)�������ṹ
			struct CThostFtdcQryStrikeOffsetField			(��Ȩִ��ƫ��ֵ��ѯ)��			�����ṹ














