// BusinessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "BusinessDlg.h"
#include "afxdialogex.h"
#include "LoadLanguage.h"
#include "ShowJosn.h"
#include "UIDemoDlg.h"


// CBusinessDlg 对话框

IMPLEMENT_DYNAMIC(CBusinessDlg, CDialogEx)

CBusinessDlg::CBusinessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBusinessDlg::IDD, pParent)
{
	m_pSesureDlg = NULL;
	m_pDarkTxDlg = NULL;
	m_pAnonymTxDlg = NULL;
	m_emType = emSesureTx;
}

CBusinessDlg::~CBusinessDlg()
{
}

void CBusinessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBusinessDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_TRADE1, &CBusinessDlg::OnBnClickedTrade1)
	ON_BN_CLICKED(IDC_BTN_TRADE2, &CBusinessDlg::OnBnClickedBtnTrade2)
	ON_BN_CLICKED(IDC_BTN_TRADE3, &CBusinessDlg::OnBnClickedBtnTrade3)
	ON_BN_CLICKED(IDC_BTN_TRADE4, &CBusinessDlg::OnBnClickedBtnTrade4)
	ON_BN_CLICKED(IDC_BTN_DARK1, &CBusinessDlg::OnBnClickedBtnDark1)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_BN_CLICKED(IDC_BTN_DARK2, &CBusinessDlg::OnBnClickedBtnDark2)
	ON_BN_CLICKED(IDC_RADIO_SESURE, &CBusinessDlg::OnBnClickedRadioSesure)
	ON_BN_CLICKED(IDC_RADIO_DARK, &CBusinessDlg::OnBnClickedRadioDark)
	ON_BN_CLICKED(IDC_BTN_ANONYM, &CBusinessDlg::OnBnClickedBtnAnonym)
	ON_BN_CLICKED(IDC_RADIO_ANONYM, &CBusinessDlg::OnBnClickedRadioAnonym)
	ON_BN_CLICKED(IDC_BTN_SEND_BET, &CBusinessDlg::OnBnClickedBtnSendBet)
	ON_BN_CLICKED(IDC_BTN_ACCEPT_BET, &CBusinessDlg::OnBnClickedBtnAcceptBet)
	ON_BN_CLICKED(IDC_BTN_AEXPOSE, &CBusinessDlg::OnBnClickedBtnAexpose)
	ON_BN_CLICKED(IDC_RADIO_P2P_BET, &CBusinessDlg::OnBnClickedRadioP2pBet)
	ON_BN_CLICKED(IDC_RADIO_LOTTERY, &CBusinessDlg::OnBnClickedRadioLottery)
	ON_BN_CLICKED(IDC_BTN_LOTTERT_REGISTER, &CBusinessDlg::OnBnClickedBtnLottertRegister)
	ON_BN_CLICKED(IDC_BTN_LOTTERY_BET, &CBusinessDlg::OnBnClickedBtnLotteryBet)
	ON_BN_CLICKED(IDC_BTN_LOTTERT_DRAW, &CBusinessDlg::OnBnClickedBtnLottertDraw)
	ON_BN_CLICKED(IDC_BTN_DARK3, &CBusinessDlg::OnBnClickedBtnDark3)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_SESURE, &CBusinessDlg::OnNMClickSyslinkSesure)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_DARK, &CBusinessDlg::OnNMClickSyslinkDark)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_BET, &CBusinessDlg::OnNMClickSyslinkBet)
	ON_BN_CLICKED(IDC_BUTTON5, &CBusinessDlg::OnBnClickedButton5)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_BET2, &CBusinessDlg::OnNMClickSyslinkBet2)
END_MESSAGE_MAP()


// CBusinessDlg 消息处理程序


BOOL CBusinessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CLoadLanguage::getInstance()->Load(_T("business"),this);
	m_pSesureDlg = new CSesureDlg;
	m_pSesureDlg->Create(CSesureDlg::IDD,this);

	m_pDarkTxDlg = new CDarkTxDlg;
	m_pDarkTxDlg->Create(CDarkTxDlg::IDD,this);
	
	m_pAnonymTxDlg = new CAnonymTxDlg;
	m_pAnonymTxDlg->Create(CAnonymTxDlg::IDD,this);
	
	m_pP2PBetDlg = new CP2PBetDlg;
	m_pP2PBetDlg->Create(CP2PBetDlg::IDD,this);
	
	/*m_pLotteryDlg = new CLotteryDlg;
	m_pLotteryDlg->Create(CLotteryDlg::IDD,this);*/
	
	UpdateSelectState(emP2PBet);
	return TRUE; 
}


void CBusinessDlg::Init()
{

}

void CBusinessDlg::OnBnClickedTrade1()
{
	ASSERT(m_pSesureDlg);
	m_pSesureDlg->ShowStep(emFirstStep);
}


void CBusinessDlg::OnBnClickedBtnTrade2()
{
	ASSERT(m_pSesureDlg);
	m_pSesureDlg->ShowStep(emSecondStep);
}


