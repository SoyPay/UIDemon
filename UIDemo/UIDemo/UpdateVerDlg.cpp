// UpdateVerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "UpdateVerDlg.h"
#include "afxdialogex.h"


// CUpdateVerDlg 对话框

IMPLEMENT_DYNAMIC(CUpdateVerDlg, CDialogEx)

CUpdateVerDlg::CUpdateVerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdateVerDlg::IDD, pParent)
{

}

CUpdateVerDlg::~CUpdateVerDlg()
{
}

void CUpdateVerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpdateVerDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CUpdateVerDlg::OnNMClickSyslink1)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK3, &CUpdateVerDlg::OnNMClickSyslink3)
END_MESSAGE_MAP()


// CUpdateVerDlg 消息处理程序


BOOL CUpdateVerDlg::OnInitDialog()
{
	/*CDialogEx::OnInitDialog();

	int x=8;
	int y=220;
	m_LinkStatic.CreateEx(0,_T("SysLink")
		,_T("设计制作:邓学彬 <a href=\"http://www.it608.com/\">CSDN博客</a> <a href=\"http://t.qq.com/dengxuebin\">腾讯微博</a>")
		,WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,CRect(x,y,x+300,y+40),this,IDC_SYSLINK1);
	HFONT hFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT); 
	m_LinkStatic.SendMessage(WM_SETFONT,(WPARAM)hFont);
	m_LinkStatic.ShowWindow(SW_SHOW);*/
	return TRUE;  
}


void CUpdateVerDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShellExecuteW(NULL, L"open", _T("http://bbs.dspay.org/forum.php?mod=viewthread&tid=1442&extra=page%3D1"), NULL, NULL, SW_SHOWNORMAL);
	CDialogEx::OnOK();
}


void CUpdateVerDlg::OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShellExecuteW(NULL, L"open", _T("http://bbs.dspay.org/portal.php"), NULL, NULL, SW_SHOWNORMAL);
}
