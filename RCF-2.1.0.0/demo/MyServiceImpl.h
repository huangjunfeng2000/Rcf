#pragma once

#include <string>
#include <vector>

class CNetFile;
namespace MQRPC
{
class MyServiceImpl
{
public:
	// Reverses the order of strings in the vector.
	void reverse(std::vector<std::string> &v);
	bool fopen(const std::string & _Filename, const std::string & _Mode);
	bool fgets(std::vector<unsigned char> &_Buf, int _MaxCount, CNetFile & _File);
	size_t fread(std::vector<unsigned char> &_Buf, size_t _ElementSize, size_t _Count, CNetFile & _File);

};


}