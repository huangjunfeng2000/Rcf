
#pragma once
class CNetFile;
namespace MQRPC
{ 
	void UnintializeClient();

	void reverse(std::vector<std::string> &val);
	CNetFile *fopen_net(const char * _Filename, const char * _Mode);
	int fclose_net(CNetFile * &_File);
	int feof_net(CNetFile *_File);
	char * fgets_net(char * _Buf, int _MaxCount, CNetFile * _File);
	size_t fread_net(void * _DstBuf, size_t _ElementSize, size_t _Count, CNetFile * _File);

	int fopen_s_net(CNetFile ** _File, const char * _Filename, const char * _Mode);
	int _fseeki64_net(CNetFile * _File, __int64 _Offset, int _Origin);
	__int64 _ftelli64_net(CNetFile * _File);
	int fgetpos_net(CNetFile *_File, fpos_t *_Pos);


}

