#pragma once
//�������ͷ�ļ��ж���������˸�ģ�麯���ӿڵĴ�����
typedef int CF_ERROR;

#define  MAKE_ERROR_CODE(_base,_offset) \
	((int)( (int)(_base) << 16 ) + (int)(_offset))

#define CF_ERROR_SUCCESS 0               //�ɹ�
#define CF_ERROR_FAILURE -1              //ʧ��(ͨ��)

#define CF_ERROR_COMMON_BASE				 0x0000   //���ù���
#define CF_ERROR_NET_BASE					 0x0010   //�������
#define CF_ERROR_BUSSINESS_BASE				 0x0020   //ҵ�����
#define CF_ERROR_OPERATION_BASE				 0x0030   //�û������Դ���
#define CF_ERROR_DATABASE_BASE				 0x0040   //���ݿ����
#define CF_ERROR_LOGIN_BASE					 0x0050   //��½����
#define CF_ERROR_INSTRUMENT_BASE			 0x0060   //��Լ����
#define CF_ERROR_RATIO_BASE					 0x0070   //���ʴ���
#define CF_ERROR_SETTLEMENT_BASE			 0x0080   //�������
#define CF_ERROR_INPURORDER_BASE			 0x0090	  //����ִ�д���
#define CF_ERROR_TRADERDATA_BASE			 0x00A0	  //�������ݹ���
#define CF_ERROR_RISK_BASE					 0x00B0	  //���
#define CF_ERROR_OFFER_BASE					 0x00C0	  //����
#define CF_ERROR_DATA_UPDATE_BASE			 0x00D0	  //�������ݸ���
#define CF_ERROR_DATA_QUOT_BASE			     0x00E0	  //����
#define CF_ERROR_DATA_STRATEGY_BASE			 0x00F0	  //����
//��������
#define CF_ERROR_COMMON_INPUT_PARAM    MAKE_ERROR_CODE(CF_ERROR_COMMON_BASE,0x0001)  //�������
#define CF_ERROR_COMMON_MEMEORY	       MAKE_ERROR_CODE(CF_ERROR_COMMON_BASE,0x0002)  //�ڴ����
#define CF_ERROR_COMMON_NO_PRIVILEGE   MAKE_ERROR_CODE(CF_ERROR_COMMON_BASE,0x0003)  //Ȩ�޲���

//�������
#define CF_ERROR_NET_CONNECT MAKE_ERROR_CODE(CF_ERROR_NET_BASE,0x0001)  //�������Ӳ���
#define CF_ERROR_NET_TIMEOUT MAKE_ERROR_CODE(CF_ERROR_NET_BASE,0x0002)  //�������ӳ�ʱ
//��½ģ�������
#define CF_ERROR_LOGIN_PASS_INVALIDATE      MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x0001)   //��½�Ƿ��������
#define CF_ERROR_LOGIN_SOCKET_NOT_EXIST     MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x0002)  //�����ڸ�socket
#define CF_ERROR_LOGIN_USER_NOT_EXIST       MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x0003)  //�����ڸ��û�
#define CF_ERROR_LOGIN_USER_NOT_USE         MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x0004)  //���û�û������
#define CF_ERROR_LOGIN_USER_PASSWORD_ERROR	MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x0005)	 //�û�(����Ա)�޸�����ʱ��ԭ�����������
#define CF_ERROR_LOGIN_USER_DATA_NOTREADY   MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x0006)	 //�û��������ڳ�ʼ�������Ժ��¼
#define CF_ERROR_LOGIN_TADRE_STATUS_INIT    MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x0007)	 //���������ڳ�ʼ�������Ժ��¼
#define CF_ERROR_LOGIN_TADRE_STATUS_SETT    MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x0008)	 //���������ڽ��㣬���Ժ��¼
#define CF_ERROR_LOGIN_TADRE_STATUS_NOSETT  MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x0009)	 //û���ճ����㵥����֪ͨ����Ա���н��㣬���Ժ��¼
#define CF_ERROR_LOGIN_TADRE_STATUS_END     MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x000a)	 //���ս��׽�����
#define CF_ERROR_LOGIN_INVALIDE_USERID		MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x000b)	 //��Ч���û�ID
#define CF_ERROR_LOGIN_USER_UNPASSWORD_ERROR	MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x000c)	 //�û���������ʱ�������������
#define CF_ERROR_LOGIN_TADRE_STATUS_INITFAIL    MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x000d)	 //��������ʼ��ʧ�ܣ��޷���¼
#define CF_ERROR_LOGIN_IP_INVALIDATE			MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x000e)   //���Ǵ�ָ����IP��ַ�����Ĵӷ�������¼
#define CF_ERROR_LOGIN_USERTYPE_INVALIDATE      MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x000f)   //����������û�����
#define CF_ERROR_LOGIN_QUOTSERVER_ERROR         MAKE_ERROR_CODE(CF_ERROR_LOGIN_BASE,0x0010)   //�û�δ��¼��������� 
//���ݿ������
#define CF_ERROR_DATABASE_NOT_CONNECT		MAKE_ERROR_CODE(CF_ERROR_DATABASE_BASE,0x0001)	//���ݿ�δ����
#define CF_ERROR_DATABASE_DUPLICATE_FIELD	MAKE_ERROR_CODE(CF_ERROR_DATABASE_BASE,0x0002)	//ORA-00001 �ֶ��ظ���Υ��Լ������
#define CF_ERROR_DATABASE_RECODE_USED		MAKE_ERROR_CODE(CF_ERROR_DATABASE_BASE,0x0003)	//ORA-02292 ��������������ʹ��
#define CF_ERROR_DATABASE_NO_DEPENDENT		MAKE_ERROR_CODE(CF_ERROR_DATABASE_BASE,0x0004)	//ORA-02291 �޷��ҵ�������
#define CF_ERROR_DATABASE_OTHER_ERROR		MAKE_ERROR_CODE(CF_ERROR_DATABASE_BASE,0x0005)	//��������


//��Լ����
#define CF_ERROR_NO_INSTRUMENTS MAKE_ERROR_CODE(CF_ERROR_INSTRUMENT_BASE,0x0001)  //û�к�Լ


//���ʴ���
#define CF_ERROR_NO_COMMISSION  MAKE_ERROR_CODE(CF_ERROR_RATIO_BASE,0x0001)  //û�з���
#define CF_ERROR_NO_MARGIN      MAKE_ERROR_CODE(CF_ERROR_RATIO_BASE,0x0002)  //û�б�֤����

