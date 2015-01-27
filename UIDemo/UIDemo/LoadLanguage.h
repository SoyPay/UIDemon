#pragma once
#include "CommonStruct.h"
class CLoadLanguage
{
DEFINE_SINGLETON(CLoadLanguage)
private:
	void LoadSectionToMap(TCHAR* pData,map<CString,CString>&mapData);

public:
	void Init(const CString& strPath);
	void Load(const CString& strSection,CDialogEx* pDialog);
	int GetAllKeysAndValues(const CString& strSection,map<CString,CString>& mapData);

private:
	CString m_strPath;
};

