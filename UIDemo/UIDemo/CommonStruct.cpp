#include "stdafx.h"
#include "CommonStruct.h"


string EncodeBase64(const unsigned char* pch, size_t len) {
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

string EncodeBase64(const string& str) {
	return EncodeBase64((const unsigned char*) str.c_str(), str.size());
}

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

void ProductHttpHead(const CStringA& configdir,CStringA &strPort,CStringA& strPreHeadstr,CStringA& strendHeadstr)
{
	CStringA configpath = "";
	configpath.AppendFormat("%s",configdir);
	configpath.AppendFormat("\\%s","soypay.conf");
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
		string strUserPass64 = EncodeBase64(UserPass.GetString());

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
			if (param.GetLength()<10)
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

void CreateScriptTx(const char* pAccount,bool bPath,const char* pScript,int nFee,int nHeight,
	int nAuthTime,int nMoneyPerTime,int nMoneyTotal,int nMoneyPerDay,const string& strUserData)
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

	SendRPCData(root.toStyledString(),emBaseFunction);
}

void CreateScriptAccountTx(const char* pAccount,int nFee,int nHeight)
{
	Json::Value root;

	root["method"] = "registeraccounttx"; 
	root["params"].append(pAccount);
	root["params"].append(nFee);
	root["params"].append(nHeight);

	SendRPCData(root.toStyledString(),emBaseFunction);
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

void PacketFirstContract(enumSesureTrade emType,const char*pBuyID,const char* pSellID,const vector<string>& vArRegID,
	int nHeight,int nFine,int nPay,int nFee,int ndeposit,FIRST_CONTRACT* pContract)
{
	ASSERT(pContract);
	ASSERT(emFirstStep == emType);
	memset(pContract,0,sizeof(FIRST_CONTRACT));
	pContract->nType = emType;
	pContract->nArbitratorCount = static_cast<unsigned char>(vArRegID.size());
	pContract->nHeight = nHeight;

	unsigned char nSize = sizeof(int);
	vector<unsigned char> v = ParseHex(pBuyID);
	memcpy(pContract->buyer.accounid,&v[0],ACCOUNT_ID_SIZE);

	v = ParseHex(pSellID);
	memcpy(pContract->seller.accounid,&v[0],ACCOUNT_ID_SIZE);

	for(size_t i = 0;i<vArRegID.size();++i)
	{
		v = ParseHex(vArRegID.at(i));
		memcpy(pContract->arbitrator[i].accounid,&v[0],ACCOUNT_ID_SIZE);
	}
	
	memcpy(&pContract->nFineMoney,(const char*)&nFine,nSize);//100
	memcpy(&pContract->nPayMoney,(const char*)&nPay,nSize);//80
	memcpy(&pContract->ndeposit,(const char*)&ndeposit,nSize);//20
	memcpy(&pContract->nFee,(const char*)&nFee,nSize);//10
}

void packetNextContract(enumSesureTrade emType,unsigned char* pHash,NEXT_CONTRACT* pNextContract)
{
	memset(pNextContract,0,sizeof(NEXT_CONTRACT));
	pNextContract->nType = emType;
	memcpy(pNextContract->hash,pHash,HASH_SIZE);
}

void packetLastContract(enumSesureTrade emType,unsigned char* pHash,int nFine,ARBIT_RES_CONTRACT* pLastContract)
{
	ASSERT(emType == emLastStep);
	memset(pLastContract,0,sizeof(ARBIT_RES_CONTRACT));
	pLastContract->nType = emType;
	memcpy(pLastContract->hash,pHash,HASH_SIZE);
	memcpy(&pLastContract->nMinus,(const char*)&nFine,sizeof(int));
}

void CreateContractTx(const string& strScript,const vector<string>& vAddr,const vector<unsigned char>& contract,
	int nHeight,int nFee)
{
	ASSERT(vAddr.size());
	Json::Value root;
	root["method"] = "createcontracttx";
	root["params"].append(strScript.c_str());

	Json::Value item;
	item.append(vAddr.at(0));
	root["params"].append(item);
	string s = HexStr(contract);
	int n = s.size();
	root["params"].append(HexStr(contract));
	root["params"].append(nFee);
	root["params"].append(nHeight);

	SendRPCData(root.toStyledString(),emContract);
}

bool GetErrorMsg(const string& strRecvData,string& strErrorMsg)
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
			if (strRegisterd != " ")
			{
				Json::Value v2 = arrayObj[i]["addr"];
				string strAddr = v2.asString();
				strRegID.Format(_T("%S"),strRegisterd.c_str());
				strAddres.Format(_T("%S"),strAddr.c_str());
				mapAccount.insert(make_pair(strRegID,strAddres));
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
		if(GetErrorMsg(pRecvData,strErrorMsg))
		{
			strShowData = strErrorMsg.c_str();
		}
	}

	return strShowData;
}

void CreateNomalTx(const char* pFrom,const char* pTo,int nMoney,int nFee,int nHeight)
{
	CStringA str;
	int i = 1;
	int nfee = 0;
	str.Format("{\"%s\":\"%s\",\"%s\":","method","createnormaltx","params");
	str.AppendFormat("[\"%s\",\"%s\",%ld,%d,%d],\"id\":%d}\n",pFrom,pTo,nMoney,nFee,nHeight);

	SendRPCData(str.GetString(),emBaseFunction);
}

void Setgenerate(bool bStart)
{
	CString str;
	int i = 1;
	str.Format(_T("{\"%s\":\"%s\",\"%s\":"),_T("method"),_T("setgenerate"),_T("params") );

	if(bStart)
	{
		str.AppendFormat(_T("[%s],\"id\":1}\n"),_T("true"));
	}
	else
	{
		str.AppendFormat(_T("[%s],\"id\":1}\n"),_T("false"));
	}
	
	USES_CONVERSION;
	SendRPCData(W2A(str),emBaseFunction);
}

vector<unsigned char> ParseHex(const char* psz) {
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

vector<unsigned char> ParseHex(const string& str) {
	return ParseHex(str.c_str());
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
	return HexStr(vData);
}

CString GetCompressRegID(const CString& strRegID)
{
	USES_CONVERSION;
	char* pData = W2A(strRegID);
	vector<unsigned char>vRegID = ::ParseHex(pData);

	int nHeight = 0;
	short nIndex = 0;
	memcpy(&nHeight,&vRegID[0],sizeof(nHeight));
	memcpy(&nIndex,&vRegID[sizeof(nHeight)],sizeof(nIndex));
	CString str;
	str.Format(_T("%d-%d"),nHeight,nIndex);
	return str;
}