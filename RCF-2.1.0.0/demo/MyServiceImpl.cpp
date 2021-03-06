
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "MyServiceImpl.h"
#include "CNetFile.h"
#pragma warning (disable: 4996)

namespace MQRPC
{
	// Reverses the order of strings in the vector.
	void MyServiceImpl::reverse(std::vector<std::string> &v)
	{
		std::cout << "Reversing a vector of strings...\n";
		std::vector<std::string> w;
		std::copy(v.rbegin(), v.rend(), std::back_inserter(w));
		v.swap(w);
	}
	bool MyServiceImpl::fopen(const std::string &_Filename, const std::string & _Mode)
	{
		bool bResult = false;
		if (FILE *fp = ::fopen(_Filename.c_str(), _Mode.c_str()))
		{
			bResult = true;
			fclose(fp);
		}
		return bResult;
	}

	bool MyServiceImpl::fgets(std::vector<unsigned char> &_Buf, int _MaxCount, CNetFile & _File)
	{
		bool bResult = false;
		if (FILE *fp = ::fopen(_File.m_strFileName.c_str(), _File.m_strMode.c_str()))
		{
			::_fseeki64(fp, _File.m_iPos, SEEK_SET);
			//char *strBuf = new char[iReadCount+1];
			_Buf.resize(_MaxCount, 0);
			if (_MaxCount > 0)
			{
				memset(&_Buf[0], 0, _MaxCount);
				char *pResult = ::fgets((char *)(&_Buf[0]), _MaxCount, fp);
				int bEof = feof(fp);
				__int64 iPos = _ftelli64(fp);
				if (iPos - _File.m_iPos != _MaxCount)
					_Buf.resize(iPos-_File.m_iPos);
				_File.m_iPos = iPos;
				_File.m_bEof = bEof;
				bResult = !_Buf.empty();
			}
			else
			{
				bResult = true;
			}
			fclose(fp);
		}
		return bResult;
	}
	size_t MyServiceImpl::fread(std::vector<unsigned char> &_Buf, size_t _ElementSize, size_t _Count, CNetFile & _File)
	{
		size_t iResult = 0;
		if (FILE *fp = ::fopen(_File.m_strFileName.c_str(), _File.m_strMode.c_str()))
		{
			if (_File.m_ptrVal.get())
				_File.m_ptrVal->m_iPos = 1;
			if (!_File.m_vecPtrVal.empty())
				_File.m_vecPtrVal[0]->m_iPos = 1;
			::_fseeki64(fp, _File.m_iPos, SEEK_SET);
			size_t iReadCount = _Count * _ElementSize;
			_Buf.resize(iReadCount, 0);
			if (iReadCount > 0)
			{
				memset(&_Buf[0], 0, iReadCount);
				iResult = ::fread((char *)(&_Buf[0]), _ElementSize, _Count, fp);
				int bEof = feof(fp);
				int iPos = ftell(fp);
				if (iPos - _File.m_iPos != iReadCount)
					_Buf.resize(iPos-_File.m_iPos);
				_File.m_iPos = iPos;
				_File.m_bEof = bEof;
			}
			fclose(fp);
		}
		return iResult;
	}

	int MyServiceImpl::_fseeki64(CNetFile &_File, __int64 _Offset, int _Origin)
	{
		int iResult = -1;
		if (FILE *fp = ::fopen(_File.m_strFileName.c_str(), _File.m_strMode.c_str()))
		{
			iResult = ::_fseeki64(fp, _Offset, _Origin);
			if (iResult == 0)
			{
				_File.m_iPos = _ftelli64(fp);
				_File.m_bEof = feof(fp);
			}
			fclose(fp);
		}
		return iResult;
	}
}