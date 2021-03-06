﻿#pragma once
#include <Windows.h>
#include <fstream>
#include <Tlhelp32.h>  
#include <shlwapi.h>
#include <afxmt.h >
#pragma comment(lib,"Shlwapi.lib")
#include <string>
#include <vector>
#include "json\json.h"
#include "RpcCmd.h"
#include "RPCDlg.h"

using namespace std;
typedef long long  int64_t;
static const ULONGLONG COIN = 100000000;
#define REAL_MONEY(x) static_cast<int64_t>((x)*COIN)

#define BEGIN(a)            ((char*)&(a))
#define END(a)              ((char*)&((&(a))[1]))

#define WM_SEND_RPC_DATA		WM_USER+1
#define WM_SHOW_RECV_DATA		WM_USER+2
#define WM_SOCKET_RECEIVE		WM_USER+3
#define WM_SOCKET_CLOSE			WM_USER+4
#define WM_SOCKET_CONNECT		WM_USER+5
#define WM_SHOW_SEND_DATA		WM_USER+6
#define WM_UPDATE_HEIGHT		WM_USER+7
#define WM_GET_ACCOUNTINFO		WM_USER+8
#define WM_LIST_TX				WM_USER+9
#define WM_GET_ACCLIST			WM_USER+10
#define WM_GET_BET_RANDOM_DATA	WM_USER+11
#define WM_UPDATE				WM_USER+12

#define WINDOW_TAG				_T("Dacrs-UIDemo")

typedef enum
{
	emRPCCommand = 1,
	emBaseFunction,
	emRegisterScript,
	emContract,
	emGetAccData,
	emGetAccountInfo,
	emGetRegScriptHash,
	emGetRegScriptID,
	emGetBetRandomData,
	emListTx,
	emNoType
}emSendDataType;

#define DEFINE_SINGLETON(cls)\
 private:\
 static std::auto_ptr<cls> m_pInstance;\
 protected:\
 cls(){}\
 public:\
 ~cls(){}\
 static cls* getInstance(){\
 if(!m_pInstance.get()){\
 m_pInstance = std::auto_ptr<cls>(new cls());\
 }\
 return m_pInstance.get();\
	}

#define IMPLEMENT_SINGLETON(cls) \
	std::auto_ptr<cls> cls::m_pInstance(NULL);

typedef enum 
{
	emSesureTx=1,
	emDarkTx,
	emAnonymTx,
	emP2PBet,
	emLottery,
	emNonType,
}emBusinessType;

//权限结构
typedef struct _AUTHORIZATE
{
	_AUTHORIZATE()
	{
		nAuthorizeTime = 0;
		nMaxMoneyPerTime = 0;
		nMaxMoneyTotal = 0;
		nMaxMoneyPerDay = 0;
	}

	int nAuthorizeTime;
	double nMaxMoneyPerTime;
	double nMaxMoneyTotal;
	double nMaxMoneyPerDay;
	string strUserData;
}AUTHORIZATE;

//*****************************担保合约结构************************************
#define ACCOUNT_ID_SIZE 6
#define MAX_ARBITRATOR 3
#define MAX_ACCOUNT_LEN 20
#define HASH_SIZE	32
#pragma pack(1)
typedef enum
{
	emFirstStep = 1,
	emSecondStep,
	emThirdStep,
	emLastStep,
	emInvalidStep
}enumSesureTrade;

typedef enum
{
	emDarkFirstStep = 1,
	emDarkSecondStep = 2,
	emDarkThirdStep = 3,
	emInvalidDarkStep
}enumDarkTx;

typedef enum
{
	emP2PBetASend = 1,
	emP2PBetBAccept,
	emP2PBetAExpose,
	emP2PBetBExpose,
	emInvalidP2PBet
}enumP2PBet;

typedef struct tag_INT64 {
	unsigned char data[8];
} Int64;

typedef struct tagACCOUNT_ID
{
	char accounid[ACCOUNT_ID_SIZE];
}ACCOUNT_ID;