//�������
#define CF_ERROR_NOT_SETTLEMENTTIME           MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0001)  //δ������ʱ��
#define CF_ERROR_NO_SETTLEMENTPRICE           MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0002)  //û�н����
#define CF_ERROR_NO_TRADEDATA                 MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0003)  //û�гɽ���¼
#define CF_ERROR_REDUPLICATE_TRADE            MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0004)  //���ݿ������д����ɽ���¼
#define CF_ERROR_NO_POSITIONDETAIL            MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0005)  //û�гֲ���ϸ
#define CF_ERROR_NO_POSITION                  MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0006)  //û�гֲּ�¼
#define CF_ERROR_NO_FUND                      MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0007)  //û���ʽ��¼
#define CF_ERROR_REDUPLICATE_POSITIONDETAIL   MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0008)  //���ݿ������д����ֲ���ϸ
#define CF_ERROR_REDUPLICATE_POSITION         MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0009)  //���ݿ������д����ֲ�
#define CF_ERROR_TRADINGDAY_ERROR             MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0010)  //�����մ���ֻ�ܽ�����ʷ����
#define CF_ERROR_NO_LASTSETTLEMENTPRICE       MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0011)  //û����һ�����ս������ݣ��޷����н��ս���
#define CF_ERROR_NO_SETTLEMENTINFO            MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0012)  //û�н��㵥
#define CF_ERROR_ADDTRADE_FAILD               MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0013)  //���ӳɽ�ʧ��
#define CF_ERROR_MANULSETTLEMENT_FAILD        MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0014)  //���������� ����״̬
#define CF_ERROR_NOSERVICEGROUP_FAILD         MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0015)  //û�����ý����������
#define CF_ERROR_TRADEDATASTATUSERROR_FAILD   MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0016)  //���ݹ���ģ��Ϊ�ǽ���״̬���Զ�����ʧ��
#define CF_ERROR_LOADHISTORYCOMMISSION_FAILD  MAKE_ERROR_CODE(CF_ERROR_SETTLEMENT_BASE,0x0017)  //�޷����ش����ڷ�������

//����ִ�д���
#define CF_ERROR_USER_NOT_ENABLE				MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x0000)	//�˻�δ����
#define CF_ERROR_TRADE_FORBIDDEN				MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x0001)	//�޽���Ȩ��
#define CF_ERROR_TRADE_LIMITED					MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x0002)	//��Ʒ�ֻ��Լ�����ƽ���
#define CF_ERROR_TRADE_VERIFY_FAILD				MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x0003)	//���δͨ��
#define CF_ERROR_TRADE_RISK_LIMITED				MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x0004)	//�ú�Լ���µ����򱻷�ط��������µ�
#define CF_ERROR_TRADE_STRATEGY_LIMITED			MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x0005)	//�ú�Լ���µ����򱻽��ײ��������µ�
#define CF_ERROR_TRADE_ALREADY_CANCELED			MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x0006)	//�ѳ���
#define CF_ERROR_TRADE_USER_TRADE_NOTREADY		MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x0007)	//���û��Ĳ�ѯ�������ݻ�û��׼���ã����Ժ�����
#define CF_ERROR_TRADE_HAS_VERIFYED				MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x0008)	//��������
#define CF_ERROR_TRADE_VERIFYING				MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x0009)	//�ȴ����
#define CF_ERROR_TRADE_HAS_RECEVIED				MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x000a)    //�����ѽ���
#define CF_ERROR_TRADE_HAS_SENDCTP              MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x000b)    //������ת����CTP
#define CF_ERROR_TRADE_HAS_RISK_RECEVIED        MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x000c)    //ǿƽ���ѽ���
#define CF_ERROR_TRADE_FORCE_ORDED_NO_ACTION    MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x000d)    //ǿƽ�����ܳ���
#define CF_ERROR_RISK_ALL_INSTRUMENT_ORDER	    MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x000e)    //������� ���к�Լ�����µ�
#define CF_ERROR_RISK_SINGLE_INSTRUMENT_ORDER	MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x000f)    //������� ����Լ�����µ�
#define CF_ERROR_RISK_ALL_INSTRUMENT_OPEN	    MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x0010)    //������� ���к�Լ���ƿ���
#define CF_ERROR_RISK_SINGLE_INSTRUMENT_OPEN	MAKE_ERROR_CODE(CF_ERROR_INPURORDER_BASE,0x0011)    //������� ����Լ���ƿ���
//�������ݹ������
#define CF_ERROR_TRADEDATA_NO_USER_DATA   		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0000)	//û�и��û�������
#define CF_ERROR_TRADEDATA_NO_INSTRUMENTS 		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0001)	//��������û��׼���ã����Ժ��ٴβ�ѯ
#define CF_ERROR_TRADEDATA_NO_COMMISSION 		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0002)	//��������û��׼���ã����Ժ��ٴβ�ѯ
#define CF_ERROR_TRADEDATA_NO_MARGIN     		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0003)	//��������û��׼���ã����Ժ��ٴβ�ѯ
#define CF_ERROR_TRADEDATA_NO_FUND      		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0004)	//��������û��׼���ã����Ժ��ٴβ�ѯ
#define CF_ERROR_TRADEDATA_NO_POSITION     		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0005)	//��������û��׼���ã����Ժ��ٴβ�ѯ
#define CF_ERROR_TRADEDATA_NO_POSDETAIL   		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0006)	//��������û��׼���ã����Ժ��ٴβ�ѯ
#define CF_ERROR_TRADEDATA_NO_COMPOS      		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0007)	//��������û��׼���ã����Ժ��ٴβ�ѯ
#define CF_ERROR_TRADEDATA_NO_COMPOSDETAIL		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0008)	//��������û��׼���ã����Ժ��ٴβ�ѯ
#define CF_ERROR_TRADEDATA_NO_ORDER      		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0009)	//��������û��׼���ã����Ժ��ٴβ�ѯ
#define CF_ERROR_TRADEDATA_NO_TRADE     		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x000A)	//��������û��׼���ã����Ժ��ٴβ�ѯ
#define CF_ERROR_NOINITED                		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x000b)	//״̬û�г�ʼ�����޷����ָ���Ķ���
#define CF_ERROR_TRADEDATA_FUNDINOUT_DENY		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x000c)	//״̬û�г�ʼ�����޷����ָ���Ķ���	
#define CF_ERROR_TRADEDATA_SETTMENT_NOINIT		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x000d)	//�ճ�����û��׼���ã�ֻ������ʷ����	
#define CF_ERROR_TRADEDATA_SETTMENT_NOTRAING	MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x000e)	//�������ݴ���ֻ������ʷ����
#define CF_ERROR_TRADEDATA_TRADE_FROM_OTHER		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x000f)	//�Ǵӱ�Ŀͻ��˲����ĳɽ�
#define CF_ERROR_TRADEDATA_NOSETTLEMENT_PRICE	MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0010)	//����۲�ȫ
#define CF_ERROR_TRADEDATA_NOTRADINGDAY			MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0011)	//û�н����գ����ܽ������н���
#define CF_ERROR_TRADEDATA_TODAYHAD				MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0012)	//�����ѽ�����ˣ�ֻ�ܽ�����ʷ����
#define CF_ERROR_TRADEDATA_EXCHANGE_ISTRADE     MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0013)	//���ڽ��ף����ܽ��н��㶯��
#define CF_ERROR_TRADEDATA_INITING			    MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0014)    //�������ڳ�ʼ���׶Σ����ܽ��иò���
#define CF_ERROR_TRADEDATA_INIT_NORELEATION	    MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0015)    //���û�û�й�������Ա���޷���ʼ��
#define CF_ERROR_TRADEDATA_INIT_ACCOUNT_NOLOGIN MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0016)    //���û������ί�н����˺�û�е�½�����ȵ�½��ί�н����˺�
#define CF_ERROR_TRADEDATA_INIT_ACCOUNT_NOCOMM  MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0017)    //���û������ί�н����˺�û�в�ѯ�����ʣ������˺ŵ�½���쳣
#define CF_ERROR_TRADEDATA_NOTTRADINGDAY		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0018)    //�ǽ����գ����ܽ�����ʷ����
#define CF_ERROR_TRADEDATA_NOTMATCH_STATUS		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0019)    //�����������ڽ����л�����У���֧�ָò���
#define CF_ERROR_TRADEDATA_HAS_POSITION			MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0020)    //���û����гֲ֣�����ɾ����
#define CF_ERROR_TRADEDATA_HAS_INITED			MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0021)    //�ѳ�ʼ���ɹ��������ٳ�ʼ��
#define CF_ERROR_TRADEDATA_FORBID_REINIT		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0022)    //��ǰ������״̬�����������趨
#define CF_ERROR_TRADEDATA_NOT_ENOUGH_FUND      MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0023)	//�ʽ���
#define CF_ERROR_TRADEDATA_ACCOUNT_ISLOGIN      MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0024)	//��ǰί�н����ʺ��ǵ�½״̬������ɾ��
#define CF_ERROR_TRADEDATA_FUNDIN_DENY_NOINIT	MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0025)	//�û�δ��ʼ�������ܽ��г�������
#define CF_ERROR_TRADEDATA_USER_LOGIN			MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0026)	//�û��ѵ�½
#define CF_ERROR_TRADEDATA_FORBID_USERINIT      MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0027)	//��ǰ������״̬���ܳ�ʼ���û��������³�ʼ�����������ٴγ���
#define CF_ERROR_TRADEDATA_FORBID_ACCOUNTLOING  MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0028)	//��ǰ������״̬���ܵ�½ί�н����ʺţ������³�ʼ�����������ٴγ���
#define CF_ERROR_TRADEDATA_FORBID_QRYUSER		MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x0029)	//��ǰ������״̬���ǽ���״̬����ѯ�����û���ʼ��״̬
#define CF_ERROR_TRADEDATA_FORBID_QRYACCOUNT    MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x002a)	//��ǰ������״̬���ǽ���״̬����ѯ����ί�н����ʺŵ�½״̬
#define CF_ERROR_TRADEDATA_WRONG_INSTRUMENT     MAKE_ERROR_CODE(CF_ERROR_TRADERDATA_BASE,0x002b)	//��Լ��������

