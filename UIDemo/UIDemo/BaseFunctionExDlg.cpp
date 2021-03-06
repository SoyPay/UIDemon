// BaseFunctionExDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "BaseFunctionExDlg.h"
#include "afxdialogex.h"
#include "CommonStruct.h"
#include "LoadLanguage.h"


// CBaseFunctionExDlg 对话框

IMPLEMENT_DYNAMIC(CBaseFunctionExDlg, CDialogEx)

CBaseFunctionExDlg::CBaseFunctionExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBaseFunctionExDlg::IDD, pParent)
	, m_strModifyAddr(_T(""))
	, m_nModifyFee(0)
	, m_strScriptTx_Path(_T(""))
	, m_nValidTime(0)
	, m_nMaxPerTime(0)
	, m_nMaxTotal(0)
	, m_nMaxPerDay(0)
	, m_strUserData(_T(""))
	, m_strRegisterAddr(_T(""))
	, m_nRegisterFee(0)
{
	m_emScriptType = emNonType;
}

CBaseFunctionExDlg::~CBaseFunctionExDlg()
{
}

void CBaseFunctionExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_strModifyAddr);
	DDX_Text(pDX, IDC_EDIT_FEE, m_nModifyFee);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strScriptTx_Path);
	DDX_Text(pDX, IDC_EDIT_VALIDITY, m_author.nAuthorizeTime);
	DDX_Text(pDX, IDC_EDIT_MAX_PER_TIME, m_author.nMaxMoneyPerTime);
	DDX_Text(pDX, IDC_EDIT_MAX_TOTAL, m_author.nMaxMoneyTotal);
	DDX_Text(pDX, IDC_EDIT_MAX_PER_DAY, m_author.nMaxMoneyPerDay);
	DDX_Text(pDX, IDC_EDIT_USER_DATA, m_strUserData);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT1, m_strRegisterAddr);
	DDX_Text(pDX, IDC_EDIT_FEE1, m_nRegisterFee);
	DDX_Text(pDX, IDC_EDIT_ID, m_strScriptID);
	DDX_Text(pDX, IDC_EDIT_APP_HASH, m_strRegisterScriptHash);
}


BEGIN_MESSAGE_MAP(CBaseFunctionExDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEND, &CBaseFunctionExDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_SCAN, &CBaseFunctionExDlg::OnBnClickedBtnScan)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CBaseFunctionExDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO_REGISTER, &CBaseFunctionExDlg::OnBnClickedRadioRegister)
	ON_BN_CLICKED(IDC_RADIO_MODIFY, &CBaseFunctionExDlg::OnBnClickedRadioModify)
END_MESSAGE_MAP()


// CBaseFunctionExDlg 消息处理程序


BOOL CBaseFunctionExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CLoadLanguage::getInstance()->Load(_T("baseex"),this);
	
	m_nRegisterFee = 0.001;
	m_strModifyAddr = _T("");
	m_strScriptTx_Path = _T("");
	m_nModifyFee = 0.001;
	m_author.nAuthorizeTime = 1000000;
	m_author.nMaxMoneyPerDay = 0.01;
	m_author.nMaxMoneyPerTime = 0.01;
	m_author.nMaxMoneyTotal = 0.01;
	UpdateData(FALSE);
	UpdateOption(emRegister);
	return TRUE; 
}


void CBaseFunctionExDlg::OnBnClickedBtnSend()
{
	UpdateData(TRUE);
	USES_CONVERSION;
	if (emRegister == m_emType)
	{
		if (m_strScriptTx_Path.IsEmpty())
		{
			MessageBox(CLoadLanguage::getInstance()->GetTips(SELELT_APP_TO_REGISTER));
			return ;
		}
		string strData = CSoyPayHelp::getInstance()->CreateScriptTx(W2A(m_strRegisterAddr),false,W2A(m_strScriptTx_Path)
			,REAL_MONEY(m_nRegisterFee),0);
		SendRPCData(strData,emRegisterScript);
	}
	else
	{
		if (m_strScriptID.IsEmpty())
		{
			MessageBox(CLoadLanguage::getInstance()->GetTips(INPUT_APP_ID));
			return;
		}

		CString strUserData;
		string strScriptID = GetFullRegID(m_strScriptID);
		GetDlgItem(IDC_EDIT_USER_DATA)->GetWindowTextW(strUserData);
		m_author.strUserData = W2A(strUserData);
		string strData = CSoyPayHelp::getInstance()->CreateScriptTx(W2A(m_strModifyAddr),true,strScriptID.c_str()
			,REAL_MONEY(m_nModifyFee),0,m_author.nAuthorizeTime,REAL_MONEY(m_author.nMaxMoneyPerTime),
			REAL_MONEY(m_author.nMaxMoneyTotal),REAL_MONEY(m_author.nMaxMoneyPerDay),m_author.strUserData);
		SendRPCData(strData,emRegisterScript);
	}
}


