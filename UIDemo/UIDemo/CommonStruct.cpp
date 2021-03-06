﻿#include "stdafx.h"
#include "CommonStruct.h"

IMPLEMENT_SINGLETON(CSoyPayHelp)

CStringA rand_str(int len)
{
	CStringA str;
	int i;
	for(i=0;i<len;++i)
	{
		char Temp = 'A'+rand()%26;
		str.AppendChar(Temp);
	}
	return str;
}

void ProductHttpHead(const CStringA& configdir,const string& strCfgFileName,CStringA &strPort,CStringA& strPreHeadstr,CStringA& strendHeadstr)
{
	CStringA configpath = "";
	configpath.AppendFormat("%s",configdir);
	configpath.AppendFormat("\\%s",strCfgFileName.c_str());
	BOOL userflag = false;
	BOOL passFlag = false;
	CStringA strTemp = "";
	CStringA user = "";
	CStringA regtest = "";
	CStringA testnet = "";
	string rpcpassword;
	string rpcuser;
	if (PathFileExistsA(configpath))
	{
		fstream mfile;
		mfile.open(configpath,ios::in);
		string linestr = "";
		while(!mfile.eof())
		{
			getline(mfile,linestr);
			strTemp=linestr.c_str() ;
			int upos = strTemp.Find("rpcuser");
			int passpos = strTemp.Find("rpcpassword");
			int regpos = strTemp.Find("regtest");
			int netpos =  strTemp.Find("testnet");
			int rpcpos =  strTemp.Find("rpcport");
			if (passpos>=0)
			{
				passpos = strTemp.Find("=",passpos);
				rpcpassword= strTemp.Mid(passpos+1);
				userflag = TRUE;
			}
			if (upos >=0)
			{
				upos = strTemp.Find("=",upos);
				rpcuser= strTemp.Mid(upos+1);
				user = strTemp.Mid(upos+1);
				passFlag = TRUE;
			}

			if (regpos>=0)
			{
				regpos = strTemp.Find("=",regpos);
				regtest= strTemp.Mid(regpos+1);
				regtest.TrimLeft();
				regtest.TrimRight();
			}
			if (netpos>=0)
			{
				netpos = strTemp.Find("=",netpos);
				testnet= strTemp.Mid(netpos+1);
				testnet.TrimLeft();
				testnet.TrimRight();
			}
			if (rpcpos>=0)
			{
				rpcpos = strTemp.Find("=",rpcpos);
				strPort= strTemp.Mid(rpcpos+1);
				strPort.TrimLeft();
				strPort.TrimRight();
			}
		}
	}
	if (strPort == "")
	{
		if (testnet == "1" || regtest == "1")
		{
			strPort = "18332";
		}
		else
		{
			strPort = "8332";
		}
	}
	static const int LEN_NAME = 8;
	if (!userflag)
	{
		srand(static_cast<unsigned int>(time(NULL)));
		CStringA name = rand_str(LEN_NAME+1);
		strTemp.AppendFormat("%s%s\r\n","rpcuser=",name);
		rpcuser = name.GetString();
	}
	if (!passFlag)
	{
		srand(static_cast<unsigned int>(time(NULL)+100));
		CStringA rpcpass= rand_str(LEN_NAME+1);
		strTemp.AppendFormat("%s%s\r\n","rpcpassword=",rpcpass);
		rpcpassword = rpcpass.GetString();
	}
	if (!userflag||!passFlag )
	{
		CFile mFile;
		mFile.Open(CString(configpath.GetString()),CFile::modeCreate|CFile::modeWrite);

		mFile.Write((LPSTR)strTemp.GetString(),strTemp.GetLength());
		mFile.Close();
	}

	CStringA UserPass = "";
	if (rpcuser!= "" && rpcpassword!="")
	{

		UserPass = rpcuser.c_str();
		UserPass.TrimLeft();
		UserPass.TrimRight();
		CStringA temp = rpcpassword.c_str();
		temp.TrimLeft();
		temp.TrimRight();
		UserPass.AppendFormat(":%s",temp);
		string strUserPass64 = CSoyPayHelp::getInstance()->EncodeBase64(UserPass.GetString());

		strPreHeadstr += "POST / HTTP/1.1\r\n";
		strPreHeadstr +=	"bitcoin-json-rpc/v1.0.0.0-unk-beta\r\n";
		strPreHeadstr +=	"Host: 127.0.0.1\r\n";
		strPreHeadstr +="Content-Type: application/json\r\n";
		strendHeadstr +="Connection: close\r\n";
		strendHeadstr +="Accept: application/json\r\n";
		strendHeadstr +="Authorization: Basic ";//ZHNwYXk6YWRtaW4=\r\n\r\n";
		strendHeadstr +=strUserPass64.c_str();
		strendHeadstr +="\r\n\r\n";
	}
	else
	{
		AfxMessageBox(_T("Invald config error"));
	}
}

