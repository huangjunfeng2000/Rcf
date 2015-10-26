
#include "CNetFile.h"



CNetFile::CNetFile() : m_pFile(NULL), m_bEof(false), m_iPos(0) 
{

}
CNetFile::~CNetFile()
{

}
CNetFile::CNetFile(FILE *fp) : m_pFile(fp), m_bEof(false), m_iPos(0)
{

}
CNetFile::CNetFile(const char * _Filename, const char * _Mode, bool bEof) : 
		m_strFileName(_Filename), m_strMode(_Mode), m_bEof(bEof), m_iPos(0), m_pFile(NULL)
{
}

void serialize(SF::Archive &ar, CNetFile &x)
{
	ar & x.m_strFileName & x.m_strMode & x.m_bEof & x.m_iPos & x.m_ptrVal & x.m_vecPtrVal;
}