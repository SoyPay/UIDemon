// WalletDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "WalletDlg.h"
#include "afxdialogex.h"
#include "LoadLanguage.h"
#include <algorithm>
#define START_CONNECT		1
// CWalletDlg 对话框

IMPLEMENT_DYNAMIC(CWalletDlg, CDialogEx)

CWalletDlg::CWalletDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWalletDlg::IDD, pParent)
{
	m_bConnected = FALSE;
	m_szBuffer = new char[SOCKET_BUFFER];
	m_mapTxType.insert(make_pair("RegisterAccTx",REG_ACCT_TX));
	m_mapTxType.insert(make_pair("NormalTx",COMMON_TX));
	m_mapTxType.insert(make_pair("ContractTx",CONTRACT_TX));
	m_mapTxType.insert(make_pair("FreezeTx",FREEZE_TX));
	m_mapTxType.insert(make_pair("RewardTx",REWARD_TX));
	m_mapTxType.insert(make_pair("RegScriptTx",REG_SCRIPT_TX));
}

CWalletDlg::~CWalletDlg()
{
	delete []m_szBuffer;
}

void CWalletDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ACC, m_TreeAcc);
	DDX_Control(pDX, IDC_TREE_TX, m_TreeTx);
}


BEGIN_MESSAGE_MAP(CWalletDlg, CDialogEx)
	ON_MESSAGE(WM_SOCKET_RECEIVE,&OnSocketReceive)
	ON_MESSAGE(WM_SOCKET_CLOSE,&OnSocketClose)
	ON_MESSAGE(WM_SOCKET_CONNECT,&OnSocketConnnect)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_FRESH_ACCINFO, &CWalletDlg::OnBnClickedBtnFreshAccinfo)
	ON_BN_CLICKED(IDC_BTN_FRESH_TX_INFO, &CWalletDlg::OnBnClickedBtnFreshTxInfo)
	ON_CBN_SELCHANGE(IDC_COMBO_KEY, &CWalletDlg::OnCbnSelchangeComboKey)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CWalletDlg::OnNMClickSyslink1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ACC, &CWalletDlg::OnTvnSelchangedTreeAcc)
	ON_STN_CLICKED(IDC_STATIC_HEIGHT, &CWalletDlg::OnStnClickedStaticHeight)
END_MESSAGE_MAP()


// CWalletDlg 消息处理程序
void CWalletDlg::Init()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	/*m_txList.MoveWindow(10,rcClient.bottom/3,rcClient.Width()-20,rcClient.bottom*2/3-10);
	m_txList.GetClientRect(&rcClient);*/

	SetTimer(START_CONNECT,3000,NULL);
}

BOOL CWalletDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CLoadLanguage::getInstance()->Load(_T("wallet"),this);
	m_MsgSocket.SetParent(this);
	return TRUE; 
}

LRESULT CWalletDlg::OnSocketConnnect(WPARAM wparam,LPARAM lparam)
{
	return TRUE;
}

LRESULT CWalletDlg::OnSocketClose(WPARAM wparam,LPARAM lparam)
{
	m_MsgSocket.Close();
	m_bConnected  = FALSE;
	return TRUE;
}