bool IsAllDigtal(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		if (!isdigit(pData[i]))
			return false;
	}

	return true;
}

void RPCCommandToJson(const CString& strRPCCommand,CStringA& strSendData)
{
	USES_CONVERSION;
	char* pData = W2A(strRPCCommand);
	CStringA rpcCommand(pData);

	Json::Value root;
	Json::Value arrayObj;  
	Json::Value item;

	int pos = rpcCommand.Find(" ");
	CStringA method;
	CStringA param;
	if (pos >=0)
	{
		method = rpcCommand.Left(pos);
		rpcCommand = rpcCommand.Mid(pos + 1);

	}
	else if (!rpcCommand.IsEmpty())
	{
		root["method"] = rpcCommand.GetString();
		root["params"]=arrayObj;
	}
	if (method != "")
	{
		root["method"] = method.GetString();
		pos = rpcCommand.Find(" ");
		while(pos >=0)
		{
			param = rpcCommand.Left(pos);
			param.TrimLeft();
			param.TrimRight();
			rpcCommand = rpcCommand.Mid(pos+1);
			rpcCommand = rpcCommand.TrimLeft();
			if (param.GetLength()<10 &&IsAllDigtal(param))
			{
				root["params"].append(atoi(param));
			}
			else
			{
				root["params"].append(param.GetString());
			}
			pos = rpcCommand.Find(" ");
		}

		if (IsAllDigtal(rpcCommand) && rpcCommand.GetLength() != 12)
		{
			root["params"].append(atoi(rpcCommand));
		}
		else
		{
			if (rpcCommand == "true")
			{
				root["params"].append(true);
			}
			else if (rpcCommand == "false")
			{
				root["params"].append(false);
			}
			else
			{
				root["params"].append(rpcCommand.GetString());
			}
			
		}
	}

	strSendData = root.toStyledString().c_str();
}

void SendRPCData(const string& strData,emSendDataType emType)
{
	CStringA str(strData.c_str());
	CWnd* pWnd = AfxGetApp()->m_pMainWnd;
	pWnd->SendMessage(WM_SEND_RPC_DATA,(WPARAM)&str,emType);
}



bool Int64Inital(Int64 * p64, char const *pdata, unsigned char len) 
{
	memset(p64, 0, sizeof(*p64));

	if (len <= sizeof(*p64)) {
		while (len--) {
			((char *) p64)[len] = *(pdata++);
		}
		return true;
	}
	return false;
}


void ParseJsonAddrToMap(const CStringA& strValue,map<CString,CString>& mapAccount)
{
	CStringA strData = strValue;
	Json::Reader reader; 
	Json::Value value;
	CString strRegID;
	CString strAddres;
	if(reader.parse(strData.GetString(),value))
	{
		Json::Value &arrayObj = value["result"];
		int n = arrayObj.size();
		for(int i = 0;i<n;i++)
		{
			Json::Value v1 = arrayObj[i]["RegID"];
			string strRegisterd = v1.asString();
			if (/*strRegisterd != " "*/1)
			{
				Json::Value v2 = arrayObj[i]["addr"];
				string strAddr = v2.asString();
				strRegID.Format(_T("%S"),strRegisterd.c_str());
				strAddres.Format(_T("%S"),strAddr.c_str());
				mapAccount.insert(make_pair(strAddres,strRegID));
			}
		}
	}
}