void CBusinessDlg::OnBnClickedBtnTrade3()
{
	ASSERT(m_pSesureDlg);
	m_pSesureDlg->ShowStep(emThirdStep);
}


void CBusinessDlg::OnBnClickedBtnTrade4()
{
	ASSERT(m_pSesureDlg);
	m_pSesureDlg->ShowStep(emLastStep);
}

LRESULT CBusinessDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	ASSERT(m_pSesureDlg && m_pDarkTxDlg);
	switch (m_emType)
	{
	case emSesureTx:
		m_pSesureDlg->SendMessage(WM_SHOW_RECV_DATA,wparam,lparam);
		break;

	case emDarkTx:
		m_pDarkTxDlg->SendMessage(WM_SHOW_RECV_DATA,wparam,lparam);
		break;

	case emAnonymTx:
		m_pAnonymTxDlg->SendMessage(WM_SHOW_RECV_DATA,wparam,lparam);
		break;

	case emP2PBet:
		m_pP2PBetDlg->SendMessage(WM_SHOW_RECV_DATA,wparam,lparam);
		break;

	case emLottery:
		break;

	default:
		ASSERT(0);
		break;
	}
	
	return TRUE;
}

void CBusinessDlg::OnBnClickedBtnDark1()
{
	m_pDarkTxDlg->ShowStep(emDarkFirstStep);
}


void CBusinessDlg::OnBnClickedBtnDark2()
{
	m_pDarkTxDlg->ShowStep(emDarkSecondStep);
}

void CBusinessDlg::OnBnClickedBtnDark3()
{
	m_pDarkTxDlg->ShowStep(emDarkThirdStep);
}


void CBusinessDlg::OnBnClickedRadioSesure()
{
	UpdateSelectState(emSesureTx);
}

void CBusinessDlg::OnBnClickedRadioDark()
{
	UpdateSelectState(emDarkTx);
}


void CBusinessDlg::EnableSesureTx(bool bEnable)
{
	GetDlgItem(IDC_BTN_TRADE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_TRADE2)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_TRADE3)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_TRADE4)->EnableWindow(bEnable);
}

void CBusinessDlg::EnableDarkTx(bool bEnable)
{
	GetDlgItem(IDC_BTN_DARK1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_DARK2)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_DARK3)->EnableWindow(bEnable);
}

void CBusinessDlg::EnableAnonymTx(bool bEnable)
{
	GetDlgItem(IDC_BTN_ANONYM)->EnableWindow(bEnable);
}

void CBusinessDlg::EnableP2PBet(bool bEnable)
{
	GetDlgItem(IDC_BTN_SEND_BET)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_ACCEPT_BET)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_AEXPOSE)->EnableWindow(bEnable);
}

void CBusinessDlg::EnableLottery(bool bEnable)
{

}

void CBusinessDlg::EnableStock(bool bEnable)
{

}

void CBusinessDlg::UpdateSelectState(emBusinessType type)
{
	EnableSesureTx(FALSE);
	EnableDarkTx(FALSE);
	EnableAnonymTx(FALSE);
	EnableP2PBet(FALSE);
	//EnableLottery(FALSE);
	//EnableStock(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_SESURE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_DARK))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_ANONYM))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_P2P_BET))->SetCheck(FALSE);

	switch (type)
	{
	case emSesureTx:
		EnableSesureTx(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_SESURE))->SetCheck(TRUE);
		break;

	case emDarkTx:
		EnableDarkTx(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_DARK))->SetCheck(TRUE);
		break;

	case emAnonymTx:
		EnableAnonymTx(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_ANONYM))->SetCheck(TRUE);
		break;

	case emP2PBet:
		EnableP2PBet(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_P2P_BET))->SetCheck(TRUE);
		break;

	case emLottery:
		EnableLottery(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_LOTTERY))->SetCheck(TRUE);
		break;

	default:
		ASSERT(0);
		break;
	}

	m_emType = type;
	ShowWindow(SW_SHOW);
}

void CBusinessDlg::OnBnClickedBtnAnonym()
{
	m_pAnonymTxDlg->CenterWindow();
	m_pAnonymTxDlg->Show();
}


void CBusinessDlg::OnBnClickedRadioAnonym()
{
	UpdateSelectState(emAnonymTx);
}


void CBusinessDlg::OnBnClickedBtnSendBet()
{
	m_pP2PBetDlg->ShowStep(emP2PBetASend);
}


void CBusinessDlg::OnBnClickedBtnAcceptBet()
{
	m_pP2PBetDlg->ShowStep(emP2PBetBAccept);
}


void CBusinessDlg::OnBnClickedBtnAexpose()
{
	m_pP2PBetDlg->ShowStep(emP2PBetAExpose);
}

void CBusinessDlg::OnBnClickedBtnBexpose()
{
	//m_pP2PBetDlg->ShowStep(emP2PBetBExpose);
}

