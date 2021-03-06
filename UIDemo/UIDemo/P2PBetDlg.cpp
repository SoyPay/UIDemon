// P2PBetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "P2PBetDlg.h"
#include "afxdialogex.h"
#include "JsonConfigHelp.h"
#include "UIDemoDlg.h"
#include "LoadLanguage.h"
// CP2PBetDlg 对话框

IMPLEMENT_DYNAMIC(CP2PBetDlg, CDialogEx)
CP2PBetDlg::CP2PBetDlg(CWnd* pParent /*=NULL*/)
	: CBaseDlg(CP2PBetDlg::IDD, pParent)
	, m_nSendMoney(0)
	, m_nAcceptMoney(0)
	, m_strOpenAddr(_T(""))
{

}

CP2PBetDlg::~CP2PBetDlg()
{
}

void CP2PBetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEND_MONEY, m_nSendMoney);
	DDX_Text(pDX, IDC_EDIT_SEND_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_ACCEPT_MONEY, m_nAcceptMoney);
	DDX_Text(pDX, IDC_EDIT_SCRIPT, m_strScript);
	DDX_Text(pDX, IDC_EDIT_SEND_SIGN_ADDR, m_strSendSignAddr);
	DDX_Text(pDX, IDC_EDIT_ACCEPT_SIGN_ADDR, m_strAcceptSignAddr);
	DDX_Text(pDX, IDC_EDIT_OPEN_ADDR, m_strOpenAddr);
}


BEGIN_MESSAGE_MAP(CP2PBetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEND, &CP2PBetDlg::OnBnClickedBtnSend)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_BN_CLICKED(IDC_BUTTON1, &CP2PBetDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CP2PBetDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CP2PBetDlg 消息处理程序


BOOL CP2PBetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CLoadLanguage::getInstance()->Load(_T("p2pbet"),this);

	//Init();
	m_nTxFee = 0.001;
	if(m_strScript.IsEmpty())
	{
	  m_strScript = _T("32997-1");
	}
	UpdateData(FALSE);
	return TRUE; 
}

void CP2PBetDlg::ShowStep(enumP2PBet emStep)
{
	EnableP2PSend(FALSE);
	EnableP2PAccept(FALSE);
	EnableP2PAExpose(FALSE);
	//EnableP2PBExpose(FALSE);

	switch (emStep)
	{
	case emP2PBetASend:
		if(m_strRandomAHash.empty()){
		AfxGetMainWnd()->PostMessage(WM_GET_BET_RANDOM_DATA);
		}
	
		EnableP2PSend(TRUE);
		break;
	case emP2PBetBAccept:
		EnableP2PAccept(TRUE);
		OnBnClickedButton2();
		break;
	case emP2PBetAExpose:
		EnableP2PAExpose(TRUE);
		break;
	case emP2PBetBExpose:
		EnableP2PBExpose(TRUE);
		break;
	default:
		ASSERT(0);
		break;
	}

	CenterWindow();
	ShowWindow(SW_SHOW);
	m_emStep = emStep;

	if (!m_strSendHash.IsEmpty())
	{
		GetDlgItem(IDC_EDIT_SEND_HASH)->SetWindowText(m_strSendHash);
		GetDlgItem(IDC_EDIT_SEND_HASH2)->SetWindowText(m_strSendHash);
		//GetDlgItem(IDC_EDIT_SEND_HASH3)->SetWindowText(m_strSendHash);
	}
	if(!m_strScript.IsEmpty())
	{
	GetDlgItem(IDC_EDIT_SCRIPT2)->SetWindowTextW(m_strScript);
	GetDlgItem(IDC_EDIT_SCRIPT)->SetWindowTextW(m_strScript);
	GetDlgItem(IDC_EDIT_SCRIPT3)->SetWindowTextW(m_strScript);
	}

	UpdateData(FALSE);
}

void CP2PBetDlg::EnableP2PSend(bool bEnable)
{
	GetDlgItem(IDC_STATIC_SCRIPT)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SCRIPT)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_SEND_SIGN_ADDR)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SEND_SIGN_ADDR)->EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_SEND_MONEY)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SEND_MONEY)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_SEND_HEIGHT)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SEND_HEIGHT)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_AHASH)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_DATA_A)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_SEND)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(bEnable);
	((CButton*)(GetDlgItem(IDC_RADIO_SEND)))->SetCheck(bEnable);
}

void CP2PBetDlg::EnableP2PAccept(bool bEnable)
{
	GetDlgItem(IDC_STATIC_SCRIPT2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SCRIPT2)->EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_ACCEPT_ADDR2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_ACCEPT_SIGN_ADDR)->EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_ACCEPT_MONEY)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_ACCEPT_MONEY)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_SEND_HASH)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SEND_HASH)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_BHASH)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_DATA_B)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_ACCEPT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(bEnable);
	((CButton*)(GetDlgItem(IDC_RADIO_ACCEPT)))->SetCheck(bEnable);
}