CStringA ParseJsonToList(const CStringA& strValue)
{
	CStringA strData = strValue;
	Json::Reader reader; 
	Json::Value value;
	if(reader.parse(strData.GetString(),value))
	{
		Json::Value obj = value["result"];
		if(obj.isString())
		{
			strData = obj.asCString();
		}
		else
		{
			strData = obj.toStyledString().c_str();
		}		
	}
	int pos = strData.Find(",");
	CStringA strTemp;

	while(pos >=0)
	{
		CStringA addstr = strData.Left(pos+1);
		strData = strData.Mid(pos+1);
		pos = strData.Find(",");
		strTemp.AppendFormat("%s\r\n",addstr);
	}
	strTemp.AppendFormat("%s\r\n",strData);
	return strTemp;
}

CStringA ParseRecvData(const char*pRecvData)
{
	CStringA strShowData = ParseJsonToList(pRecvData);
	if (0 == strShowData.Find("null"))
	{
		string strErrorMsg;
		if(CSoyPayHelp::getInstance()->GetErrorMsg(pRecvData,strErrorMsg))
		{
			strShowData = strErrorMsg.c_str();
		}
	}

	return strShowData;
}

void Setgenerate(bool bStart)
{
	string str = CSoyPayHelp::getInstance()->Setgenerate(bStart);
	SendRPCData(str,emBaseFunction);
}

string GetFullRegID(const CString& strRegID)
{
	int nHeight = 0;
	short nIndex = 0;
	int nPos = 0;

	nPos = strRegID.Find('-');
	ASSERT(-1 != nPos);
	nHeight = _ttoi(strRegID.Left(nPos));
	nIndex = _ttoi(strRegID.Mid(nPos+1));

	vector<unsigned char> vData;
	vData.insert(vData.end(), BEGIN(nHeight), END(nHeight));
	vData.insert(vData.end(), BEGIN(nIndex), END(nIndex));
	return CSoyPayHelp::getInstance()->HexStr(vData);
}

CString GetCompressRegID(const CString& strRegID)
{
	if (strRegID.IsEmpty())
	{
		return CString(_T(""));
	}
	USES_CONVERSION;
	char* pData = W2A(strRegID);
	vector<unsigned char>vRegID = CSoyPayHelp::getInstance()->ParseHex(pData);

	int nHeight = 0;
	short nIndex = 0;
	memcpy(&nHeight,&vRegID[0],sizeof(nHeight));
	memcpy(&nIndex,&vRegID[sizeof(nHeight)],sizeof(nIndex));
	CString str;
	str.Format(_T("%d-%d"),nHeight,nIndex);
	return str;
}
int CSoyPayHelp::SendRpc(CString cmd,CStringA &rev)
{
	string revtemp;
	mRpcCmd.SendRpc(cmd,revtemp);
	CStringA strShowData = ParseRecvData(revtemp.c_str());
	if(m_pRPCDlg != NULL)
	{
		m_pRPCDlg->SendMessage(WM_SHOW_SEND_DATA,(WPARAM)&mRpcCmd.RpcJosnStr);
		m_pRPCDlg->SendMessage(WM_SHOW_RECV_DATA,(WPARAM)&strShowData);
	}
	rev = strShowData;
	return rev.GetLength();
}
CStringA CSoyPayHelp::ParseRecvData(const char*pRecvData)
{
	CStringA strShowData = ParseJsonToList(pRecvData);
	if (0 == strShowData.Find("null"))
	{
		string strErrorMsg;
		if(CSoyPayHelp::getInstance()->GetErrorMsg(pRecvData,strErrorMsg))
		{
			strShowData = strErrorMsg.c_str();
		}
	}

	return strShowData;
}

