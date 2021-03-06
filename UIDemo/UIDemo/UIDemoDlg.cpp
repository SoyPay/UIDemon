
// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "UIDemoDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <algorithm>  
#include <functional>      // For greater<int>( )  
#include <afxinet.h>
#include "UpdateVerDlg.h"
#include "LoadLanguage.h"
#include "SynchronousSocket.h"
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
public:
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
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
	m_bStartServer = TRUE;
	m_IsChinese = -1;
	m_strServerCfgFileName = "dacrs.conf";
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
	CloseProcess("dacrs-d.exe");
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
	ON_COMMAND(ID_MENU_IMPORTWALLET, &CTestDlg::OnMenuImportWallet)
	ON_COMMAND(ID_MENU_DUMPWALLET, &CTestDlg::OnMenuDumpWallet)
	ON_COMMAND(ID_CHINESE, &CTestDlg::OnChinse)
	ON_COMMAND(ID_ENGLISH, &CTestDlg::OnEnglish)


	ON_WM_CLOSE()
	ON_COMMAND(IDC_RPC_COMMAND, &CTestDlg::OnRpcCommand)
	ON_COMMAND(ID_BASE, &CTestDlg::OnMenuBase)
	ON_COMMAND(ID_BASEEX, &CTestDlg::OnMenuBaseEx)
	ON_COMMAND(ID_ABOUT,&CTestDlg::OnMenuAboutSmartCoin)
	ON_MESSAGE(WM_SEND_RPC_DATA,&OnSendRPCData)
	ON_MESSAGE(WM_SOCKET_RECEIVE,&OnSocketReceive)
	ON_MESSAGE(WM_SOCKET_CLOSE,&OnSocketClose)
	ON_MESSAGE(WM_SOCKET_CONNECT,&OnSocketConnnect)
	ON_MESSAGE(WM_UPDATE_HEIGHT,&OnUpdateHeight)
	ON_MESSAGE(WM_GET_ACCOUNTINFO,&OnGetAccountInfo)
	ON_MESSAGE(WM_LIST_TX,&OnListTx)
	ON_MESSAGE(WM_GET_ACCLIST,&OnGetAccList)
	ON_MESSAGE(WM_GET_BET_RANDOM_DATA,&OnGetBetRandomData)
	ON_MESSAGE(WM_UPDATE,&OnUpdate)
	ON_WM_TIMER()
END_MESSAGE_MAP()

DWORD WINAPI CheckUpdateProc(LPVOID lpParam)
{
	CTestDlg* pDlg = (CTestDlg*)lpParam;
	if (NULL == pDlg)
	{
		return FALSE;
	}

	string strVersion;
	if (!pDlg->GetVersionFromServer(strVersion))
	{
		return FALSE;
	}

	CString strTitle(_T("V0.02Beta"));
// 	pDlg->GetWindowText(strTitle);
// 	int nPosBegin = strTitle.Find('(');
// 	int nPosEnd = strTitle.Find(')');
// 	strTitle = strTitle.Mid(nPosBegin+1,nPosEnd-nPosBegin-1);
	USES_CONVERSION;
	string strCurVer(W2A(strTitle));

	if (1 == strcmp(strVersion.c_str(),strCurVer.c_str()))
	{
		pDlg->PostMessage(WM_UPDATE);
	}

	return TRUE;
}

// CTestDlg 消息处理程序
BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
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
	
	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);
	
	SetProp(this->GetSafeHwnd(),WINDOW_TAG,(HANDLE)1);

	//获取EXE文件所在路径
	CStringA strDir = GetExePath();
	//检测EXE文件所在路径是否包含中文或者空格

	CheckPathValid(strDir);

	ParseUIConfigFile(strDir.GetString());

	//根据操作系统语言加载不同的语言。



	LoadLanguage(strDir);

	//检测自动升级
	CheckUpdate();

	//启动服务程序
	StartSeverProcess(strDir);
	

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

