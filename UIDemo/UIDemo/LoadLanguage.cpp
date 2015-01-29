#include "StdAfx.h"
#include "LoadLanguage.h"


IMPLEMENT_SINGLETON(CLoadLanguage)
void CLoadLanguage::Init(const CString& strPath)
{
	m_strPath = strPath;
	LoadTips();
}

void CLoadLanguage::LoadSectionToMap(TCHAR* pData,map<int,CString>&mapData)
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
		mapData[_ttoi(strKey)] = strValue;
		pData += strData.GetLength()+1;
	}
}

void CLoadLanguage::Load(const CString& strSection,CDialogEx* pDialog)
{
	static const int nSize = 4*1024;
	TCHAR szData[nSize] = {0};
	DWORD dwRes = GetPrivateProfileSection(strSection,szData,nSize-1,m_strPath);
	
	map<int,CString> mapData;
	LoadSectionToMap(szData,mapData);
	for (map<int,CString>::iterator it = mapData.begin();it != mapData.end();++it)
	{
		int nID = it->first;
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

void CLoadLanguage::LoadTips()
{
	TCHAR szData[MAX_PATH+500] = {0};
	DWORD dwRes = GetPrivateProfileSection(_T("tip"),szData,MAX_PATH+500-1,m_strPath);

	LoadSectionToMap(szData,m_mapTips);
}

CString CLoadLanguage::GetTips(int nTipID)
{
	auto it = m_mapTips.find(nTipID);
	if (it == m_mapTips.end())
	{
		return CString(_T(""));
	}
	else
	{
		return m_mapTips[nTipID];
	}
}