void CBaseFunctionExDlg::OnBnClickedBtnScan()
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,_T("脚本文件(*.bin)|*.bin|所有文件(*.*)|*.*||"));
	if (IDOK == dlg.DoModal())
	{
		CString strPath = dlg.GetPathName();
		CString strFileName = dlg.GetFileName();
		m_strScriptTx_Path = dlg.GetPathName();
		GetDlgItem(IDC_EDIT_PATH)->SetWindowTextW(m_strScriptTx_Path);
		//ShowRegScriptData(strFileName);
	}
}

LRESULT CBaseFunctionExDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	emSendDataType emType = static_cast<emSendDataType>(lparam);
	
	CString strShow(str.GetString());
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SHOW);
	pEdit->SetWindowTextW(strShow);

	if (emRegisterScript == emType)
	{
		if (emRegister == m_emType)
		{
			string strHash = CSoyPayHelp::getInstance()->ParseTxHashFromJson(str.GetString());
			m_strRegisterScriptHash = CString(strHash.c_str());
			GetDlgItem(IDC_EDIT_APP_HASH)->SetWindowTextW(m_strRegisterScriptHash);
			//UpdateData(FALSE);
		}
	}
	else if (emGetRegScriptID == emType)
	{
		string strScriptID = CSoyPayHelp::getInstance()->ParseScriptIDFromJson(str.GetString());
		if (!strScriptID.empty() )
		{
			m_strScriptID = CString(strScriptID.c_str());
			m_strScriptID = GetCompressRegID(m_strScriptID);
			GetDlgItem(IDC_EDIT_ID)->SetWindowTextW(m_strScriptID);
		}
	}
	
	return TRUE;
}

void CBaseFunctionExDlg::UpdateOption(emOption emTyle)
{
	BOOL bShowRegister = FALSE;
	BOOL bShowModify = FALSE;
	if (emRegister == emTyle)
	{
		bShowRegister = TRUE;
		bShowModify = FALSE;
	}
	else if (emModify == emTyle)
	{
		bShowRegister = FALSE;
		bShowModify = TRUE;
	}
	else
	{
		ASSERT(0);
	}

	((CButton*)GetDlgItem(IDC_RADIO_REGISTER))->SetCheck(bShowRegister);
	GetDlgItem(IDC_STATIC_ACC1)->EnableWindow(bShowRegister);
	GetDlgItem(IDC_EDIT_ACCOUNT1)->EnableWindow(bShowRegister);
	GetDlgItem(IDC_STATIC_FEE1)->EnableWindow(bShowRegister);
	GetDlgItem(IDC_EDIT_FEE1)->EnableWindow(bShowRegister);
	GetDlgItem(IDC_STATIC_PATH)->EnableWindow(bShowRegister);
	GetDlgItem(IDC_EDIT_PATH)->EnableWindow(bShowRegister);
	GetDlgItem(IDC_BTN_SCAN)->EnableWindow(bShowRegister);

	((CButton*)GetDlgItem(IDC_RADIO_MODIFY))->SetCheck(bShowModify);
	GetDlgItem(IDC_STATIC_ACC)->EnableWindow(bShowModify);
	GetDlgItem(IDC_EDIT_ACCOUNT)->EnableWindow(bShowModify);
	GetDlgItem(IDC_STATIC_FEE)->EnableWindow(bShowModify);
	GetDlgItem(IDC_EDIT_FEE)->EnableWindow(bShowModify);
	GetDlgItem(IDC_STATIC_HASH)->EnableWindow(bShowModify);
	GetDlgItem(IDC_EDIT_APP_HASH)->EnableWindow(bShowModify);
	GetDlgItem(IDC_STATIC_ID)->EnableWindow(bShowModify);
	GetDlgItem(IDC_EDIT_ID)->EnableWindow(bShowModify);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(bShowModify);

	GetDlgItem(IDC_STATIC_VALIDITY)->EnableWindow(bShowModify);
	GetDlgItem(IDC_EDIT_VALIDITY)->EnableWindow(bShowModify);
	GetDlgItem(IDC_STATIC_MAX_PER_TIME)->EnableWindow(bShowModify);
	GetDlgItem(IDC_EDIT_MAX_PER_TIME)->EnableWindow(bShowModify);
	GetDlgItem(IDC_STATIC_MAX_TOTAL)->EnableWindow(bShowModify);
	GetDlgItem(IDC_EDIT_MAX_TOTAL)->EnableWindow(bShowModify);
	GetDlgItem(IDC_STATIC_MAX_PER_DAY)->EnableWindow(bShowModify);
	GetDlgItem(IDC_EDIT_MAX_PER_DAY)->EnableWindow(bShowModify);
	GetDlgItem(IDC_STATIC_USER_DATA)->EnableWindow(bShowModify);
	GetDlgItem(IDC_EDIT_USER_DATA)->EnableWindow(bShowModify);
	m_emType = emTyle;

	//ShowRegScriptData();
}

