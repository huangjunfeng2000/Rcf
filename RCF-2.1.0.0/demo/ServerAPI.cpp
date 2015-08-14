
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "ServerAPI.h"

#include <RCF/RCF.hpp>

#include "MyService.hpp"

#include <RCF/ThreadPool.hpp>
#include <RCF/Asio.hpp>
#include <RCF/Win32NamedPipeEndpoint.hpp>
#include <RCF/UdpServerTransport.hpp>

namespace MQRPC
{
class MyServiceImpl
{
public:
	// Reverses the order of strings in the vector.
	void reverse(std::vector<std::string> &v)
	{
		std::cout << "Reversing a vector of strings...\n";
		std::vector<std::string> w;
		std::copy(v.rbegin(), v.rend(), std::back_inserter(w));
		v.swap(w);
	}
};

class RpcServer
{
public:
	RpcServer()
	{
		RCF::initMQ();
		//LogManager::init();
		Init();
	}
	~RpcServer()
	{
		mRcfSessionPtr.reset();

		RCF::deinitMQ();
	}
	static RpcServer * Instance()
	{
		if (!s_pRpcServer)
			s_pRpcServer = new RpcServer;
		return s_pRpcServer;
	}
	static void ReleaseInstance()
	{
		delete s_pRpcServer;
		s_pRpcServer = NULL;
	}
	void Init()
	{
		m_pServer.reset(new RCF::RcfServer( RCF::MQEndpoint("abc") ));
		RCF::ServerTransportPtr serverTransport = m_pServer->getServerTransportPtr();
		RCF::MQServerTransport &mQServerTransport = *(RCF::MQServerTransport *)serverTransport.get();
		m_pServer->bind<MyService>(myServiceImpl);
		mRcfSessionPtr = m_pServer->createSession();
		//MQNetworkSessionPtr networkSessionPtr( implCreateNetworkSession() );
		mQNetworkSession = RCF::MQNetworkSessionPtr ( new RCF::MQNetworkSession(mQServerTransport) );
		mRcfSessionPtr->setNetworkSession( *mQNetworkSession.get() );
	}
	bool ProcessMessage(std::vector<unsigned char> &vecResult, void *pStr, int iSize)
	{
		mQNetworkSession->SetMessage(pStr, iSize);
		m_pServer->onReadCompleted(mRcfSessionPtr);
		RCF::ReallocBufferPtr pResult = mQNetworkSession->GetResult();
		size_t iResultSize = pResult->size();
		vecResult.assign(pResult->getPtr(), pResult->getPtr()+iResultSize);
		pResult->clear();
		return iResultSize > 0;
	}
	static RpcServer *s_pRpcServer;
	std::auto_ptr<RCF::RcfServer> m_pServer;
	RCF::RcfSessionPtr mRcfSessionPtr;
	RCF::MQNetworkSessionPtr mQNetworkSession;
	MyServiceImpl myServiceImpl;
};

RpcServer *RpcServer::s_pRpcServer = NULL;

bool ProcessMessage(std::vector<unsigned char> &vecResult, void *pStr, int iSize)
{
	return RpcServer::Instance()->ProcessMessage(vecResult, pStr, iSize);
}
void UnintializeServer()
{
	RpcServer::ReleaseInstance();
}

}