#pragma once
//�ײ�ƽ̨����Type
typedef enum _tagPTYPE
{
	PTYPE_NULL,
	PTYPE_CTP,
	PTYPE_PATSAPI,
	PTYPE_CME,
}ePLATFORMTYPE;
//#define PTYPE_NULL	0
//#define PTYPE_CME	    1
//#define PTYPE_CTP		2
//#define PTYPE_PATSAPI	3


//����Ĭ��ƽ̨TYPEΪ��
#define  PLATFORM_DEFAULT  PTYPE_NULL
//���������_PTYPE_CME_ʹ��CMEƽ̨
#ifdef  _PTYPE_CME_
	#ifdef PLATFORM_DEFAULT
	#undef PLATFORM_DEFAULT
	#endif
	#define PLATFORM_DEFAULT  PTYPE_CME
#endif
//���������_PTYPE_CTP_ʹ���ۺϽ���ƽ̨ƽ̨
#ifdef  _PTYPE_CTP_
	#ifdef PLATFORM_DEFAULT
	#undef PLATFORM_DEFAULT
	#endif
	#define PLATFORM_DEFAULT  PTYPE_CTP
#endif
//���������_PTYPE_PATSAPI_ʹ��PATSAPIƽ̨
#ifdef  _PTYPE_PATSAPI_
	#ifdef PLATFORM_DEFAULT
	#undef PLATFORM_DEFAULT
	#endif
	#define PLATFORM_DEFAULT  PTYPE_PATSAPI
#endif