void CBaseFunctionExDlg::Init(map<emBusinessType,CBaseDlg*>&& mapBasePtr)
{
	m_mapBasePtr = std::forward<map<emBusinessType,CBaseDlg*> >(mapBasePtr);
}

void CBaseFunctionExDlg::GetScriptIDByRPC()
{
	CString strCommand = _T("getscriptid ");
	CString strHash(m_strRegisterScriptHash);
	strCommand += strHash;
	CStringA strSendData;
	RPCCommandToJson(strCommand,strSendData);
	AfxGetApp()->m_pMainWnd->SendMessage(WM_SEND_RPC_DATA,(WPARAM)&strSendData,emGetRegScriptID);
}

void CBaseFunctionExDlg::OnTimer(UINT_PTR nIDEvent)
{
	/*if (m_strScriptID.IsEmpty())
	{
	GetScriptID();
	}
	else
	{
	KillTimer(nIDEvent);
	}*/
	
	CDialogEx::OnTimer(nIDEvent);
}


void CBaseFunctionExDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	GetScriptIDByRPC();
}

CString CBaseFunctionExDlg::GetScriptID()
{
	return m_strScriptID;
}

void CBaseFunctionExDlg::ShowRegScriptData()
{
	/*if (emNonType != m_emScriptType)
	{
	CBaseDlg* pBaseDlg = m_mapBasePtr[m_emScriptType];
	pBaseDlg->GetInfoToRegScript(m_strModifyAddr,m_nModifyHeight,m_nModifyFee,m_author);
	m_strUserData = CString(m_author.strUserData.c_str());
	UpdateData(FALSE);
	}*/
}

void CBaseFunctionExDlg::ShowRegScriptData(const CString& strFileName)
{
	CBaseDlg* pBaseDlg = nullptr;
	if (_T("SecuredTrade.bin") == strFileName)
	{
		pBaseDlg = m_mapBasePtr[emSesureTx];
		m_emScriptType = emSesureTx;
	}
	else if (_T("darksecure.bin") == strFileName)
	{
		pBaseDlg = m_mapBasePtr[emDarkTx];
		m_emScriptType = emDarkTx;
	}
	else if (_T("anony.bin") == strFileName)
	{
		pBaseDlg = m_mapBasePtr[emAnonymTx];
		m_emScriptType = emAnonymTx;
	}
	else if (_T("p2pbet.bin") == strFileName)
	{
		pBaseDlg = m_mapBasePtr[emP2PBet];
		m_emScriptType = emP2PBet;
	}
	else
	{
		ASSERT(0);
	}

	//pBaseDlg->GetInfoToRegScript(m_strRegisterAddr,m_nRegisterHeight,m_nRegisterFee,m_author);
	UpdateData(FALSE);
}

void CBaseFunctionExDlg::Show()
{
	//ShowRegScriptData();
	ShowWindow(SW_SHOW);
}

void CBaseFunctionExDlg::OnBnClickedRadioRegister()
{
	UpdateOption(emRegister);
}


void CBaseFunctionExDlg::OnBnClickedRadioModify()
{
	UpdateOption(emModify);
}


BOOL CBaseFunctionExDlg::PreTranslateMessage(MSG* pMsg)
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
