
#pragma once
#include <vector>

namespace MQRPC
{
	bool ProcessMessage(std::vector<unsigned char> &vecResult, void *pStr, int iSize);
	void UnintializeServer();
}

