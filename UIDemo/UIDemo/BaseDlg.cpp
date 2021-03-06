// BaseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "BaseDlg.h"
#include "afxdialogex.h"
#include "UIDemoDlg.h"


// CBaseDlg 对话框

IMPLEMENT_DYNAMIC(CBaseDlg, CDialogEx)


CBaseDlg::CBaseDlg(UINT nIDD,CWnd *pParent) 
	: CDialogEx(nIDD, pParent)
{
	m_nHeight = 0;
	m_nTxFee = 0;
}

CBaseDlg::~CBaseDlg()
{
}

void CBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBaseDlg, CDialogEx)
END_MESSAGE_MAP()


// CBaseDlg 消息处理程序
void CBaseDlg::ConfirmGetScriptID()
{
	/*while (false == HasGetScriptID())
	{
	GetScriptID();
	Sleep(2000);
	}*/
}

void CBaseDlg::GetInfoToRegScript(CString& strAddr,int& nHeight,int& nFee,AUTHORIZATE& author)
{
	strAddr = CString(m_strScriptSignAddr.c_str());
	/*nHeight = m_nHeight;
	nFee = m_nTxFee;
	author = m_author;*/
}

void CBaseDlg::GetScriptID()
{
	CTestDlg* pDlg = (CTestDlg*)AfxGetApp()->m_pMainWnd;
	m_strScript = pDlg->GetScriptID();
}