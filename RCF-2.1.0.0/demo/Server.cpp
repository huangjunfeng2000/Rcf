
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <RCF/RCF.hpp>

#include "MyService.hpp"

//#include <RCF/ThreadPool.hpp>
#include <RCF/Asio.hpp>
//#include <RCF/Win32NamedPipeEndpoint.hpp>
//#include <RCF/UdpServerTransport.hpp>

#include "ServerAPI.h"
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


void getCommand(std::vector<unsigned char> &vecCommand)
{
	if (FILE *fp = fopen("c:\\implWrite.txt", "rb"))
	{
		fseek(fp, 0, SEEK_END);
		int iSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		//ReallocBuffer buffer;// = *mReadVecPtr;
		vecCommand.resize(iSize);
		fread(&vecCommand[0], iSize, 1, fp);
		fclose(fp);
	}
}
int mainTest()
{
	bool bSwitch = true;
	if (bSwitch)
	{
		std::vector<unsigned char> buffer;
		getCommand(buffer);
		std::vector<unsigned char> vecResult;
		MQRPC::ProcessMessage(vecResult, &buffer[0], buffer.size());
		MQRPC::UnintializeServer();

		/*{
			RpcServer sevObj;
			std::vector<unsigned char> vecResult;
			for (int i=0; i<1000; ++i)
				RpcServer::Instance()->ProcessMessage(vecResult, &buffer[0], buffer.size());
			RpcServer::ReleaseInstance();
		}*/

		//RCF::RcfServer server1( RCF::MQEndpoint("abc") );
		//RCF::ServerTransportPtr serverTransport = server1.getServerTransportPtr();
		//RCF::MQServerTransport &mQServerTransport = *(RCF::MQServerTransport *)serverTransport.get();
		////mQServerTransport.createNetworkSession();
		//server1.bind<MyService>(myServiceImpl);
		//RCF::RcfSessionPtr mRcfSessionPtr = server1.createSession();
		////MQNetworkSessionPtr networkSessionPtr( implCreateNetworkSession() );
		//RCF::MQNetworkSessionPtr mQNetworkSession( new RCF::MQNetworkSession(mQServerTransport) );
		//mRcfSessionPtr->setNetworkSession( *mQNetworkSession.get() );
		//server1.onReadCompleted(mRcfSessionPtr);
	}
	else
	{
		RCF::RcfInitDeinit rcfInit;

		// Start a TCP server on port 50001, and expose MyServiceImpl.
		MyServiceImpl myServiceImpl;
		RCF::RcfServer server( RCF::TcpEndpoint("0.0.0.0", 50001) );
		server.bind<MyService>(myServiceImpl);

		server.start();

		RCF::ThreadPoolPtr threadPoolPtr = server.getThreadPool();
		//threadPoolPtr->enableMuxerType(RCF::Mt_Asio);
		RCF::AsioIoService * asioIoService = threadPoolPtr->getIoService();
		while(true)
		{
			asioIoService->run_one();
			Sleep(100);
		}
	}
	//server.start();

	std::cout << "Press Enter to exit..." << std::endl;
	std::cin.get();
	return 0;
}
int main()
{
	return mainTest();

    RCF::RcfInitDeinit rcfInit;

    // Start a TCP server on port 50001, and expose MyServiceImpl.
    MyServiceImpl myServiceImpl;
    RCF::RcfServer server( RCF::TcpEndpoint("0.0.0.0", 50001) );
    server.bind<MyService>(myServiceImpl);
    server.start();

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}