//	m_pWalletDlg->m_Parent = this;

	m_pBusinessDlg = new CBusinessDlg;
	m_pBusinessDlg->Create(CBusinessDlg::IDD,this);
	m_pWalletDlg->MoveWindow(&rcCLient);
	m_pBusinessDlg->ShowWindow(SW_HIDE);
	
	m_pRPCDlg = new CRPCDlg;
	m_pRPCDlg->Create(CRPCDlg::IDD,this);
	m_pRPCDlg->ShowWindow(SW_HIDE);

	CSoyPayHelp::getInstance()->InitialRpcCmd(m_pRPCDlg,m_sendPreHeadstr,m_sendendHeadstr);

	m_pBaseFunctionDlg = new CBaseFuncionDlg;
	m_pBaseFunctionDlg->Create(CBaseFuncionDlg::IDD,this);
	m_pBaseFunctionDlg->MoveWindow(&rcCLient);
	m_pBaseFunctionDlg->ShowWindow(SW_HIDE);

	m_pBaseFunctionExDlg = new CBaseFunctionExDlg;
	m_pBaseFunctionExDlg->Create(CBaseFunctionExDlg::IDD,this);
	m_pBaseFunctionExDlg->MoveWindow(&rcCLient);
	m_pBaseFunctionExDlg->ShowWindow(SW_HIDE);
	m_pBaseFunctionExDlg->Init(std::move(m_pBusinessDlg->GetBaseDlgPtrMap()));
	
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
	ASSERT(m_pBusinessDlg&& m_pWalletDlg&&m_pBaseFunctionDlg&&m_pBaseFunctionExDlg);
	m_pBusinessDlg->ShowWindow(SW_HIDE);
	m_pBaseFunctionDlg->ShowWindow(SW_HIDE);
	m_pWalletDlg->ShowWindow(SW_SHOW);
	m_pBaseFunctionExDlg->ShowWindow(SW_HIDE);
}


void CTestDlg::OnMenuTrade()
{
	ASSERT(m_pBusinessDlg&& m_pWalletDlg&&m_pBaseFunctionDlg&&m_pBaseFunctionExDlg);
	m_pBusinessDlg->ShowWindow(SW_SHOW);
	m_pBaseFunctionDlg->ShowWindow(SW_HIDE);
	m_pWalletDlg->ShowWindow(SW_HIDE);
	m_pBaseFunctionExDlg->ShowWindow(SW_HIDE);
}

void CTestDlg::OnMenuBase()
{
	ASSERT(m_pBusinessDlg&& m_pWalletDlg&&m_pBaseFunctionDlg&&m_pBaseFunctionExDlg);
	m_pBaseFunctionDlg->ShowWindow(SW_SHOW);
	m_pBusinessDlg->ShowWindow(SW_HIDE);
	m_pWalletDlg->ShowWindow(SW_HIDE);
	m_pBaseFunctionExDlg->ShowWindow(SW_HIDE);
}

void CTestDlg::OnMenuBaseEx()
{
	ASSERT(m_pBusinessDlg&& m_pWalletDlg&&m_pBaseFunctionDlg&&m_pBaseFunctionExDlg);
	m_pBaseFunctionExDlg->Show();
	m_pBaseFunctionDlg->ShowWindow(SW_HIDE);
	m_pBusinessDlg->ShowWindow(SW_HIDE);
	m_pWalletDlg->ShowWindow(SW_HIDE);
}

void CTestDlg::OnMenuImportWallet()
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,_T("所有文件(*.*)|*.*||"));
	if (IDOK == dlg.DoModal())
	{
		CString strPath = dlg.GetPathName();
		CString strCommand;
		strCommand.Format(_T("%s %s"),_T("importwallet"),strPath);
		CStringA strSendData;
	
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);	
		m_pWalletDlg->Clearn();
		m_mapAccount.clear();
		MessageBox(_T("导入钱包成功请重新启动钱包\r\nImport wallet succesfully pls resart the app"));
		PostMessage(WM_CLOSE);
	}
}