typedef struct  {
	unsigned char nType;					//!<类型
	unsigned char nArbitratorCount;			//!<仲裁者个数
	ACCOUNT_ID 	buyer;						//!<买家ID（采用6字节的账户ID）
	ACCOUNT_ID seller;						//!<卖家ID（采用6字节的账户ID）
	ACCOUNT_ID arbitrator[MAX_ARBITRATOR];	//!<仲裁者ID（采用6字节的账户ID）
	long nHeight;							//!<超时绝对高度
	Int64 nFineMoney;						//!<卖家违约后最大罚款金额
	Int64 nPayMoney;						//!<买家向卖家支付的金额
	Int64 nFee;								//!<仲裁手续费
	Int64 ndeposit;							//!<仲裁押金,申诉时从仲裁者扣除的押金(如果仲裁不响应钱归买家否则买家退还给仲裁者)
}FIRST_CONTRACT;

typedef struct {
	unsigned char nType;					//!<交易类型
	unsigned char hash[HASH_SIZE];			//!<上一个交易包的哈希
} NEXT_CONTRACT;

typedef struct {
	unsigned char nType;					//!<交易类型
	unsigned char hash[HASH_SIZE];			//!<上一个交易的哈希
	Int64 nMinus;
}ARBIT_RES_CONTRACT;

//暗黑币
typedef struct  {
	unsigned char	dnType;					//!<类型
	char			buyer[6];				//!<买家ID（采用6字节的账户ID）
	char			seller[6];				//!<卖家ID（采用6字节的账户ID）
	int				nHeight;				//!<超时绝对高度
	ULONGLONG		nPayMoney;				//!<买家向卖家支付的金额

}DARK_FIRST_CONTRACT;

typedef struct {
	unsigned char dnType;					//!<交易类型
	char hash[32];							//!<上一个交易包的哈希
}DARK_NEXT_CONTRACT;


//匿名交易
typedef struct  
{
	char 			account[6];				//!<接受钱的ID（采用6字节的账户ID）
	ULONGLONG		nReciMoney;				//!<	收到钱的金额
} ACCOUNT_INFO;

struct BASE_ANONYM_CONTRACT
{
	char 			szSender[6];			//!<转账人ID（采用6字节的账户ID）
	int				nHeight;				//!<超时绝对高度
	ULONGLONG		nPayMoney;				//!<转账的人支付的金额
	unsigned short	len;                    //!<接受钱账户信息长度
};

struct ANONYM_CONTRACT:public BASE_ANONYM_CONTRACT
{
	vector<ACCOUNT_INFO> vAccInfo;
};

//P2P赌博
typedef struct {
	unsigned char type;
	ULONGLONG money;
	int hight;
	unsigned char dhash[32];
} SEND_DATA;

typedef struct {
	unsigned char type;
	ULONGLONG money;
	unsigned char targetkey[32];		//发起对赌的哈希，也是对赌数据的关键字
	unsigned char data[5];
} ACCEPT_DATA;

typedef struct {
	unsigned char type;
	unsigned char targetkey[32];		//发起对赌的哈希，也是对赌数据的关键字
	unsigned char dhash[5];
} OPEN_DATA;

//彩票
typedef struct {
	unsigned char type;
	ULONGLONG money;
	int hight;
}REG_DATA;


typedef struct {
	unsigned char type;
	unsigned char num[15];
	unsigned char numlen;
	ULONGLONG money;
}ORDER_DATA;


typedef struct {
	unsigned char type;
}DRAW_DATA;

#pragma pack()





const signed char p_util_hexdigit[256] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0xa, 0xb, 0xc, 0xd, 0xe,
	0xf, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, };

inline signed char HexDigit(char c)
{
	return p_util_hexdigit[(unsigned char)c];
}



class CSoyPayHelp
{
	DEFINE_SINGLETON(CSoyPayHelp)
	static const int		REG_ID_SIZE = 6;
	CRPCDlg*		 m_pRPCDlg;		
	CRpcCmd mRpcCmd;

	
private:
	CStringA ParseRecvData(const char*pRecvData);

public:

	void InitialRpcCmd(CRPCDlg * pdlg,const CStringA	 &sendPreHeadstr,const CStringA &sendendHeadstr)
	{
		mRpcCmd.SetHeadString(sendPreHeadstr,sendendHeadstr);
		m_pRPCDlg = pdlg;
	}

	int SendRpc(CString cmd,CStringA &rev);