LRESULT CWalletDlg::OnSocketReceive(WPARAM wparam,LPARAM lparam)
{
	memset(m_szBuffer,0,SOCKET_BUFFER);
	int bufferSize = SOCKET_BUFFER - 1;
	int iRcvd; 
	CStringA str;

	iRcvd = m_MsgSocket.Receive(m_szBuffer,bufferSize);
	if (iRcvd > 0)
	{
		CString strData(m_szBuffer);
		static const CString strBlockChanged(_T("block changed:"));
		static const CString strVerifyDB(_T("hello"));
		static const CString strVerifyFinish(_T("Verifying Finished"));
		static const CString strDone(_T("initialize"));
		int nPos = strData.Find(strBlockChanged);
		if (-1 != (nPos = strData.Find(strBlockChanged) )) 
		{
			strData.Replace(strBlockChanged,_T(""));
			
			nPos = strData.Find(_T(" "));
			if (-1 != strData)
			{
				CString strHeight = strData.Left(nPos);
				int nHeight = _ttoi(strHeight);

				nPos = strData.Find(_T(":"));
				strData = strData.Mid(nPos+1);

				CString strOutPut;
				strOutPut.Format(_T("%d time:%s"),nHeight,FormatBlockTime(_ttoi64(strData)));
				GetDlgItem(IDC_STATIC_HEIGHT)->SetWindowTextW(strOutPut);

				AfxGetMainWnd()->SendMessage(WM_UPDATE_HEIGHT,nHeight,0);
			}
			
		}
		else if (-1 != (nPos = strData.Find(strDone) )) 
		{
			GetDlgItem(IDC_STATIC_LOG)->SetWindowText(strData);
			CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_KEY);
			int nCount = pCombox->GetCount();
			if (0 == nCount)
			{
				AfxGetMainWnd()->SendMessage(WM_GET_ACCLIST);
			}
		}
		else
		{
			GetDlgItem(IDC_STATIC_LOG)->SetWindowText(strData);
		}
	}

	return TRUE;
}

void CWalletDlg::ConnetSever()
{
	if (m_bConnected)
	{
		return;
	}

	int err = 0;
	m_MsgSocket.m_hSocket = INVALID_SOCKET;

	if(m_MsgSocket.Create())
	{
		err = m_MsgSocket.Connect(_T("127.0.0.1"),3200);
		if (err != SOCKET_ERROR && err != EWOULDBLOCK)
		{
			m_bConnected = true;
		}
	}
}

void CWalletDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (START_CONNECT == nIDEvent)
	{
		KillTimer(nIDEvent);
		m_MsgSocket.SetParent(this);
		ConnetSever();
	}
	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CWalletDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	map<CString,CString>* pMap = (map<CString,CString>*)wparam;
	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_KEY);
	ASSERT(pMap && pCombox);

	for(map<CString,CString>::iterator it = pMap->begin();it != pMap->end();++it)
	{
		pCombox->AddString(it->first);
	}
	
	pCombox->SetCurSel(0);
	return TRUE;
}

void CWalletDlg::InitTxInfoTree(const string& strAccountInfo)
{
	m_TreeTx.DeleteAllItems();
	HTREEITEM hItemRoot = m_TreeTx.InsertItem(L"Confirmed",TVI_ROOT);
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strAccountInfo, root)) 
		return ;
	const Json::Value& txArray = root["Confirmed"]; 
	LoadTxInfo(txArray,hItemRoot);
	m_TreeTx.Expand(hItemRoot,TVE_EXPAND);

	hItemRoot = m_TreeTx.InsertItem(L"UnConfirmed",TVI_ROOT);
	const Json::Value& txArray2 = root["UnConfirmed"]; 
	LoadTxInfo(txArray2,hItemRoot,FALSE);
	m_TreeTx.Expand(hItemRoot,TVE_EXPAND);
}