//�ն��û�����������
#define CF_ERROR_OPERATION_DEL_TRADEACCOUNT      MAKE_ERROR_CODE(CF_ERROR_OPERATION_BASE,0x0001)   //ɾ��ί�н����˺Ŵ���
#define CF_ERROR_OPERATION_DEL_FINANCEPRODUCT    MAKE_ERROR_CODE(CF_ERROR_OPERATION_BASE,0x0002)   //ɾ����Ʋ�Ʒ����
#define CF_ERROR_OPERATION_DEL_TRADESTRATEGY     MAKE_ERROR_CODE(CF_ERROR_OPERATION_BASE,0x0003)   //ɾ�����ײ��Դ���
#define CF_ERROR_OPERATION_DEL_ORGANIZATION_HAVE_TRDER      MAKE_ERROR_CODE(CF_ERROR_OPERATION_BASE,0x0004)   //ɾ����֯��������1
#define CF_ERROR_OPERATION_DEL_ORGANIZATION_NO_LEAFNODE     MAKE_ERROR_CODE(CF_ERROR_OPERATION_BASE,0x0005)   //ɾ����֯��������2
#define CF_ERROR_OPERATION_DEL_ROLE             MAKE_ERROR_CODE(CF_ERROR_OPERATION_BASE,0x0006)    //ɾ����ɫ����
#define CF_ERROR_TRADEDATA_NO_QUTO				MAKE_ERROR_CODE(CF_ERROR_OPERATION_BASE,0x000f)   //û������
#define CF_ERROR_TRADEDATA_NOT_ENOUGH_VOLOMN	MAKE_ERROR_CODE(CF_ERROR_OPERATION_BASE,0x0010)   //û���㹻�Ŀ�ƽ��
#define CF_ERROR_OPERATION_DEL_BROKER	        MAKE_ERROR_CODE(CF_ERROR_OPERATION_BASE,0x0011)   //��ί�н����˺���˹������޷�ɾ��

//��ط���˷��ش�����Ϣ
#define CF_ERROR_RISK_SETLIMITTRADE							MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0001)   //�������ƽ���ʧ��
#define CF_ERROR_RISK_SETMEMUALVERIFY						MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0002)   //�����ֶ����ʧ��
#define CF_ERROR_RISK_VERIFYORDERFAIL						MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0003)   //δ�ɽ������ʧ��
#define CF_ERROR_RISK_SETMEMUALVERIFY_NOPRIVILEDGE			MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0004)   //û�������ֶ����Ȩ��
#define CF_ERROR_RISK_LIMITTRADE_NOPRIVILEDGE				MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0005)   //û�����ƽ���Ȩ��
#define CF_ERROR_RISK_FORCECLOSE_NOPRIVILEDGE				MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0006)   //û��ǿƽȨ��
#define CF_ERROR_RISK_ADDPLAN_NOPRIVILEDGE					MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0007)   //û������ɾ����ط���Ȩ��
#define CF_ERROR_RISK_DELETEPLAN_NOPRIVILEDGE				MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0008)   //û��ɾ����ط���Ȩ��
#define CF_ERROR_RISK_USEPLAN_NOPRIVILEDGE					MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0009)   //û�����ÿ��ط�ط���Ȩ��
#define CF_ERROR_HISTORY_RISK_EVENT_QUERY_NOPRIVILEDGE		MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x000a)   //û����ʷ�����¼���ѯȨ��
#define CF_ERROR_HISTROY_MSG_RECORD_QUERY_NOPRIVILEDGE		MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x000b)   //û����ʷ��Ϣ��¼��ѯȨ��
#define CF_ERROR_HISTROY_FUND_SUTTLE_QUERY_NOPRIVILEDGE		MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x000c)   //û����ʷ����ֵ��ѯȨ��
#define CF_ERROR_ORDER_VERIFY_RECORD_QUERY_NOPRIVILEDGE		MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x000d)   //û�б�����˼�¼��ѯȨ��
#define CF_ERROR_FORCE_CLOSE_RECORD_QUERY_NOPRIVILEDGE		MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x000e)   //û��ǿƽ��¼��ѯȨ��
#define CF_ERROR_QUERY_HISTORY_FUND_NOPRIVILEDGE			MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x000f)   //û�в�ѯ��ʷ�ʽ�Ȩ��
#define CF_ERROR_QUERY_HISTORY_POSITION_NOPRIVILEDGE		MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0010)   //û�в�ѯ��ʷ�ֲ�Ȩ��
#define CF_ERROR_QUERY_HISTORY_POSITION_DETAIL_NOPRIVILEDGE	MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0011)   //û�в�ѯ��ʷ�ֲ���ϸȨ��
#define CF_ERROR_QUERY_HISTORY_TRADE_NOPRIVILEDGE			MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0012)   //û�в�ѯ��ʷ�ɽ�Ȩ��
#define CF_ERROR_QUERY_HISTORY_ORDER_NOPRIVILEDGE			MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0013)   //û�в�ѯ��ʷ����Ȩ��
#define CF_ERROR_CONTROL_SERVER_QUOT_RESET_NOPRIVILEDGE		MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0014)   //û���������������Ȩ��
#define CF_ERROR_CONTROL_TIME_QUOT_SETUP_NOPRIVILEDGE		MAKE_ERROR_CODE(CF_ERROR_RISK_BASE,0x0015)   //û�������������ʱ����ʱ������Ȩ��
//����
#define CF_ERROR_RISK_NETWORKCONNECTFAIL		MAKE_ERROR_CODE(CF_ERROR_OFFER_BASE,0x0001)   //��������ʧ��
#define CF_ERROR_RISK_NOTPROCESSFULL     		MAKE_ERROR_CODE(CF_ERROR_OFFER_BASE,0x0002)   //��ʾδ�������󳬹������
#define CF_ERROR_RISK_NOTPROCESSFULLEACHSECOND  MAKE_ERROR_CODE(CF_ERROR_OFFER_BASE,0x0003)   //��ʾÿ�뷢�����������������
#define CF_ERROR_RISK_NOTVALIDORDER 			MAKE_ERROR_CODE(CF_ERROR_OFFER_BASE,0x0004)   //������Ч���µ�
#define CF_ERROR_RISK_NOTVALIDACTION			MAKE_ERROR_CODE(CF_ERROR_OFFER_BASE,0x0005)   //�޷���Ч�ĳ������Ҳ������еı�����
#define CF_ERROR_RISK_ACCOUNTCONNECTFAIL		MAKE_ERROR_CODE(CF_ERROR_OFFER_BASE,0x0006)   //�˺���ctp���ӶϿ�
//�������ݸ��º����ʾ
#define CF_ERROR_DATA_UPDATE_ORG            MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0001)   //��֯�����������ݸ��£������µ�¼
#define CF_ERROR_DATA_UPDATE_PRODUCT        MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0002)   //��Ʋ�Ʒ�������ݸ��£������µ�¼
#define CF_ERROR_DATA_UPDATE_BROKER  	    MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0003)   //���͹�˾�������ݸ��£������µ�¼
#define CF_ERROR_DATA_UPDATE_SERVERGROUP    MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0004)   //��������������ݸ��£������µ�¼
#define CF_ERROR_DATA_UPDATE_SERVERADDR     MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0005)   //���������ַ�������ݸ��£������µ�¼
#define CF_ERROR_DATA_UPDATE_ACCOUNT        MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0006)   //ί�н����˻��������ݸ��£������µ�¼
#define CF_ERROR_DATA_UPDATE_STRATEGY  	    MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0007)   //���ײ��Ի������ݸ��£������µ�¼
#define CF_ERROR_DATA_UPDATE_USER           MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0008)   //ϵͳ�û��������ݸ��£������µ�¼
#define CF_ERROR_DATA_UPDATE_ROLE           MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0009)   //��ɫ�������ݸ��£������µ�¼

#define CF_ERROR_DATA_UPDATE_ROLEANDPRIVILEGE   MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0010)   //��ɫ��Ȩ�޹�ϵ���£������µ�¼
#define CF_ERROR_DATA_UPDATE_USERANDROLE        MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0011)   //�û��ͽ�ɫ��ϵ���£������µ�¼
#define CF_ERROR_DATA_UPDATE_USERANDORG         MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0012)   //�û�����֯������ϵ���£������µ�¼
#define CF_ERROR_DATA_UPDATE_USERANDPRODUCT     MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0013)   //�û�����Ʋ�Ʒ��ϵ���£������µ�¼
#define CF_ERROR_DATA_UPDATE_STRATEGYANDORG     MAKE_ERROR_CODE(CF_ERROR_DATA_UPDATE_BASE,0x0014)   //���ײ��Ժ���֯�������£������µ�¼
//����
#define CF_ERROR_DATA_QUOT_SUBSCRIBE        MAKE_ERROR_CODE(CF_ERROR_DATA_QUOT_BASE,0x0001)   //��������ʧ��
#define CF_ERROR_DATA_QUOT_UNSUBSCRIBE      MAKE_ERROR_CODE(CF_ERROR_DATA_QUOT_BASE,0x0002)   //�˶�����ʧ��

//����
#define CF_ERROR_DATA_STRATEGY_EXISTANDUSE				MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0001)   //[%s]�����Ѿ����ڣ����Ҵ�������״̬
#define CF_ERROR_DATA_STRATEGY_SAVEDLLFAIL				MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0002)   //�洢����dllʧ��
#define CF_ERROR_DATA_STRATEGY_SAVEZIPFAIL				MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0003)   //�洢����ZIPʧ��
#define CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER      MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0004)   //�洢���Բ����ڸý���Ա
#define CF_ERROR_DATA_STRATEGY_NOTEXIST					MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0005)   //���Բ�����
#define CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PRIVATE				MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0006)   //�ò��Բ������ڸý���Ա�����Ҹò���Ϊ˽��
#define CF_ERROR_DATA_READDLLFAIL						MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0007)   //��ȡ�ò���dll�ļ�ʧ��
#define CF_ERROR_DATA_READZIPFAIL						MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0008)   //��ȡ�ò���Դ�����ļ�ʧ��
#define CF_ERROR_DATA_IndexIsUsing						MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0009)   //[%s]ָ������Ӧ��[%s]�����д������е���
#define CF_ERROR_DATA_INDEX_NOTEXIST					MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0010)   //ָ�겻����
#define CF_ERROR_DATA_INDEX_NOTBELONGTOUPLOADER_PRIVATE				MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0011)   //��ָ�겻�����ڸý���Ա�����Ҹò���Ϊ˽��
#define CF_ERROR_DATA_INDEX_SAVEDLLFAIL				MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0012)   //�洢ָ��dllʧ��
#define CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_USE				MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0013)   //�ò��Բ������ڸý���Ա�����Ҹò���Ϊ����״̬
#define CF_ERROR_DATA_INSTANCE_NOTBELONGTOUPLOADER				MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0014)   //�ò��Է����������ڸý���Ա
#define CF_ERROR_DATA_INSTANCE_NOTEXIST					MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0015)   //���Է���������
#define CF_ERROR_DATA_STRATEGY_NOUSERNAME				MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0016)   //û���ϴ��ˣ��������ϴ�
#define CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PUBLIC				MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0017)   //���Ի�ָ��Դ����������������Ա��������
#define CF_ERROR_DATA_UPLOAD_BACKFAIL				MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0027)   //�ϴ�֮ǰ����ԭ��ָ����߲���ʧ��
#define CF_ERROR_DATA_UPLOAD_RollBACKFAIL				MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0028)   //���ݵ�ԭ��ָ����߲��Ի�ԭʧ��
#define CF_ERROR_DATA_UPLOAD_FAIL 				    MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0029)   //�����ϴ�ʧ�ܣ�ϵͳ�Ѿ��ָ�ԭ���Ĳ��Ի���ָ��
#define CF_ERROR_GET_COMMISSION_FAIL 				    MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0032)   //��ѯ����[%s]ʧ��
#define CF_ERROR_GET_MARGINATE_FAIL 				    MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0033)   //��ѯ��֤����[%s]ʧ��
#define CF_ERROR_NO_ACCOUNT_FAIL 				    MAKE_ERROR_CODE(CF_ERROR_DATA_STRATEGY_BASE,0x0034)   //��ѯʧ��,�����˺Ŷ�Ӧ�����˺Ų�����

///////////////////////////////////////////////////////////////////////////////
//��ʽ���������Ϊ�����ַ���FormatErrorCode
//Input: nErrorCode Ϊ����Ĵ�����
//Output: ������򷵻ؾ���Ĵ����ַ���
//        ���û���ҵ���Ӧ�Ĵ����룬�򷵻ؿմ�
/////////////////////////////////////////////////////////////////////////////////
inline const char * FormatErrorCode(const int nErrorCode)
{
	switch(nErrorCode)
	{
	case CF_ERROR_COMMON_MEMEORY:               return "�ڴ����";
	case CF_ERROR_COMMON_INPUT_PARAM:           return "�����������";
	case CF_ERROR_COMMON_NO_PRIVILEGE:          return "���û�û��Ȩ��ִ�иö���";
	case CF_ERROR_LOGIN_PASS_INVALIDATE:		return "�û������������";
    case CF_ERROR_LOGIN_USER_NOT_USE:		    return "�û�״̬û�����ã��޷���½";
	case CF_ERROR_LOGIN_SOCKET_NOT_EXIST:       return "��������ʧЧ���޷�ע��";
	case CF_ERROR_LOGIN_USER_DATA_NOTREADY:     return "�û��������ڳ�ʼ�������Ժ���";
	case CF_ERROR_LOGIN_TADRE_STATUS_INIT:      return "���������ڳ�ʼ�������Ժ��¼";
	case CF_ERROR_LOGIN_TADRE_STATUS_SETT:      return "���������ڽ��㣬���Ժ��¼";
	case CF_ERROR_LOGIN_TADRE_STATUS_NOSETT:    return "û���ճ����㵥����֪ͨ����Ա���н��㣬���Ժ��¼";
	case CF_ERROR_LOGIN_TADRE_STATUS_END:       return "���ս��׽������޷���¼";
	case CF_ERROR_LOGIN_INVALIDE_USERID:		return "��Ч���û�ID���޷���¼";
	case CF_ERROR_LOGIN_USER_NOT_EXIST:         return "�����ڸ��û����޷���¼";
	case CF_ERROR_LOGIN_TADRE_STATUS_INITFAIL:  return "��������ʼ��ʧ�ܣ��޷���¼";
	case CF_ERROR_LOGIN_IP_INVALIDATE:			return "���Ǵ�ָ����IP��ַ�����Ĵӷ�������¼";
	case CF_ERROR_LOGIN_USERTYPE_INVALIDATE:	return "�ÿͻ��˲��ܵ�½���÷�����";
	case CF_ERROR_LOGIN_QUOTSERVER_ERROR:	    return "�û�δ��¼���������";

	case CF_ERROR_NO_INSTRUMENTS:				return "û�п��Է��صĺ�Լ";	
	case CF_ERROR_NOT_SETTLEMENTTIME:		    return "δ������ʱ��";
	case CF_ERROR_NO_SETTLEMENTPRICE:		    return "û�н����";
	case CF_ERROR_DATABASE_NOT_CONNECT:		    return "���ݿ�λ����";
	case CF_ERROR_DATABASE_DUPLICATE_FIELD:		return "�ֶ��ظ���Υ��Ψһ��Լ������";
	case CF_ERROR_DATABASE_NO_DEPENDENT:		return "�޷��ҵ�������";
	case CF_ERROR_DATABASE_RECODE_USED:			return "��¼���ڱ�ʹ��";
	case CF_ERROR_DATABASE_OTHER_ERROR:			return "���ݿ��������";
	case CF_ERROR_REDUPLICATE_TRADE:		    return "���ݿ������д����ɽ���¼";
	case CF_ERROR_NO_COMMISSION:		        return "û�з��ʼ�¼";
	case CF_ERROR_NO_TRADEDATA:		            return "û�гɽ���¼";
	case CF_ERROR_NO_FUND:		                return "û���ʽ��¼";
	case CF_ERROR_NO_POSITIONDETAIL:		    return "û�гֲ���ϸ";
	case CF_ERROR_NO_POSITION:		            return "û�гֲּ�¼";
	case CF_ERROR_REDUPLICATE_POSITIONDETAIL:	return "���ݿ������д����ֲ���ϸ";
	case CF_ERROR_REDUPLICATE_POSITION:	        return "���ݿ������д����ֲ�";
	case CF_ERROR_NO_LASTSETTLEMENTPRICE:	    return "û����һ�����ս������ݣ��޷����н��ս���";
	case CF_ERROR_NO_SETTLEMENTINFO:	        return "û�н��㵥";
	case CF_ERROR_ADDTRADE_FAILD:	            return "�������޸ĳɽ�ʧ�ܣ���ƽ������";
	case CF_ERROR_MANULSETTLEMENT_FAILD:	    return "�������Ѵ��ڽ���״̬";
	case CF_ERROR_NOSERVICEGROUP_FAILD:	        return "û�����ý����������";
	case CF_ERROR_TRADEDATASTATUSERROR_FAILD:	return "���ݹ���ģ��Ϊ�ǽ���״̬���Զ�����ʧ��";
	case CF_ERROR_LOADHISTORYCOMMISSION_FAILD:	return "�޷����ش����ڷ�������";

	
	case CF_ERROR_TRADEDATA_NO_USER_DATA:       return "��������û�и��û������ݣ�����ϵ����Ա";
	case CF_ERROR_TRADEDATA_NO_INSTRUMENTS:     return "��Լ����û��׼���ã����Ժ��ٴβ�ѯ";
	case CF_ERROR_TRADEDATA_NO_COMMISSION:      return "������������û��׼���ã����Ժ��ٴβ�ѯ";
	case CF_ERROR_TRADEDATA_NO_MARGIN:          return "��֤��������û��׼���ã����Ժ��ٴβ�ѯ";
	case CF_ERROR_TRADEDATA_NO_FUND:            return "�ʽ�����û��׼���ã����Ժ��ٴβ�ѯ";
	case CF_ERROR_TRADEDATA_NO_POSITION:        return "�ֲ�����û��׼���ã����Ժ��ٴβ�ѯ";
	case CF_ERROR_TRADEDATA_NO_POSDETAIL:       return "�ֲ���ϸ����û��׼���ã����Ժ��ٴβ�ѯ";
	case CF_ERROR_TRADEDATA_NO_COMPOS:          return "��ϳֲ�����û��׼���ã����Ժ��ٴβ�ѯ";
	case CF_ERROR_TRADEDATA_NO_COMPOSDETAIL:    return "��ϳֲ���ϸû��׼���ã����Ժ��ٴβ�ѯ";
	case CF_ERROR_TRADEDATA_NO_ORDER :          return "��������û��׼���ã����Ժ��ٴβ�ѯ";
	case CF_ERROR_TRADEDATA_NO_TRADE:           return "�ɽ�����û��׼���ã����Ժ��ٴβ�ѯ";
	case CF_ERROR_TRADEDATA_FUNDINOUT_DENY :    return "�����ֻ�������н���";
	case CF_ERROR_NOINITED:                     return "״̬û�г�ʼ�����޷����ָ���Ķ���";
	case CF_ERROR_TRADEDATA_SETTMENT_NOINIT:    return "���������ڳ�ʼ��״̬�����ܽ��н��㣬���Ժ�����";
	case CF_ERROR_TRADEDATA_SETTMENT_NOTRAING:	return "�������ݴ���ֻ������ʷ����";
	case CF_ERROR_TRADEDATA_NO_QUTO:			return "û������";
	case CF_ERROR_TRADEDATA_NOTRADINGDAY:		return "û�н����գ����ܽ��н���";
	case CF_ERROR_TRADEDATA_TODAYHAD:			return "�����ѽ�����ˣ�ֻ�ܽ�����ʷ����";
	case CF_ERROR_TRADEDATA_NOT_ENOUGH_VOLOMN:	return "û���㹻�Ŀ�ƽ��";
	case CF_ERROR_TRADEDATA_NOSETTLEMENT_PRICE: return "����۲�ȫ";
	case CF_ERROR_TRADINGDAY_ERROR:             return "�����մ���ֻ�ܽ�����ʷ����";
	case CF_ERROR_TRADEDATA_EXCHANGE_ISTRADE :  return "���ڽ���״̬�����ܽ��н���";
	case CF_ERROR_TRADEDATA_INITING:            return "�������ڳ�ʼ�������ܽ��иò���";
	case CF_ERROR_TRADEDATA_INIT_NORELEATION:   return "���û�û�й���ί�н����ʺţ��޷���ʼ��";
	case CF_ERROR_TRADEDATA_INIT_ACCOUNT_NOLOGIN: return "���û������ί�н����˺�û�е�½�����ȵ�½��ί�н����˺�";
	case CF_ERROR_TRADEDATA_INIT_ACCOUNT_NOCOMM:  return "���û������ί�н����˺�û�в�ѯ�����ʣ������˺ŵ�½���쳣";
	case CF_ERROR_TRADEDATA_NOTTRADINGDAY:      return "�ǽ����գ����ܽ��н���";
	case CF_ERROR_TRADEDATA_NOTMATCH_STATUS:    return "�ò�����ֻ���ڷ��������ڽ������״̬,�޽�����״̬�²��ܲ���";
	case CF_ERROR_TRADEDATA_HAS_POSITION:		return "���û����гֲ֣������޸�ί�н����ʺ�";
	case CF_ERROR_TRADEDATA_HAS_INITED:			return "�ѳ�ʼ���ɹ��������ٳ�ʼ��";
	case CF_ERROR_TRADEDATA_FORBID_REINIT:      return "��ǰ������״̬�����������趨";
	case CF_ERROR_TRADEDATA_NOT_ENOUGH_FUND:    return "�ʽ���";
	case CF_ERROR_TRADEDATA_ACCOUNT_ISLOGIN:    return "��ǰί�н����ʺ��ǵ�½״̬������ɾ��";
	case CF_ERROR_TRADEDATA_FUNDIN_DENY_NOINIT: return "�û�δ��ʼ�������ܽ��г�������";
	case CF_ERROR_TRADEDATA_USER_LOGIN:			return "�й������û����ڵ�¼״̬���޷�ɾ��";

    case CF_ERROR_OPERATION_DEL_TRADEACCOUNT:   return "�н���Ա����ʹ�ô�ί�н����˺ţ��޷�ɾ��";
    case CF_ERROR_OPERATION_DEL_FINANCEPRODUCT: return "�н���Ա����ʹ�ô���Ʋ�Ʒ���޷�ɾ��";
    case CF_ERROR_OPERATION_DEL_TRADESTRATEGY:  return "���ʹ���֯���ڽ��ײ��ԣ��޷�ɾ��";
    case CF_ERROR_OPERATION_DEL_ORGANIZATION_HAVE_TRDER:   return "���û��ڴ��ʹ���֯�£��޷�ɾ��";
    case CF_ERROR_OPERATION_DEL_ORGANIZATION_NO_LEAFNODE:   return "���ʹ���֯����������֯���޷�ɾ��";
    case CF_ERROR_OPERATION_DEL_ROLE:           return "�й���Ա����Ա����ʹ�ô˽�ɫ���޷�ɾ��";
    case CF_ERROR_OPERATION_DEL_BROKER:         return "��ί�н����˺���˹������޷�ɾ��";


	case CF_ERROR_USER_NOT_ENABLE:				return "�˻�δ����";
	case CF_ERROR_TRADE_FORBIDDEN:				return "�޽���Ȩ��";
	case  CF_ERROR_TRADE_LIMITED:				return "��Ʒ�ֻ��Լ�����ƽ���";
	case CF_ERROR_TRADE_VERIFY_FAILD:			return "���δͨ��";
	case CF_ERROR_TRADE_RISK_LIMITED:			return "�ú�Լ���µ����򱻷�ط��������µ�";
	case CF_ERROR_TRADE_STRATEGY_LIMITED:		return "�ú�Լ���µ����򱻽��ײ��������µ�";
	case CF_ERROR_TRADE_ALREADY_CANCELED:		return "�ѳ���";
	case CF_ERROR_TRADE_USER_TRADE_NOTREADY:	return "���û��Ĳ�ѯ�������ݻ�û��׼���ã����Ժ�����";
	case CF_ERROR_TRADE_HAS_VERIFYED:           return "���������";
	case CF_ERROR_TRADE_VERIFYING:				return "���������";
	case CF_ERROR_TRADE_HAS_RECEVIED:			return "�����ѽ���";
	case CF_ERROR_TRADE_HAS_SENDCTP:			return "������ת����CTP";
	case CF_ERROR_TRADE_HAS_RISK_RECEVIED:		return "ǿƽ���ѽ���";
	case CF_ERROR_TRADE_FORCE_ORDED_NO_ACTION:  return "ǿƽ�����ܳ���";
	case CF_ERROR_RISK_ALL_INSTRUMENT_ORDER:	return "������� ���к�Լ�����µ�";
	case CF_ERROR_RISK_SINGLE_INSTRUMENT_ORDER:	return "������� ����Լ�����µ�";
	case CF_ERROR_RISK_ALL_INSTRUMENT_OPEN:	    return "������� ���к�Լ���ƿ���";
	case CF_ERROR_RISK_SINGLE_INSTRUMENT_OPEN:	return "������� ����Լ���ƿ���";

	case CF_ERROR_TRADEDATA_FORBID_USERINIT:	return "��ǰ������״̬���ܳ�ʼ���û��������³�ʼ�����������ٴγ���";
	case CF_ERROR_TRADEDATA_FORBID_ACCOUNTLOING:return "��ǰ������״̬���ܵ�½ί�н����ʺţ������³�ʼ�����������ٴγ���";
	case CF_ERROR_TRADEDATA_FORBID_QRYUSER:		return "��ǰ������״̬���ǽ���״̬����ѯ�����û���ʼ��״̬";
	case CF_ERROR_TRADEDATA_FORBID_QRYACCOUNT:  return "��ǰ������״̬���ǽ���״̬����ѯ����ί�н����ʺŵ�½״̬";
	case CF_ERROR_TRADEDATA_WRONG_INSTRUMENT:   return "��Լ��������";
	case CF_ERROR_RISK_SETLIMITTRADE:           return "�������ƽ���ʧ��";
	case CF_ERROR_RISK_SETMEMUALVERIFY:			return "�����ֶ����ʧ��";
	case CF_ERROR_RISK_VERIFYORDERFAIL:         return "δ�ɽ������ʧ��";
	case CF_ERROR_RISK_SETMEMUALVERIFY_NOPRIVILEDGE: return "û���ֶ����Ȩ��";
	case CF_ERROR_RISK_LIMITTRADE_NOPRIVILEDGE:		 return "û�����ƽ���Ȩ��";
	case CF_ERROR_RISK_FORCECLOSE_NOPRIVILEDGE:		 return "û��ǿƽȨ��";
	case CF_ERROR_RISK_ADDPLAN_NOPRIVILEDGE:		 return "û�������޸ķ�ط�����Ȩ��";
	case CF_ERROR_RISK_DELETEPLAN_NOPRIVILEDGE:		 return "û��ɾ����ط�����Ȩ��";
	case CF_ERROR_RISK_USEPLAN_NOPRIVILEDGE:		 return "û������ͣ�÷�ط�����Ȩ��";
	case CF_ERROR_HISTORY_RISK_EVENT_QUERY_NOPRIVILEDGE:		 return "û����ʷ�����¼���ѯȨ��";
	case CF_ERROR_HISTROY_MSG_RECORD_QUERY_NOPRIVILEDGE:		 return "û����ʷ��Ϣ��¼��ѯȨ��";
	case CF_ERROR_HISTROY_FUND_SUTTLE_QUERY_NOPRIVILEDGE:		 return "û����ʷ����ֵ��ѯȨ��";
	case CF_ERROR_ORDER_VERIFY_RECORD_QUERY_NOPRIVILEDGE:		 return "û�б�����˼�¼��ѯȨ��";
	case CF_ERROR_FORCE_CLOSE_RECORD_QUERY_NOPRIVILEDGE:		 return "û��ǿƽ��¼��ѯȨ��";
	case CF_ERROR_QUERY_HISTORY_FUND_NOPRIVILEDGE:				 return "û�в�ѯ��ʷ�ʽ�Ȩ��";
	case CF_ERROR_QUERY_HISTORY_POSITION_NOPRIVILEDGE:			 return "û�в�ѯ��ʷ�ֲ�Ȩ��";
	case CF_ERROR_QUERY_HISTORY_POSITION_DETAIL_NOPRIVILEDGE:	 return "û�в�ѯ��ʷ�ֲ���ϸȨ��";
	case CF_ERROR_QUERY_HISTORY_TRADE_NOPRIVILEDGE:				 return "û�в�ѯ��ʷ�ɽ�Ȩ��";
	case CF_ERROR_QUERY_HISTORY_ORDER_NOPRIVILEDGE:			     return "û�в�ѯ��ʷ����Ȩ��";
	case CF_ERROR_CONTROL_SERVER_QUOT_RESET_NOPRIVILEDGE:		 return "û���������������Ȩ��";
	case CF_ERROR_CONTROL_TIME_QUOT_SETUP_NOPRIVILEDGE:		     return "û�������������ʱ����ʱ������Ȩ��";

	case CF_ERROR_RISK_NETWORKCONNECTFAIL:				return "��������ʧ��";
	case CF_ERROR_RISK_NOTPROCESSFULL:					return "��ʾδ�������󳬹������";
	case CF_ERROR_RISK_NOTPROCESSFULLEACHSECOND:		return "��ʾÿ�뷢�����������������";
	case CF_ERROR_RISK_NOTVALIDORDER:					return "������Ч���µ�";
	case CF_ERROR_RISK_NOTVALIDACTION:					return "�޷���Ч�ĳ������Ҳ������еı�����";
	case CF_ERROR_RISK_ACCOUNTCONNECTFAIL:				return "�˺���ctp���ӶϿ�";

    case CF_ERROR_LOGIN_USER_PASSWORD_ERROR:    return "�û��޸�����ʱ��ԭ�����������";
    case CF_ERROR_LOGIN_USER_UNPASSWORD_ERROR:  return "�û���������ʱ�������������";

    case CF_ERROR_DATA_UPDATE_ORG:            return "��֯�����������ݸ��£������µ�¼!";
    case CF_ERROR_DATA_UPDATE_PRODUCT:        return "��Ʋ�Ʒ�������ݸ��£������µ�¼!";
    case CF_ERROR_DATA_UPDATE_BROKER:  	      return "���͹�˾�������ݸ��£������µ�¼!";
    case CF_ERROR_DATA_UPDATE_SERVERGROUP:    return "��������������ݸ��£������µ�¼!";
    case CF_ERROR_DATA_UPDATE_SERVERADDR:     return "���������ַ�������ݸ��£������µ�¼!";
    case CF_ERROR_DATA_UPDATE_ACCOUNT:        return "ί�н����˻��������ݸ��£������µ�¼!";
    case CF_ERROR_DATA_UPDATE_STRATEGY:  	  return "���ײ��Ի������ݸ��£������µ�¼!";
    case CF_ERROR_DATA_UPDATE_USER:           return "ϵͳ�û��������ݸ��£������µ�¼!";
    case CF_ERROR_DATA_UPDATE_ROLE:           return "��ɫ�������ݸ��£������µ�¼!";

    case CF_ERROR_DATA_UPDATE_ROLEANDPRIVILEGE:   return "��ɫ��Ȩ�޹�ϵ���£������µ�¼!";
    case CF_ERROR_DATA_UPDATE_USERANDROLE:        return "�û��ͽ�ɫ��ϵ���£������µ�¼!";
    case CF_ERROR_DATA_UPDATE_USERANDORG:         return "�û�����֯������ϵ���£������µ�¼!";
    case CF_ERROR_DATA_UPDATE_USERANDPRODUCT:     return "�û�����Ʋ�Ʒ��ϵ���£������µ�¼!";
    case CF_ERROR_DATA_UPDATE_STRATEGYANDORG:     return "���ײ��Ժ���֯�������£������µ�¼!";

    case CF_ERROR_DATA_QUOT_SUBSCRIBE:       return "��������ʧ��,�����¶���!";
    case CF_ERROR_DATA_QUOT_UNSUBSCRIBE:     return "�˶�����ʧ��,�������˶�!";

	case CF_ERROR_DATA_STRATEGY_EXISTANDUSE: return "[%s]�����Ѿ����ڣ����Ҵ�������״̬,�����ϴ�ʧ��";
	case CF_ERROR_DATA_STRATEGY_SAVEDLLFAIL: return "�洢����dllʧ��";
	case CF_ERROR_DATA_STRATEGY_SAVEZIPFAIL: return "�洢����ZIPʧ��";
	case CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER: return "���Ի�ָ�����Ѵ��ڣ�Ϊ��������Աʹ�ã���Ȩ���¡�";
	case CF_ERROR_DATA_STRATEGY_NOTEXIST: return "�ò��Բ�����";
	case CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PRIVATE: return "���Ի�ָ��������������Ա,˽�в�������";
	case CF_ERROR_DATA_READDLLFAIL: return "��ȡ�ò���dll�ļ�ʧ��";
	case CF_ERROR_DATA_READZIPFAIL: return "��ȡ�ò���Դ�����ļ�ʧ��";
	case CF_ERROR_DATA_IndexIsUsing: return "[%s]ָ������Ӧ��[%s]���Դ������е���,�����ϴ�ʧ��";
	case CF_ERROR_DATA_INDEX_NOTEXIST: return "��ָ�겻����";
	case CF_ERROR_DATA_INDEX_NOTBELONGTOUPLOADER_PRIVATE: return "��ָ�겻�����ڸý���Ա�����Ҹò���Ϊ˽��";
	case CF_ERROR_DATA_INDEX_SAVEDLLFAIL: return "�洢ָ��dllʧ��";
	case CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_USE: return "�ò��Բ������ڸý���Ա�����Ҹò���Ϊ����״̬";
	case CF_ERROR_DATA_INSTANCE_NOTBELONGTOUPLOADER: return "�ò��Է����������ڸý���Ա";
	case CF_ERROR_DATA_INSTANCE_NOTEXIST: return "�ò��Է���������";
	case CF_ERROR_DATA_STRATEGY_NOUSERNAME: return "û���ϴ��ˣ��������ϴ�";
	case CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PUBLIC: return "���Ի�ָ��Դ����������������Ա��������";
	case CF_ERROR_DATA_UPLOAD_BACKFAIL: return "�ϴ�֮ǰ����ԭ��ָ����߲���ʧ��";
	case CF_ERROR_DATA_UPLOAD_RollBACKFAIL: return "���ݵ�ԭ��ָ����߲��Ի�ԭʧ��";
	case CF_ERROR_DATA_UPLOAD_FAIL:			return "�����ϴ�ʧ�ܣ�ϵͳ�Ѿ��ָ�ԭ���Ĳ��Ի���ָ��";
	case CF_ERROR_GET_COMMISSION_FAIL:			return "��ѯ����[%s]ʧ��";
	case CF_ERROR_GET_MARGINATE_FAIL:			return "��ѯ��֤����[%s]ʧ��";
	case CF_ERROR_NO_ACCOUNT_FAIL:				return "��ѯʧ��,�����˺Ŷ�Ӧ�����˺Ų�����";
	default:	return "δ֪����";
	}
}