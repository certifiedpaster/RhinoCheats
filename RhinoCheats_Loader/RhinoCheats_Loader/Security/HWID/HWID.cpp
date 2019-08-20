#include "HWID.h"

std::string GetHWID()
{
	int nSize = VMProtectGetCurrentHWID(NULL, 0); // get the required buffer size
	char* pBuf = new char[nSize]; // allocate memory for the buffer
	VMProtectGetCurrentHWID(pBuf, nSize); // obtain the identifier
	std::string hwid(pBuf); // use the identifier
	delete[] pBuf; // release memory
	
	return hwid;
}