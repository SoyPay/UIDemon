// SesureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "SesureDlg.h"
#include "afxdialogex.h"
#include "UIDemoDlg.h"
#include "JsonConfigHelp.h"
#include "LoadLanguage.h"

// CSesureDlg 对话框

IMPLEMENT_DYNAMIC(CSesureDlg, CDialogEx)

CSesureDlg::CSesureDlg(CWnd* pParent /*=NULL*/)
	: CBaseDlg(CSesureDlg::IDD, pParent)
	, m_strBuyer(_T(""))
	, m_nPay(0)
	, m_nDeposit(0)
	, m_nArFee(0)
	, m_nMaxFine(0)
	, m_strSignAddr(_T(""))
	, m_strHash(_T(""))
	, m_nFine(0)
	, m_strAr(_T(""))
{
	m_emStep = emInvalidStep;
	m_author.nAuthorizeTime = 100000;
	m_author.nMaxMoneyPerDay = 100000;
	m_author.nMaxMoneyTotal = 100000;
	m_author.nMaxMoneyPerTime = 100000;
}

CSesureDlg::~CSesureDlg()
{
}

void CSesureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SCRIPT, m_strScript);
	DDX_Text(pDX, IDC_EDIT_BUYER, m_strBuyer);
	DDX_Text(pDX, IDC_EDIT_SELLER, m_strSeller);
	DDX_Text(pDX, IDC_EDIT_BUY_MONEY, m_nPay);
	DDX_Text(pDX, IDC_EDIT_DEPOSIT, m_nDeposit);
	DDX_Text(pDX, IDC_EDIT_AR_FEE, m_nArFee);
	DDX_Text(pDX, IDC_EDIT_MAX_FINE_MONEY, m_nMaxFine);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_SIGN_ADDRs, m_strSignAddr);
	DDX_Text(pDX, IDC_EDIT_TX_FEE, m_nTxFee);
	DDX_Text(pDX, IDC_EDIT_HASH, m_strHash);
	DDX_Control(pDX, IDC_COMBO_AR, m_comboxAr);
	DDX_Text(pDX, IDC_EDIT_FINE_MONEY, m_nFine);
	DDX_Text(pDX, IDC_EDIT_AR, m_strAr);
}


BEGIN_MESSAGE_MAP(CSesureDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEND, &CSesureDlg::OnBtnSend)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_CBN_SELCHANGE(IDC_COMBO_AR, &CSesureDlg::OnCbnSelchangeComboAr)
END_MESSAGE_MAP()


// CSesureDlg 消息处理程序
BOOL CSesureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CLoadLanguage::getInstance()->Load(_T("sesure"),this);


	GetDlgItem(IDC_STATIC_HASH)->MoveWindow(CRect(35,136,145,151));
	GetDlgItem(IDC_EDIT_HASH)->MoveWindow(CRect(149,131,350,152));
	GetDlgItem(IDC_STATIC_FINE_MONEY)->MoveWindow(CRect(35,189,120,204));
	GetDlgItem(IDC_EDIT_FINE_MONEY)->MoveWindow(CRect(149,189,350,210));

	//Init();
	m_author.strUserData = "01";
	UpdateData(FALSE);
	HideAllControl();
	return TRUE; 
}

