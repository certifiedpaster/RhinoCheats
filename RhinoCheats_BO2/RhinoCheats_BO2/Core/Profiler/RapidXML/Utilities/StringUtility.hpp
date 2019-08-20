//=====================================================================================

#pragma once

#include <Windows.h>
#include <string>
#include <sstream>
#include <random>
#include <algorithm>

//=====================================================================================

namespace acut
{
	static std::string GetParent(const std::string& path)
	{
		if (path.empty())
			return path;

		auto idx = path.rfind('\\');
		
		if (idx == path.npos)
			idx = path.rfind('/');

		if (idx != path.npos)
			return path.substr(0, idx);
		
		else
			return path;
	}
	/*
	//=====================================================================================
	*/
	static std::string GetExeDirectory()
	{
		char imgName[MAX_PATH] = { NULL };
		DWORD len = ARRAYSIZE(imgName);

		GetModuleFileName(NULL, imgName, len);

		return GetParent(imgName);
	}
	/*
	//=====================================================================================
	*/
	static std::string RandomANString(int length)
	{
		static constexpr char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZbcdefghijklmnopqrstuvwxyz1234567890";
		
		static std::random_device rd;
		static std::uniform_int_distribution<> dist(0, _countof(alphabet) - 1), dist_len(5, 15);
		
		std::string result;

		if (length == 0)
			length = dist_len(rd);

		for (int i = 0; i < length; i++)
			result.push_back(alphabet[dist(rd)]);

		return result;
	}
	/*
	//=====================================================================================
	*/
	static std::string ToLower(std::string str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}
	/*
	//=====================================================================================
	*/
	static std::string ToUpper(std::string str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		return str;
	}
	/*
	//=====================================================================================
	*/
	static std::wstring AnsiToWstring(const std::string& input, DWORD locale)
	{
		wchar_t buf[2048] = { NULL };
		MultiByteToWideChar(locale, 0, input.c_str(), (int)input.length(), buf, ARRAYSIZE(buf));
		return buf;
	}
	/*
	//=====================================================================================
	*/
	static std::string WstringToAnsi(const std::wstring& input, DWORD locale)
	{
		char buf[2048] = { NULL };
		WideCharToMultiByte(locale, 0, input.c_str(), (int)input.length(), buf, ARRAYSIZE(buf), nullptr, nullptr);
		return buf;
	}
	/*
	//=====================================================================================
	*/
	static std::wstring UTF8ToWstring(const std::string& str)
	{
		return AnsiToWstring(str, CP_UTF8);
	}
	/*
	//=====================================================================================
	*/
	static std::string WstringToUTF8(const std::wstring& str)
	{
		return WstringToAnsi(str, CP_UTF8);
	}
	/*
	//=====================================================================================
	*/
    template <typename tstring, typename Container>
    void tsplit(const tstring& str, Container* result, const tstring& delimiters)
    {
        size_t current, next = static_cast<size_t>(-1);
       
		do
        {
            next = str.find_first_not_of( delimiters, next + 1 );
            
			if (next == tstring::npos) 
				break;
            
			next -= 1;

            current = next + 1;
            next = str.find_first_of( delimiters, current );
            result->push_back( str.substr( current, next - current ) );
        } while (next != tstring::npos);
    }
	/*
	//=====================================================================================
	*/
    template <typename Container>
    void split(const std::string& str, Container* result, const std::string& delimiters = " ")
    {
        tsplit(str, result, delimiters);
    }
	/*
	//=====================================================================================
	*/
    template <typename Container>
    void split(const std::wstring& str, Container* result, const std::wstring& delimiters = L" ")
    {
        tsplit(str, result, delimiters);
    }
	/*
	//=====================================================================================
	*/
    inline std::string trim(const std::string& str, const std::string& whitespace = " ")
    {
        const size_t strBegin = str.find_first_not_of(whitespace), strEnd = str.find_last_not_of(whitespace);
       
		if (strBegin == std::string::npos)
            return "";

        const size_t strRange = strEnd - strBegin + 1;

        return str.substr(strBegin, strRange);
    }
	/*
	//=====================================================================================
	*/
    template <typename Ch>
    inline std::basic_string<Ch> ensure_tchar(LPCSTR ptr);
	/*
	//=====================================================================================
	*/
    template <typename Ch>
    inline std::basic_string<Ch> ensure_tchar(LPCWSTR ptr);
	/*
	//=====================================================================================
	*/
    template <>
    inline std::basic_string<char> ensure_tchar(LPCSTR ptr)
    {
        return ptr;
    }
	/*
	//=====================================================================================
	*/
    template <>
    inline std::basic_string<wchar_t> ensure_tchar(LPCSTR ptr)
    {
        return UTF8ToWstring(ptr);
    }
	/*
	//=====================================================================================
	*/
    template <>
    inline std::basic_string<char> ensure_tchar(LPCWSTR ptr)
    {
        return WstringToUTF8(ptr);
    }
	/*
	//=====================================================================================
	*/
    template <>
    inline std::basic_string<wchar_t> ensure_tchar(LPCWSTR ptr)
    {
        return ptr;
    }
	/*
	//=====================================================================================
	*/
    template <typename Ch>
    inline Ch ensure_tchar(char ch);
	/*
	//=====================================================================================
	*/
    template <typename Ch>
    inline Ch ensure_tchar(wchar_t ch);
	/*
	//=====================================================================================
	*/
    template <>
    inline char ensure_tchar(char ch)
    {
        return ch;
    }
	/*
	//=====================================================================================
	*/
    template <>
    inline wchar_t ensure_tchar(char ch)
    {
        return UTF8ToWstring(std::string(1, ch)).front();
    }
	/*
	//=====================================================================================
	*/
    template <>
    inline char ensure_tchar(wchar_t ch)
    {
        return WstringToUTF8(std::wstring(1, ch)).front();
    }
	/*
	//=====================================================================================
	*/
    template <>
    inline wchar_t ensure_tchar(wchar_t ch)
    {
        return ch;
    }
}

//=====================================================================================