void CWalletDlg::LoadTxInfo(const Json::Value& txArray,HTREEITEM hItemRoot,BOOL bComfirmed)
{
	for (size_t i = 0;i<txArray.size();++i)
	{
		const Json::Value& tx = txArray[i];
		CStringA strData;
		strData.Format("txhash:%s",tx["hash"].asString().c_str());
		HTREEITEM hItem = m_TreeTx.InsertItem(CString(strData),hItemRoot);

		string strTxType = tx["txtype"].asString().c_str();
		if (bComfirmed)
		{
			strData.Format("blockhash:%s",tx["blockhash"].asString().c_str());
			m_TreeTx.InsertItem(CString(strData),hItem);
		}

		strData.Format("txtype:%s",strTxType.c_str());
		m_TreeTx.InsertItem(CString(strData),hItem);

		strData.Format("ver:%d",tx["ver"].asInt());
		m_TreeTx.InsertItem(CString(strData),hItem);

		strData.Format("height:%d",tx["height"].asInt());
		m_TreeTx.InsertItem(CString(strData),hItem);

		switch (m_mapTxType[strTxType])
		{
		case REG_ACCT_TX:
			{
				strData.Format("addr:%s",tx["addr"].asString().c_str());
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("pubkey:%s",tx["pubkey"].asString().c_str());
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("miner_pubkey:%d",tx["miner_pubkey"].asString().c_str());
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("fees:%f",tx["fees"].asDouble()/COIN);
				m_TreeTx.InsertItem(CString(strData),hItem);
			}
			break;

		case REWARD_TX:
			{
				strData.Format("addr:%s",tx["addr"].asString().c_str());
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("reward money:%f",tx["reward money"].asDouble()/COIN);
				m_TreeTx.InsertItem(CString(strData),hItem);
			}
			break;

		case COMMON_TX:
			{
				strData.Format("srcaddr:%s",tx["srcaddr"].asString().c_str());
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("desaddr:%s",tx["desaddr"].asString().c_str());
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("money:%f",tx["money"].asDouble()/COIN);
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("fees:%f",tx["fees"].asDouble()/COIN);
				m_TreeTx.InsertItem(CString(strData),hItem);
			}
			break;

		case CONTRACT_TX:
			{
				strData.Format("appid:%s",tx["appid"].asString().c_str());
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("fees:%f",tx["fees"].asDouble()/COIN);
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("Contract:%s",tx["Contract"].asString().c_str());
				m_TreeTx.InsertItem(CString(strData),hItem);

				const Json::Value& accRegID = tx["accountid"];
				HTREEITEM hSubItem = m_TreeTx.InsertItem(CString(_T("accountid:")),hItem);
				for (size_t i = 0;i<accRegID.size();++i)
				{
					strData.Format("%s",accRegID[i].asString().c_str());
					m_TreeTx.InsertItem(CString(strData),hSubItem);
				}

				const Json::Value& vSign = tx["Signature"];
				for (size_t i = 0;i<vSign.size();++i)
				{
					strData.Format("%s",vSign[i].asString().c_str());
					m_TreeTx.InsertItem(CString(strData),hSubItem);
				}
			}
			break;

		case FREEZE_TX:
			{
				strData.Format("addr:%s",tx["addr"].asString().c_str());
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("frozen amt:%f",tx["frozen amt"].asDouble()/COIN);
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("fees:%f",tx["fees"].asDouble()/COIN);
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("free height:%d",tx["free height"].asInt());
				m_TreeTx.InsertItem(CString(strData),hItem);
			}
			break;

		case REG_SCRIPT_TX:
			{
				strData.Format("addr:%s",tx["addr"].asString().c_str());
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("script:%s",tx["script"].asString().c_str());
				m_TreeTx.InsertItem(CString(strData),hItem);

				strData.Format("fees:%f",tx["fees"].asDouble()/COIN);
				m_TreeTx.InsertItem(CString(strData),hItem);

				FillAuthorToTree(hItem,tx);
			}
			break;

		default:
			ASSERT(0);
		}
	}
}

void CWalletDlg::InitAccountInfoTree(const string& strAccountInfo)
{
	m_TreeAcc.DeleteAllItems();
	HTREEITEM hItemRoot = m_TreeAcc.InsertItem(L"Accountinfo",TVI_ROOT);

	Json::Reader reader;  
	Json::Value root; 
	if (reader.parse(strAccountInfo, root)) 
	{
		CStringA strData;
		strData.Format("postion:%s",root["postion"].asString().c_str());
		m_TreeAcc.InsertItem(CString(strData),hItemRoot);

		//地址
		strData.Format("Address:%s",root["Address"].asString().c_str());
		m_TreeAcc.InsertItem(CString(strData),hItemRoot);

		//KeyID
		strData.Format("KeyID:%s",root["KeyID"].asString().c_str());
		m_TreeAcc.InsertItem(CString(strData),hItemRoot);

		//RegID
		strData.Format("RegID:%s",root["RegID"].asString().c_str());
		m_TreeAcc.InsertItem(CString(strData),hItemRoot);

		//Public Key
		strData.Format("PublicKey:%s",root["PublicKey"].asString().c_str());
		m_TreeAcc.InsertItem(CString(strData),hItemRoot);

		//MinerPKey Key
		strData.Format("MinerPKey:%s",root["MinerPKey"].asString().c_str());
		m_TreeAcc.InsertItem(CString(strData),hItemRoot);

		//FreeValues
		strData.Format("FreeValues:%f",root["FreeValues"].asDouble()/COIN);
		m_TreeAcc.InsertItem(CString(strData),hItemRoot);

		//RewardFund
		FillFundToTree("RewardFund",hItemRoot,root);
		
		//FreedomFund
		FillFundToTree("FreedomFund",hItemRoot,root);

		//Freeze
		FillFundToTree("Freeze",hItemRoot,root);

		//SelfFreeze
		FillFundToTree("SelfFreeze",hItemRoot,root);

		FillAuthorToTree(hItemRoot,root);
	}

	m_TreeAcc.Expand(hItemRoot,TVE_EXPAND);
}

