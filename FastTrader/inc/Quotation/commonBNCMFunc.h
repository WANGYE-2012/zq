
#pragma once


const int s_nReplationPersonTypeCount = 4;
const char s_strRelationPersonType[][64] = {
	TEXT("1 ָ���´���"), 
	TEXT("2 �ʽ������"), 
	TEXT("3 ���˴���"), 
	TEXT("4 ������Ȩ��") 
};


#define TRANSCURRFORM(pCurrForm, nModuleNum) 	if(pCurrForm!=NULL) {\
		if(pCurrForm->GetModuleNum() == nModuleNum) {\
			return;\
		}\
		if(pCurrForm->isChanged_Item()) {\
			if(AfxMessageBox("��ǰ�������ݸı�δ���棬�Ƿ��뿪��ǰ���壿", MB_YESNO|MB_ICONQUESTION)==IDNO) {\
				return;\
			}\
		}\
		delete pCurrForm;\
		pCurrForm = NULL;\
	}


int GetPackageHeadLen();
int GetPackageHeadTailLen();
bool PackageData(UINT nCMDID, UINT nModuleID, int nContentLen, char* pBuff, int nBuffLen, 
				 UINT seq=0, UINT subseq=0, UINT nData1=0, UINT nData2=0, UINT nData3=0, UINT nData4=0);