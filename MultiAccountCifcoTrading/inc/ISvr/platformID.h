#pragma once
typedef enum _tagPTYPE
{
	PTYPE_NULL,
	PTYPE_CTP,
	PTYPE_PATSAPI,
	PTYPE_CME,
}PTYPE;
//�ײ�ƽ̨����ID
//#define PID_NULL	0
//#define PID_CME	    1
//#define PTYPE_CTP		2
//#define PTYPE_PATSAPI	3
//
//
////����Ĭ��ƽ̨IDΪ��
//#define  PLATFORM_DEFAULT  PID_NULL
////���������_PID_CME_ʹ��CMEƽ̨
//#ifdef  _PID_CME_
//	#ifdef PLATFORM_DEFAULT
//	#undef PLATFORM_DEFAULT
//	#endif
//	#define PLATFORM_DEFAULT  PID_CME
//#endif
////���������_PID_CTP_ʹ���ۺϽ���ƽ̨ƽ̨
//#ifdef  _PID_CTP_
//	#ifdef PLATFORM_DEFAULT
//	#undef PLATFORM_DEFAULT
//	#endif
//	#define PLATFORM_DEFAULT  PTYPE_CTP
//#endif
////���������_PTYPE_PATSAPI_ʹ��PATSAPIƽ̨
//#ifdef  _PTYPE_PATSAPI_
//	#ifdef PLATFORM_DEFAULT
//	#undef PLATFORM_DEFAULT
//	#endif
//	#define PLATFORM_DEFAULT  PTYPE_PATSAPI
//#endif