void CSesureDlg::ShowStep(unsigned char nStep)
{
	CString strTitle;
	switch (nStep)
	{
	case emFirstStep:
		{
			GetDlgItem(IDC_STATIC_BUYER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_BUYER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SELLER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SELLER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_AR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_AR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SELECT_AR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_AR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_BUY_MONEY)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_BUY_MONEY)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_DEPOSIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_DEPOSIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_FEE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_AR_FEE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_MAX_FINE_MONEY)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_MAX_FINE_MONEY)->ShowWindow(SW_SHOW);
			
			//FillRegIDToCombox();
			//UpdateSignAddr(m_strBuyer);
			m_author.strUserData = "01";
		}
		break;

	case emSecondStep:
		GetDlgItem(IDC_STATIC_HASH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_HASH)->ShowWindow(SW_SHOW);
	
		//UpdateSignAddr(m_strSeller);
		m_author.strUserData = "02";
		break;
		
	case emThirdStep:
		GetDlgItem(IDC_STATIC_HASH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_HASH)->ShowWindow(SW_SHOW);
		//UpdateSignAddr(m_strAr);
		m_author.strUserData = "01";
		break;

	case emLastStep:
		GetDlgItem(IDC_STATIC_HASH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_HASH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_FINE_MONEY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_FINE_MONEY)->ShowWindow(SW_SHOW);

		//UpdateSignAddr(m_strAr);
		
		m_author.strUserData = "03";
		break;

	default:
		ASSERT(0);
		break;
	}

	GetScriptID();
	GetDlgItem(IDC_EDIT_SHOW)->SetWindowTextW(_T(""));
	m_strSignAddr.Empty();
	m_emStep = static_cast<enumSesureTrade>(nStep);
	UpdateData(FALSE);
	CenterWindow();
	ShowWindow(SW_SHOW);
}

void CSesureDlg::HideAllControl()
{
	GetDlgItem(IDC_STATIC_BUYER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_BUYER)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_SELLER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_SELLER)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_AR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_AR)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_SELECT_AR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO_AR)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_BUY_MONEY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_BUY_MONEY)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_DEPOSIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_DEPOSIT)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_FEE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_AR_FEE)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_MAX_FINE_MONEY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MAX_FINE_MONEY)->ShowWindow(SW_HIDE);


	GetDlgItem(IDC_STATIC_HASH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_HASH)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_FINE_MONEY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_FINE_MONEY)->ShowWindow(SW_HIDE);
}

void CSesureDlg::CreateContractTx()
{
	USES_CONVERSION;
	string strContractData;
	string strScriptID = GetFullRegID(m_strScript);
	switch (m_emStep)
	{
	case emFirstStep:
		{
			m_vArRegID.clear();
			string str = CSoyPayHelp::getInstance()->GetFullRegID(W2A(m_strAr));
			m_vArRegID.push_back(std::move(str));
			strContractData = m_SesureHelp.PacketFirstContract(
				CSoyPayHelp::getInstance()->GetFullRegID(W2A(m_strBuyer)),
				CSoyPayHelp::getInstance()->GetFullRegID(W2A(m_strSeller)),
				m_vArRegID,
				m_nHeight,
				REAL_MONEY(m_nMaxFine),
				REAL_MONEY(m_nPay),
				REAL_MONEY(m_nArFee),
				REAL_MONEY(m_nDeposit));
		}
		break;
	case emSecondStep:
		{
			string strHash = CSoyPayHelp::getInstance()->GetReverseHash(W2A(m_strHash));
			strContractData = m_SesureHelp.PacketSecondContract((unsigned char*)strHash.c_str());
		}
		break;

	case emThirdStep:
		{
			string strHash = CSoyPayHelp::getInstance()->GetReverseHash(W2A(m_strHash));
			strContractData = m_SesureHelp.PacketThirdContract((unsigned char*)strHash.c_str());
		}
		break;
	case emLastStep:
		{
			string strHash = CSoyPayHelp::getInstance()->GetReverseHash(W2A(m_strHash));
			strContractData = m_SesureHelp.PacketLastContract((unsigned char*)strHash.c_str(),REAL_MONEY(m_nFine));
		}
		break;

	default:
		ASSERT(0);
		break;
	}


	vector<string> vAccount;
	vAccount.push_back(W2A(m_strSignAddr));
	string strData = CSoyPayHelp::getInstance()->CreateContractTx(strScriptID,vAccount,
		strContractData,0,REAL_MONEY(m_nTxFee));
	SendRPCData(strData,emContract);
}

void CSesureDlg::OnBtnSend()
{
	UpdateData(TRUE);

	if (ChechParams())
	{
		CreateContractTx();
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
	}
}

void CSesureDlg::OnCancel()
{
	HideAllControl();

	CDialogEx::OnCancel();
}