void CP2PBetDlg::EnableP2PAExpose(bool bEnable)
{
	GetDlgItem(IDC_STATIC_SCRIPT3)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SCRIPT3)->EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_SEND_HASH2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SEND_HASH2)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_AHASH3)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_AHASH2)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_AEXPOSE)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_ACCEPT_ADDR3)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_OPEN_ADDR)->EnableWindow(bEnable);
	((CButton*)(GetDlgItem(IDC_RADIO_AEXPOSE)))->SetCheck(bEnable);
}

void CP2PBetDlg::EnableP2PBExpose(bool bEnable)
{
	GetDlgItem(IDC_STATIC_SEND_HASH3)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SEND_HASH3)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_AHASH4)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_BHASH3)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_BEXPOSE)->EnableWindow(bEnable);
	((CButton*)(GetDlgItem(IDC_RADIO_BEXPOSE)))->SetCheck(bEnable);
}

void CP2PBetDlg::OnBnClickedBtnSend()
{
	UpdateData(TRUE);
	if (CheckParam())
	{
		CreateContractTx();
	}
}

void CP2PBetDlg::CreateContractTx()
{
	USES_CONVERSION;
	vector<string> vSignAddr;
	string strAHash = CSoyPayHelp::getInstance()->GetReverseHash(m_strRandomAHash);
	string strSendHash = CSoyPayHelp::getInstance()->GetReverseHash(W2A(m_strSendHash));
	
	string strContractData;
	
	switch (m_emStep)
	{
	case emP2PBetASend:
		strContractData = m_P2PBetHelp.PacketP2PSendContract(REAL_MONEY(m_nSendMoney),m_nHeight,strAHash);
		vSignAddr.push_back(W2A(m_strSendSignAddr));
		break;

	case emP2PBetBAccept:
		GetDlgItem(IDC_EDIT_SEND_HASH)->GetWindowTextW(m_strSendHash);
		strSendHash = CSoyPayHelp::getInstance()->GetReverseHash(W2A(m_strSendHash));
		strContractData = m_P2PBetHelp.PacketP2PAcceptContract(REAL_MONEY(m_nAcceptMoney),strSendHash,(char*)m_szRandomB);
		vSignAddr.push_back(W2A(m_strAcceptSignAddr));
		break;

	case emP2PBetAExpose:
		{
			GetDlgItem(IDC_EDIT_SEND_HASH2)->GetWindowTextW(m_strSendHash);
			strSendHash = CSoyPayHelp::getInstance()->GetReverseHash(W2A(m_strSendHash));
			string strRandomA(m_szRandomA,m_szRandomA+sizeof(m_szRandomA));
			strContractData = m_P2PBetHelp.PacketP2PExposeContract(strSendHash,strRandomA);
			vSignAddr.push_back(W2A(m_strOpenAddr));
		}
		break;

	//case emP2PBetBExpose:
	//	{
	//		string strRandomB(m_szRandomB,m_szRandomB+sizeof(m_szRandomB));
	//		strContractData = m_P2PBetHelp.PacketP2PExposeContract(strSendHash,strRandomB);
	//		vSignAddr.push_back(m_strAcceptSignAddr);
	//	}
	//	break;

	default:
		ASSERT(0);
		break;
	}
	
	string strScriptID = GetFullRegID(m_strScript);
	string strData = CSoyPayHelp::getInstance()->CreateContractTx(strScriptID,
		vSignAddr,strContractData,0,REAL_MONEY(m_nTxFee));
	SendRPCData(strData,emContract);
}

int CP2PBetDlg::GetCurStep()
{
	return m_emStep;
}

LRESULT CP2PBetDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
		/*CStringA str = *(CStringA*)wparam;
	CString strShow;
	strShow += CString(str.GetString());
	GetDlgItem(IDC_EDIT_RES)->SetWindowTextW(strShow);

	string strParsedHash = CSoyPayHelp::getInstance()->ParseTxHashFromJson(str.GetString());
	if (emP2PBetASend == m_emStep)
	{
	m_strSendHash = CString(strParsedHash.c_str());
	}


	GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);*/

	CStringA str = *(CStringA*)wparam;
	emSendDataType emType = static_cast<emSendDataType>(lparam);

	string strParsedHash = CSoyPayHelp::getInstance()->ParseTxHashFromJson(str.GetString());
	if (emP2PBetASend == m_emStep)
	{
		m_strSendHash = CString(strParsedHash.c_str());
	}

	if (emContract == emType)
	{
		GetDlgItem(IDC_EDIT_RES)->SetWindowTextW(CString(str.GetString()));
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
	}
	else if (emGetBetRandomData == emType)
	{
		FillBetRandomData(str.GetString());
	}
	else
	{
		string strScriptID = CSoyPayHelp::getInstance()->ParseScriptIDFromJson(str.GetString());
		m_strScript = CString(strScriptID.c_str());
		UpdateData(FALSE);
	}

	return TRUE;
}

string CP2PBetDlg::GetRandomHash(const CString& strRandomHash)
{
	USES_CONVERSION;
	string strHash(W2A(strRandomHash));

	vector<unsigned char> vHash = CSoyPayHelp::getInstance()->ParseHex(strHash);
	strHash.clear();
	strHash.insert(strHash.end(),vHash.begin(),vHash.end());
	return strHash;
}