void CBusinessDlg::OnBnClickedRadioP2pBet()
{
	UpdateSelectState(emP2PBet);
}


void CBusinessDlg::OnBnClickedRadioLottery()
{
	UpdateSelectState(emLottery);
}

void CBusinessDlg::OnBnClickedBtnLottertRegister()
{
	m_pLotteryDlg->CenterWindow();
	m_pLotteryDlg->ShowWindow(SW_SHOW);
}


void CBusinessDlg::OnBnClickedBtnLotteryBet()
{
	m_pLotteryDlg->CenterWindow();
	m_pLotteryDlg->ShowWindow(SW_SHOW);
}


void CBusinessDlg::OnBnClickedBtnLottertDraw()
{
	m_pLotteryDlg->CenterWindow();
	m_pLotteryDlg->ShowWindow(SW_SHOW);
}

map<emBusinessType,CBaseDlg*> CBusinessDlg::GetBaseDlgPtrMap()
{
	map<emBusinessType,CBaseDlg*> mapBasePtr;
	mapBasePtr.insert(make_pair(emSesureTx,m_pSesureDlg));
	mapBasePtr.insert(make_pair(emDarkTx,m_pDarkTxDlg));
	mapBasePtr.insert(make_pair(emAnonymTx,m_pAnonymTxDlg));
	mapBasePtr.insert(make_pair(emP2PBet,m_pP2PBetDlg));
	return mapBasePtr;
}

void CBusinessDlg::UpdateRegScriptSignAddr()
{
	m_pSesureDlg->UpdateRegScriptSignAddr();
	m_pDarkTxDlg->UpdateRegScriptSignAddr();
	m_pAnonymTxDlg->UpdateRegScriptSignAddr();
	m_pP2PBetDlg->UpdateRegScriptSignAddr();
}

void CBusinessDlg::OnNMClickSyslinkSesure(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShellExecuteW(NULL, L"open", _T("http://bbs.dspay.org/forum.php?mod=viewthread&tid=1446&extra=page%3D1"), NULL, NULL, SW_SHOWNORMAL);
}


void CBusinessDlg::OnNMClickSyslinkDark(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShellExecuteW(NULL, L"open", _T("http://bbs.dspay.org/forum.php?mod=viewthread&tid=1452&extra=page%3D1"), NULL, NULL, SW_SHOWNORMAL);
}


void CBusinessDlg::OnNMClickSyslinkBet(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShellExecuteW(NULL, L"open", _T("http://bbs.dspay.org/forum.php?mod=viewthread&tid=1609"), NULL, NULL, SW_SHOWNORMAL);
}


void CBusinessDlg::OnBnClickedBtnP2pPool()
{

}


void CBusinessDlg::OnBnClickedButton5()
{

	CString m_strScript;
	m_pP2PBetDlg->GetDlgItem(IDC_EDIT_SCRIPT)->GetWindowTextW(m_strScript);
	if(m_strScript.IsEmpty())
		m_strScript = m_pP2PBetDlg->m_strScript;
	CShowJosn ShowJson;
	Json::Reader reader;  
	Json::Value root; 

	if(m_strScript != _T(""))
	{

		CString strCommand;
		strCommand.Format(_T("%s %s %s %s"),_T("getp2pbetdata"),m_strScript,_T("100"),_T("1"));
		CStringA strShowData;
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
		//m_AppID

		if (reader.parse(strShowData.GetString(), root)) 
		{
			ShowJson.SetShowJosn(root);
		}
	}

	if(m_strScript != _T(""))
		ShowJson.m_AppID = m_strScript;
	
	
	ShowJson.DoModal();
	if(ShowJson.mType == ACCEPT)
	{
		Json::Value selet = ShowJson.m_selectValue;
		m_pP2PBetDlg->m_strScript = ShowJson.m_AppID;
		string te = selet.toStyledString();
		m_pP2PBetDlg->m_nAcceptMoney = m_pP2PBetDlg->m_nSendMoney = selet["money"].asDouble();
		m_pP2PBetDlg->m_strSendHash = selet["key"].asCString();
	    m_pP2PBetDlg->ShowStep(emP2PBetBAccept);		
	
	}else if(ShowJson.mType == OPEN)
	{
		Json::Value selet = ShowJson.m_selectValue;
		m_pP2PBetDlg->m_strScript = ShowJson.m_AppID;

		m_pP2PBetDlg->m_nAcceptMoney = m_pP2PBetDlg->m_nSendMoney = selet["money"].asDouble();
		m_pP2PBetDlg->m_strSendHash = selet["key"].asCString();
		m_pP2PBetDlg->ShowStep(emP2PBetAExpose);	

	}
}


void CBusinessDlg::OnNMClickSyslinkBet2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecuteW(NULL, L"open", _T("http://bbs.dspay.org/forum.php?mod=viewthread&tid=1614"), NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}
