#pragma once
#include "CommonStruct.h"
#define SELELT_APP_TO_REGISTER	0
#define INPUT_APP_ID			1
#define INVALID_APP_ID			2
#define INVALID_BUYER_ID		3
#define INVALID_SELLER_ID		4
#define INVALID_AR_ID			5
#define INVALID_BET_MONEY		6
#define EMPTY_RANDOM_DATA		7
#define INVALID_HEIGHT			8
#define INVALID_EXE_PATH		9
#define WIN_TITAL_ID       		10
class CLoadLanguage
{
DEFINE_SINGLETON(CLoadLanguage)
private:
	void LoadTips();
	void LoadSectionToMap(TCHAR* pData,map<int,CString>&mapData);

public:
	void Init(const CString& strPath);
	void Load(const CString& strSection,CDialogEx* pDialog);
	CString GetTips(int nTipID);

private:
	CString m_strPath;
	map<int,CString> m_mapTips;
};

