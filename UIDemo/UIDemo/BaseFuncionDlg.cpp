// BaseFuncionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "BaseFuncionDlg.h"
#include "afxdialogex.h"
#include "CommonStruct.h"


// CBaseFuncionDlg 对话框

IMPLEMENT_DYNAMIC(CBaseFuncionDlg, CDialogEx)

CBaseFuncionDlg::CBaseFuncionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBaseFuncionDlg::IDD, pParent)
	, m_strScriptTx_Acc(_T(""))
	, m_strScriptTx_Fee(_T(""))
	, m_strScriptTx_Height(_T(""))
	, m_strScriptTx_Script(_T(""))
	, m_strAccTx_Height(_T(""))
	, m_strAccTx_Fee(_T(""))
	, m_strAccTx_Acc(_T(""))
	, m_strFrom(_T(""))
	, m_strTo(_T(""))
	, m_nMoney(0)
	, m_nHeight(0)
{
	m_nType = emTransfer;
}

CBaseFuncionDlg::~CBaseFuncionDlg()
{
}

void CBaseFuncionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_strScriptTx_Acc);
	DDX_Text(pDX, IDC_EDIT_FEE, m_strScriptTx_Fee);
	DDX_Text(pDX, IDC_EDIT_VALID_HEIGHT, m_strScriptTx_Height);
	DDX_Text(pDX, IDC_EDIT_SCRIPT, m_strScriptTx_Script);
	DDX_Text(pDX, IDC_EDIT_ACCTX_HEIGHT, m_strAccTx_Height);
	DDX_Text(pDX, IDC_EDIT_ACCTX_FEE, m_strAccTx_Fee);
	DDX_Text(pDX, IDC_EDIT_ACCTX_ACC, m_strAccTx_Acc);
	DDX_Text(pDX, IDC_EDIT_FROM, m_strFrom);
	DDX_Text(pDX, IDC_EDIT_TO, m_strTo);
	DDX_Text(pDX, IDC_EDIT_MONEY, m_nMoney);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
}


BEGIN_MESSAGE_MAP(CBaseFuncionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_START_MINE, &CBaseFuncionDlg::OnBnClickedBtnStartMine)
	ON_BN_CLICKED(IDC_BTN_STOP_MINE, &CBaseFuncionDlg::OnBnClickedBtnStopMine)
	ON_BN_CLICKED(IDC_RADIO_TRANSFER, &CBaseFuncionDlg::OnBnClickedRadioTransfer)
	ON_BN_CLICKED(IDC_RADIO_SCRIPT, &CBaseFuncionDlg::OnBnClickedRadioScript)
	ON_BN_CLICKED(IDC_RADIO_ACCOUNT, &CBaseFuncionDlg::OnBnClickedRadioAccount)
	ON_BN_CLICKED(IDC_BTN_SEND, &CBaseFuncionDlg::OnBnClickedBtnSend)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_BN_CLICKED(IDC_BTN_SCAN, &CBaseFuncionDlg::OnBnClickedBtnScan)
END_MESSAGE_MAP()


// CBaseFuncionDlg 消息处理程序


void CBaseFuncionDlg::OnBnClickedBtnStartMine()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	Setgenerate(true);
}


void CBaseFuncionDlg::OnBnClickedBtnStopMine()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	Setgenerate(false);
}

BOOL CBaseFuncionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_strFrom = _T("5zQPcC1YpFMtwxiH787pSXanUECoGsxUq3KZieJxVG");
	m_strTo = _T("5yNhSL7746VV5qWHHDNLkSQ1RYeiheryk9uzQG6C5d");

	m_strScriptTx_Fee = _T("10000");
	m_strScriptTx_Height = _T("10");
	m_strScriptTx_Acc = _T("5zQPcC1YpFMtwxiH787pSXanUECoGsxUq3KZieJxVG");
	m_strScriptTx_Script = _T("D:\\cppwork\\vmsdk\\SecuredTrade\\Debug\\Exe\\SecuredTrade.bin");

	m_strAccTx_Fee = _T("0");
	m_strAccTx_Height = _T("10");
	m_strAccTx_Acc = _T("5zQPcC1YpFMtwxiH787pSXanUECoGsxUq3KZieJxVG");
	UpdateData(FALSE);

	UpdateSelectState(emAccount);
	return TRUE;  
}

void CBaseFuncionDlg::EnableAccountTx(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_ACCTX_ACC)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_ACCTX_FEE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_ACCTX_HEIGHT)->EnableWindow(bEnable);
}

void CBaseFuncionDlg::EnableTransferTx(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_FROM)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_TO)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MONEY)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(bEnable);
}

void CBaseFuncionDlg::EnableScriptTx(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_ACCOUNT)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FEE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_VALID_HEIGHT)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SCRIPT)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_VALIDITY)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MAX_PER_TIME)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MAX_TOTAL)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MAX_PER_DAY)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_USER_DATA)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SCAN)->EnableWindow(bEnable);
}

void CBaseFuncionDlg::OnBnClickedRadioTransfer()
{
	UpdateSelectState(emTransfer);
}


void CBaseFuncionDlg::OnBnClickedRadioScript()
{
	UpdateSelectState(emScript);
}


void CBaseFuncionDlg::OnBnClickedRadioAccount()
{
	UpdateSelectState(emAccount);
}

void CBaseFuncionDlg::UpdateSelectState(emType type)
{
	EnableScriptTx(FALSE);
	EnableTransferTx(FALSE);
	EnableAccountTx(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_SCRIPT))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_TRANSFER))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_ACCOUNT))->SetCheck(FALSE);

	switch (type)
	{
	case emTransfer:
		EnableTransferTx(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_TRANSFER))->SetCheck(TRUE);
		m_nType = emTransfer; 
		break;

	case emScript:
		EnableScriptTx(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_SCRIPT))->SetCheck(TRUE);
		m_nType = emScript;
		break;

	case emAccount:
		EnableAccountTx(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_ACCOUNT))->SetCheck(TRUE);
		m_nType = emAccount;
		break;

	default:
		ASSERT(0);
		break;
	}
}

void CBaseFuncionDlg::OnBnClickedBtnSend()
{
	USES_CONVERSION;
	UpdateData(TRUE);

	switch (m_nType)
	{
	case emTransfer:
		{
			CreateNomalTx(W2A(m_strFrom),W2A(m_strTo),m_nMoney,0,m_nHeight);
		}
		break;

	case emScript:
		{
			CreateScriptTx(W2A(m_strScriptTx_Acc),false,W2A(m_strScriptTx_Script)
				,_ttoi(m_strScriptTx_Fee),_ttoi(m_strScriptTx_Height));
		}
		break;

	case emAccount:
		{
			CreateScriptAccountTx(W2A(m_strAccTx_Acc),_ttoi(m_strAccTx_Fee),_ttoi(m_strAccTx_Height));
		}
		break;

	default:
		ASSERT(0);
		break;
	}
}

LRESULT CBaseFuncionDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	CString strShow;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SHOW);
	strShow += CString(str.GetString());
	pEdit->SetWindowTextW(strShow);
	return TRUE;
}


void CBaseFuncionDlg::OnBnClickedBtnScan()
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,_T("脚本文件(*.bin)|*.bin|所有文件(*.*)|*.*||"));
	if (IDOK == dlg.DoModal())
	{
		m_strScriptTx_Script = dlg.GetPathName();
		UpdateData(FALSE);
	}
}
