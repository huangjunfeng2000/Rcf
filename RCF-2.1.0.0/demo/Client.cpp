
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include "ClientAPI.h"
#include "MyService.hpp"
#include "CNetFile.h"

#include <RCF/RCF.hpp>
#include <RCF/Win32NamedPipeEndpoint.hpp>
#include <RCF/UdpClientTransport.hpp>

#include "ServerAPI.h"
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
			if (CNetFile *fp = MQRPC::fopen_net("c:\\path.txt", "rt"))
			{
				char strLine[1025];
				memset(strLine, 0, 1025);
				size_t iRes = MQRPC::fread_net(strLine, 10, 200, fp);
				char *strResult = MQRPC::fgets_net(strLine, 1024, fp);
				
				MQRPC::fclose_net(fp);
			}
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
