
#pragma once
#include "stdio.h"
#include <string>
#include <vector> 
#include <memory>
//#include <boost/serialization/string.hpp>
//
//#include <boost/serialization/map.hpp>
//#include <boost/serialization/vector.hpp>
#include <boost/shared_ptr.hpp>

#include <SF/string.hpp>
#include <SF/map.hpp>
#include <SF/vector.hpp>
#include <SF/shared_ptr.hpp> 


#include <SF/auto_ptr.hpp> 
//#include <SF/shared_ptr_std.hpp> 

class CNetFile
{
	//friend class boost::serialization::access; 
public:
	CNetFile();
	~CNetFile();
	CNetFile(FILE *fp);
	CNetFile(const char * _Filename, const char * _Mode, bool bEof);
	//template<typename Archive>
	//void serialize(Archive &ar, unsigned int version)
	//{
	//	ar & m_strFileName & m_strMode & m_bEof & m_iPos;
	//}
public:
	std::string m_strFileName;
	std::string m_strMode;
	FILE *m_pFile;
	int m_bEof;
	__int64 m_iPos;
	std::auto_ptr<CNetFile> m_ptrVal;
	std::vector<boost::shared_ptr<CNetFile> > m_vecPtrVal;

};

//namespace boost{
//	namespace serialization{
//template<typename Archive>
//void serialize(Archive &ar, CNetFile &x)
//{
//	ar & x.m_strFileName & x.m_strMode & x.m_bEof & x.m_iPos;
//}
//	}
//}
void serialize(SF::Archive &ar, CNetFile &x);
