#ifndef GUI_CFG_GLOBAL_H
#define GUI_CFG_GLOBAL_H

#include <string>
#include <vector>
#include "../inc/Module-Misc/tinyxml.h"
#include "../ISvr/PlatformStruct.h"
#include "zqType.h"

typedef char ZQFontType[32];
typedef char ZQShortKeyType[32];

enum _style
{
	STYLE_Q7=0,
	STYLE_SIMPLE,
	STYLE_MOUSE,
	STYLE_SunGard
};

class BaseCfgMgr
{
public:
	virtual bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false )=0;
	virtual bool SaveCfg(TiXmlElement *root)=0;
	virtual void ClearCfg() = 0;
};

//��ʾ����һ�������ļ����������������Ĭ�ϣ���ǰ���ֶ���ļ��洢��
/************������**begin********************************************************************************************************/

typedef struct tagMainWindowCfg
{
	zq_bool	bIsTop;					//�Ƿ��ö�
	zq_bool	bIsMinTray;				//�Ƿ���С��������
	zq_bool	bAutoLock;				//�Զ�����ʱ�䣨��λ���룩
	zq_int	nColorPlan;				//��ɫ����
	zq_bool	bFixPanel;               //�̶����
	zq_bool bShowPanelShortKey;      //��������ʾ��ݼ�  
	zq_int	nLogonVerify;			//��¼��֤ 0:����Ҫ��֤ 1:������ĸ�����֤ 2:������֤
	zq_bool	bDelInstrumentWarning;	//ɾ����ѡ��Լ�Ƿ񵯳���ʾ��
	zq_int	TradeInfoDlgSize;		//TradeInfoDlg�Ĵ�С
	zq_bool	bModuleIsQry;	        //1-����ģ��ʹ�óɽ��ر�������ѯ�ķ�ʽ��0��ʾ��ʹ��
}MAIN_WINDOW_CFG,*LPMAIN_WINDOW_CFG;

/************������**end********************************************************************************************************/

enum GUI_MODULE_ID
{
	QUOT_TABLE_MODULE_ID = 1001,		//���۱�
	ORDER_PANEL_MODULE_ID,				//��׼�µ���
	THRADE_RECORD_MODULE_ID,			//�ɽ���¼ģ��
	THRADE_STATISTICS_MODULE_ID,		//�ɽ�ͳ��
	POSITION_MODULE_ID,					//�ֲ�
	POSITION_DETAIL_MODULE_ID,			//�ֲ���ϸ
	INSTRUMENT_MODULE_ID,				//��Լ�б�
	ALL_TRUST_ORDER_MODULE_ID,          //����ί�е�
	WAIT_TRADE_MODULE_ID,               //δ�ɽ�
	COMBINATION_POSITION_MODULE_ID,     //��ϳֲ�
	ACCOUNT_MODULE_ID,					//�ʽ��ʻ�
	TASK_MODULE_ID,						//����

	QUOT_PANEL_MODULE_ID				//���ۿ�
};

typedef struct
{
	DWORD GroupId;
	std::string GroupName;
	std::vector<std::string>  InstrVec;
}GroupInfo2;

//list���ý�����Ϣ����
/************list**begin********************************************************************************************************/

typedef struct tagListTableCfg
{	
	int				Id;							
	ZQFontType		szFont;							//�������ã��硰underlined bold 14��
	int				TextSize;						//�����С
	unsigned long	TextColor;						//������ɫ
	unsigned long	HeadColor;						//����������ɫ
	unsigned long	HeadBackgroundColor;			//�����ɫ

	unsigned long	BackgroundColor;				//��ɫ
	unsigned long	EvenLineBackgroundColor;        //ż�е�ɫ
	unsigned long	OddLineBackgroundColor;         //���е�ɫ

	int				HeadVisibility;                 //�����Ƿ�����
	int				Visibility;						//�Ƿ�ɼ�

	tagListTableCfg()
	{
		memset(this, 0, sizeof(tagListTableCfg));
	}
} LIST_TABLE_CFG, *LPLIST_TABLE_CFG;

