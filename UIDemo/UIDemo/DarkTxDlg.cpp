// DarkTxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "DarkTxDlg.h"
#include "afxdialogex.h"
#include "UIDemoDlg.h"
#include "LoadLanguage.h"


// CDarkTxDlg 对话框
extern DWORD WINAPI DarkTxThreadPorc(LPVOID lpvoid);
IMPLEMENT_DYNAMIC(CDarkTxDlg, CDialogEx)

CDarkTxDlg::CDarkTxDlg(CWnd* pParent /*=NULL*/)
	: CBaseDlg(CDarkTxDlg::IDD, pParent)
	, m_strBuyerID(_T(""))
	, m_strSellerID(_T(""))
	, m_strRes(_T(""))
	, m_nMoney(0)
	, m_strHash(_T(""))
{

}

CDarkTxDlg::~CDarkTxDlg()
{
}

void CDarkTxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SCRIPT, m_strScript);
	DDX_Text(pDX, IDC_EDIT_BUYER, m_strBuyerID);
	DDX_Text(pDX, IDC_EDIT_SELLER, m_strSellerID);
	DDX_Text(pDX, IDC_EDIT_FEE, m_nTxFee);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_RES, m_strRes);
	DDX_Text(pDX, IDC_EDIT_MONEY, m_nMoney);
	DDX_Text(pDX, IDC_EDIT_HASH, m_strHash);
	DDX_Text(pDX, IDC_EDIT_BUYER_ADDR, m_strBuyerSignAddr);
	DDX_Text(pDX, IDC_EDIT_SELLER_ADDR, m_strSellerSignAddr);
}


BEGIN_MESSAGE_MAP(CDarkTxDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEND, &CDarkTxDlg::OnBnClickedBtnSend)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_BN_CLICKED(IDC_BTN_SIGN, &CDarkTxDlg::OnBnClickedBtnSign)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDarkTxDlg 消息处理程序
BOOL CDarkTxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CLoadLanguage::getInstance()->Load(_T("darkcoin"),this);

	GetDlgItem(IDC_STATIC_LAST_HASH)->MoveWindow(CRect(20,220,110,240));
	GetDlgItem(IDC_STATIC_CONTENT)->MoveWindow(CRect(20,220,110,240));
	GetDlgItem(IDC_EDIT_HASH)->MoveWindow(CRect(113,220,299,240));
	//Init();
	UpdateData(FALSE);
	HideAllControl();
	return TRUE;  
}

void CDarkTxDlg::OnBnClickedBtnSend()
{
	UpdateData(TRUE);
	if (CheckParam())
	{
		CreateContractTx();
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
	}
}

void CDarkTxDlg::CreateContractTx()
{
	USES_CONVERSION;
	
	vector<unsigned char> vData;
	vector<string> vSignAddr;
	string strContractData;
	string strScriptID = GetFullRegID(m_strScript);
	if (emDarkFirstStep == m_emStep) 
	{
		strContractData = m_DarkTxHelp.PacketFirstContract(CSoyPayHelp::getInstance()->GetFullRegID(W2A(m_strBuyerID)),
			CSoyPayHelp::getInstance()->GetFullRegID(W2A(m_strSellerID)),
			REAL_MONEY(m_nMoney),m_nHeight);
		vSignAddr.push_back(W2A(m_strBuyerSignAddr));
		vSignAddr.push_back(W2A(m_strSellerSignAddr));
		string strData = CSoyPayHelp::getInstance()->CreateContractTx(strScriptID,vSignAddr,strContractData,0,REAL_MONEY(m_nTxFee));
		SendRPCData(strData,emContract);
	}
	else if (emDarkThirdStep == m_emStep)
	{
		string strHash = CSoyPayHelp::getInstance()->GetReverseHash(W2A(m_strHash));
		strContractData = m_DarkTxHelp.PacketNextContract(strHash);
		vSignAddr.push_back(W2A(m_strBuyerSignAddr));
		string strData = CSoyPayHelp::getInstance()->CreateContractTx(strScriptID,vSignAddr,strContractData,0,REAL_MONEY(m_nTxFee));
		SendRPCData(strData,emContract);
	}
	else
	{
		SignContract();
	}
}

