#pragma once
#include <Windows.h>
#include <fstream>
#include <Tlhelp32.h>  
#include <shlwapi.h>
#include <afxmt.h >
#pragma comment(lib,"Shlwapi.lib")
#include <string>
#include <vector>
#include "json\json.h"
using namespace std;

#define BEGIN(a)            ((char*)&(a))
#define END(a)              ((char*)&((&(a))[1]))

#define WM_SEND_RPC_DATA	WM_USER+1
#define WM_SHOW_RECV_DATA	WM_USER+2
#define WM_SOCKET_RECEIVE	WM_USER+3
#define WM_SOCKET_CLOSE		WM_USER+4
#define WM_SOCKET_CONNECT	WM_USER+5
#define WM_SHOW_SEND_DATA	WM_USER+6

typedef enum
{
	emRPCCommand = 1,
	emBaseFunction,
	emContract,
	emGetData,
	emNoType
}emSendDataType;

//担保合约结构
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

typedef struct tag_INT64 {
	unsigned char data[8];
} Int64;

typedef struct tagACCOUNT_ID
{
	char accounid[MAX_ACCOUNT_LEN];
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
	unsigned char nType;				//!<交易类型
	unsigned char hash[HASH_SIZE];		//!<上一个交易包的哈希
} NEXT_CONTRACT;

typedef struct {
	unsigned char nType;				//!<交易类型
	unsigned char hash[HASH_SIZE];		//!<上一个交易的哈希
	Int64 nMinus;
}ARBIT_RES_CONTRACT;

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

template<typename T>
std::string HexStr(const T itbegin, const T itend, bool fSpaces = false) {
	std::string rv;
	static const char hexmap[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	rv.reserve((itend - itbegin) * 3);
	for (T it = itbegin; it < itend; ++it) {
		unsigned char val = (unsigned char) (*it);
		if (fSpaces && it != itbegin)
			rv.push_back(' ');
		rv.push_back(hexmap[val >> 4]);
		rv.push_back(hexmap[val & 15]);
	}

	return rv;
}

inline signed char HexDigit(char c)
{
	return p_util_hexdigit[(unsigned char)c];
}

template<typename T>
inline std::string HexStr(const T& vch, bool fSpaces = false) {
	return HexStr(vch.begin(), vch.end(), fSpaces);
}

extern void ProductHttpHead(const CStringA& configdir,CStringA &strPort,CStringA& strPreHeadstr,CStringA& strendHeadstr);
extern void RPCCommandToJson(const CString& strRPCCommand,CStringA& strSendData);
extern void CreateScriptTx(const char* pAccount,bool bPath,const char* pScript,int nFee,int nHeight,
	int nAuthTime=0,int nMoneyPerTime=0,int nMoneyTotal=0,int nMoneyPerDay=0,const string& strUserData="");
extern void CreateScriptAccountTx(const char* pAccount,int nFee,int nHeight);
extern bool GetErrorMsg(const string& strRecvData,string& strErrorMsg);
extern void PacketFirstContract(enumSesureTrade emType,const char*pBuyID,const char* pSellID,const vector<string>& vArRegID,
								int nHeight,int nFine,int nPay,int nFee,int ndeposit,FIRST_CONTRACT* pContract);
extern void packetNextContract(enumSesureTrade emType,unsigned char* pHash,NEXT_CONTRACT* pNextContract);
extern void packetLastContract(enumSesureTrade emType,unsigned char* pHash,int nFine,ARBIT_RES_CONTRACT* pLastContract);
extern void CreateContractTx(const string& strScript,const vector<string>& vAddr,const vector<unsigned char>& contract,
	int nHeight,int nFee);
extern void ParseJsonAddrToMap(const CStringA& strValue,map<CString,CString>& mapAccount);
extern CStringA ParseRecvData(const char*pRecvData);
extern void CreateNomalTx(const char* pFrom,const char* pTo,int nMoney,int nFee,int nHeight);
extern void Setgenerate(bool bStart);
extern vector<unsigned char> ParseHex(const char* psz);
extern vector<unsigned char> ParseHex(const string& str);
extern string GetFullRegID(const CString& strRegID);
extern CString GetCompressRegID(const CString& strRegID);