//list����Ϣ����
typedef struct tagListColumnCfg
{
	int				id;								//��ID
	int				Width;                          //��ʾ��ȣ�Ĭ��Ϊ100
	unsigned long	TextColor;					    //��������ɫ
	int				Visibility;                     //�Ƿ�ɼ�

	tagListColumnCfg()
	{
		memset(this, 0, sizeof(tagListColumnCfg));
	}
} LIST_COLUMN_CFG, *LPLIST_COLUMN_CFG;

/************list**end********************************************************************************************************/
/************��Լ��**begin********************************************************************************************************/
#define GROUPNAME_LENGTH 63
typedef struct tagInstrumentGroupCfg
{
    tagInstrumentGroupCfg()
    {
        memset(this, 0, sizeof(tagInstrumentGroupCfg));
    }
	int			GroupId;			//��ID
	char		GroupName[GROUPNAME_LENGTH+1];			//������
	bool		IsVisibility;		//�Ƿ���ʾ
}INSTRUMENT_GROUP_CFG, *LPINSTRUMENT_GROUP_CFG;

//��Լ���Լ��Ա��Ϣ
typedef struct tagInstrumentGroupMemberInfoCfg
{
	InstrumentIDType	szInstrumentID;		//��ԼID
	int		    PositionIndex;      //�ں�Լ������ʾ������λ��

	tagInstrumentGroupMemberInfoCfg()
	{
		memset(this, 0, sizeof(tagInstrumentGroupMemberInfoCfg));
	}
}INSTRUMENT_GROUP_MEMBER_INFO_CFG, *LPINSTRUMENT_GROUP_MEMBER_INFO_CFG;

/************��Լ��**end********************************************************************************************************/
/************�µ�**begin********************************************************************************************************/

//�Զ���ƽ����
enum AUTO_CLOSE_MODE
{
	ALL_CLOSE = 1001,                                    //���ֲ���ȫƽ��
	DEFAUlT_VOLUME_CLOSE_ABANDON,						 //��Ĭ������ƽ�֣������ֲ������ֲ�����
	DEFAUlT_VOLUME_CLOSE_OPEN,							 //��Ĭ������ƽ�֣������ֲ��������Զ����򿪲֡�
};

enum AUTO_CLOSE_PRE_ORDER_MODE
{
	IGNORE_PRE_ORDER = 1001,							 //������
	AUTO_CANCEL_PRE_ORDER,								 //ÿ��ƽ��ʱ�Զ�����ԭƽ�ֵ�
	INSUFFICIENT_AUTO_CANCEL_PRE_ORDER,					 //��ƽ����ʱ�Զ�����ԭƽ�ֵ�
};

//�µ�����
typedef struct tagOrderMultiply
{
	ZQShortKeyType ShortKey;
	int Volume;

	tagOrderMultiply()
	{
		memset(this, 0, sizeof(tagOrderMultiply));
	}
}OrderMultiply;


typedef struct tagKeyOrderInfo
{
	ZQShortKeyType ShortKey;
	int nBuySell;	//0 ���� 1 ����
	int nPrice;
	int nOverPrice;
	int nAction;
	int nActionArea;

	tagKeyOrderInfo()
	{
		memset(this, 0, sizeof(tagKeyOrderInfo));
	}
}KeyOrderInfo;

