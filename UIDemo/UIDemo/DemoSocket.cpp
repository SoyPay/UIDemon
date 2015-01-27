// mySocket.cpp : ÊµÏÖÎÄ¼þ
//

#include "stdafx.h"
#include "DemoSocket.h"
#include "UIDemoDlg.h"

// mySocket

CDemoSocket::CDemoSocket()
: m_pWnd(NULL)
{
}

CDemoSocket::~CDemoSocket()
{
	if(m_hSocket !=INVALID_SOCKET) 
	{
		Close();
	}
}

void CDemoSocket::SetParent(CDialog *pWnd)
{
	m_pWnd = pWnd;
}
void CDemoSocket::OnReceive(int nErrorCode)
{
	ASSERT(m_pWnd);
	m_pWnd->SendMessage(WM_SOCKET_RECEIVE);
	CAsyncSocket::OnReceive(nErrorCode);
}

 void CDemoSocket::OnClose(int nErrorCode) 
 {
	 ASSERT(m_pWnd);
	 m_pWnd->SendMessage(WM_SOCKET_CLOSE);
	 CAsyncSocket::OnClose(nErrorCode);
 }

 void CDemoSocket::OnConnect(int nErrorCode) 
 {
	 if (!nErrorCode) 
	 {
		 ASSERT(m_pWnd);
		 m_pWnd->SendMessage(WM_SOCKET_CONNECT);
		 CAsyncSocket::OnClose(nErrorCode);
	 }
	 else
	 {
		 /* CString str;
		 str.Format(_T("连接错误,错误码:%d"),nErrorCode);
		 AfxMessageBox(str);*/
	 }
 }