// RPCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "RPCDlg.h"
#include "afxdialogex.h"
#include "CommonStruct.h"
#include "LoadLanguage.h"


// CRPCDlg 对话框

IMPLEMENT_DYNAMIC(CRPCDlg, CDialogEx)

CRPCDlg::CRPCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRPCDlg::IDD, pParent)
{

}

CRPCDlg::~CRPCDlg()
{
}

void CRPCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRPCDlg, CDialogEx)
	//ON_MESSAGE(WM_SEND_RPCCOMMAND,&OnInputRPCCommand)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_MESSAGE(WM_SHOW_SEND_DATA,OnShowSendData)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CRPCDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()

BOOL CRPCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CLoadLanguage::getInstance()->Load(_T("rpc"),this);

	CRect rcClient;
	GetClientRect(&rcClient);

	CEdit* pShowInfoEdit = (CEdit*)GetDlgItem(IDC_EDIT_SHOWINFO);
	ASSERT(pShowInfoEdit);
	pShowInfoEdit->MoveWindow(5,10,rcClient.Width()-60,rcClient.Height()-130);

	CStatic* pStaticInput = (CStatic*)GetDlgItem(IDC_STATIC_INPUT);
	ASSERT(pStaticInput);
	pStaticInput->MoveWindow(5,rcClient.Height()-100,40,20);

	CEdit* pInputEdit = (CEdit*)GetDlgItem(IDC_EDIT_INPUT);
	ASSERT(pInputEdit);
	pInputEdit->MoveWindow(50,rcClient.Height()-103,rcClient.Width()-160,25);

	CButton* pBtnClear = (CButton*)GetDlgItem(IDC_BTN_CLEAR);
	ASSERT(pBtnClear);
	pBtnClear->MoveWindow(rcClient.Width()-95,rcClient.Height()-103,30,25);
	return TRUE; 
}


BOOL CRPCDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_EDIT_INPUT)->GetSafeHwnd())
		{
			CString strShow;
			GetDlgItem(IDC_EDIT_SHOWINFO)->GetWindowTextW(strShow);

			CString strCommand;
			GetDlgItem(IDC_EDIT_INPUT)->GetWindowTextW(strCommand);
			GetDlgItem(IDC_EDIT_INPUT)->SetWindowTextW(_T(""));
	
			if (!strShow.IsEmpty() )
			{
				strShow += _T("\r\n");
			}

			strShow += strCommand;
			strShow += _T(":");	
			GetDlgItem(IDC_EDIT_SHOWINFO)->SetWindowTextW(strShow);

			CStringA strSendData;
			RPCCommandToJson(strCommand,strSendData);
			AfxGetMainWnd()->SendMessage(WM_SEND_RPC_DATA,(WPARAM)&strSendData,emRPCCommand);
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CRPCDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	UpdateEditContent(str,_T("RecvData:"));
	return TRUE;
}



void CRPCDlg::OnBnClickedBtnClear()
{
	GetDlgItem(IDC_EDIT_SHOWINFO)->SetWindowTextW(_T(""));
}

LRESULT CRPCDlg::OnShowSendData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	UpdateEditContent(str,_T("SendData:"));
	return TRUE;
}

void CRPCDlg::UpdateEditContent(const CStringA& strNewData,const CString& strKeyWord)
{
	CString strShow;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SHOWINFO);
	pEdit->GetWindowTextW(strShow);
	if(strShow.GetLength()> 1024*10)
	strShow.Empty();
	strShow += _T("\r\n");
	strShow += strKeyWord;
	strShow += _T("\r\n");
	strShow += CString(strNewData.GetString());
	pEdit->SetWindowTextW(strShow);

	int nLines = pEdit->GetLineCount();
	pEdit->LineScroll(nLines);
}