typedef struct tagOrderCfg
{
	zq_bool		bBuy5Sell5;							// �Ƿ���ʾ�嵵����
	zq_bool		bMouseInputInstrumentId;			// �Ƿ��������������Լ����
	zq_bool		bInputAbbreviation;					// ����������д��ƴ��
	zq_int		nContractListStyle;					//��Լ��������
	zq_bool		bOrderConfirm;						//�µ�ǰȷ��
	zq_bool     bAutoOpenClose;						//�̶�ʹ���Զ���ƽ
	zq_bool     b13Change;							//13�л�
	zq_bool     bReTrack;							//��������
	zq_bool     bSimulate;							//�Ƿ�ģ��
	zq_int		nMKTPriceUse;						// 0 ������ 1 ���¼� 2 ������
	zq_bool		bMouseInputPrice;					// �Ƿ�������������۸�
	zq_bool		bKeyBoardFastPrice;					// ���̿�������۸�
	zq_bool		bMouseInputVolume;					// �Ƿ������������볣������
	zq_int9		Volume;							// ������������
	InstrumentIDType Name[5];							//�µ���Ŀ˳��

	//�Զ���ƽ
	zq_int		ProcessPositionType;				//����гֲִ���ʽ 1ȫƽ 2���������� 3�������򿪲�

	//���̿����µ�
	zq_bool		bOpenQuotKeyBoardOrder;				//�Ƿ����ü����µ�
	zq_bool		bSyncQuotPanel;						//���������Ƿ�����µ���
	zq_bool		bReturnQuotTable;					//�µ������Ƿ�ر��۱�
	int			KeyOrderDataNum;
	KeyOrderInfo KeyOrderData[100];					//���̿����µ���������

	//�������� ,˫�����ۣ� �����ֲ֣�˫���ֲ�
	//1��������(��ȷ��)   2 ��������(����ȷ��)  3 ����겻��
	//4 ����굽��Լ����  5 ����굽�۸� 6  ����꿪ƽ
	//7 ����굽����   8 ����굽���� 9 ����굽�µ���ť
	//10 ""
	zq_int 		ClickQuotTableAction;                                                       
	zq_int		DoubleClickQuotTableAction;			//˫������
	zq_int		MouseFastOrderDirection;			//�µ�����
	zq_bool		bCtrlReverse;						//�Ƿ�����Ctrl�����ʱ��ת����
	zq_bool		bAlwaysOpen;                        //�Ƿ���������ۿ���
	zq_int		ClickPositionAction;				//�����ֲ�
	zq_int		DoubleClickPositionAction;			//˫���ֲ�
	zq_int		PositionVolume;                     //������λ����

	//�߼�
	int			OrderMultiDataNum;
	OrderMultiply OrderMultiData[100];				//�µ���������

	//���չ���
	zq_int		VolumeLimit;							//��������
	zq_int		DifferPrice;							//ƫ���λ 
	zq_int		ActionAfterOrder;						//1,�µ�������µ���2,ȫ�����3,��ռ۸������4ֻ�������  

	//��ʾ������
	zq_bool		bOrderSuccessDlg;					// �µ��ɹ� �Ƿ� ����
	zq_bool		bOrderSuccessSound;					// �µ��ɹ� �Ƿ� ����
	zq_bool		bOrderFailDlg;						// �µ�ʧ�� �Ƿ� ����
	zq_bool		bOrderFailSound;					// �µ�ʧ�� �Ƿ� ����
	zq_bool		bTradeDlg;							// �ҵ��ɽ� �Ƿ� ����
	zq_bool		bTradeSound;						// �ҵ��ɽ� �Ƿ� ����
	zq_long		SellColor;							//��ɫ
	zq_long		BuyColor;							//��ɫ

	//��ݼ�
	zq_char		cBuyHotKey;							//�����ݼ�
	zq_char		cSelHotKey;							//������ݼ�
	zq_char		cOpenOrderHotKey;					//���ֿ�ݼ�
	zq_char		cCloseTodayVolumnHotKey;			//ƽ���ݼ�
	zq_char		cCloseOrderHotKey;					//ƽ�ֿ�ݼ�
	zq_char20	szOpenOrderBtnHotKey;			//�µ���ť��ݼ�
	zq_char20	szOrderParkedBtnHotKey;			//Ԥ�񵥰�ť��ݼ�
	zq_int		nCursorPosAfterOrder;				//�µ����궨λλ�� 0,Ĭ�� 1,��Լ 2,���� 3,��ƽ 4,�۸� 5,����

	//�����µ�����
	zq_int		nAllNotHaveOppositePosition;		//���޷���ֲ�
	zq_int		nSomeHaveOppositePosition;			//�����з���ֲ�
	zq_int		nAllHaveOppositePosition;			//ȫ���з���ֲ�
	zq_int		nMouseClosePositionStrategy;		//����ֲֻ�ֲֻ���
	zq_int		nCancelSameDirOpenOrderBeforeOrder;	//�µ�ǰ�ȳ�ͬ����ҵ�

	tagOrderCfg()
	{
		memset(this, 0, sizeof(tagOrderCfg));
	}
}ORDER_CFG, *LPORDER_CFG;

