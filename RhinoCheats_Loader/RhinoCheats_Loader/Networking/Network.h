#pragma once

#include <Windows.h>
#include <string>
#include <WinInet.h>
#pragma comment (lib, "WinInet.lib")

#include <VMProtectSDK.h>

#define INTERNET_OPEN_TYPE_DIRECT 1
#define INTERNET_SERVICE_HTTP 3

#define GET 0
#define POST 1

std::string HttpRequest(std::string site, std::string param);

