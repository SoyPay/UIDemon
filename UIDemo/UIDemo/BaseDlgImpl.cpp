#include "StdAfx.h"
#include "BaseDlgImpl.h"


CBaseDlgImpl::CBaseDlgImpl(void)
{
}


CBaseDlgImpl::~CBaseDlgImpl(void)
{
}

void CBaseDlgImpl::GetScriptID()
{
	CString strCommand = _T("getscriptid ");
	CString strHash(m_strRegisterScriptHash.c_str());
	strCommand += strHash;
	CStringA strSendData;
	RPCCommandToJson(strCommand,strSendData);
	AfxGetApp()->m_pMainWnd->SendMessage(WM_SEND_RPC_DATA,(WPARAM)&strSendData,emGetRegScriptID);
}

bool CBaseDlgImpl::HasGetScriptID()
{
	return false == m_strScript.IsEmpty();
}

void CBaseDlgImpl::ConfirmGetScriptID()
{
	while (false == HasGetScriptID())
	{
		GetScriptID();
		Sleep(2000);

		Setgenerate(true);
		Sleep(2000);
	}
}