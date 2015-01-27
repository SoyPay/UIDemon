#include "StdAfx.h"
#include "LoadLanguage.h"
#define         MAX_SECTION					260      //Section最大长度
#define         MAX_KEY						260      //KeyValues最大长度
#define         MAX_ALLSECTIONS				65535    //所有Section的最大长度
#define         MAX_ALLKEYS					65535    //所有KeyValue的最大长度

IMPLEMENT_SINGLETON(CLoadLanguage)
void CLoadLanguage::Init(const CString& strPath)
{
	m_strPath = strPath;
}

int CLoadLanguage::GetAllKeysAndValues(const CString& strSection,map<CString,CString>& mapData)
{
	return 0;
}

void CLoadLanguage::LoadSectionToMap(TCHAR* pData,map<CString,CString>&mapData)
{
	CString strKey;
	CString strValue;
	while (1)
	{
		CString strData = pData;
		if (strData.IsEmpty())
		{
			break;
		}

		int nPos = strData.Find('=');
		if (-1 == nPos)
		{
			break;
		}

		strKey = strData.Left(nPos);
		strValue = strData.Mid(nPos+1);
		mapData[strKey] = strValue;
		pData += strData.GetLength()+1;
	}
}

void CLoadLanguage::Load(const CString& strSection,CDialogEx* pDialog)
{
	static const int nSize = 4*1024;
	TCHAR szData[nSize] = {0};
	DWORD dwRes = GetPrivateProfileSection(strSection,szData,nSize-1,m_strPath);
	
	map<CString,CString> mapData;
	LoadSectionToMap(szData,mapData);
	for (map<CString,CString>::iterator it = mapData.begin();it != mapData.end();++it)
	{
		int nID = _ttoi(it->first);
		if (nID)
		{
			pDialog->GetDlgItem(nID)->SetWindowTextW(it->second);
		}
		else
		{
			pDialog->SetWindowTextW(it->second);
		}
		
	}
}