void CTestDlg::OnChinse()
{
	CString strPath;
	CString strPath2;
	strPath.Format(_T("%s%s"),GetExePath2(),_T("\\english"));
	strPath2.Format(_T("%s%s"),GetExePath2(),_T("\\chinese"));

	DeleteFile(strPath);

	HANDLE  hFile=CreateFile(strPath2,GENERIC_WRITE,FILE_SHARE_WRITE ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	CloseHandle(hFile);
	MessageBox(_T("下次启动时，将会切换至中文界面\r\nThe Next Start App Will Switch To Chinese UI"));

}
void CTestDlg::OnEnglish()
{
	CString strPath;
	CString strPath2;
	strPath.Format(_T("%s%s"),GetExePath2(),_T("\\english"));
	strPath2.Format(_T("%s%s"),GetExePath2(),_T("\\chinese"));

	DeleteFile(strPath2);

	HANDLE  hFile=CreateFile(strPath,GENERIC_WRITE,FILE_SHARE_WRITE ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	CloseHandle(hFile);
	MessageBox(_T("下次启动时，将会切换至英文界面\r\nThe Next Start App Will Switch To English UI"));
}

void CTestDlg::OnMenuDumpWallet()
{
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,_T("所有文件(*.*)|*.*||"));
	if (IDOK == dlg.DoModal())
	{
		CString strPath = dlg.GetPathName();
		CString strCommand;
		strCommand.Format(_T("%s %s"),_T("dumpwallet"),strPath);
		CStringA strSendData;
		RPCCommandToJson(strCommand,strSendData);
		AfxGetMainWnd()->SendMessage(WM_SEND_RPC_DATA,(WPARAM)&strSendData,emRPCCommand);
	}
}

void CTestDlg::OnMenuAboutSmartCoin()
{
	CAboutDlg dlg;
	dlg.DoModal();
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

void CTestDlg::StartSeverProcess(const CStringA& strdir)
{
	STARTUPINFOA si; 
	memset(&si, 0, sizeof(STARTUPINFO));  
	si.cb = sizeof(STARTUPINFO);  
	si.dwFlags = STARTF_USESHOWWINDOW;  
	si.wShowWindow =SW_HIDE;//SW_HIDE; //SW_SHOW;  
			
	ProductHttpHead(strdir,m_strServerCfgFileName,m_rpcport,m_sendPreHeadstr,m_sendendHeadstr);
	CStringA str = "dacrs-d.exe -datadir=";
	str.AppendFormat("%s",strdir);

	if (!m_bStartServer)
	{
		return ;
	}

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
			}
			break;

		case emBaseFunction:
			{
				strShowData = ParseRecvData(m_recivestr.c_str());
				m_pBaseFunctionDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&strShowData,m_emSendType);
			}
			break;

		case emRegisterScript:
			{
				strShowData = ParseRecvData(m_recivestr.c_str());
				m_pBaseFunctionExDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&strShowData,m_emSendType);
			}
			break;

		case emContract:
			{
				strShowData = ParseRecvData(m_recivestr.c_str());
				m_pBusinessDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&strShowData,m_emSendType);
			}
			break;

		case emGetAccData:
			{
				ParseJsonAddrToMap(m_recivestr.c_str(),m_mapAccount);
				if (m_pBaseFunctionDlg && IsWindow(m_pBaseFunctionDlg->GetSafeHwnd() ))
				{
					m_pWalletDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&m_mapAccount,m_emSendType);
				}
				
				//m_pBusinessDlg->UpdateRegScriptSignAddr();
				PostMessage(WM_GET_ACCOUNTINFO);
			}
			break;

		case emGetAccountInfo:
			{
				strShowData = ParseRecvData(m_recivestr.c_str());
				if (m_pBaseFunctionDlg && IsWindow(m_pBaseFunctionDlg->GetSafeHwnd() ))
				{
					m_pWalletDlg->InitAccountInfoTree(strShowData.GetString());
				}
			}
			break;

		case emGetRegScriptHash:
		case emGetRegScriptID:
			{
				strShowData = ParseRecvData(m_recivestr.c_str());
				m_pBaseFunctionExDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&strShowData,m_emSendType);
			}
			break;

		case emListTx:
			{
				strShowData = ParseRecvData(m_recivestr.c_str());
				if (m_pBaseFunctionDlg && IsWindow(m_pBaseFunctionDlg->GetSafeHwnd() ))
				{
					m_pWalletDlg->InitTxInfoTree(strShowData.GetString());
				}
			}
			break;

		case emGetBetRandomData:
			{
				strShowData = ParseRecvData(m_recivestr.c_str());
				m_pBusinessDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&strShowData,m_emSendType);
			}
			break;
		default:
			ASSERT(0);
		}
	
		m_pRPCDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&strShowData,m_emSendType);
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
			PostMessage(WM_GET_ACCLIST);
		}
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

CString CTestDlg::GetAddrByRegID(const CString& strFullRegID)
{
	if (strFullRegID.IsEmpty())
	{
		return CString();
	}

	CString strCompressRegID = GetCompressRegID(strFullRegID);
	for (map<CString,CString>::iterator it = m_mapAccount.begin();it != m_mapAccount.end();++it)
	{
		if (strCompressRegID == it->second)
		{
			return it->first;
		}
	}
	
	return CString();
}

