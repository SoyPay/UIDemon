#pragma once
#include <afxsock.h>
#include "CommonStruct.h"
class CSynchronousSocket
{
public:
	CSynchronousSocket(void);
	~CSynchronousSocket(void);
private:

	SOCKET  m_Socket ;
	BOOL    OnInitSocket();   //��ʼ��SOCKET
	SOCKET  OnConnnect(const char *pIpAddr, WORD nPort);   //���� ����SOCKET
	BOOL    OnSend(const char* sCmdBuff , int nLen);
	vector<unsigned char> buffer;
	void    RecvDataThread(); //���������߳�
	static UINT WINAPI RecvDataProc(LPVOID pParam);
	void    OnClose();
	bool IsEnd;
public:


public:

public:
	
	static int GetRpcRes(const string ip,int port,string cmd,string & rev,int timerout);
};

