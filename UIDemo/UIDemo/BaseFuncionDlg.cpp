// BaseFuncionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "BaseFuncionDlg.h"
#include "afxdialogex.h"
#include "CommonStruct.h"
#include "LoadLanguage.h"


// CBaseFuncionDlg 对话框

IMPLEMENT_DYNAMIC(CBaseFuncionDlg, CDialogEx)

CBaseFuncionDlg::CBaseFuncionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBaseFuncionDlg::IDD, pParent)
	, m_fAccTx_Fee(0)
	, m_strAccTx_Acc(_T(""))
	, m_strFrom(_T(""))
	, m_strTo(_T(""))
	, m_nMoney(0)
{
	m_nType = emTransfer;
}

CBaseFuncionDlg::~CBaseFuncionDlg()
{
}

void CBaseFuncionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACCTX_FEE, m_fAccTx_Fee);
	DDX_Text(pDX, IDC_EDIT_ACCTX_ACC, m_strAccTx_Acc);
	DDX_Text(pDX, IDC_EDIT_FROM, m_strFrom);
	DDX_Text(pDX, IDC_EDIT_TO, m_strTo);
	DDX_Text(pDX, IDC_EDIT_MONEY, m_nMoney);
}


BEGIN_MESSAGE_MAP(CBaseFuncionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_START_MINE, &CBaseFuncionDlg::OnBnClickedBtnStartMine)
	ON_BN_CLICKED(IDC_BTN_STOP_MINE, &CBaseFuncionDlg::OnBnClickedBtnStopMine)
	ON_BN_CLICKED(IDC_RADIO_TRANSFER, &CBaseFuncionDlg::OnBnClickedRadioTransfer)
	ON_BN_CLICKED(IDC_RADIO_ACCOUNT, &CBaseFuncionDlg::OnBnClickedRadioAccount)
	ON_BN_CLICKED(IDC_BTN_SEND, &CBaseFuncionDlg::OnBnClickedBtnSend)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
END_MESSAGE_MAP()


// CBaseFuncionDlg 消息处理程序


void CBaseFuncionDlg::OnBnClickedBtnStartMine()
{
	//GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
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
	
	CLoadLanguage::getInstance()->Load(_T("base"),this);
	m_nMoney = 0.001;
	m_fAccTx_Fee = 0.001;

	UpdateData(FALSE);

	UpdateSelectState(emAccount);
	return TRUE;  
}

void CBaseFuncionDlg::EnableAccountTx(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_ACCTX_ACC)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_ACCTX_FEE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ISMINER)->EnableWindow(bEnable);
}

void CBaseFuncionDlg::EnableTransferTx(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_FROM)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_TO)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MONEY)->EnableWindow(bEnable);
}

//void CBaseFuncionDlg::EnableScriptTx(BOOL bEnable)
//{
//	GetDlgItem(IDC_EDIT_ACCOUNT)->EnableWindow(bEnable);
//	GetDlgItem(IDC_EDIT_FEE)->EnableWindow(bEnable);
//	GetDlgItem(IDC_EDIT_VALID_HEIGHT)->EnableWindow(bEnable);
//	GetDlgItem(IDC_EDIT_SCRIPT)->EnableWindow(bEnable);
//	GetDlgItem(IDC_EDIT_VALIDITY)->EnableWindow(bEnable);
//	GetDlgItem(IDC_EDIT_MAX_PER_TIME)->EnableWindow(bEnable);
//	GetDlgItem(IDC_EDIT_MAX_TOTAL)->EnableWindow(bEnable);
//	GetDlgItem(IDC_EDIT_MAX_PER_DAY)->EnableWindow(bEnable);
//	GetDlgItem(IDC_EDIT_USER_DATA)->EnableWindow(bEnable);
//	GetDlgItem(IDC_BTN_SCAN)->EnableWindow(bEnable);
//}

void CBaseFuncionDlg::OnBnClickedRadioTransfer()
{
	UpdateSelectState(emTransfer);
}

void CBaseFuncionDlg::OnBnClickedRadioAccount()
{
	UpdateSelectState(emAccount);
}

void CBaseFuncionDlg::UpdateSelectState(emType type)
{
	EnableTransferTx(FALSE);
	EnableAccountTx(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_TRANSFER))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_ACCOUNT))->SetCheck(FALSE);

	switch (type)
	{
	case emTransfer:
		EnableTransferTx(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_TRANSFER))->SetCheck(TRUE);
		m_nType = emTransfer; 
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

	string strData;
	switch (m_nType)
	{
	case emTransfer:
		{
			strData = CSoyPayHelp::getInstance()->CreateNomalTx(W2A(m_strFrom),W2A(m_strTo),REAL_MONEY(m_nMoney),0,0);
		}
		break;

	case emAccount:
		{
			bool bIsMiner = false;
			if (((CButton*)GetDlgItem(IDC_CHECK_ISMINER))->GetCheck() )
			{
				bIsMiner = true;
			}
			strData = CSoyPayHelp::getInstance()->CreateScriptAccountTx(W2A(m_strAccTx_Acc),REAL_MONEY(m_fAccTx_Fee),bIsMiner);
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	SendRPCData(strData,emBaseFunction);
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

BOOL CBaseFuncionDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN)
		{
			return TRUE;
		}
	} 

	return CDialogEx::PreTranslateMessage(pMsg);
}