void CTestDlg::GetAccountInfo(map<CString,CString>& mapAccountInfo)
{
	mapAccountInfo = m_mapAccount;
}

void CTestDlg::ParseUIConfigFile(const string& strExeDir) 
{
	CStringA configpath = "";
	configpath.AppendFormat("%s",strExeDir.c_str());
	configpath.AppendFormat("\\%s","UIDemo_JSon.conf");
	
	if (PathFileExistsA(configpath))
	{
		CJsonConfigHelp::getInstance()->ReadJsonConfig(configpath.GetString());
		CMainCfg mainCfg;
		CJsonConfigHelp::getInstance()->GetMainCfgData(mainCfg);
		m_bStartServer = mainCfg.bStartServer;
		//m_IsChinese = mainCfg.IntIsChinese;
		m_strServerCfgFileName = mainCfg.strServerCfgName;
	}
}

bool CTestDlg::GetValueFormCfgFile(const string& strData,const string& strKey,string& strValue)
{
	int nPos = strData.find(strKey);
	if (string::npos == nPos)
	{
		return false;
	}


	nPos = strData.find("=",nPos);
	if (string::npos == nPos)
	{
		return false;
	}

	strValue = strData.substr(nPos+1);
	return true;
}

LRESULT CTestDlg::OnUpdateHeight(WPARAM wparam,LPARAM lparam)
{
	m_nHeight = static_cast<int>(wparam);
	return TRUE;
}

CString CTestDlg::GetScriptID()
{
	ASSERT(m_pBaseFunctionExDlg);
	return m_pBaseFunctionExDlg->GetScriptID();
}


LRESULT CTestDlg::OnGetAccountInfo(WPARAM wparam,LPARAM lparam)
{
	CString* pStrAddr = (CString*)wparam;
	CString strAddr;
	if (nullptr == pStrAddr || pStrAddr->IsEmpty())
	{
		map<CString,CString>::iterator it = m_mapAccount.begin();
		if (it == m_mapAccount.end()) 
		{
			return FALSE;
		}
		else
		{
			strAddr = it->first;
		}
	}
	else
	{
		strAddr = *pStrAddr;
	}

	CString strCommand;
	strCommand.Format(_T("%s %s"),_T("getaccountinfo"),strAddr);
	CStringA strSendData;
	RPCCommandToJson(strCommand,strSendData);
	AfxGetMainWnd()->SendMessage(WM_SEND_RPC_DATA,(WPARAM)&strSendData,emGetAccountInfo);
	return TRUE;
}

LRESULT CTestDlg::OnListTx(WPARAM wparam,LPARAM lparam)
{
	CString strCommand;
	strCommand.Format(_T("%s"),_T("getalltxinfo"));
	CStringA strSendData;
	RPCCommandToJson(strCommand,strSendData);
	AfxGetMainWnd()->SendMessage(WM_SEND_RPC_DATA,(WPARAM)&strSendData,emListTx);
	return TRUE;
}

LRESULT CTestDlg::OnGetAccList(WPARAM wparam,LPARAM lparam)
{
	CString strCommand = _T("listaddr");
	CStringA strSendData;
	RPCCommandToJson(strCommand,strSendData);
	AfxGetMainWnd()->SendMessage(WM_SEND_RPC_DATA,(WPARAM)&strSendData,emGetAccData);
	return TRUE;
}

LRESULT CTestDlg::OnGetBetRandomData(WPARAM wparam,LPARAM lparam)
{
	CString strCommand = _T("getbetrandomdata");
	CStringA strSendData;
	RPCCommandToJson(strCommand,strSendData);
	AfxGetMainWnd()->SendMessage(WM_SEND_RPC_DATA,(WPARAM)&strSendData,emGetBetRandomData);
	return TRUE;
}

LRESULT CTestDlg::OnUpdate(WPARAM wparam,LPARAM lparam)
{
	CUpdateVerDlg dlg;
	dlg.DoModal();
	exit(0);
	return TRUE;
}


void CTestDlg::CheckUpdate()
{
	CloseHandle(CreateThread(NULL,0,CheckUpdateProc,this,0,NULL));
}

