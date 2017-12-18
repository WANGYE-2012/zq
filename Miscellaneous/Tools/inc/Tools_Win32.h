// Tools_Win32.h: interface for the CTools_Win32 class.
//
//	Author		:lzq
//				 Copyright (C), 2008-2028
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLS_WIN32_H__9AD36DCF_6C92_44A3_94C5_B08759A9FCD7__INCLUDED_)
#define AFX_TOOLS_WIN32_H__9AD36DCF_6C92_44A3_94C5_B08759A9FCD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//�Ƿ�ʹ��psapi��ö�ٽ���ID����ȡ����ģ�����ơ����ʹ�ã�����Ҫlink psapi.lib��Ĭ�ϲ�ʹ��
//#define _USE_PSAPI_

#pragma warning(disable : 4996)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)

#include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "public.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CTools_Win32  
{
public:
	CTools_Win32();
	virtual ~CTools_Win32();


public:

	static void PaintClientBackGroundWithBmp(HANDLE hDC,HANDLE hWnd,HANDLE hMainWnd,HANDLE hBmp,void *pDCRect=NULL);


	//	��hbmp�����ڵı������������ų��Ӵ�������
	//	hDC��hWnd����ͼ�Ĵ��ں;��
	//	hRefWnd���ο����ھ����hbmp�����ϽǶ��뵽hRefWnd�����Ͻ�
	static void PaintBackGroundWithBmp(HANDLE hDC,HANDLE hWnd,HANDLE hBmp,HANDLE hRefWnd);


	//	��MemDC�����ݿ�����hDC�ϣ��ų��Ӵ�������
	//	hDC��hWnd����ͼ�Ĵ��ں;��
	//	pDCRect:hDC��hWnd�ϵ�λ�ã�hDC��ԭ�����pDCRect��ԭ��
	//	MemDC��hWndΪԭ�㣬hDC��pDCRect��hWndΪԭ��
	static void MemDCTohDC(HANDLE hDC,HANDLE MemDC,HANDLE hWnd,void *pDCRect=NULL);

	//	��HDC����ָ��ˢ����䣬�ų��Ӵ�������
	//	hDC��hWnd����ͼ�Ĵ��ں;��
	static void FillRectExcludeChildren(HANDLE hDC,HANDLE hWnd,HANDLE hBr);


	//	ȡhbmp�Ŀ�Ⱥ͸߶ȣ�����Ϊ��λ
	static BOOL GetSizeOfBmp(HANDLE hBmp,int &BmpWid,int &BmpHei);

	static BOOL DrawhBmp(HDC hDC,int DstPox,int DstPoy,int DstWid,int DstHei,HBITMAP hBmp,int SrcPox,int SrcPoy);

	static BOOL DrawBmp(HDC hDC,char *BmpName,int ParentWid,int ParentHei);

	//	��Bitmap�ļ�����HBiTMAP
	static HBITMAP GetHBITMAPFromFile(HWND RefWnd,char *BmpName);

	//	��hbmpǿ��ת��Ϊ24λ��hbmp
	static HBITMAP Get24BitsHBitmap(HBITMAP hSourBmp,HWND RefWnd);

	static void GetBmpSize(HBITMAP hBmp,int *pWid,int *pHei,int *pBitsCount);

	//	��hDC�ϻ�hBmp���������hMaskBmp����ִ��Mask������Ŀ��ߴ�ΪWid��Hei���ĸ������Space ����ԭ��
	//	Ҫ���9��������bmp��4����ԭ���������������ߺ���Ŵ�������������Ŵ������ݺ�Ŵ�
	//	1 2 3
	//	4 5 6
	//	7 8 9
	//	Pox��Poy��Wid��Hei���ڷŴ���bmp��Ҫȡ��λ�ô�С
	//	hDC��hDCPox��hDCPoy�ǽ�bmpָ�����ֻ���hDC�е�ָ��λ����
	//	�˺����ǻ�����Bmp
	static void DrawEntireBmpWithStretchNoMask(HDC hDC,int hDCPox,int hDCPoy,HBITMAP hBmp,int DstWid,int DstHei,int LeftSpace,int RightSpace,int TopSpace,int BomSpace);

	//	��hDC�ϻ�hBmp���������hMaskBmp����ִ��Mask������Ŀ��ߴ�ΪWid��Hei���ĸ������Space ����ԭ��
	//	��hBmp�Ŵ�DstWid��DstHei��С��Ȼ��Pox\Poy\Wid\Hei���ֿ�����hDC��hDCPox/hDCPoy λ����
	//	Ҫ���9��������bmp��4����ԭ���������������ߺ���Ŵ�������������Ŵ������ݺ�Ŵ�
	//	1 2 3
	//	4 5 6
	//	7 8 9
	//	Pox��Poy��Wid��Hei���ڷŴ���bmp��Ҫȡ��λ�ô�С
	//	hDC��hDCPox��hDCPoy�ǽ�bmpָ�����ֻ���hDC�е�ָ��λ����
	static void DrawBmpWithStretch(HDC hDC,int hDCPox,int hDCPoy,int Pox,int Poy,int Wid,int Hei,HBITMAP hBmp,HBITMAP hMaskBmp,int DstWid,int DstHei,int LeftSpace,int RightSpace,int TopSpace,int BomSpace);

	//	����hBmp��Ŀ��ߴ�ΪWid��Hei���ĸ������Space ����ԭ��
	//	Ҫ���9��������bmp��4����ԭ���������������ߺ���Ŵ�������������Ŵ������ݺ�Ŵ�
	//	1 2 3
	//	4 5 6
	//	7 8 9
	//	hDC�ǲο���DC������hBmp��λ����
	static HBITMAP CreateStretchBmp(HDC hDC,HBITMAP hBmp,int DstWid,int DstHei,int LeftSpace,int RightSpace,int TopSpace,int BomSpace);


	static void		MyLineTo(HDC hDC,int StartX,int StartY,int EndX,int EndY,COLORREF LineCol);
	static inline void	MyLineTo2(HDC hDC,int StartX,int StartY,int EndX,int EndY) { MoveToEx(hDC,StartX,StartY,NULL);  LineTo(hDC,EndX,EndY); }
	static void		Set4Pixel(HDC hDC,int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,COLORREF Col);
	static BYTE		GetDeferentBetweenTwoColor(COLORREF Col1,COLORREF Col2);
	static BYTE		GetGrayValFromRGB(COLORREF iCol);
	static COLORREF	GetPixelColorAve(HDC hDC,int Pox,int Poy);
	static void		DrawWindowFram(HDC hDC,int Pox,int Poy,int Wid,int Hei,int LineWid,COLORREF *pCol1,COLORREF *pCol2);
	static void		Draw3DHoriLine(HDC hDC,int Pox,int Poy,int Wid,HPEN hPen1,HPEN hPen2,HPEN hPen3);
	static void		DrawRect(HDC hDC,RECT*Rect);
	static void		DrawLeavenRect(HDC hDC,RECT Rect,COLORREF TopCol,COLORREF BtnCol);
	static COLORREF	GetLeavenColor(COLORREF TopCol,COLORREF BtnCol,int Topy,int Btny,int Cury);
	static COLORREF	GetAveColor(COLORREF Col1,COLORREF Col2);


	static void SleepByEvt(int TimeoutMS);

	static void GetWorkAreaSize(int &ScnWid,int &ScnHei);
	static void GetMainPath(char *pMainPath,int iSize);
	static string GetMainPath();
	static void GetFileNameWithMainPath(char *pMainPath,int iSize,const char* pPureFileName);
	static void CheckMainPath(char *pRltMainPath,int iSize,const char *refMainPath);

	static void CreateMultipleDirectory(char *iPath);

	//	�ڴ����û�������ֵ�滻%%�����Ĳ���
	static void ReplaceWithEnvironmentVariable(char *pBuf,int BufSize);


	static void ScreenToClientRect(HWND hWnd,RECT *pRect);
	static void ClientToScreenRect(HWND hWnd,RECT *pRect);
	static void ResizeChildWindow(HWND hWnd,HWND hParentWnd,int NewWid,int NewHei,BOOL RePaintID);
	//	�ƶ�����λ�ú�/��ı䴰�ڴ�С��ʹ�����λ�ú���Գߴ磬�����Ǹ�ֵ
	static void MyMoveWindow(HWND hWnd,int DeltaPox,int DeltaPoy,int DeltaWid,int DeltaHei);		
	static int MyGetEditWndVal(HWND iWnd);
	static void MyGetEditWndStr(HWND iWnd,char *Buf,int BufSize);
	static void MySetEditWndVal(HWND iWnd,int Val);
	static BOOL IsFileUpdate(const char *FileName,struct _stat *pLastFileStat);

																
	//	ȡ��ǰCpu��Tick������
	static LONGLONG MyGetCpuTickCounter(void);						
	//	ȡ��ǰCpu��Tick��Ƶ�ʣ���λΪÿ����ٴ�
	static LONGLONG MyGetCpuTickFreq(void);							
	//	ȡ��ǰ�ĸ���ʱ�䣬64λ����32λΪtime_t����32λ�ĸ�16λΪ������������16λ��Ϊ��������-��������ͬ��ʱ������õļ�����
	static LONGLONG MyGetComplexTime_Exclusize(void);					

	//	��ȡ��ǰϵͳ�ڴ�ʹ������������ڴ�ʹ�ðٷֱȡ�ȫ���ڴ��С����ʹ���ڴ��С
	static void GetSystemMemeryUsage(DWORD *pCurUsagePercent,DWORD *pTotalMemSize,DWORD *pRltUsedMemSize);
																
	//	�жϲ���ϵͳ�Ƿ���WindowsNT���ϵİ汾
	static BOOL IsWindowsNt(void);	




	//	�����д�ź������ɹ�����ָ�룬ʧ�ܷ���0
	static char* AllocWinSem(void);									
	//	�ͷŶ�д�ź���
	static void ReleaseWinSem(char* pSem);							
	//	Ҫ���ռ�ź���
	static void TakeWinSem(char* pSem);								
	//	ֹͣ��ռ�ź���
	static void GiveWinSem(char* pSem);								

	static void GetClientSize(HWND hWnd,int* prltwid,int* prlthei);
	static int GetClientWid(HWND hWnd);
	static int GetClientHei(HWND hWnd);
	static void GetTextoutSize(HDC hDC,const char* pbuf,int buflen,int*prltwid,int*prlthei);
	static void GetTextoutSize2(HDC hDC,int textoutval,int*prltwid,int*prlthei);
	static SIZE GetTextoutSize3(HDC hDC,const char* pbuf,int buflen);
	static SIZE GetTextoutSize4(HDC hDC,int textoutval);

	//	ֹͣ�̣߳�ֹͣǰ���ȴ�timeoutseconds��
	static void StopThread(HANDLE hThread,int timeoutseconds=8);

	//GBK��UTF8ת��
	static string GBKToUTF8(const string& strGBK);
	static string UTF8ToGBK(const string& strUTF8);  

	//ͨ��EnumWindow�ķ��������Ҷ��㴰�ڡ���FindWindow��������Ҫƥ��hParent
	static HWND FindDesktopWindowByParent(HWND hParent,const char* ClassName,const char* WindowName);

	//��ȡָ��ģ�����ƵĽ���ID����������Ʋ�����·��
	static vector<DWORD> GetProcessIDByExeFileName(const char* ExeFileName);
	//ɱ�����̡���������Ʋ�����·��
	static bool KillProcess(const char* ExeFileName);
};


#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TOOLS_WIN32_H__9AD36DCF_6C92_44A3_94C5_B08759A9FCD7__INCLUDED_)