void CP2PBetDlg::Init()
{
	/*CP2PBetCfg p2pCfg;
	CJsonConfigHelp::getInstance()->GetP2PBetCfgData(p2pCfg);
	m_nSendMoney = p2pCfg.nMoney;
	m_nAcceptMoney = m_nSendMoney;
	m_nTxFee = p2pCfg.nFee;
	m_nHeight = p2pCfg.nHeight;
	m_strSendID = CString(p2pCfg.strSendID.c_str());
	m_strAcceptID = CString(p2pCfg.strAcceptID.c_str());
	m_strBinFilePath = p2pCfg.strBinPath;*/
}

void CP2PBetDlg::UpdateRegScriptSignAddr()
{
	/*if (m_strSendSignAddr.empty())
	{
	CTestDlg* pDlg = (CTestDlg*)AfxGetApp()->m_pMainWnd;
	CString strSendAddr = pDlg->GetAddrByRegID(m_strSendID);
	CString strAcceptAddr = pDlg->GetAddrByRegID(m_strAcceptID);

	USES_CONVERSION;
	m_strSendSignAddr = W2A(strSendAddr);
	m_strAcceptSignAddr = W2A(strAcceptAddr);

	m_strScriptSignAddr = m_strSendSignAddr;
	}*/
}

void CP2PBetDlg::FillBetRandomData(const string& strData)
{
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strData, root)) 
		return ;
	string strDataA = root["dataA"].asString();
	m_strRandomAHash = root["HashA"].asString();
	//string strDataB = root["dataB"].asString();
	
	sscanf(strDataA.c_str(),"%d,%d,%d,%d,%d",&m_szRandomA[0],&m_szRandomA[1],&m_szRandomA[2],&m_szRandomA[3],&m_szRandomA[4]);
	//sscanf(strDataB.c_str(),"%d,%d,%d,%d,%d",&m_szRandomB[0],&m_szRandomB[1],&m_szRandomB[2],&m_szRandomB[3],&m_szRandomB[4]);

	GetDlgItem(IDC_EDIT_DATA_A)->SetWindowText(CString(strDataA.c_str()));
	GetDlgItem(IDC_EDIT_AHASH2)->SetWindowText(CString(strDataA.c_str()));
}

void CP2PBetDlg::CreateRandomData()
{
	vector<int> vData;
	while (1)
	{
		int n = rand()%255;
		vector<int>::iterator it = vData.begin();
		for (;it != vData.end();it++)
		{
			if (n == *it)
			{
				break;
			}
		}

		if (it == vData.end())
		{
			vData.push_back(n);
		}

		if (vData.size() == 5)
		{
			break;
		}
	}

	for (size_t i = 0;i<vData.size();++i)
	{
		m_szRandomB[i] = vData[i];
	}
}

void CP2PBetDlg::OnBnClickedButton1()
{
	AfxGetMainWnd()->PostMessage(WM_GET_BET_RANDOM_DATA);
}


void CP2PBetDlg::OnBnClickedButton2()
{
	CreateRandomData();
	CString strData;
	strData.Format(_T("%d,%d,%d,%d,%d"),static_cast<int>(m_szRandomB[0]),static_cast<int>(m_szRandomB[1]),
		static_cast<int>(m_szRandomB[2]),static_cast<int>(m_szRandomB[3]),static_cast<int>(m_szRandomB[4]));
	GetDlgItem(IDC_EDIT_DATA_B)->SetWindowText(strData);
}

bool CP2PBetDlg::CheckParam()
{
	if (emP2PBetASend == m_emStep)
	{
		if (!CheckRegIDValid(m_strScript))
		{
			MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_APP_ID));
			return false;
		}

		if (REAL_MONEY(m_nSendMoney)<=100000)
		{
			MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_BET_MONEY));
			return false;
		}

		if (m_nHeight<=10)
		{
			MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_HEIGHT));
			return false;
		}
	}
	else if (emP2PBetBAccept == m_emStep)
	{
		GetDlgItem(IDC_EDIT_SCRIPT2)->GetWindowTextW(m_strScript);
		if (!CheckRegIDValid(m_strScript))
		{
			MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_APP_ID));
			return false;
		}

		if (REAL_MONEY(m_nAcceptMoney)<=100000)
		{
			MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_BET_MONEY));
			return false;
		}

		CString strBetData;
		GetDlgItem(IDC_EDIT_DATA_B)->GetWindowTextW(strBetData);
		if (strBetData.IsEmpty())
		{
			MessageBox(CLoadLanguage::getInstance()->GetTips(EMPTY_RANDOM_DATA));
			return false;
		}
	}
	else
	{
		GetDlgItem(IDC_EDIT_SCRIPT3)->GetWindowTextW(m_strScript);
		if(m_strOpenAddr.IsEmpty())
		{
		   MessageBox(_T("input the address"));
		   return false;
		}
		if (!CheckRegIDValid(m_strScript))
		{
			MessageBox(CLoadLanguage::getInstance()->GetTips(INVALID_APP_ID));
			return false;
		}
	}
	
	return true;
}