	string	GetReverseHash(const string& strHash);
	string	GetHexData(const char*pData,size_t nLen);
	string	ParseTxHashFromJson(const string& strJsonData);
	string	ParseScriptIDFromJson(const string& strJsonData);
	string  EncodeBase64(const unsigned char* pch, size_t len);
	string  EncodeBase64(const string& str);
	string	CreateContractTx(const string& strScript,const vector<string>& vAddr,const string& strContract,
			int nHeight,int64_t nFee);
	string	SignContractTx(const string& strRawTxHash);
	string	CreateScriptTx(const char* pAccount,bool bPath,const char* pScript,int64_t nFee,int nHeight,
				int nAuthTime=0,int64_t nMoneyPerTime=0,int64_t nMoneyTotal=0,int64_t nMoneyPerDay=0,const string& strUserData="");
	string	CreateScriptAccountTx(const char* pAccount,int64_t nFee,bool bIsMiner);
	string	CreateNomalTx(const char* pFrom,const char* pTo,int64_t nMoney,int64_t nFee,int nHeight);
	bool	GetErrorMsg(const string& strRecvData,string& strErrorMsg);
	string	Setgenerate(bool bStart);
	string  GetFullRegID(const string& strRegID);

	template<typename T>
	std::string HexStr(const T itbegin, const T itend, bool fSpaces = false);

	template<typename T>
	inline std::string HexStr(const T& vch, bool fSpaces = false);

	
	

	vector<unsigned char> ParseHex(const char* psz);
	vector<unsigned char> ParseHex(const string& str);
};

//*******************************担保交易************************************************
class CSesureTradeHelp
{
public:
	string PacketFirstContract(const string& strBuyID,const string& strSellID,const vector<string>& vArRegID,
		int nHeight,int64_t nFine,int64_t nPay,int64_t nFee,int64_t ndeposit);
	string PacketSecondContract(unsigned char* pHash);
	string PacketThirdContract(unsigned char* pHash);
	string PacketLastContract(unsigned char* pHash,int64_t nFine);

private:
	FIRST_CONTRACT		m_FirstContract;
	NEXT_CONTRACT		m_SecondContract;
	NEXT_CONTRACT		m_ThirdContract;
	ARBIT_RES_CONTRACT	m_LastContract;
};

//********************************暗黑币************************************
class CDarkTxHelp
{
public:
	string PacketFirstContract(const string& strBuyID,const string& strSellID,int64_t nMoney,int nHeight);
	string PacketNextContract(const string& strTxHash);

private:
	DARK_FIRST_CONTRACT	 m_FirstContract;
	DARK_NEXT_CONTRACT	 m_NextContract;
};

//********************************匿名交易************************************
class CAnonymTxHelp
{
public:
	string PackAnonymContract(const string& strSender,int nSendMoney,const vector<string>& vRecv,int nHeight);

private:
	ANONYM_CONTRACT	m_anonymContract;
};

//********************************P2P对赌************************************
class CP2PBetHelp
{
public:
	string PacketP2PSendContract(int64_t nMoney,int nHeight,const string& strRandomHash);
	string PacketP2PAcceptContract(int64_t nMoney,const string& strSendHash,char* pData);
	string PacketP2PExposeContract(const string& SendHash,const string& strRandomHash);

private:
	SEND_DATA		m_sendContract;
	ACCEPT_DATA		m_acceptContract;
	OPEN_DATA		m_openContract;
};


















extern void ProductHttpHead(const CStringA& configdir,const string& strCfgFileName,CStringA &strPort,CStringA& strPreHeadstr,CStringA& strendHeadstr);
extern void RPCCommandToJson(const CString& strRPCCommand,CStringA& strSendData);


extern void ParseJsonAddrToMap(const CStringA& strValue,map<CString,CString>& mapAccount);
extern CStringA ParseRecvData(const char*pRecvData);
extern void Setgenerate(bool bStart);
extern string GetFullRegID(const CString& strRegID);
extern CString GetCompressRegID(const CString& strRegID);
extern void SendRPCData(const string& strData,emSendDataType emType);
extern bool CheckRegIDValid(const CString& strRegID);
extern bool CheckHash(const CString& strRegID);