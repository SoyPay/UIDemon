// AnonymTxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "AnonymTxDlg.h"
#include "afxdialogex.h"
#include "JsonConfigHelp.h"
#include "UIDemoDlg.h"

// CAnonymTxDlg 对话框

IMPLEMENT_DYNAMIC(CAnonymTxDlg, CDialogEx)
extern DWORD WINAPI AnonymTxThreadPorc(LPVOID lpvoid);
CAnonymTxDlg::CAnonymTxDlg(CWnd* pParent /*=NULL*/)
	: CBaseDlg(CAnonymTxDlg::IDD, pParent)
	, m_strSend(_T(""))
	, m_strRecv1(_T(""))
	, m_strRecv2(_T(""))
	, m_nMoney(0)
	, m_strRes(_T(""))
{

}

CAnonymTxDlg::~CAnonymTxDlg()
{
}

void CAnonymTxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SENDER, m_strSend);
	DDX_Text(pDX, IDC_EDIT_RECV1, m_strRecv1);
	DDX_Text(pDX, IDC_EDIT_RECV2, m_strRecv2);
	DDX_Text(pDX, IDC_EDIT_MONEY, m_nMoney);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_FEE, m_nTxFee);
	DDX_Text(pDX, IDC_EDIT_RES, m_strRes);
}


BEGIN_MESSAGE_MAP(CAnonymTxDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEND, &CAnonymTxDlg::OnBnClickedBtnSend)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
END_MESSAGE_MAP()


// CAnonymTxDlg 消息处理程序


BOOL CAnonymTxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Init();
	UpdateData(FALSE);

	return TRUE;  
}

void CAnonymTxDlg::OnBnClickedBtnSend()
{
	//CloseHandle(CreateThread(NULL,0,AnonymTxThreadPorc,this,0,NULL) );
	CreateContractTx();
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
}

void CAnonymTxDlg::CreateContractTx()
{
	USES_CONVERSION;

	vector<string> vSignAddr;
	vSignAddr.push_back(m_strSignAddrSend);


	vector<string> vRecv;
	vRecv.push_back(W2A(m_strRecv1));
	vRecv.push_back(W2A(m_strRecv2));

	string strContractData = m_AnonymTxHelp.PackAnonymContract(W2A(m_strSend),m_nMoney,vRecv,m_nHeight);

	string strData = CSoyPayHelp::getInstance()->CreateContractTx(W2A(m_strScript),vSignAddr,strContractData,m_nHeight,REAL_MONEY(m_nTxFee));
	SendRPCData(strData,emContract);
}

LRESULT CAnonymTxDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	emSendDataType emType = static_cast<emSendDataType>(lparam);

	GetDlgItem(IDC_EDIT_RES)->SetWindowTextW(CString(str.GetString()));
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
	return TRUE;
}

void CAnonymTxDlg::Init()
{
	CAnonmyCfg anonmy;
	CJsonConfigHelp::getInstance()->GetAnonymCfgData(anonmy);
	m_strSend = CString(anonmy.strSendID.c_str());
	m_strRecv1 = CString(anonmy.strRecvID1.c_str());
	m_strRecv2 = CString(anonmy.strRecvID2.c_str());
	m_strBinFilePath = anonmy.strBinPath;
	m_nMoney = anonmy.nMoney;
	m_nHeight = anonmy.nHeight;
	m_nTxFee = anonmy.nFee;
}

void CAnonymTxDlg::GetSignAddr()
{
	if (m_strSignAddrSend.empty())
	{
		CTestDlg* pDlg = (CTestDlg*)AfxGetApp()->m_pMainWnd;
		CString strSendAddr = pDlg->GetAddrByRegID(m_strSend);

		USES_CONVERSION;
		m_strSignAddrSend = W2A(strSendAddr);
		m_strScriptSignAddr = m_strSignAddrSend;
	}
}

void CAnonymTxDlg::Show()
{
	GetScriptID();
	GetDlgItem(IDC_EDIT_SCRIPTID)->SetWindowText(m_strScript);
	ShowWindow(SW_SHOW);
}

void CAnonymTxDlg::UpdateRegScriptSignAddr()
{
	GetSignAddr();
}