LRESULT CSesureDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	emSendDataType emType = static_cast<emSendDataType>(lparam);

	
	string strParsedHash = CSoyPayHelp::getInstance()->ParseTxHashFromJson(str.GetString());
	CString strTxHash(strParsedHash.c_str());
	m_strHash = strTxHash;

	if (emContract == emType)
	{
		GetDlgItem(IDC_EDIT_SHOW)->SetWindowTextW(CString(str.GetString()));
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
	}

	return TRUE;
}

int CSesureDlg::GetCurStep()
{
	return m_emStep;
}

void CSesureDlg::OnCbnSelchangeComboAr()
{
	/*int nIndex = m_comboxAr.GetCurSel();
	CString strRegID;
	m_comboxAr.GetLBText(nIndex,strRegID);

	int nCount = m_listAr.GetCount();
	if (nCount>=MAX_ARBITRATOR)
	{
	MessageBox(_T("目前最多支持三级仲裁！"));
	return ;
	}

	for(int i = 0;i<m_listAr.GetCount();i++)
	{
	CString strItemText;
	m_listAr.GetText(i,strItemText);
	if (strRegID == strItemText)
	{
	MessageBox(_T("重复的仲裁者，请重新选择！"));
	return ;
	}
	}

	m_listAr.AddString(strRegID);*/
}

void CSesureDlg::GetArRegID(vector<string>& vRegID)
{
	/*for(int i = 0;i<m_listAr.GetCount();i++)
	{
	CString strItemText;
	m_listAr.GetText(i,strItemText);

	vRegID.push_back(GetFullRegID(strItemText));
	}*/
}

void CSesureDlg::FillRegIDToCombox()
{
	CTestDlg* pMainDlg = (CTestDlg*)AfxGetMainWnd();
	pMainDlg->GetAccountInfo(m_mapAccount);
	m_comboxAr.ResetContent();
	for(map<CString,CString>::iterator it = m_mapAccount.begin();it != m_mapAccount.end();++it)
	{
		m_comboxAr.AddString(it->second);
	}
}

void CSesureDlg::UpdateSignAddr(const CString &strFullRegID)
{
	CTestDlg* pDlg = (CTestDlg*)AfxGetApp()->m_pMainWnd;
	m_strSignAddr = pDlg->GetAddrByRegID(strFullRegID);

	USES_CONVERSION;
	m_strScriptSignAddr = W2A(m_strSignAddr);
}

void CSesureDlg::Init()
{
	CSesureTradeCfg sesureCfg;
	CJsonConfigHelp::getInstance()->GetSesureTradeCfgData(sesureCfg);
	
	m_strBuyer = CString(sesureCfg.strBuyerID.c_str());
	m_strSeller =  CString(sesureCfg.strSellerID.c_str());
	m_nDeposit = sesureCfg.nDeposite;
	m_nArFee = sesureCfg.nArFee;
	m_nMaxFine = sesureCfg.nMaxFine;
	m_nFine = sesureCfg.nFine;
	m_nHeight = sesureCfg.nHeight;
	m_nPay = sesureCfg.nPay;
	m_nTxFee = sesureCfg.nTxFee;
	m_strBinPath = sesureCfg.strBinPath;

	m_strAr = CString(sesureCfg.strArID.c_str());
}

void CSesureDlg::UpdateRegScriptSignAddr()
{
	UpdateSignAddr(m_strBuyer);
}

bool CSesureDlg::ChechParams()
{
	if (!CheckRegIDValid(m_strScript))
	{
		MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_APP_ID));
		return false;
	}

	switch (m_emStep)
	{
	case emFirstStep:
		{
			if (!CheckRegIDValid(m_strBuyer))
			{
				MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_BUYER_ID));
				return false;
			}

			if (!CheckRegIDValid(m_strSeller))
			{
				MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_SELLER_ID));
				return false;
			}

			if (!CheckRegIDValid(m_strAr))
			{
				MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_AR_ID));
				return false;
			}
		}
		break;
	case emSecondStep:
	case emThirdStep:
	case emLastStep:
		break;

	default:
		ASSERT(0);
		break;
	}

	return true;
}