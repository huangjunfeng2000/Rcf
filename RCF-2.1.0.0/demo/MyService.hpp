
#ifndef INCLUDE_MYSERVICE_HPP
#define INCLUDE_MYSERVICE_HPP

#include <string>
#include <vector>

#include <RCF/Idl.hpp>
#include <SF/vector.hpp>

//typedef const char * const_char;
//#include "CNetFile.h"
class CNetFile;
RCF_BEGIN(MyService, "MyService")
    RCF_METHOD_V1(void, reverse, std::vector<std::string> &);
	RCF_METHOD_R2(bool, fopen, const std::string &, const std::string &);
	RCF_METHOD_R3(bool, fgets, std::vector<unsigned char> &, int , CNetFile &);
	RCF_METHOD_R4(size_t, fread, std::vector<unsigned char> &, size_t, size_t, CNetFile &);


RCF_END(MyService);

#endif // ! INCLUDE_MYSERVICE_HPP'