/************�µ�**end********************************************************************************************************/

/************����**begin********************************************************************************************************/
typedef struct tagCancelOrderCfg
{
	zq_bool		bDoubleClickMouseCancel;				//���˫������
	zq_bool		bCancelConfirm;							//����ǰȷ��
	zq_bool		bHideCancelButton;			            //���س�����ť
	zq_bool		bHideCancelAllButton;			        //����ȫ����ť

	zq_bool		bOpenModifyOrder;				        //���øĵ�
	zq_bool		bModifyOrderConfirm;			    	//�ĵ�ǰȷ��

	zq_bool		bCancelSuccessDlg;						// �µ��ɹ� �Ƿ� ����
	zq_bool		bCancelSuccessSound;					// �µ��ɹ� �Ƿ� ����

	zq_bool		bCancelFailDlg;							// �µ��ɹ� �Ƿ� ����
	zq_bool		bCancelFailSound;						// �µ��ɹ� �Ƿ� ����

	zq_bool		bOpenKeyBoardCancelOrder;				//�Ƿ����ñ��۱�����µ�
	zq_bool	    bReturnQuotTable;						//�µ������Ƿ�ر��۱�
	zq_char20	szCancelOrderBtnHotKey;				//������ť��ݼ�
	zq_char20	szCancelAllOrderBtnHotKey;			//ȫ��������ť��ݼ�

	tagCancelOrderCfg()
	{
		memset(this, 0, sizeof(tagCancelOrderCfg));
	}
}CANCEL_ORDER_CFG, *LPCANCEL_ORDER_CFG;
/************����**end********************************************************************************************************/

/************�ʽ��˻�**begin********************************************************************************************************/
typedef struct tagFundAccountCfg
{
	zq_long	TextColor;			//������ɫ
	zq_long	BgColor;			//��ɫ	
}FUND_ACCOUNT_CFG, *LPFUND_ACCOUNT_CFG;
/************�ʽ��˻�**end********************************************************************************************************/


/************���չ���**begin********************************************************************************************************/

enum ORDER_PANEL_CLEAR_MODE
{
	CLEAR_ORDER_PANEL_NO = 2001,
	CLEAR_ORDER_PANEL_ALL,
	CLEAR_ORDER_PANEL_PRICE_VOLUME,
	CLEAR_ORDER_PANEL_VOLUME
};

typedef struct tagRiskMgrCfg
{
	zq_int SingleMaxVolume;
	zq_int ClearOrderPanelMode;
}RISK_MGR_CFG, *LPRISK_MGR_CFG;

/************���չ���**end********************************************************************************************************/

/***********��ݼ�**begin********************************************************************************************************/

//��ݼ�����
enum SHORTCUTKEYS_TYPE
{
	SHORTCUTKEYS_UNDEFINE_TYPE = 0,					//�޶���
	SHORTCUTKEYS_MAIN_CONTROL_TYPE = 3001,			//�����ڿؼ� �л���ݼ�
	SHORTCUTKEYS_INSTRUMENT_GROUP_TYPE,				//��Լ��     �л���ݼ�
	SHORTCUTKEYS_ORDER_TYPE,						//�µ�����   ��ݼ�
	SHORTCUTKEYS_CANCEL_ORDER_TYPE,					//��������   
	SHORTCUTKEYS_ORDER_TIMES_TYPE					//�µ�����
};


//�µ�����
enum ORDER_ACTION
{
	ORDER_UNDEFINE = 0,						//�޶���
	SEND_ORDER_CONFIRM = 4001,              //������������ȷ�ϣ�
	SEND_ORDER_NOT_CONFIRM,					//��������������ȷ�ϣ�
	FILL_ORDER,								//����겻��
	FILL_ORDER_FOCUS_INSTRUMENT_ID,         //����굽��Լ����
	FILL_ORDER_FOCUS_DIRECTION,				//����굽����
	FILL_ORDER_FOCUS_OPEN_CLOSE,			//����굽��ƽ
	FILL_ORDER_FOCUS_VOLUME,				//����굽����
	FILL_ORDER_FOCUS_PRICE,					//����굽�۸�
	FILL_ORDER_FOCUS_ORDER_BUTTON			//����굽�µ���ť
};

