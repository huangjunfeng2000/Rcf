
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include "ClientAPI.h"
#include "MyService.hpp"

#include <RCF/RCF.hpp>
#include <RCF/Win32NamedPipeEndpoint.hpp>
#include <RCF/UdpClientTransport.hpp>

#include "ServerAPI.h"
namespace MQRPC
{

struct MQIOManager
{
	bool SendCommand(void *pStr, size_t iSize)
	{
		return ProcessMessage(m_vecResult, pStr, iSize);
		m_vecResult.resize(0);
		m_vecSend.resize(iSize);
		memcpy(&m_vecSend[0], pStr, iSize);
		if (FILE *fp = fopen("c:\\send.txt", "wb"))
		{
			fwrite(pStr, iSize, 1, fp);
			fclose(fp);
		}
		//send & recv

		//temp read
		if (FILE *fp = fopen("c:\\implRead.txt", "rb"))
		{
			fseek(fp, 0, SEEK_END);
			int iSize = ftell(fp);
			m_vecResult.resize(iSize);
			fseek(fp, 0, SEEK_SET);
			fread(&m_vecResult[0], iSize, 1, fp);
			fclose(fp);
		}
		return true;
	}
	void *GetResult(size_t &iSize)
	{
		iSize = m_vecResult.size();
		return iSize ? &m_vecResult[0] : NULL;
	} 
	std::vector<unsigned char> m_vecSend;
	std::vector<unsigned char> m_vecResult;
};
class RpcClient;
class RpcClient
{
public:
	RpcClient()
	{
		RCF::initMQ();
		m_pClient = new RcfClient<MyService>( RCF::MQEndpoint() );
		RCF::ClientStub &clientStub = m_pClient->getClientStub();
		RCF::ClientTransport &clientTransport = clientStub.getTransport();
		RCF::MQClientTransport &mqClientTransport = *(RCF::MQClientTransport*)(&clientTransport);
		//MQIOManager ioManager;
		mqClientTransport.SetCallBackFunc(
			boost::bind(&MQIOManager::SendCommand, &ioManager, _1, _2), 
			boost::bind(&MQIOManager::GetResult, &ioManager, _1));
	}
	~RpcClient()
	{
		delete m_pClient;
		RCF::deinitMQ();
	}
	static RpcClient * Instance()
	{
		if (!s_pRpcClient)
			s_pRpcClient = new RpcClient;
		return s_pRpcClient;
	}
	static void ReleaseInstance()
	{
		delete s_pRpcClient;
		s_pRpcClient = NULL;
	}
	MQIOManager ioManager;
	RcfClient<MyService> *m_pClient;
	static RpcClient *s_pRpcClient;
};
RpcClient *RpcClient::s_pRpcClient = NULL;

void reverse(std::vector<std::string> &val)
{
	RpcClient::Instance()->m_pClient->reverse(val);
}

void UnintializeClient()
{
	RpcClient::ReleaseInstance();
}

}