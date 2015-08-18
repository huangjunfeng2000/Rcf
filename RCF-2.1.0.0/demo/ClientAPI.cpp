
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
#include "CNetFile.h"
//class CNetFile
//{
//public:
//	CNetFile() : m_pFile(NULL), m_bEof(false), m_iPos(0) {}
//	~CNetFile(){}
//	CNetFile(FILE *fp) : m_pFile(fp), m_bEof(false), m_iPos(0)
//	{
//	}
//	CNetFile(const char * _Filename, const char * _Mode, bool bEof) : 
//	m_strFileName(_Filename), m_strMode(_Mode), m_bEof(bEof), m_iPos(0)
//	{
//	}
//public:
//	std::string m_strFileName;
//	std::string m_strMode;
//	FILE *m_pFile;
//	bool m_bEof;
//	int m_iPos;
//};

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

	void UnintializeClient()
	{
		RpcClient::ReleaseInstance();
	}

	void reverse(std::vector<std::string> &val)
	{
		RpcClient::Instance()->m_pClient->reverse(val);
	}

	bool IsNetFile()
	{
		return true;
	}

	CNetFile *fopen_net(const char * _Filename, const char * _Mode)
	{
		if (!IsNetFile())
			return new CNetFile(fopen(_Filename, _Mode));

		bool bResult = RpcClient::Instance()->m_pClient->fopen(_Filename, _Mode);
		return bResult ? new CNetFile(_Filename, _Mode, false) : NULL;
	}

	int fclose_net(CNetFile * &_File)
	{
		if (!IsNetFile())
			return fclose(_File->m_pFile);
		delete _File;
		_File = NULL;
		return true;
	}

	int feof_net(CNetFile *_File)
	{
		if (!IsNetFile())
			return feof(_File->m_pFile);
		return _File->m_bEof;
	}

	char * fgets_net(char * _Buf, int _MaxCount, CNetFile * _File)
	{
		if (!IsNetFile())
			return fgets(_Buf, _MaxCount, _File->m_pFile);
		std::vector<unsigned char> vecResult;
		bool bResult = RpcClient::Instance()->m_pClient->fgets(vecResult, _MaxCount, *_File);
		if (bResult  && !vecResult.empty())
		{
			memcpy(_Buf, &vecResult[0], vecResult.size());
		}
		return bResult ? _Buf : NULL;
	}
	size_t fread_net(void * _DstBuf, size_t _ElementSize, size_t _Count, CNetFile * _File)
	{
		if (!IsNetFile())
			return fread(_DstBuf, _ElementSize, _Count, _File->m_pFile);
		std::vector<unsigned char> vecResult;
		size_t iResult = RpcClient::Instance()->m_pClient->fread(vecResult, _ElementSize, _Count, *_File);
		if (iResult && !vecResult.empty())
		{
			memcpy(_DstBuf, &vecResult[0], vecResult.size());
		}
		return iResult;
	}

}