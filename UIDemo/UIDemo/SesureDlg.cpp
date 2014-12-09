// SesureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "SesureDlg.h"
#include "afxdialogex.h"
#include "UIDemoDlg.h"


// CSesureDlg 对话框

IMPLEMENT_DYNAMIC(CSesureDlg, CDialogEx)

CSesureDlg::CSesureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSesureDlg::IDD, pParent)
	, m_strBuyer(_T(""))
	, m_nPay(0)
	, m_nDeposit(0)
	, m_nArFee(0)
	, m_nMaxFine(0)
	, m_nHeight(0)
	, m_strScript(_T(""))
	, m_strSignAddr(_T(""))
	, m_nTxFee(0)
	, m_strHash(_T(""))
	, m_nFine(0)
{
	m_emStep = emInvalidStep;
}

CSesureDlg::~CSesureDlg()
{
}

void CSesureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BUYER, m_strBuyer);
	DDX_Text(pDX, IDC_EDIT_SELLER, m_strSeller);
	DDX_Text(pDX, IDC_EDIT_BUY_MONEY, m_nPay);
	DDX_Text(pDX, IDC_EDIT_DEPOSIT, m_nDeposit);
	DDX_Text(pDX, IDC_EDIT_AR_FEE, m_nArFee);
	DDX_Text(pDX, IDC_EDIT_MAX_FINE_MONEY, m_nMaxFine);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_SCRIPT, m_strScript);
	DDX_Text(pDX, IDC_EDIT_SIGN_ADDRs, m_strSignAddr);
	DDX_Text(pDX, IDC_EDIT_TX_FEE, m_nTxFee);
	DDX_Text(pDX, IDC_EDIT_HASH, m_strHash);
	DDX_Control(pDX, IDC_COMBO_AR, m_comboxAr);
	DDX_Control(pDX, IDC_LIST_AR, m_listAr);
	DDX_Text(pDX, IDC_EDIT_FINE_MONEY, m_nFine);
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


	GetDlgItem(IDC_STATIC_HASH)->MoveWindow(CRect(35,136,130,151));
	GetDlgItem(IDC_EDIT_HASH)->MoveWindow(CRect(149,131,317,152));
	GetDlgItem(IDC_STATIC_FINE_MONEY)->MoveWindow(CRect(35,189,120,204));
	GetDlgItem(IDC_EDIT_FINE_MONEY)->MoveWindow(CRect(149,189,317,210));

	m_strSignAddr = _T("5zQPcC1YpFMtwxiH787pSXanUECoGsxUq3KZieJxVG");
	m_strScript = _T("010000000100");
	m_strBuyer = _T("000000000900");
	m_strSeller = _T("000000000500");
	m_nDeposit = 100000;
	m_nArFee = 100000;
	m_nMaxFine = 100000;
	m_nFine = 100000;
	m_nHeight = 100;
	m_nPay = 100000;
	m_nTxFee = 100000;
	m_listAr.AddString(_T("0-6"));

	UpdateData(FALSE);
	HideAllControl();
	return TRUE; 
}

void CSesureDlg::ShowStep(unsigned char nStep)
{
	switch (nStep)
	{
	case emFirstStep:
		{
			GetDlgItem(IDC_STATIC_BUYER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_BUYER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SELLER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SELLER)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_AR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_LIST_AR)->ShowWindow(SW_SHOW);
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
			
			FillRegIDToCombox();
			UpdateSignAddr(m_strBuyer);
			m_strUserDefineData = "01";
		}
		break;

	case emSecondStep:
		GetDlgItem(IDC_STATIC_HASH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_HASH)->ShowWindow(SW_SHOW);
	
		UpdateSignAddr(m_strSeller);
		m_strUserDefineData = "02";
		break;
		
	case emThirdStep:
		GetDlgItem(IDC_STATIC_HASH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_HASH)->ShowWindow(SW_SHOW);
		UpdateSignAddr(m_strBuyer);
		m_strUserDefineData = "01";
		break;

	case emLastStep:
		GetDlgItem(IDC_STATIC_HASH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_HASH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_FINE_MONEY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_FINE_MONEY)->ShowWindow(SW_SHOW);

		if (!m_vArRegID.empty())
		{
			UpdateSignAddr(CString(m_vArRegID.at(0).c_str()));
		}
		
		m_strUserDefineData = "03";
		break;

	default:
		ASSERT(0);
		break;
	}

	GetDlgItem(IDC_EDIT_SHOW)->SetWindowTextW(_T(""));
	m_emStep = static_cast<enumSesureTrade>(nStep);
	UpdateData(FALSE);
	ShowWindow(SW_SHOW);
}

void CSesureDlg::HideAllControl()
{
	GetDlgItem(IDC_STATIC_BUYER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_BUYER)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_SELLER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_SELLER)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_AR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LIST_AR)->ShowWindow(SW_HIDE);

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

void CSesureDlg::CreateScriptTx(bool bRigsterScript)
{
	USES_CONVERSION;

	if (bRigsterScript)
	{
		::CreateScriptTx(W2A(m_strSignAddr),false,"D:\\cppwork\\vmsdk\\SecuredTrade\\Debug\\Exe\\SecuredTrade.bin",
			m_nTxFee,m_nHeight,100000,0,0,0,m_strUserDefineData);
	}
	else
	{
		::CreateScriptTx(W2A(m_strSignAddr),true,"010000000100",
			m_nTxFee,m_nHeight,100000,0,0,0,m_strUserDefineData);
	}
	
}

