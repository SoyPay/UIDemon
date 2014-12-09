// BusinessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "BusinessDlg.h"
#include "afxdialogex.h"


// CBusinessDlg 对话框

IMPLEMENT_DYNAMIC(CBusinessDlg, CDialogEx)

CBusinessDlg::CBusinessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBusinessDlg::IDD, pParent)
{
	m_pSesureDlg = NULL;
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
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
END_MESSAGE_MAP()


// CBusinessDlg 消息处理程序


BOOL CBusinessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE; 
}


void CBusinessDlg::Init()
{

}

void CBusinessDlg::OnBnClickedTrade1()
{
	if (!m_pSesureDlg)
	{
		m_pSesureDlg = new CSesureDlg;
		m_pSesureDlg->Create(CSesureDlg::IDD,this);
	}
	
	m_pSesureDlg->ShowStep(emFirstStep);
}


void CBusinessDlg::OnBnClickedBtnTrade2()
{
	if (!m_pSesureDlg)
	{
		m_pSesureDlg = new CSesureDlg;
		m_pSesureDlg->Create(CSesureDlg::IDD,this);
	}
	m_pSesureDlg->ShowStep(emSecondStep);
}


void CBusinessDlg::OnBnClickedBtnTrade3()
{
	if (!m_pSesureDlg)
	{
		m_pSesureDlg = new CSesureDlg;
		m_pSesureDlg->Create(CSesureDlg::IDD,this);
	}
	m_pSesureDlg->ShowStep(emThirdStep);
}


void CBusinessDlg::OnBnClickedBtnTrade4()
{
	if (!m_pSesureDlg)
	{
		m_pSesureDlg = new CSesureDlg;
		m_pSesureDlg->Create(CSesureDlg::IDD,this);
	}
	m_pSesureDlg->ShowStep(emLastStep);
}

LRESULT CBusinessDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	if (m_pSesureDlg)
	{
		m_pSesureDlg->SendMessage(WM_SHOW_RECV_DATA,wparam,lparam);
	}
	return TRUE;
}