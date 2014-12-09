// WalletDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "WalletDlg.h"
#include "afxdialogex.h"
#include "CommonStruct.h"


// CWalletDlg 对话框

IMPLEMENT_DYNAMIC(CWalletDlg, CDialogEx)

CWalletDlg::CWalletDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWalletDlg::IDD, pParent)
{
	m_bConnected = FALSE;
	m_szBuffer = new char[SOCKET_BUFFER];
}

CWalletDlg::~CWalletDlg()
{
	delete []m_szBuffer;
}

void CWalletDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_txList);
}


BEGIN_MESSAGE_MAP(CWalletDlg, CDialogEx)
	ON_MESSAGE(WM_SOCKET_RECEIVE,&OnSocketReceive)
	ON_MESSAGE(WM_SOCKET_CLOSE,&OnSocketClose)
	ON_MESSAGE(WM_SOCKET_CONNECT,&OnSocketConnnect)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWalletDlg 消息处理程序
void CWalletDlg::Init()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	m_txList.MoveWindow(10,rcClient.bottom/3,rcClient.Width()-20,rcClient.bottom*2/3-10);
	m_txList.GetClientRect(&rcClient);

	m_txList.InsertColumn( 0, _T("交易哈希"), LVCFMT_LEFT, rcClient.Width()/7 ); 
	m_txList.InsertColumn( 1, _T("block高度"), LVCFMT_LEFT, rcClient.Width()/7 );
	m_txList.InsertColumn( 2, _T("注册脚本ID"), LVCFMT_LEFT, rcClient.Width()/7 );
	m_txList.InsertColumn( 3, _T("账号ID"), LVCFMT_LEFT, rcClient.Width()/7 );
	m_txList.InsertColumn( 4, _T("收入"), LVCFMT_LEFT, rcClient.Width()/7 );
	m_txList.InsertColumn( 5, _T("支出"), LVCFMT_LEFT, rcClient.Width()/7 );
	m_txList.InsertColumn( 6, _T("手续费"), LVCFMT_LEFT, rcClient.Width()/7 );

	for(int i = 0;i<10;i++)
	{
		m_txList.InsertItem(i,_T("zcsf45f47e8f4dxzfedfresfd5"));
		m_txList.SetItemText(i,1,_T("32767"));
		m_txList.SetItemText(i,2,_T("888888"));
		m_txList.SetItemText(i,3,_T("666666"));
		m_txList.SetItemText(i,4,_T("+100"));
		m_txList.SetItemText(i,5,_T("-200"));
		m_txList.SetItemText(i,6,_T("10"));
	}
	m_txList.InsertColumn( 0, _T("block高度"), LVCFMT_LEFT, 50);

	SetTimer(1,3000,NULL);
}

BOOL CWalletDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE; 
}

LRESULT CWalletDlg::OnSocketConnnect(WPARAM wparam,LPARAM lparam)
{
	return TRUE;
}

LRESULT CWalletDlg::OnSocketClose(WPARAM wparam,LPARAM lparam)
{
	m_MsgSocket.Close();
	m_bConnected  = FALSE;
	return TRUE;
}

LRESULT CWalletDlg::OnSocketReceive(WPARAM wparam,LPARAM lparam)
{
	memset(m_szBuffer,0,SOCKET_BUFFER);
	int bufferSize = SOCKET_BUFFER - 1;
	int iRcvd; 
	CStringA str;

	iRcvd = m_MsgSocket.Receive(m_szBuffer,bufferSize);
	if (iRcvd > 0)
	{
		CString strData(m_szBuffer);
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText(strData);
	}

	return TRUE;
}

void CWalletDlg::ConnetSever()
{
	if (m_bConnected)
	{
		return;
	}

	int err = 0;
	m_MsgSocket.m_hSocket = INVALID_SOCKET;

	if(m_MsgSocket.Create())
	{
		err = m_MsgSocket.Connect(_T("127.0.0.1"),3200);
		if (err != SOCKET_ERROR && err != EWOULDBLOCK)
		{
			m_bConnected = true;
		}
	}
}

void CWalletDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	m_MsgSocket.SetParent(this);
	ConnetSever();

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CWalletDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	map<CString,CString>* pMap = (map<CString,CString>*)wparam;
	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_KEY);
	ASSERT(pMap && pCombox);

	for(map<CString,CString>::iterator it = pMap->begin();it != pMap->end();++it)
	{
		pCombox->AddString(it->second);
	}
	
	pCombox->SetCurSel(0);
	return TRUE;
}