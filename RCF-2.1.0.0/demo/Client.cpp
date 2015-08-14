
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
struct MQIOManager
{
	bool SendCommand(void *pStr, size_t iSize)
	{
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
		MQIOManager ioManager;
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

int mainTest()
{
	//RCF::RcfInitDeinit rcfInit;
	//RCF::initMQ();
	try
	{
		// Setup a vector of strings.
		std::vector<std::string> v;
		v.push_back("one");
		v.push_back("two");
		v.push_back("three");

		// Print them out.
		std::cout << "Before:\n";
		std::copy(
			v.begin(), 
			v.end(), 
			std::ostream_iterator<std::string>(std::cout, "\n"));

		// Make the call.
		{
			//gpAmiThreadPool not needed 
			/*RcfClient<MyService>( RCF::MQEndpoint() ).reverse(v);*/
			//RcfClient<MyService>( RCF::TcpEndpoint("127.0.0.1", 50001) ).reverse(v);
		}
		{
			//RcfClient<MyService> *pClient = new RcfClient<MyService>( RCF::MQEndpoint() );
			//RCF::ClientStub &clientStub = pClient->getClientStub();
			//RCF::ClientTransport &clientTransport = clientStub.getTransport();
			//RCF::MQClientTransport &mqClientTransport = *(RCF::MQClientTransport*)(&clientTransport);
			//MQIOManager ioManager;
			//mqClientTransport.SetCallBackFunc(
			//	boost::bind(&MQIOManager::SendCommand, &ioManager, _1, _2), 
			//	boost::bind(&MQIOManager::GetResult, &ioManager, _1));
			//pClient->reverse(v);
			//delete pClient;
		}
		{
			//RpcClient::Instance()->m_pClient->reverse(v);
			//RpcClient::ReleaseInstance();
		}
		{
			MQRPC::reverse(v);
			MQRPC::UnintializeClient();
			MQRPC::UnintializeServer();
		}

		// Print them out again. This time they are in reverse order.
		std::cout << "\nAfter:\n";
		std::copy(
			v.begin(), 
			v.end(), 
			std::ostream_iterator<std::string>(std::cout, "\n"));
		//RCF::deinitMQ();
	}
	catch(const RCF::Exception & e)	{
		std::cout << "Caught exception:\n";
		std::cout << e.getError().getErrorString() << std::endl;
		return 1;
	}

	return 0;
}


int main()
{
	return mainTest();
    //RCF::RcfInitDeinit rcfInit;
	RCF::init();
    try
    {
        // Setup a vector of strings.
        std::vector<std::string> v;
        v.push_back("one");
        v.push_back("two");
        v.push_back("three");

        // Print them out.
        std::cout << "Before:\n";
        std::copy(
            v.begin(), 
            v.end(), 
            std::ostream_iterator<std::string>(std::cout, "\n"));

        // Make the call.
		//gpAmiThreadPool not needed 
        RcfClient<MyService>( RCF::MQEndpoint() ).reverse(v);
        //RcfClient<MyService>( RCF::TcpEndpoint("127.0.0.1", 50001) ).reverse(v);

        // Print them out again. This time they are in reverse order.
        std::cout << "\nAfter:\n";
        std::copy(
            v.begin(), 
            v.end(), 
            std::ostream_iterator<std::string>(std::cout, "\n"));
    }
    catch(const RCF::Exception & e)
    {
        std::cout << "Caught exception:\n";
        std::cout << e.getError().getErrorString() << std::endl;
        return 1;
    }

    return 0;
}