int CDarkTxDlg::GetCurStep()
{
	return m_emStep;
}

LRESULT CDarkTxDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	emSendDataType emType = static_cast<emSendDataType>(lparam);


	string strParsedHash = CSoyPayHelp::getInstance()->ParseTxHashFromJson(str.GetString());
	CString strTxHash(strParsedHash.c_str());
	m_strHash = strTxHash;

	if (emContract == emType)
	{
		GetDlgItem(IDC_EDIT_RES)->SetWindowTextW(CString(str.GetString()));
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
	}

	return TRUE;
}

void CDarkTxDlg::SignContract()
{
	USES_CONVERSION;
	string strData = CSoyPayHelp::getInstance()->SignContractTx(W2A(m_strHash));
	SendRPCData(strData,emContract);
}

void CDarkTxDlg::ShowStep(enumDarkTx emStep)
{
	if (m_strScript.IsEmpty())
	{
		GetScriptID();
	}

	switch (emStep)
	{
	case emDarkFirstStep:
		{
			/*CRect rc;
			GetDlgItem(IDC_STATIC_BUYER)->GetWindowRect(&rc);
			ScreenToClient(&rc);
			GetDlgItem(IDC_EDIT_BUYER)->GetWindowRect(&rc);
			ScreenToClient(&rc);
			GetDlgItem(IDC_STATIC_SELLER)->GetWindowRect(&rc);
			ScreenToClient(&rc);
			GetDlgItem(IDC_EDIT_SELLER)->GetWindowRect(&rc);
			ScreenToClient(&rc);
			GetDlgItem(IDC_STATIC_SELLER)->GetWindowRect(&rc);
			ScreenToClient(&rc);
			GetDlgItem(IDC_EDIT_SELLER)->GetWindowRect(&rc);
			ScreenToClient(&rc);
			GetDlgItem(IDC_STATIC_SELLER_ADDR)->GetWindowRect(&rc);
			ScreenToClient(&rc);
			GetDlgItem(IDC_EDIT_SELLER_ADDR)->GetWindowRect(&rc);
			ScreenToClient(&rc);*/

			GetDlgItem(IDC_STATIC_BUYER)->MoveWindow(CRect(27,54,110,70));
			GetDlgItem(IDC_EDIT_BUYER)->MoveWindow(CRect(113,54,299,74));
			GetDlgItem(IDC_STATIC_BUYER_ADDR)->MoveWindow(CRect(27,85,110,100));
			GetDlgItem(IDC_EDIT_BUYER_ADDR)->MoveWindow(CRect(113,85,299,105));

			GetDlgItem(IDC_STATIC_SELLER)->MoveWindow(CRect(27,119,110,134));
			GetDlgItem(IDC_EDIT_SELLER)->MoveWindow(CRect(113,115,299,135));
			GetDlgItem(IDC_STATIC_SELLER_ADDR)->MoveWindow(CRect(27,153,110,168));
			GetDlgItem(IDC_EDIT_SELLER_ADDR)->MoveWindow(CRect(113,150,299,170));

			GetDlgItem(IDC_STATIC_BUYER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_BUYER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_BUYER_ADDR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_BUYER_ADDR)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STATIC_SELLER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SELLER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SELLER_ADDR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SELLER_ADDR)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STATIC_FEE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_FEE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_MONEY)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_MONEY)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_HEIGHT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_HEIGHT)->ShowWindow(SW_SHOW);
			m_strRes.Empty();
		}
		break;

	case emDarkSecondStep:
		{
			GetDlgItem(IDC_STATIC_SELLER)->MoveWindow(CRect(27,94,110,113));
			GetDlgItem(IDC_EDIT_SELLER)->MoveWindow(CRect(113,94,299,113));
			GetDlgItem(IDC_STATIC_SELLER_ADDR)->MoveWindow(CRect(27,150,110,170));
			GetDlgItem(IDC_EDIT_SELLER_ADDR)->MoveWindow(CRect(113,150,299,170));

			GetDlgItem(IDC_STATIC_SELLER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SELLER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SELLER_ADDR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SELLER_ADDR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_HASH)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_LAST_HASH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_CONTENT)->ShowWindow(SW_SHOW);
		}
		break;

	case emThirdStep:
		{
			GetDlgItem(IDC_STATIC_BUYER)->MoveWindow(CRect(27,94,110,113));
			GetDlgItem(IDC_EDIT_BUYER)->MoveWindow(CRect(113,94,299,113));
			GetDlgItem(IDC_STATIC_BUYER_ADDR)->MoveWindow(CRect(27,150,110,170));
			GetDlgItem(IDC_EDIT_BUYER_ADDR)->MoveWindow(CRect(113,150,299,170));

			GetDlgItem(IDC_STATIC_BUYER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_BUYER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_BUYER_ADDR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_BUYER_ADDR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_HASH)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STATIC_LAST_HASH)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_CONTENT)->ShowWindow(SW_HIDE);
		}
		break;
	default:
		break;
	}

	UpdateData(FALSE);
	CenterWindow();
	ShowWindow(SW_SHOW);
	m_emStep = emStep;
}