void CWalletDlg::FillFundToTree(const string& strFundType,HTREEITEM hRoot,const Json::Value& v)
{
	const Json::Value& reward = v[strFundType];
	CString strTitle(strFundType.c_str());
	HTREEITEM hItem = m_TreeAcc.InsertItem(strTitle,hRoot);
	CStringA strData;
	for (size_t i = 0;i<reward.size();++i)
	{
		CString str;
		str.Format(_T("[%d]"),i);
		HTREEITEM hSubItem = m_TreeAcc.InsertItem(str,hItem);
		strData.Format("nType:%s",reward[i]["nType"].asString().c_str());
		m_TreeAcc.InsertItem(CString(strData),hSubItem);

		strData.Format("scriptID :%s",reward[i]["scriptID"].asString().c_str());
		m_TreeAcc.InsertItem(CString(strData),hSubItem);

		strData.Format("value:%f",reward[i]["value"].asDouble()/COIN);
		m_TreeAcc.InsertItem(CString(strData),hSubItem);

		strData.Format("timeout hight:%d",reward[i]["timeout hight"].asInt());
		m_TreeAcc.InsertItem(CString(strData),hSubItem);
	}
}

void CWalletDlg::OnBnClickedBtnFreshAccinfo()
{
	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_KEY);
	int nCount = pCombox->GetCount();
	if (0 == nCount)
	{
		AfxGetMainWnd()->SendMessage(WM_GET_ACCLIST);
	}
	else
	{
		AfxGetMainWnd()->SendMessage(WM_GET_ACCOUNTINFO);
	}
}
void CWalletDlg::Jiexi(Json::Value &obj, HTREEITEM hItemRoot)
{

switch (obj.type())
	{
	case Json::objectValue: //对象
		//IDictionary<string, object> iDic = (IDictionary<string, object>)obj;
		{

			Json::Value::Members members( obj.getMemberNames() );
			std::sort( members.begin(), members.end() );
			for ( Json::Value::Members::iterator it = members.begin();	
				it != members.end();
				++it )	
			{
				const std::string &name = *it;
				HTREEITEM varRoot = m_TreeTx.InsertItem(CString(name.c_str())+":",hItemRoot);
				Jiexi(obj[name],varRoot);
			}	

		}
		break;

	case Json::arrayValue: //数组
		{
			int size = obj.size();
			for ( int index =0; index < size; ++index )
			{

				static char buffer[500];
				sprintf( buffer, "[%d]", index );
				string name(buffer);
				HTREEITEM varRoot = m_TreeTx.InsertItem(CString(buffer),hItemRoot);
				Jiexi(obj[index],varRoot);
			}

		}
		break;
	default:	
		CString strContent = m_TreeTx.GetItemText(hItemRoot); 
		strContent += CString(obj.toStyledString().c_str()); 
		m_TreeTx.SetItemText(hItemRoot,strContent);
		break;
	}
}

void CWalletDlg::OnBnClickedBtnFreshTxInfo()
{

	CString strCommand;
	strCommand.Format(_T("%s"),_T("getalltxinfo"));
	CStringA strShowData ;

    CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

// 	Json::Reader reader;  
// 	Json::Value root; 
// 	if (!reader.parse(strShowData.GetString(), root)) 
// 		return ;
// 		m_TreeTx.DeleteAllItems();
// 	Jiexi(root,TVI_ROOT);

	InitTxInfoTree(strShowData.GetString());
	
	//AfxGetMainWnd()->SendMessage(WM_LIST_TX);
}

