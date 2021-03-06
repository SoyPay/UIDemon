// LotteryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "LotteryDlg.h"
#include "afxdialogex.h"

// CLotteryDlg 对话框

IMPLEMENT_DYNAMIC(CLotteryDlg, CDialogEx)

CLotteryDlg::CLotteryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLotteryDlg::IDD, pParent)
{

}

CLotteryDlg::~CLotteryDlg()
{
}

void CLotteryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLotteryDlg, CDialogEx)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_BN_CLICKED(IDC_BTN_SEND, &CLotteryDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CLotteryDlg 消息处理程序


BOOL CLotteryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	return TRUE;
}


void CLotteryDlg::OnBnClickedBtnSend()
{
	
}

LRESULT CLotteryDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	CString strShow;
	strShow += CString(str.GetString());
	GetDlgItem(IDC_EDIT_RES)->SetWindowTextW(strShow);

	GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
	return TRUE;
}

void CLotteryDlg::EnableRegister(bool bEnable)
{
	GetDlgItem(IDC_STATIC_MONEY)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MONEY)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_HEIGHT)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(bEnable);


	((CButton*)(GetDlgItem(IDC_RADIO_REGISTER)))->SetCheck(bEnable);
}

void CLotteryDlg::EnableBet(bool bEnable)
{
	GetDlgItem(IDC_STATIC_NUM1)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_NUM1)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_MONEY1)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MONEY1)->EnableWindow(bEnable);
	
	GetDlgItem(IDC_STATIC_NUM2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_NUM2)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_MONEY2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MONEY2)->EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_NUM3)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_NUM3)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_MONEY3)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MONEY3)->EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_NUM4)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_NUM4)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_MONEY4)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MONEY4)->EnableWindow(bEnable);

	GetDlgItem(IDC_EDIT_MONEY)->EnableWindow(bEnable);
	((CButton*)(GetDlgItem(IDC_RADIO_SEND)))->SetCheck(bEnable);
}

void CLotteryDlg::EnableDraw(bool bEnable)
{
	GetDlgItem(IDC_STATIC_SEND_MONEY)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SEND_MONEY)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_SEND_HEIGHT)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SEND_HEIGHT)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_AHASH)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_AHASH)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_SEND)->EnableWindow(bEnable);
	((CButton*)(GetDlgItem(IDC_RADIO_SEND)))->SetCheck(bEnable);
}