bool CTestDlg::GetVersionFromServer(string& strVersion)
{
	CString url(_T("http://api.dspay.org/dacrs/dacrsUpdate.json"));    
	CInternetSession session;
	std::string strHtml;

	try
	{
		CHttpFile* pfile = (CHttpFile*)session.OpenURL(url,1,INTERNET_FLAG_TRANSFER_ASCII||INTERNET_FLAG_RELOAD,NULL,0);

		DWORD dwStatusCode;    
		pfile->QueryInfoStatusCode(dwStatusCode);    
		if(dwStatusCode == HTTP_STATUS_OK)    
		{    
			char strBuff[1025] = {0};
			while ((pfile->Read((void*)strBuff, 1024)) > 0) 
			{ 
				strHtml += strBuff; 
			} 
		}
		else
		{
			return false;
		}

		pfile->Close();    
		delete pfile;    
		session.Close();
	}
	catch (CException* e)
	{
		e;//消除警告
		return false;
	}
	

	if (strHtml.empty())
	{
		return false;
	}

	Json::Reader reader;  
	Json::Value root; 
	
	if (reader.parse(strHtml, root)) 
	{
		strVersion = root["version"].asString();
		return true;
	}
	return false;
}

CStringA  CTestDlg::GetExePath()
{
	char  exepath[MAX_PATH] = {0};
	CStringA   strdir,tmpdir; 
	memset(exepath,0,MAX_PATH); 
	GetModuleFileNameA(NULL,exepath,MAX_PATH); 
	tmpdir = exepath; 
	strdir = tmpdir.Left(tmpdir.ReverseFind('\\'));
	return strdir;
}
CString  CTestDlg::GetExePath2()
{
	char  exepath[MAX_PATH] = {0};
	CString   strdir,tmpdir; 
	memset(exepath,0,MAX_PATH); 
	GetModuleFileNameA(NULL,exepath,MAX_PATH); 
	tmpdir = exepath; 
	strdir = tmpdir.Left(tmpdir.ReverseFind('\\'));
	return strdir;
}

void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShellExecuteW(NULL, L"open", _T("http://bbs.dspay.org/portal.php"), NULL, NULL, SW_SHOWNORMAL);
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CLoadLanguage::getInstance()->Load(_T("about"),this);
	return TRUE; 
}

void CTestDlg::LoadLanguage(const CStringA& strDir1)
{
	CString strPath;
	CString strPath2;
	strPath.Format(_T("%s%s"),GetExePath2(),_T("\\english"));
	strPath2.Format(_T("%s%s"),GetExePath2(),_T("\\chinese"));
	int falg = 0 ;

	if(PathFileExists(strPath2))
	{
		falg = 1;
	}
	else if(PathFileExists(strPath))
	{
		falg = 2;
	}



	LCID lcid = GetSystemDefaultLCID();
	bool bUseChinese = false;
	switch (lcid)
	{
	case 0x0c04://中国香港
	case 0x1404://中国澳门
	case 0x0804://中国大陆
	case 0x1004://中国新加坡
	case 0x0404://中国台湾
		{
			bUseChinese = true;
		}
		break;

	default:
		break;
	}

	if (falg == 1)
	{
		bUseChinese = true;
	}
	if (falg == 2)
	{
		bUseChinese = false;
	}
	CStringA str = GetExePath();
	if (bUseChinese)
	{

		m_hMenu = LoadMenu(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MENU1));

		ASSERT(NULL != m_hMenu);
		::SetMenu(this->GetSafeHwnd(),m_hMenu);

		//初始化语言配置文件路径
		CStringA strDir = GetExePath();
		CString str(strDir.GetString());
		str += _T("\\language_Chinese.ini");
		CLoadLanguage::getInstance()->Init(str);
	}
	else
	{
		m_hMenu = LoadMenu(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MENU2));

		ASSERT(NULL != m_hMenu);
		::SetMenu(this->GetSafeHwnd(),m_hMenu);

		//初始化语言配置文件路径
		CStringA strDir = GetExePath();
		CString str(strDir.GetString());
		str += _T("\\language_English.ini");
		CLoadLanguage::getInstance()->Init(str);
	}
	CString te = CLoadLanguage::getInstance()->GetTips(INVALID_EXE_PATH);
	SetWindowTextW(te);
}

void CTestDlg::CheckPathValid(const CStringA& strDir)
{
	BOOL bExist = FALSE;
	for(int i = 0;i <= strDir.GetLength();i++)
	{
		BYTE bchar = (BYTE)strDir.GetAt(i);
		if(bchar == ' ')
		{
			bExist = TRUE;
			break;
		}
	}

	if (bExist)
	{
		MessageBox(_T("程序不可以放在含有空格的目录下\r\n Don't save the program in the folder named with space"));
		exit(0);
	}
}