string CSoyPayHelp::GetReverseHash(const string& strHash)
{
	vector<unsigned char>vHash = ParseHex(strHash);
	reverse(vHash.begin(),vHash.end());
	string strReverseHash;
	strReverseHash.assign(vHash.begin(),vHash.end());
	return strReverseHash;
}

string CSoyPayHelp::GetHexData(const char*pData,size_t nLen)
{
	vector<unsigned char> vTemp;
	vTemp.assign(pData,pData +nLen);
	return HexStr(vTemp);
}

string CSoyPayHelp::CreateContractTx(const string& strScript,const vector<string>& vAddr,const string& strContract,
	int nHeight,int64_t nFee)
{
	ASSERT(vAddr.size());
	Json::Value root;
	root["method"] = "createcontracttx";
	root["params"].append(strScript.c_str());

	Json::Value item;
	for (size_t i = 0;i<vAddr.size();i++)
	{
		item.append(vAddr.at(i));
	}

	root["params"].append(item);
	root["params"].append(strContract);
	root["params"].append(nFee);
	root["params"].append(nHeight);

	return root.toStyledString();
}

string CSoyPayHelp::SignContractTx(const string& strRawTxHash)
{
	Json::Value root;
	root["method"] = "signcontracttx";
	root["params"].append(strRawTxHash.c_str());
	return root.toStyledString();
}

