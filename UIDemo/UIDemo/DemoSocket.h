#pragma once
#include <afxsock.h>

class CDemoSocket : public CAsyncSocket
{
public:
	CDemoSocket();
	virtual ~CDemoSocket();
public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
private:
	CDialog *m_pWnd;
public:
	void SetParent(CDialog *pWnd);

};


