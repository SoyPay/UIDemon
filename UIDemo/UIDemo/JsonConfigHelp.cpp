#include "StdAfx.h"
#include "JsonConfigHelp.h"

IMPLEMENT_SINGLETON(CJsonConfigHelp)
void CJsonConfigHelp::Init()
{

}

void CJsonConfigHelp::ReadJsonConfig(const string& strFilePath)
{
	ifstream ifs;
	ifs.open(strFilePath.c_str(),ios::in);
	if (!ifs.is_open())
	{
		return ;
	}
	
	Json::Reader reader;  
	Json::Value root;  
	if (reader.parse(ifs, root,false))
	{
		ReadMainCfgData(root);
		ReadSesureTradeCfgData(root);
		ReadDarkCoinCfgData(root);
		ReadP2PCfgData(root);
		ReadAnonymCfgData(root);
	}

	ifs.close();
}

void CJsonConfigHelp::ReadMainCfgData(const Json::Value& root)
{
	Json::Value mainCfg = root["mainconfig"];
	ASSERT(!mainCfg.isNull());	
	m_MainCfg.IntIsChinese=mainCfg["chinese"].asInt();
	m_MainCfg.bStartServer = mainCfg["startserver"].asBool();
	m_MainCfg.strServerCfgName = mainCfg["server_config_name"].asString();
}

void CJsonConfigHelp::ReadSesureTradeCfgData(const Json::Value& root)
{
	Json::Value sesureTrade = root["sesuretrade"];
	ASSERT(!sesureTrade.isNull());

	m_SesureTradeCfg.strAddr = sesureTrade["signaddr"].asString();
	m_SesureTradeCfg.strBuyerID = sesureTrade["buyerid"].asString();
	m_SesureTradeCfg.strSellerID = sesureTrade["sellerid"].asString();
	m_SesureTradeCfg.strArID = sesureTrade["arid"].asString();
	m_SesureTradeCfg.nTxFee = sesureTrade["txfee"].asInt();
	m_SesureTradeCfg.nHeight = sesureTrade["height"].asInt();
	m_SesureTradeCfg.nDeposite = sesureTrade["deposite"].asInt();
	m_SesureTradeCfg.nPay = sesureTrade["pay"].asInt();
	m_SesureTradeCfg.nFine = sesureTrade["fine"].asInt();
	m_SesureTradeCfg.nArFee = sesureTrade["arfee"].asInt();
	m_SesureTradeCfg.nMaxFine = sesureTrade["maxfine"].asInt();
	m_SesureTradeCfg.strBinPath = sesureTrade["binpath"].asString();
}

void CJsonConfigHelp::ReadDarkCoinCfgData(const Json::Value& root)
{
	Json::Value darkcoin = root["darkcoin"];
	ASSERT(!darkcoin.isNull());
	m_DarkCfg.strBuyID = darkcoin["buyerid"].asString();
	m_DarkCfg.strSellID = darkcoin["sellerid"].asString();
	m_DarkCfg.nFee = darkcoin["txfee"].asInt();
	m_DarkCfg.nHeight = darkcoin["height"].asInt();
	m_DarkCfg.nMoney = darkcoin["money"].asInt();
	m_DarkCfg.strBinPath = darkcoin["binpath"].asString();
}

void CJsonConfigHelp::ReadP2PCfgData(const Json::Value& root)
{
	Json::Value p2pbet = root["p2pbet"];
	ASSERT(!p2pbet.isNull());
	m_P2PBetCfg.strBinPath = p2pbet["binpath"].asString();
	m_P2PBetCfg.strHashA = p2pbet["hash1"].asString();
	m_P2PBetCfg.strHashB = p2pbet["hash2"].asString();
	m_P2PBetCfg.strSendID = p2pbet["sendid"].asString();
	m_P2PBetCfg.strAcceptID = p2pbet["acceptid"].asString();
	m_P2PBetCfg.nFee = p2pbet["txfee"].asInt();
	m_P2PBetCfg.nHeight = p2pbet["height"].asInt();
	m_P2PBetCfg.nMoney = p2pbet["money"].asInt();

	Json::Value dataA = p2pbet["data1"];
	for (size_t i = 0;i<dataA.size();++i)
	{
		m_P2PBetCfg.szDataA[i] = dataA[i].asInt();
	}

	Json::Value dataB = p2pbet["data2"];
	for (size_t i = 0;i<dataA.size();++i)
	{
		m_P2PBetCfg.szDataB[i] = dataB[i].asInt();
	}
}

void CJsonConfigHelp::ReadAnonymCfgData(const Json::Value& root)
{
	Json::Value anonym = root["anonym"];
	ASSERT(!anonym.isNull());
	m_AnonymCfg.strBinPath = anonym["binpath"].asString();
	m_AnonymCfg.strSendID = anonym["sendid"].asString();
	m_AnonymCfg.strRecvID1 = anonym["recvid1"].asString();
	m_AnonymCfg.strRecvID2 = anonym["recvid2"].asString();
	m_AnonymCfg.nFee = anonym["txfee"].asInt();
	m_AnonymCfg.nHeight = anonym["height"].asInt();
	m_AnonymCfg.nMoney = anonym["money"].asInt();
}

void CJsonConfigHelp::GetMainCfgData(CMainCfg& mainCfg)
{
	mainCfg = m_MainCfg;
}

void CJsonConfigHelp::GetDarkCfgData(CDarkTxCfg& darkCfg)
{
	darkCfg = m_DarkCfg;
}

void CJsonConfigHelp::GetAnonymCfgData(CAnonmyCfg& anonymCfg)
{
	anonymCfg = m_AnonymCfg;
}

void CJsonConfigHelp::GetSesureTradeCfgData(CSesureTradeCfg& sesureCfg)
{
	sesureCfg = m_SesureTradeCfg;
}

void CJsonConfigHelp::GetP2PBetCfgData(CP2PBetCfg& p2pCfg)
{
	p2pCfg = m_P2PBetCfg;
}