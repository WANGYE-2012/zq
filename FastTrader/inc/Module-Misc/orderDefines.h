#pragma once

// �µ�ʹ�õ����ݽṹ


typedef struct {
	LPVOID		pWin;
	int			left;
	int			top;
	int			width;
	int			height;
	bool		isShow;
	bool		isEnable;
} UIENTITY, *PUIENTITY;

enum {
	conSubStyle_Std,
	conSubStyle_Batch,
	conSubStyle_Condition,
	conSubStyle_Position,
	conSubStyle_Stop,
	conSubStyle_FAKFOK
};

enum {
	conUI_WDList,					// �嵵�����б��
	conUI_StdInput,					// ��׼�����
	conUI_Command,					// ��ť���ɿ�
	conUI_RealMsg,					// ��Ϣ��ʾ���
	conUI_Batch,					// ��������
	conUI_Condition,				// ��������
	conUI_Stop,						// ֹͣ����
	conUI_Position,					// ӯ�𵥿�
	conUI_CheckWD,					// Check�嵵����
	conUI_CheckTopMost,				// Checkǰ�˴���
	conUI_CheckPosition,			// Checkֹӯֹ��
	conUI_CheckCondition,			// Check����ί��
	conUI_PatsInput,				// Pats�����
	conUI_FAKFOK,					// FAKFOK����
	conUI_MAX
};

// �µ�����
enum {
	OrderType_FT_Unknow,
	OrderType_FT_Immediately,						// ����
	OrderType_FT_ImmeFAK,							// ����FAK
	OrderType_FT_ImmeFOK,							// ����FOK
	OrderType_FT_Touch,								// ֹ��
	OrderType_FT_TouchProfit,						// ֹӮ
	OrderType_FT_ParkedOrder,						// Ԥ��
	OrderType_FT_LastPriceGreaterThanStopPrice=50,	///���¼۴���������
	OrderType_FT_LastPriceGreaterEqualStopPrice,	///���¼۴��ڵ���������
	OrderType_FT_LastPriceLesserThanStopPrice,		///���¼�С��������
	OrderType_FT_LastPriceLesserEqualStopPrice,		///���¼�С�ڵ���������
	OrderType_FT_AskPriceGreaterThanStopPrice,		///��һ�۴���������
	OrderType_FT_AskPriceGreaterEqualStopPrice,		///��һ�۴��ڵ���������
	OrderType_FT_AskPriceLesserThanStopPrice,		///��һ��С��������
	OrderType_FT_AskPriceLesserEqualStopPrice,		///��һ��С�ڵ���������
	OrderType_FT_BidPriceGreaterThanStopPrice,		///��һ�۴���������
	OrderType_FT_BidPriceGreaterEqualStopPrice,		///��һ�۴��ڵ���������
	OrderType_FT_BidPriceLesserThanStopPrice,		///��һ��С��������
	OrderType_FT_BidPriceLesserEqualStopPrice		///��һ��С�ڵ���������
};


// PATS�µ�����
enum {
	OrderType_PATS_LIMIT,							// �޼�
	OrderType_PATS_MARKET,							// �м�
	OrderType_PATS_STOP,							// ֹͣ��
	OrderType_PATS_STOPLIMIT						// ֹͣ�޼۵�
};


#define VK_TAB		0x09  //TAB key  
#define VK_RETURN	0x0D  //ENTER key  
#define VK_LEFT		0x25  //LEFT ARROW key  
#define VK_UP		0x26  //UP ARROW key  
#define VK_RIGHT	0x27  //RIGHT ARROW key  
#define VK_DOWN		0x28  //DOWN ARROW key  


// ��׼�����UIID
enum
{
    ID_OISTD_FIRST=31000,
	ID_OISTD_INSTRUMENTID_TEXT,
	ID_OISTD_INSTRUMENTID_COMBOCTRL,
	ID_OISTD_ACCOUNT_COMBOCTRL,
    ID_OISTD_PRICE_TEXT,
    ID_OISTD_VOLUME_TEXT,
	ID_OISTD_BUYSELL_COMBOBOX,
	ID_OISTD_OPENCLOSE_COMBOBOX,
    ID_OISTD_PRICE_SPINCTRL,
	ID_OISTD_VOLUME_SPINCTRL,
	ID_OISTD_STATIC1,
	ID_OISTD_STATIC2,
	ID_OISTD_STATIC3,
	ID_OISTD_STATIC4,
    ID_OISTD_AUTOOPENCLOSE_STATIC,
    ID_OISTD_AUTOTRACKPRICE_STATIC,
    ID_OISTD_HEDGE_CHECKBOX,
	SUMNUM_OISTD_ID	// ��־ID�ŵ�����
};


// �µ����İ�ťUIID
enum
{
	ID_BUTTON_OK=31100,
	ID_BUTTON_CANCEL,
	ID_BUTTON_MARKETPRICE,
	ID_BUTTON_BATCHORDER,
	ID_BUTTON_CONDITIONORDER,
	ID_BUTTON_POSITIONORDER,
	ID_BUTTON_PARKEDORDER,
	ID_BUTTON_STOPORDER,
	ID_BUTTON_PATSORDER,
	ID_BUTTON_FAKFOKORDER,
	ID_BUTTON_MAX
};


