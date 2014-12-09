
// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "UIDemoDlg.h"
#include "afxdialogex.h"
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestDlg 对话框

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hMenu = NULL;
	m_pRPCDlg = NULL;
	m_pWalletDlg = NULL;
	m_pBusinessDlg = NULL;
	m_pBaseFunctionDlg = NULL;
	m_bConnected = FALSE;
	m_szRecvBuffer = new char[BUFFER_SIZE];
	m_emSendType = emNoType;
}

CTestDlg::~CTestDlg()
{
	m_pWalletDlg->DestroyWindow();
	delete m_pWalletDlg;

	m_pBusinessDlg->DestroyWindow();
	delete m_pBusinessDlg;

	m_pRPCDlg->DestroyWindow();
	delete m_pRPCDlg;

	m_pBaseFunctionDlg->DestroyWindow();
	delete m_pBaseFunctionDlg;

	delete []m_szRecvBuffer;
	m_szRecvBuffer = nullptr;
	CloseProcess("soypayd.exe");
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CTestDlg::OnBnClickedCancel)
	ON_COMMAND(ID_MENU_WALLET, &CTestDlg::OnMenuWallet)
	ON_COMMAND(ID_MENU_TRADE, &CTestDlg::OnMenuTrade)
	ON_WM_CLOSE()
	ON_COMMAND(IDC_RPC_COMMAND, &CTestDlg::OnRpcCommand)
	ON_COMMAND(ID_BASE, &CTestDlg::OnMenuBase)
	ON_MESSAGE(WM_SEND_RPC_DATA,&OnSendRPCData)
	ON_MESSAGE(WM_SOCKET_RECEIVE,&OnSocketReceive)
	ON_MESSAGE(WM_SOCKET_CLOSE,&OnSocketClose)
	ON_MESSAGE(WM_SOCKET_CONNECT,&OnSocketConnnect)
	ON_WM_TIMER()
	ON_COMMAND(ID_TEST, &CTestDlg::OnTest)
END_MESSAGE_MAP()


// CTestDlg 消息处理程序

BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_hMenu = LoadMenu(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MENU1));
	ASSERT(NULL != m_hMenu);
	::SetMenu(this->GetSafeHwnd(),m_hMenu);

	StartSeverProcess();

	m_socket.SetParent(this);
	if (!AfxSocketInit())
	{
		MessageBox(L"WindowSocket initial failed!", L"Receive", MB_ICONSTOP);
	}

	CRect rcCLient;
	GetClientRect(&rcCLient);
	m_pWalletDlg = new CWalletDlg;
	m_pWalletDlg->Create(CWalletDlg::IDD,this);
	m_pWalletDlg->MoveWindow(&rcCLient);
	m_pWalletDlg->Init();
	m_pWalletDlg->ShowWindow(SW_SHOW);

	m_pBusinessDlg = new CBusinessDlg;
	m_pBusinessDlg->Create(CBusinessDlg::IDD,this);
	m_pWalletDlg->MoveWindow(&rcCLient);
	m_pBusinessDlg->ShowWindow(SW_HIDE);
	
	m_pRPCDlg = new CRPCDlg;
	m_pRPCDlg->Create(CRPCDlg::IDD,this);
	m_pRPCDlg->ShowWindow(SW_HIDE);

	m_pBaseFunctionDlg = new CBaseFuncionDlg;
	m_pBaseFunctionDlg->Create(CBaseFuncionDlg::IDD,this);
	m_pBaseFunctionDlg->MoveWindow(&rcCLient);
	m_pBaseFunctionDlg->ShowWindow(SW_HIDE);

	//启动定时器延时获取addr数据
	SetTimer(1,3000,NULL);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void CTestDlg::OnMenuWallet()
{
	ASSERT(m_pBusinessDlg&& m_pWalletDlg&&m_pBaseFunctionDlg);
	m_pBusinessDlg->ShowWindow(SW_HIDE);
	m_pBaseFunctionDlg->ShowWindow(SW_HIDE);
	m_pWalletDlg->ShowWindow(SW_SHOW);
}


void CTestDlg::OnMenuTrade()
{
	ASSERT(m_pBusinessDlg&& m_pWalletDlg&&m_pBaseFunctionDlg);
	m_pBusinessDlg->ShowWindow(SW_SHOW);
	m_pBaseFunctionDlg->ShowWindow(SW_HIDE);
	m_pWalletDlg->ShowWindow(SW_HIDE);
}