void CWalletDlg::FillAuthorToTree(HTREEITEM hItemRoot,const Json::Value& root)
{
	const Json::Value& author = root["Authorizates"];
	HTREEITEM hItem = m_TreeAcc.InsertItem(_T("Authorizates"),hItemRoot);
	CStringA strData;
	for (size_t i = 0;i<author.size();++i)
	{
		Json::Value::Members members( author[i].getMemberNames() );
		Json::Value::Members::iterator it = members.begin();
		const Json::Value& item = author[i];
		const Json::Value& v = item[*it];

		string s = *it;
		CString str;
		str.Format(_T("authorizate[%d]"),i);

		HTREEITEM hSubItem = m_TreeAcc.InsertItem(str,hItem);
		strData.Format("appid:%s",item["appid"].asString().c_str());
		m_TreeAcc.InsertItem(CString(strData),hSubItem);

		strData.Format("AuthorizeTime:%d",item["AuthorizeTime"].asInt());
		m_TreeAcc.InsertItem(CString(strData),hSubItem);

		strData.Format("MaxMoneyPerTime:%f",item["MaxMoneyPerTime"].asDouble()/COIN);
		m_TreeAcc.InsertItem(CString(strData),hSubItem);

		strData.Format("MaxMoneyTotal:%f",item["MaxMoneyTotal"].asDouble()/COIN);
		m_TreeAcc.InsertItem(CString(strData),hSubItem);

		strData.Format("MaxMoneyPerDay:%f",item["MaxMoneyPerDay"].asDouble()/COIN);
		m_TreeAcc.InsertItem(CString(strData),hSubItem);

		strData.Format("UserDefine:%s",item["UserDefine"].asString().c_str());
		m_TreeAcc.InsertItem(CString(strData),hSubItem);

		strData.Format("LastOperHeight:%d",item["LastOperHeight"].asInt());
		m_TreeAcc.InsertItem(CString(strData),hSubItem);

		strData.Format("CurMaxMoneyPerDay:%f",item["CurMaxMoneyPerDay"].asDouble()/COIN);
		m_TreeAcc.InsertItem(CString(strData),hSubItem);
	}
}

void CWalletDlg::OnCbnSelchangeComboKey()
{
	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_KEY);
	ASSERT(pCombox);
	int nIndex = pCombox->GetCurSel();
	if (CB_ERR == nIndex)
	{
		return ;
	}

	CString strAddr;
	pCombox->GetLBText(nIndex,strAddr);
	if (!strAddr.IsEmpty())
	{
		AfxGetMainWnd()->SendMessage(WM_GET_ACCOUNTINFO,(WPARAM)&strAddr,0);
	}
}


BOOL CWalletDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN)
		{
			return TRUE;
		}
	} 
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CWalletDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShellExecuteW(NULL, L"open", _T("http://bbs.dspay.org/forum.php?mod=viewthread&tid=1420"), NULL, NULL, SW_SHOWNORMAL);
}


CString CWalletDlg::FormatBlockTime(ULONGLONG nTime)
{
	CTimeSpan tm(nTime);
	CString str;
	if (tm.GetDays())
	{
		long leog = tm.GetDays();
		if(leog < 0 )
			leog = 0;
		str.Format(_T("%d days %d hours %d min behind"),tm.GetDays(),tm.GetHours(),tm.GetMinutes());
	}
	else
	{
		if (tm.GetHours())
		{
			long leog = tm.GetHours();
			if(leog < 0 )
				leog = 0;
			str.Format(_T("%d hours %d min behind"),tm.GetHours(),tm.GetMinutes());
		}
		else
		{
			long leog = tm.GetMinutes();
			if(leog < 0 )
				leog = 0;
			str.Format(_T("%d min behind"),tm.GetMinutes());
		}
	}
	
	return str;
}

void CWalletDlg::OnTvnSelchangedTreeAcc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CWalletDlg::OnStnClickedStaticHeight()
{
	// TODO: 在此添加控件通知处理程序代码
}