template<typename T>
std::string CSoyPayHelp::HexStr(const T itbegin, const T itend, bool fSpaces) {
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

template<typename T>
inline std::string CSoyPayHelp::HexStr(const T& vch, bool fSpaces) {
	return HexStr(vch.begin(), vch.end(), fSpaces);
}

vector<unsigned char> CSoyPayHelp::ParseHex(const char* psz) {
	vector<unsigned char> vch;
	while (true) {
		while (isspace(*psz))
			psz++;
		signed char c = HexDigit(*psz++);
		if (c == (signed char) -1)
			break;
		unsigned char n = (c << 4);
		c = HexDigit(*psz++);
		if (c == (signed char) -1)
			break;
		n |= c;
		vch.push_back(n);
	}
	return vch;
}

vector<unsigned char> CSoyPayHelp::ParseHex(const string& str) {
	return ParseHex(str.c_str());
}

string CSoyPayHelp::CreateScriptTx(const char* pAccount,bool bPath,const char* pScript,int64_t nFee,int nHeight,
	int nAuthTime,int64_t nMoneyPerTime,int64_t nMoneyTotal,int64_t nMoneyPerDay,const string& strUserData)
{
	Json::Value root;

	root["method"] = "registerscripttx";
	root["params"].append(pAccount);
	root["params"].append(bPath?1:0);
	root["params"].append(pScript);
	root["params"].append(nFee);
	root["params"].append(nHeight);

	root["params"].append("this is description");

	root["params"].append(nAuthTime);
	root["params"].append(nMoneyPerTime);
	root["params"].append(nMoneyTotal);
	root["params"].append(nMoneyPerDay);
	root["params"].append(strUserData);

	return root.toStyledString();
}

string CSoyPayHelp::CreateScriptAccountTx(const char* pAccount,int64_t nFee,bool bIsMiner)
{
	Json::Value root;

	root["method"] = "registaccounttx"; 
	root["params"].append(pAccount);
	root["params"].append(nFee);
	root["params"].append(bIsMiner);

	return root.toStyledString();
}

string CSoyPayHelp::CreateNomalTx(const char* pFrom,const char* pTo,int64_t nMoney,int64_t nFee,int nHeight)
{
	/*CStringA str;
	int nfee = 0;
	str.Format("{\"%s\":\"%s\",\"%s\":","method","sendtoaddress","params");
	str.AppendFormat("[\"%s\",\"%s\",%ld,%d,%d],\"id\":%d}\n",pFrom,pTo,nMoney,nFee,nHeight);*/

	Json::Value root;

	root["method"] = "sendtoaddress"; 
	root["params"].append(pFrom);
	root["params"].append(pTo);
	root["params"].append(nMoney);
	return root.toStyledString();
}

string CSoyPayHelp::ParseTxHashFromJson(const string& strJsonData)
{
	Json::Reader reader; 
	Json::Value value;
	if(reader.parse(strJsonData.c_str(),value))
	{
		Json::Value txhash = value["hash"];
		if (txhash.isNull()) 
		{
			txhash = value["rawtx"];
		}
		return txhash.asString();
	}

	return "";
}

string CSoyPayHelp::ParseScriptIDFromJson(const string& strJsonData)
{
	Json::Reader reader; 
	Json::Value value;
	if(reader.parse(strJsonData.c_str(),value))
	{
		Json::Value scriptID = value["script"];
		if (scriptID.isNull()) 
		{
			scriptID = value["script"];
		}
		return scriptID.asString();
	}

	return "";
}

bool CSoyPayHelp::GetErrorMsg(const string& strRecvData,string& strErrorMsg)
{
	Json::Reader reader; 
	Json::Value value;
	if(reader.parse(strRecvData,value))
	{  
		if (!value.isNull())
		{
			Json::Value obj = value["result"];
			strErrorMsg = value["error"].toStyledString();
			return true;
		}
	}

	return false;
}

string CSoyPayHelp::EncodeBase64(const unsigned char* pch, size_t len) {
	static const char *pbase64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	string strRet = "";
	strRet.reserve((len + 2) / 3 * 4);

	int mode = 0, left = 0;
	const unsigned char *pchEnd = pch + len;

	while (pch < pchEnd) {
		int enc = *(pch++);
		switch (mode) {
		case 0: // we have no bits
			strRet += pbase64[enc >> 2];
			left = (enc & 3) << 4;
			mode = 1;
			break;

		case 1: // we have two bits
			strRet += pbase64[left | (enc >> 4)];
			left = (enc & 15) << 2;
			mode = 2;
			break;

		case 2: // we have four bits
			strRet += pbase64[left | (enc >> 6)];
			strRet += pbase64[enc & 63];
			mode = 0;
			break;
		}
	}

	if (mode) {
		strRet += pbase64[left];
		strRet += '=';
		if (mode == 1)
			strRet += '=';
	}

	return strRet;
}

string CSoyPayHelp::EncodeBase64(const string& str) {
	return EncodeBase64((const unsigned char*) str.c_str(), str.size());
}

string CSoyPayHelp::Setgenerate(bool bStart)
{
	if(bStart)
	{
		return string("{\"method\":\"setgenerate\",\"params\":[true],\"id\":1}");
	}
	else
	{
		return string("{\"method\":\"setgenerate\",\"params\":[false],\"id\":1}");
	}
}

string CSoyPayHelp::GetFullRegID(const string& strRegID)
{
	int nHeight = 0;
	short nIndex = 0;
	int nPos = 0;

	nPos = strRegID.find('-');
	if (string::npos == nPos ||strRegID.size() == REG_ID_SIZE*2)
	{
		return strRegID;
	}

	string str = strRegID.substr(0,nPos);
	nHeight = atoi(str.c_str());
	str = strRegID.substr(nPos+1);
	nIndex = atoi(str.c_str());

	vector<unsigned char> vData;
	vData.insert(vData.end(), BEGIN(nHeight), END(nHeight));
	vData.insert(vData.end(), BEGIN(nIndex), END(nIndex));
	return CSoyPayHelp::getInstance()->HexStr(vData);
}




























//*******************************担保交易************************************************
string CSesureTradeHelp::PacketFirstContract(const string& strBuyID,const string& strSellID,const vector<string>& vArRegID,
	int nHeight,int64_t nFine,int64_t nPay,int64_t nFee,int64_t ndeposit)
{
	memset(&m_FirstContract,0,sizeof(FIRST_CONTRACT));
	m_FirstContract.nType = 1;
	m_FirstContract.nArbitratorCount = static_cast<unsigned char>(vArRegID.size());
	m_FirstContract.nHeight = nHeight;

	unsigned char nSize = sizeof(int64_t);
	vector<unsigned char> v = CSoyPayHelp::getInstance()->ParseHex(strBuyID);
	memcpy(m_FirstContract.buyer.accounid,&v[0],ACCOUNT_ID_SIZE);

	v = CSoyPayHelp::getInstance()->ParseHex(strSellID);
	memcpy(m_FirstContract.seller.accounid,&v[0],ACCOUNT_ID_SIZE);

	for(size_t i = 0;i<vArRegID.size();++i)
	{
		v = CSoyPayHelp::getInstance()->ParseHex(vArRegID.at(i));
		memcpy(m_FirstContract.arbitrator[i].accounid,&v[0],ACCOUNT_ID_SIZE);
	}

	memcpy(&m_FirstContract.nFineMoney,(const char*)&nFine,nSize);//100
	memcpy(&m_FirstContract.nPayMoney,(const char*)&nPay,nSize);//80
	memcpy(&m_FirstContract.ndeposit,(const char*)&ndeposit,nSize);//20
	memcpy(&m_FirstContract.nFee,(const char*)&nFee,nSize);//10

	return CSoyPayHelp::getInstance()->GetHexData((const char*)&m_FirstContract,sizeof(FIRST_CONTRACT));
}

string CSesureTradeHelp::PacketSecondContract(unsigned char* pHash)
{
	memset(&m_SecondContract,0,sizeof(NEXT_CONTRACT));
	m_SecondContract.nType = 2;
	memcpy(m_SecondContract.hash,pHash,HASH_SIZE);

	return CSoyPayHelp::getInstance()->GetHexData((const char*)&m_SecondContract,sizeof(NEXT_CONTRACT));
}

string CSesureTradeHelp::PacketThirdContract(unsigned char* pHash)
{
	memset(&m_ThirdContract,0,sizeof(NEXT_CONTRACT));
	m_ThirdContract.nType = 3;
	memcpy(m_ThirdContract.hash,pHash,HASH_SIZE);

	return CSoyPayHelp::getInstance()->GetHexData((const char*)&m_ThirdContract,sizeof(NEXT_CONTRACT));
}

string CSesureTradeHelp::PacketLastContract(unsigned char* pHash,int64_t nFine)
{
	memset(&m_LastContract,0,sizeof(ARBIT_RES_CONTRACT));
	m_LastContract.nType = 4;
	memcpy(m_LastContract.hash,pHash,HASH_SIZE);
	memcpy(&m_LastContract.nMinus,(const char*)&nFine,sizeof(int64_t));

	return CSoyPayHelp::getInstance()->GetHexData((const char*)&m_LastContract,sizeof(ARBIT_RES_CONTRACT));
}


//********************************暗黑币************************************
string CDarkTxHelp::PacketFirstContract(const string& strBuyID,const string& strSellID,int64_t nMoney,int nHeight)
{
	memset(&m_FirstContract,0,sizeof(DARK_FIRST_CONTRACT));
	m_FirstContract.dnType = emDarkFirstStep;

	vector<unsigned char> v = CSoyPayHelp::getInstance()->ParseHex(strBuyID);
	memcpy(m_FirstContract.buyer,&v[0],ACCOUNT_ID_SIZE);
	v = CSoyPayHelp::getInstance()->ParseHex(strSellID);
	memcpy(m_FirstContract.seller,&v[0],ACCOUNT_ID_SIZE);
	m_FirstContract.nHeight = nHeight;
	m_FirstContract.nPayMoney = nMoney;

	return CSoyPayHelp::getInstance()->GetHexData((const char*)&m_FirstContract,sizeof(DARK_FIRST_CONTRACT));
}

string CDarkTxHelp::PacketNextContract(const string& strTxHash)
{
	memset(&m_NextContract,0,sizeof(DARK_NEXT_CONTRACT));
	m_NextContract.dnType = 2;
	memcpy(m_NextContract.hash,strTxHash.c_str(),HASH_SIZE);

	return CSoyPayHelp::getInstance()->GetHexData((const char*)&m_NextContract,sizeof(DARK_NEXT_CONTRACT));
}

//********************************匿名交易************************************
string CAnonymTxHelp::PackAnonymContract(const string& strSender,int nSendMoney,const vector<string>& vRecv,int nHeight)
{
	int nRecvNum = vRecv.size();
	ASSERT(0 != nRecvNum);
	vector<unsigned char> v = CSoyPayHelp::getInstance()->ParseHex(strSender);
	memcpy(m_anonymContract.szSender,&v[0],ACCOUNT_ID_SIZE);
	m_anonymContract.nHeight = nHeight;
	m_anonymContract.nPayMoney = nSendMoney;
	m_anonymContract.len = sizeof(ACCOUNT_INFO)*nRecvNum;

	ACCOUNT_INFO accInfo;
	memset(&accInfo,0,sizeof(ACCOUNT_INFO));
	accInfo.nReciMoney = nSendMoney/nRecvNum;
	for (size_t i = 0;i<vRecv.size();i++)
	{
		v = CSoyPayHelp::getInstance()->ParseHex(vRecv[i]);
		memcpy(accInfo.account,&v[0],ACCOUNT_ID_SIZE);
		m_anonymContract.vAccInfo.push_back(accInfo);
	}


	char* pData = (char*)&m_anonymContract;
	vector<unsigned char> vData;
	vData.assign(pData,pData +sizeof(BASE_ANONYM_CONTRACT));

	unsigned char* pFirstInfo = (unsigned char*)&m_anonymContract.vAccInfo[0];
	size_t nAccountInfoSize = m_anonymContract.vAccInfo.size();
	vData.insert(vData.end(),pFirstInfo,pFirstInfo+sizeof(ACCOUNT_INFO)*nAccountInfoSize);

	return CSoyPayHelp::getInstance()->HexStr(vData);
}

//********************************P2P对赌************************************
string CP2PBetHelp::PacketP2PSendContract(int64_t nMoney,int nHeight,const string& strRandomHash)
{
	memset(&m_sendContract,0,sizeof(SEND_DATA));
	m_sendContract.type = 1;
	m_sendContract.hight = nHeight;
	m_sendContract.money = nMoney;
	memcpy(m_sendContract.dhash,strRandomHash.c_str(),HASH_SIZE);

	return CSoyPayHelp::getInstance()->GetHexData((const char*)&m_sendContract,sizeof(SEND_DATA));
}

string CP2PBetHelp::PacketP2PAcceptContract(int64_t nMoney,const string& strSendHash,char* pData)
{
	memset(&m_acceptContract,0,sizeof(ACCEPT_DATA));
	m_acceptContract.type = 2;
	m_acceptContract.money = nMoney;
	memcpy(m_acceptContract.targetkey,strSendHash.c_str(),HASH_SIZE);
	memcpy(m_acceptContract.data,pData,5);

	return CSoyPayHelp::getInstance()->GetHexData((const char*)&m_acceptContract,sizeof(ACCEPT_DATA));
}

string CP2PBetHelp::PacketP2PExposeContract(const string& SendHash,const string& strRandomHash)
{
	memset(&m_openContract,0,sizeof(OPEN_DATA));
	m_openContract.type = 3;
	memcpy(m_openContract.targetkey,SendHash.c_str(),HASH_SIZE);
	memcpy(m_openContract.dhash,strRandomHash.c_str(),sizeof(m_openContract.dhash));

	return CSoyPayHelp::getInstance()->GetHexData((const char*)&m_openContract,sizeof(OPEN_DATA));
}

bool CheckRegIDValid(const CString& strRegID)
{
	int nLen = strRegID.GetLength();
	int nPos = strRegID.Find('-');
	if (0 == nLen || -1 == nPos)
	{
		return false;
	}

	for (int i = 0;i<nLen;++i)
	{
		TCHAR ch = strRegID.GetAt(i);
		if (!((ch>='0' && ch<='9') || (ch>='a' && ch<='f') || ch == '-'))
		{
			return false;
		}
	}

	return true;
}

bool CheckHash(const CString& strRegID)
{
	if (HASH_SIZE != strRegID.GetLength())
	{
		return false;
	}

	return true;
}