void CDarkTxDlg::Init()
{
	/*CDarkTxCfg darkCfg;
	CJsonConfigHelp::getInstance()->GetDarkCfgData(darkCfg);

	m_strBuyerID = CString(darkCfg.strBuyID.c_str());
	m_strSellerID = CString(darkCfg.strSellID.c_str());
	m_nTxFee = darkCfg.nFee;
	m_nMoney = darkCfg.nMoney;
	m_nHeight = darkCfg.nHeight;
	m_strRes.Empty();
	m_emStep = emDarkFirstStep;*/
}

void CDarkTxDlg::GetSignAddr()
{
	/*if (m_strBuyerSignAddr.empty() || m_strSellerSignAddr.empty() )
	{
	CTestDlg* pDlg = (CTestDlg*)AfxGetApp()->m_pMainWnd;
	CString strBuyerAddr = pDlg->GetAddrByRegID(m_strBuyerID);
	CString strSellerAddr = pDlg->GetAddrByRegID(m_strSellerID);

	USES_CONVERSION;
	m_strBuyerSignAddr = W2A(strBuyerAddr);
	m_strSellerSignAddr = W2A(strSellerAddr);
	m_strScriptSignAddr = m_strBuyerSignAddr;
	}*/
}

void CDarkTxDlg::OnBnClickedBtnSign()
{
	SignContract();
}

void CDarkTxDlg::UpdateRegScriptSignAddr()
{
	GetSignAddr();
}

void CDarkTxDlg::OnClose()
{
	HideAllControl();

	CBaseDlg::OnClose();
}

void CDarkTxDlg::HideAllControl()
{
	GetDlgItem(IDC_STATIC_BUYER)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_BUYER)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_BUYER_ADDR)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_BUYER_ADDR)->ShowWindow(FALSE);

	GetDlgItem(IDC_STATIC_SELLER)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_SELLER)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_SELLER_ADDR)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_SELLER_ADDR)->ShowWindow(FALSE);

	GetDlgItem(IDC_STATIC_FEE)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_FEE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_MONEY)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_MONEY)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_HEIGHT)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_HEIGHT)->ShowWindow(FALSE);

	GetDlgItem(IDC_STATIC_LAST_HASH)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_HASH)->ShowWindow(FALSE);
}

bool CDarkTxDlg::CheckParam()
{
	if (!CheckRegIDValid(m_strScript))
	{
		MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_APP_ID));
		return false;
	}

	if (emDarkFirstStep == m_emStep) 
	{
		if (!CheckRegIDValid(m_strBuyerID))
		{
			MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_BUYER_ID));
			return false;
		}

		if (!CheckRegIDValid(m_strSellerID))
		{
			MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_SELLER_ID));
			return false;
		}
	}
	else if (emDarkThirdStep == m_emStep)
	{
		
	}
	else
	{
	
	}
	return true;
}