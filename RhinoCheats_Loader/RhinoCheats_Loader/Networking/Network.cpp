#include "Network.h"

#define nBuffSize 1024

std::string HttpRequest(std::string site, std::string param)
{
	HINTERNET hInternet = InternetOpen(
		VMProtectDecryptString("RhinoLoader/3.3 (id;35EFF45628CF271AF4D77D99F2EE7578EEA1C13F96E98931F5CE43D22A6A1F9D7AFF5BED714F7)"),
		INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	if (hInternet == NULL)
	{
		return "";
	}

	HINTERNET hConnect = InternetConnect(hInternet, site.c_str(), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hConnect == NULL)
	{
		InternetCloseHandle(hInternet);
		return "";
	}

	const char* parrAcceptTypes[] = { VMProtectDecryptString("text/*"), NULL }; // accepted types. We'll choose text.

	HINTERNET hRequest = HttpOpenRequest(hConnect, VMProtectDecryptString("POST"), param.c_str(), NULL, NULL, parrAcceptTypes, 
		INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_RELOAD|INTERNET_FLAG_SECURE, 0);
	if (hRequest == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return "";
	}

	BOOL bRequestSent = HttpSendRequest(hRequest, NULL, 0, NULL, 0);
	if (!bRequestSent)
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return "";
	}

	char buff[nBuffSize];

	BOOL bKeepReading = true;
	DWORD dwBytesRead = -1;

	std::string data;

	while (bKeepReading && dwBytesRead != 0)
	{
		bKeepReading = InternetReadFile(hRequest, buff, nBuffSize, &dwBytesRead);
		data.append(buff, dwBytesRead);
	}

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);

	return data;
}