void CTestDlg::OnMenuBase()
{
	ASSERT(m_pBusinessDlg&& m_pWalletDlg&&m_pBaseFunctionDlg);
	m_pBaseFunctionDlg->ShowWindow(SW_SHOW);
	m_pBusinessDlg->ShowWindow(SW_HIDE);
	m_pWalletDlg->ShowWindow(SW_HIDE);
}



void CTestDlg::OnClose()
{
	DestroyMenu(m_hMenu);

	CDialogEx::OnClose();
}


void CTestDlg::OnRpcCommand()
{
	ASSERT(m_pRPCDlg);
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	
	m_pRPCDlg->MoveWindow(rcWindow.right+10,rcWindow.top+5,300,rcWindow.Height()-8);
	m_pRPCDlg->ShowWindow(SW_SHOW);
}

void CTestDlg::StartSeverProcess()
{
	STARTUPINFOA si; 
	memset(&si, 0, sizeof(STARTUPINFO));  
	si.cb = sizeof(STARTUPINFO);  
	si.dwFlags = STARTF_USESHOWWINDOW;  
	si.wShowWindow =SW_HIDE;//SW_HIDE; //SW_SHOW;  

	char  exepath[MAX_PATH] = {0};
	CStringA  strdir,tmpdir; 
	memset(exepath,0,MAX_PATH); 
	GetModuleFileNameA(NULL,exepath,MAX_PATH); 
	tmpdir=exepath; 
	strdir=tmpdir.Left(tmpdir.ReverseFind('\\'));
	ProductHttpHead(strdir,m_rpcport,m_sendPreHeadstr,m_sendendHeadstr);
	CStringA str = "soypayd.exe -datadir=";
	str.AppendFormat("%s",strdir);

	//return ;
	if(!CreateProcessA(NULL,(LPSTR)str.GetString(),NULL,NULL,FALSE,0,NULL,NULL,&si,&sever_pi))   
	{  
		int n = GetLastError();
		AfxMessageBox(L"sever error!");
		exit(1);  
	}  
}
void CTestDlg::CloseProcess(const string& exename)
{
	HANDLE SnapShot,ProcessHandle;  
	SHFILEINFO shSmall;  
	PROCESSENTRY32 ProcessInfo;   
	int count  = 0;
	while(1)  
	{  
		SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  
		if(SnapShot!=NULL)   
		{  
			ProcessInfo.dwSize=sizeof(ProcessInfo);
			BOOL Status=Process32First(SnapShot,&ProcessInfo);  
			while(Status)  
			{    
				SHGetFileInfo(ProcessInfo.szExeFile,0,&shSmall,  
					sizeof(shSmall),SHGFI_ICON|SHGFI_SMALLICON);
				USES_CONVERSION;
				CString str(ProcessInfo.szExeFile);
				char* pData = W2A(str);
				strlwr(pData);  
				for(int i=0;i<3;i++)  
					if(!strcmp(strlwr((LPSTR)exename.c_str()),pData))   
					{  

						ProcessHandle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,ProcessInfo.th32ProcessID);  
						TerminateProcess(ProcessHandle,0);  
						return;  
					}  
					Status=Process32Next(SnapShot,&ProcessInfo);  
			}  
			break;
		}  

		Sleep(1000);  
	}  

}

void CTestDlg::ConnetSever()
{
	if (m_bConnected)
	{
		return;
	}

	int err = 0;
	m_socket.m_hSocket = INVALID_SOCKET;

	if(m_socket.Create())
	{
		err = m_socket.Connect(_T("127.0.0.1"),atoi(m_rpcport));
		if (err != SOCKET_ERROR && err != EWOULDBLOCK)
		{
			//m_bConnected = true;
		}
	}
}

CStringA CTestDlg::GetResultStr(const CStringA& message)
{
	CStringA ret;
	int pos = message.Find("Server:");
	if (pos >=0)
	{
		pos = message.Find('\n',pos);
		if (pos >=0)
		{
			ret = message.Mid(pos);

		}
	}
	return ret;
}

LRESULT CTestDlg::OnSocketConnnect(WPARAM wparam,LPARAM lparam)
{
	Send();
	return TRUE;
}

LRESULT CTestDlg::OnSocketClose(WPARAM wparam,LPARAM lparam)
{
	m_socket.Close();
	m_bConnected  = FALSE;
	return TRUE;
}