//��������
enum CANCEL_ORDER_ACTION
{
	CANCEL_ORDER_UNDEFINE = 0,              //�޶���
	CANCEL_ORDER_SELECT = 5001,             //����ѡ�е�
	CANCEL_ORDER_LAST,						//�������һ����
	CANCEL_ORDER_FIRST,						//��������һ����
	CANCEL_ORDER_INSTRUMENT,				//����Լ����
	CANCEL_ORDER_DIRECTION,					//�����򳷵�
	CANCEL_ORDER_INSTRUMENT_DIRECTION,		//����Լ�����򳷵�
	CANCEL_ORDER_ALL,						//ȫ������
	MODIFY_ORDER_PRICE,						//�ļ۸�
	MODIFY_ORDER_VOLUME						//������
};

typedef struct tagPanelInfo
{
	int   id;     
	ZQShortKeyType    ShortKey;
	int   bVisiable;
	char AliasName[32];

	tagPanelInfo()
	{
		memset(this, 0, sizeof(tagPanelInfo));
	}
} PanelInfo;

typedef struct tagVarietyInfo
{
	int    volume;
	InstrumentIDType  InstruName;
	ZQShortKeyType  ShortKey;
	char  profit[20];
	char  loss[20];
	char  reverse[20];

	tagVarietyInfo()
	{
		memset(this, 0, sizeof(tagVarietyInfo));
	}
} VarietyInfo;

typedef struct tagNumberShortKey
{
	ZQShortKeyType ShortKey;
	char Symbol[100];
	char Comment[100];

	tagNumberShortKey()
	{
		memset(this, 0, sizeof(tagNumberShortKey));
	}
}NumberShortKey;

typedef struct tagSystemParamInfo
{
	zq_bool bWebCifco;			  //��¼�Ƿ񵯳��������ڿͻ�����ƽ̨
	zq_bool bUse;               //ʹ�������ȼ�
	zq_int prompt;              //��ʾ�� 1.���� 2.pC���� 3.��ý������
	zq_char32  key1;       //F1~F12
	zq_char32  key2;       //Alt��     �գ���ѡ�˼�
	zq_char32  key3;       //Ctrl��    �գ���ѡ�˼�
	zq_char32  key4;       //Shift��   �գ���ѡ�˼�
	zq_char32  key5;       //Window��  �գ���ѡ�˼�

	zq_int nTradeTimeSoundPrompt;	//�������̼�С����Ϣ��������ʾ
	zq_int nTradeEndSoundPrompt;	//�гֲֵĺ�Լ���׽���ǰ��������ʾ
	zq_int nSoundPromptBeforeTradeEndSec;	//�гֲֵĺ�Լ���׽���ǰ������ʾ��ǰ����

	tagSystemParamInfo()
	{
		memset(this, 0, sizeof(tagSystemParamInfo));
	}
}SystemParamInfo;

typedef struct tagMovePosition
{
	zq_int nSourcePriceLimit;		//ԭʼ��Լ�޼ۼ۸�0,�м� 1,���¼� 2������
	zq_int nTargetPriceLimit;		//Ŀ���Լ�޼ۼ۸�
	zq_int nSourcePriceOffset;		//ԭʼ��Լ�����۲�
	zq_int nTargetPriceOffset;		//Ŀ���Լ�����۲�

	zq_int nOperateOrder;			//����˳��
	zq_int nOutTimeSec;			//��ʱʱ��
	zq_int nRetryTimes;			//���Դ���

	zq_int nSourceNotDealAction;	//ԭʼ��Լδ�ɽ���������0,�޶��� 1,�����¼۸����ĵ� 2,�������۸����ĵ� 3,���м۸ĵ� 4,����
	zq_int nTargetNotDealAction;	//Ŀ���Լδ�ɽ�������
	zq_int nSourceNotDealPriceOffset;	//ԭʼ��Լδ�ɽ��������۲�
	zq_int nTargetNotDealPriceOffset;	//Ŀ���Լδ�ɽ��������۲�
}MOVEPOSITIONCFG;

