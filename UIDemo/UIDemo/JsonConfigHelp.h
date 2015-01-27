#pragma once
#include "CommonStruct.h"

class CMainCfg 
{
public:
	CMainCfg()
	{
		bStartServer = false;
	}

	bool	bStartServer;
	string	strServerCfgName;
};

class CDarkTxCfg
{
public:
	string	strBinPath;
	string	strBuyID;
	string	strSellID;
	int		nMoney;
	int		nHeight;
	int		nFee;
};

class CAnonmyCfg
{
public:
	string	strBinPath;
	string	strSendID;
	string	strRecvID1;
	string	strRecvID2;
	int		nMoney;
	int		nHeight;
	int		nFee;
};

class CSesureTradeCfg
{
public:
	string	strBinPath;
	string strAddr;
	string strBuyerID;
	string strSellerID;
	string strArID;
	int nTxFee;
	int nHeight;
	int nDeposite;
	int nPay;
	int nMaxFine;
	int nFine;
	int nArFee;
};

class CP2PBetCfg
{
private:
	static const int DATA_SIZE = 5;
public:
	int				nFee;
	int				nMoney;
	int				nHeight;
	string			strBinPath;
	string			strHashA;
	string			strHashB;
	string			strSendID;
	string			strAcceptID;
	unsigned char	szDataA[DATA_SIZE];
	unsigned char	szDataB[DATA_SIZE];
};

class CJsonConfigHelp
{
	DEFINE_SINGLETON(CJsonConfigHelp)
public:
	void Init();
	void ReadJsonConfig(const string& strFilePath);
	void GetMainCfgData(CMainCfg& mainCfg);
	void GetDarkCfgData(CDarkTxCfg& darkCfg);
	void GetAnonymCfgData(CAnonmyCfg& anonymCfg);
	void GetSesureTradeCfgData(CSesureTradeCfg& sesureCfg);
	void GetP2PBetCfgData(CP2PBetCfg& p2pCfg);

private:
	void ReadMainCfgData(const Json::Value& root);
	void ReadSesureTradeCfgData(const Json::Value& root);
	void ReadDarkCoinCfgData(const Json::Value& root);
	void ReadP2PCfgData(const Json::Value& root);
	void ReadAnonymCfgData(const Json::Value& root);

private:
	BOOL			m_bInvalid;
	CMainCfg		m_MainCfg;
	CDarkTxCfg		m_DarkCfg;
	CAnonmyCfg		m_AnonymCfg;
	CP2PBetCfg		m_P2PBetCfg;
	CSesureTradeCfg	m_SesureTradeCfg;
};