LRESULT CTestDlg::OnSocketReceive(WPARAM wparam,LPARAM lparam)
{
	memset(m_szRecvBuffer,0,BUFFER_SIZE);
	int bufferSize = BUFFER_SIZE - 1;
	int iRcvd; 
	CStringA str;

	TRACE(_T("recv: %d\r\n"),GetTickCount());
	iRcvd = m_socket.Receive(m_szRecvBuffer,bufferSize);
	if (iRcvd > 0)
	{
		m_szRecvBuffer[iRcvd] = '\0';
		m_recivestr.clear();
		str = m_szRecvBuffer;
		m_recivestr = GetResultStr(str);

		CStringA strShowData;
		switch(m_emSendType)
		{
		case emRPCCommand:
			{
				strShowData = ParseRecvData(m_recivestr.c_str());
				m_pRPCDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&strShowData);
			}
			break;

		case emBaseFunction:
			{
				strShowData = ParseRecvData(m_recivestr.c_str());
				m_pBaseFunctionDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&strShowData);
				m_pRPCDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&strShowData);
			}
			break;

		case emContract:
			{
				strShowData = ParseRecvData(m_recivestr.c_str());
				m_pBusinessDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&strShowData);
				m_pRPCDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&strShowData);

			}
			break;

		case emGetData:
			{
				ParseJsonAddrToMap(m_recivestr.c_str(),m_mapAccount);
				if (m_pBaseFunctionDlg && IsWindow(m_pBaseFunctionDlg->GetSafeHwnd() ))
				{
					m_pWalletDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&m_mapAccount);
				}
				
			}
			break;
		default:
			ASSERT(0);
		}
	
	}

	return TRUE;
}

LRESULT CTestDlg::OnSendRPCData(WPARAM wparam,LPARAM lparam)
{	
	CStringA strCommand = *(CStringA*)wparam;
	m_sendCommand = strCommand.GetString();
	m_emSendType = static_cast<emSendDataType>(lparam);

	//显示发送的json数据
	if (m_pRPCDlg)
	{
		CStringA strSendData(m_sendCommand.c_str());
		m_pRPCDlg->SendMessage(WM_SHOW_SEND_DATA,(WPARAM)&strSendData);
	}

	//连接服务器发送数据
	ConnetSever();
	return TRUE;
}

void CTestDlg::Send()
{
	int nLen = 0;
	int nSend = 0;

	string nSendStr = "";
	nSendStr += m_sendPreHeadstr;
	CStringA nMidStr = "Content-Length: ";
	nMidStr.AppendFormat("%d\r\n",m_sendCommand.length());
	nSendStr += nMidStr.GetString();
	nSendStr += m_sendendHeadstr.GetString();
	nSendStr += m_sendCommand;
	int len = m_sendCommand.length();
	if (nSendStr.c_str() != NULL)
	{
		nLen = nSendStr.length();

		nSend = m_socket.Send(nSendStr.c_str(),nLen);
		if (nSend == SOCKET_ERROR)
		{
			int n = WSAGetLastError();
			AfxMessageBox(L"socket error");
		}
	}
}

void CTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		{
			KillTimer(nIDEvent);
			CString strCommand = _T("listaddr");
			CStringA strSendData;
			RPCCommandToJson(strCommand,strSendData);
			AfxGetMainWnd()->SendMessage(WM_SEND_RPC_DATA,(WPARAM)&strSendData,emGetData);
		}
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);
}


DWORD WINAPI ThreadPorc(LPVOID lpvoid)
{
	////注册脚本交易
	//CreateScriptTx("5zQPcC1YpFMtwxiH787pSXanUECoGsxUq3KZieJxVG",false,"D:\\cppwork\\vmsdk\\SecuredTrade\\Debug\\Exe\\SecuredTrade.bin",10000,10);
	//Sleep(2000);

	////挖矿
	//Setgenerate(true);
	//Sleep(2000);

	////第一个合约交易
	//TRACE(_T("3333:%d\r\n"),GetTickCount());
	//FIRST_CONTRACT firstContract;
	//USES_CONVERSION;
	//PacketFirstContract(emFirstStep,
	//	"000000000900",
	//	"000000000500",
	//	"000000000300",
	//	100,
	//	1000,
	//	50000,
	//	10000,
	//	10000,
	//	&firstContract);
	//vector<unsigned char> vTemp;
	//char* pData = (char*)&firstContract;
	//vTemp.assign(pData,pData +sizeof(FIRST_CONTRACT));
	//vector<string> vAccount;
	//vAccount.push_back("5zQPcC1YpFMtwxiH787pSXanUECoGsxUq3KZieJxVG");
	//CreateContractTx("010000000100",vAccount,vTemp,100,20000);
	return 0;
}

void CTestDlg::OnTest()
{
	CloseHandle(CreateThread(NULL,0,ThreadPorc,NULL,0,NULL) );
}

void CTestDlg::GetAccountInfo(map<CString,CString>& mapAccountInfo)
{
	mapAccountInfo = m_mapAccount;
}