typedef struct tagReplacePosition
{
	zq_int nClosePositionPriceLimit1;		//��λ1ƽ���޼�
	zq_int nClosePositionPriceOffset1;		//��λ1ƽ�ֻ����۲�
	zq_int nOpenPositionPriceLimit1;		//��λ1�����޼�
	zq_int nOpenPositionPriceOffset1;		//��λ1���ֻ����۲�

	zq_int nClosePositionPriceLimit2;		//��λ2ƽ���޼�
	zq_int nClosePositionPriceOffset2;		//��λ2ƽ�ֻ����۲�
	zq_int nOpenPositionPriceLimit2;		//��λ2�����޼�
	zq_int nOpenPositionPriceOffset2;		//��λ2���ֻ����۲�

	zq_int nOperateOrder;					//����˳��
	zq_int nOutTimeSec;					//��ʱʱ��
	zq_int nRetryTimes;					//���Դ���

	zq_int nOpenPositionNotDealAction1;	//��λ1����δ�ɽ�������
	zq_int nOpenPositionNotDealPriceOffset1;	//��λ1����δ�ɽ��������۲�
	zq_int nClosePositionNotDealAction1;	//��λ1ƽ��δ�ɽ�������
	zq_int nClosePositionNotDealPriceOffset1;	//��λ1ƽ��δ�ɽ��������۲�

	zq_int nOpenPositionNotDealAction2;	//��λ2����δ�ɽ�������
	zq_int nOpenPositionNotDealPriceOffset2;	//��λ2����δ�ɽ��������۲�
	zq_int nClosePositionNotDealAction2;	//��λ2ƽ��δ�ɽ�������
	zq_int nClosePositionNotDealPriceOffset2;	//��λ2ƽ��δ�ɽ��������۲�
}REPLACEPOSITION;

struct AccountStru
{
	int nID;
	char szAccount[64];
};

enum OrderStrategy
{
	BaseMultipliedWay,
	UserDefinedVolumnWay,
	SumDividedWay,
	FirstAccountAlign,
	AccountScaleDivided,
};

enum QuickOrderStrategy
{
	QOrder_OpenDefaultAndStrategy = 1,		//��Ĭ�������Ͳ��Կ���
	QOrder_CloseAll,						//���ֲ���ȫƽ
	QOrder_AutoCloseDefaultAndNotOpen,		//�в��˻���Ĭ�������Ͳ���ƽ�֣������������޲��˻�������
	QOrder_AutoReverseOpenAndNotOpen,		//�в��˻���Ĭ�������Ͳ���ƽ�֣��������򿪲֣��޲��˻�������
	QOrder_AutoCloseDefaultAndOpen,			//��Ĭ�������Ͳ��ԣ��в��˻�ƽ�֣������������޲��˻�����
	QOrder_AutoReverseOpenAndOpen,			//��Ĭ�������Ͳ��ԣ��в��˻�ƽ�֣��������򿪲֣��޲��˻�����
	QOrder_AutoCloseDefault,				//��Ĭ�������Ͳ���ƽ�֣�����������
	QOrder_AutoReverseOpen,					//��Ĭ�������Ͳ���ƽ�֣��������򿪲�
};

typedef struct tagMULTIACCOUNT
{
	zq_int nSaveType;		//�Զ������˺�����
	zq_int nLoadType;		//�Զ���ȡ�˺�����
	zq_int nSaveTypeManual;		//�Զ������˺�����
	zq_int nLoadTypeManual;		//�Զ���ȡ�˺�����
	zq_int nStrategyItem;	//�µ���������
	zq_int nFirstAccountAlign;	//���ʽ������˻�����
	zq_int nAccountScaleDivided;	//���˻���ģ��������
}MULTIACCOUNT;

#include <iostream>
#include <sstream>
#include <limits>
#pragma warning( disable : 4996 )



#endif