void CSesureDlg::CreateContractTx()
{
	USES_CONVERSION;
	vector<unsigned char> vTemp;
	switch (m_emStep)
	{
	case emFirstStep:
		{
			PacketFirstContract(emFirstStep,
				W2A(m_strBuyer),
				W2A(m_strSeller),
				m_vArRegID,
				m_nHeight,
				m_nMaxFine,
				m_nPay,
				m_nArFee,
				m_nDeposit,
				&m_firstContract);

			char* pData = (char*)&m_firstContract;
			vTemp.assign(pData,pData +sizeof(FIRST_CONTRACT));
		}
		break;
	case emSecondStep:
		{
			string strHash(W2A(m_strHash));
			vector<unsigned char>vHash = ParseHex(strHash);
			reverse(vHash.begin(),vHash.end());

			strHash.assign(vHash.begin(),vHash.end());
			ASSERT(HASH_SIZE == strHash.size());
			packetNextContract(emSecondStep,(unsigned char*)strHash.c_str(),&m_secondContract);

			char* pData = (char*)&m_secondContract;
			vTemp.assign(pData,pData +sizeof(NEXT_CONTRACT));
		}
		break;

	case emThirdStep:
		{
			string strHash(W2A(m_strHash));
			vector<unsigned char>vHash = ParseHex(strHash);
			reverse(vHash.begin(),vHash.end());

			strHash.assign(vHash.begin(),vHash.end());
			ASSERT(HASH_SIZE == strHash.size());
			packetNextContract(emThirdStep,(unsigned char*)strHash.c_str(),&m_thirdContract);

			char* pData = (char*)&m_thirdContract;
			vTemp.assign(pData,pData +sizeof(NEXT_CONTRACT));
		}
		break;
	case emLastStep:
		{
			string strHash(W2A(m_strHash));
			vector<unsigned char>vHash = ParseHex(strHash);
			reverse(vHash.begin(),vHash.end());

			strHash.assign(vHash.begin(),vHash.end());
			ASSERT(HASH_SIZE == strHash.size());
			packetLastContract(emLastStep,(unsigned char*)strHash.c_str(),m_nFine,&m_arContract);

			char* pData = (char*)&m_arContract;
			vTemp.assign(pData,pData +sizeof(ARBIT_RES_CONTRACT));
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	vector<string> vAccount;
	vAccount.push_back(W2A(m_strSignAddr));
	::CreateContractTx("010000000100",vAccount,vTemp,m_nHeight,m_nTxFee);
}

DWORD WINAPI SesureThreadPorc(LPVOID lpvoid)
{
	CSesureDlg* pDlg = (CSesureDlg*)lpvoid;
	if (NULL == pDlg)
	{
		return FALSE;
	}

	if (emFirstStep == pDlg->GetCurStep())
	{
		//注册脚本交易
		pDlg->CreateScriptTx(true);
		Sleep(2000);

		//挖矿
		Setgenerate(true);
		Sleep(2000);
	}
	
	//发送注册脚本交易修改权限
	pDlg->CreateScriptTx(false);
	Sleep(2000);

	//挖矿
	Setgenerate(true);
	Sleep(2000);

	//合约交易
	pDlg->CreateContractTx();
	Sleep(2000);

	//挖矿
	Setgenerate(true);
	Sleep(2000);
	return 0;
}

void CSesureDlg::OnBtnSend()
{
	switch(m_emStep)
	{
	case emFirstStep:
		{
			if (!m_listAr.GetCount())
			{
				MessageBox(_T("没有选择仲裁者！"));
				return ;
			}

			GetArRegID(m_vArRegID);
			ASSERT(m_vArRegID.size() && m_vArRegID.size()<=MAX_ARBITRATOR);
		}
		break;

	case emLastStep:
		{
			ASSERT(m_vArRegID.size());
			m_vArRegID.erase(m_vArRegID.begin());
		}
		break;
	}
	

	UpdateData(TRUE);
	CloseHandle(CreateThread(NULL,0,SesureThreadPorc,this,0,NULL) );
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
}

void CSesureDlg::OnCancel()
{
	HideAllControl();

	CDialogEx::OnCancel();
}

LRESULT CSesureDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	CString strShow;
	strShow += CString(str.GetString());
	GetDlgItem(IDC_EDIT_SHOW)->SetWindowTextW(strShow);

	CStringA strData = str;
	Json::Reader reader; 
	Json::Value value;
	if(reader.parse(strData.GetString(),value))
	{
		Json::Value txhash = value["hash"];
		string str = txhash.asString();
		CString strTxHash(str.c_str());
		m_strHash = strTxHash;
	}

	GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
	return TRUE;
}

int CSesureDlg::GetCurStep()
{
	return m_emStep;
}

void CSesureDlg::OnCbnSelchangeComboAr()
{
	int nIndex = m_comboxAr.GetCurSel();
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

	m_listAr.AddString(strRegID);
}

void CSesureDlg::GetArRegID(vector<string>& vRegID)
{
	for(int i = 0;i<m_listAr.GetCount();i++)
	{
		CString strItemText;
		m_listAr.GetText(i,strItemText);

		vRegID.push_back(GetFullRegID(strItemText));
	}
}

void CSesureDlg::FillRegIDToCombox()
{
	CTestDlg* pMainDlg = (CTestDlg*)AfxGetMainWnd();
	pMainDlg->GetAccountInfo(m_mapAccount);
	m_comboxAr.ResetContent();
	for(map<CString,CString>::iterator it = m_mapAccount.begin();it != m_mapAccount.end();++it)
	{
		m_comboxAr.AddString(it->first);
	}
}

void CSesureDlg::UpdateSignAddr(const CString &strFullRegID)
{
	CString strCompressRegID = GetCompressRegID(strFullRegID);
	if (m_mapAccount.end() == m_mapAccount.find(strCompressRegID))
	{
		ASSERT(0);
	}
	m_strSignAddr = m_mapAccount[strCompressRegID];
}