// �µ���������UIID
enum
{
	ID_STATIC1=31200,
	ID_CTL_WDSHOW,
	ID_CHECK_WDSHOW,
	ID_CHECK_WINDOWTOPALWAYS,
	ID_DOSTOPPOSITION_CHECK,
	ID_DOCONDITION_CHECK,
	ID_QUOTTIMER,
	SUMNUM_ID	// ��־ID�ŵ�����
};


// ʵʱ��Ϣ���UIID
enum
{
	ID_LISTORDERREALMSG=31300
};


// ���������UIID
enum
{
	ID_OBATCH_RADIOINPUTPRICE=31400,
	ID_OBATCH_RADIONEWPRICE,
	ID_OBATCH_RADIOBSPRICE,
	ID_OBATCH_SPINCTRLVOLUME,
	ID_OBATCH_SPINCTRLTIMER,
	ID_OBATCH_STATICE1,
	ID_OBATCH_STATICE2,
	OBATCHSUMNUM_ID	// ��־ID�ŵ�����
};


// ���������UIID
enum
{
	ID_CONDITION_RADIONEWPRICE=31500,
	ID_CONDITION_RADIOBUYPRICE,
	ID_CONDITION_RADIOSELLPRICE,
	ID_CONDITION_COMBOCONDITION,
	ID_CONDITION_TEXTPRICE,
	ID_CONDITION_SPINCTRLBUTTON,
	ID_CONDITION_CHECKTOSERVER,
	OCONDITIONSUMNUM_ID	// ��־ID�ŵ�����
};


// ӯ�𵥰��UIID
enum
{
	ID_POSITION_CHECKSTOPLOSE=31600,
	ID_POSITION_CHECKSTOPGAIN,
	ID_POSITION_COMBOPRICETYPE,
	ID_POSITION_RADIONEWPRICE,
	ID_POSITION_RADIOBSPRICE,
	ID_POSITION_RADIOTOPRICELIMIT,
	ID_POSITION_RADIOTOPRICEOFFSET,
	ID_POSITION_TEXTSTOPLOSEPRICEOFFSET,
	ID_POSITION_TEXTSTOPGAINPRICEOFFSET,
	ID_POSITION_TEXTDOPRICEOFFSET,
	ID_POSITION_SPINCTRLSTOPLOSEPRICEOFFSET,
	ID_POSITION_SPINCTRLSTOPGAINPRICEOFFSET,
	ID_POSITION_SPINCTRLDOPRICEOFFSET,
	ID_POSITION_CHECKTOSERVER,
	ID_POSITION_STATIC1,
	ID_POSITION_STATIC2,
	ID_POSITION_STATIC3,
	ID_POSITION_STATIC4,
	OPOSITIONSUMNUM_ID	// ��־ID�ŵ�����
};


// ֹͣ�����UIID
enum
{
	ID_STOP_RADIOSTOP=31700,
	ID_STOP_RADIOSTOPLIMIT,
	ID_STOP_TEXTPRICE,
	ID_STOP_SPINPRICE,
	ID_STOP_TEXTTRIGGERPRICE,
	ID_STOP_SPINTRIGGERPRICE,
	ID_STOP_STATICPRICE,
	ID_STOP_STATICTRIGGERPRICE,
	OSTOPSUMNUM_ID	// ��־ID�ŵ�����
};


// PATS��׼�����UIID
enum
{
    ID_OIPATS_FIRST=31800,
	ID_OIPATS_INSTRUMENTID_TEXT,
	ID_OIPATS_INSTRUMENTID_COMBOCTRL,
	ID_OIPATS_ACCOUNT_COMBOCTRL,
    ID_OIPATS_PRICE_TEXT,
    ID_OIPATS_LIMITPRICE_TEXT,
    ID_OIPATS_VOLUME_TEXT,
	ID_OIPATS_ORDERTYPE_COMBOBOX,
	ID_OIPATS_BUYSELL_COMBOBOX,
	ID_OIPATS_OPENCLOSE_COMBOBOX,
    ID_OIPATS_PRICE_SPINCTRL,
    ID_OIPATS_LIMITPRICE_SPINCTRL,
	ID_OIPATS_VOLUME_SPINCTRL,
	ID_OIPATS_INSTRUMENTID_STATIC,
	ID_OIPATS_ORDERTYPE_STATIC,
	ID_OIPATS_ACCOUNT_STATIC,
	ID_OIPATS_BUYSELL_STATIC,
    ID_OIPATS_AUTOOPENCLOSE_STATIC,
    ID_OIPATS_AUTOTRACKPRICE_STATIC,
	ID_OIPATS_AUTOTRACKLIMITPRICE_STATIC,
	ID_OIPATS_VOLUME_STATIC,
    ID_OIPATS_HEDGE_CHECKBOX,
	SUMNUM_OIPATS_ID	// ��־ID�ŵ�����
};


// FAKFOK���UIID
enum
{
	ID_FAKFOK_RADIOFAK=31900,
	ID_FAKFOK_RADIOFOK,
	ID_FAKFOK_SPINCTRLMINVOLUME,
	ID_FAKFOK_STATICE1,
	SUMNUM_OIFAKFOK_ID	// ��־ID�ŵ�����
};


#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) 
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1) 


#define ROW_HEIGHT			14
#define ROW_WIDTH			12
#define WDPANEL_WIDTH		160
#define ORDERPANEL_WIDTH	180


#define HWND_TOPMOST		((HWND)-1)
#define HWND_NOTOPMOST		((HWND)-2)
#define SWP_NOSIZE			0x0001
#define SWP_NOMOVE			0x0002

