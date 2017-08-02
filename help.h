
#pragma once
#ifndef __HELP_H__
#define __HELP_H__
#include <Windows.h>
#include <memory>
#include <fstream>
#include <vector>
#include <string>
//============================================================================
//								帮助函数
//============================================================================
namespace helper
{ 
/*
template<class _SmartPtr>
bool read_file(
    _In_ const std::wstring& file_name,
    _Out_ _SmartPtr&        readbuf,
    _Out_ size_t&           file_size
    )
{
    HANDLE hFile = NULL;
    DWORD nRead = 0;
    bool result = false;

    hFile = CreateFileW(file_name.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    file_size = GetFileSize(hFile, NULL);
    readbuf.reset((_SmartPtr::element_type*)new unsigned char[file_size]);
    if (ReadFile(hFile, readbuf.get(), file_size, &nRead, NULL))
        result = true;
    CloseHandle(hFile);
    return result;
}

// 上面的read_file使用了c++11标准 vc90以上支持
bool read_filev90(
	_In_ const std::string& file_name,
	_Out_  char*& buff,
	_Out_ size_t& file_size
	)
{
	ifstream rf(file_name.c_str(),std::ios::binary);
	if(!rf.is_open())
	{
		return false;
	}
	rf.seekg(0, rf.end);
	file_size = rf.tellg();
	rf.seekg(0,rf.beg);
	buff = new char[file_size];
	rf.read(buff, file_size);
//	buff[nsize] = 0;
	rf.close();
	return true;
}


inline
bool write_file(
    const std::wstring&  file_name,
    const void*         file_buf,
    size_t              file_size
    )
{
    HANDLE hFile = NULL;
    DWORD nWrite = 0;
    bool result = false;

    hFile = CreateFileW(file_name.c_str(), GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        return false;
    if (WriteFile(hFile, file_buf, file_size, &nWrite, NULL))
    {
        SetFilePointer(hFile, file_size, NULL, FILE_BEGIN);
        SetEndOfFile(hFile);
        result = true;
    }
    else
    {
        result = false;
    }
    CloseHandle(hFile);
    return result;
}

inline
bool delete_file(
    _In_ const std::wstring& file_name
    )
{
    return DeleteFile(file_name.c_str()) != FALSE;
}

// int to_hex(_In_ const void* data, _In_ int count, _Out_ char* hex)
// {
// 	for(int i = 0; i < count; i++)
// 		sprintf(hex + 2*i, "%02X", ((unsigned char*)data)[i]);
// 	
//     return 0;
// }


int from_hex(_In_ const char* hex, _In_ int count, _Out_ void* data)
{
    unsigned char temp[2];

    if (data == NULL || hex == NULL || (int)strlen(hex) < count * 2)
        return -1;

    for (int i = 0; i != count; ++i)
    {
        for (int j = 0; j != 2; ++j)
        {
            if (hex[2 * i + j] >= '0' && hex[2 * i + j] <= '9')
                temp[j] = hex[2 * i + j] - '0';
            else if (hex[2 * i + j] >= 'A' && hex[2 * i + j] <= 'F')
                temp[j] = hex[2 * i + j] - 'A' + 10;
            else if (hex[2 * i + j] >= 'a' && hex[2 * i + j] <= 'f')
                temp[j] = hex[2 * i + j] - 'a' + 10;
            else
                return -1;
        }

        ((unsigned char*)data)[i] = (temp[0] << 4) + temp[1];
    }

    return 0;
}

std::string wstr2str(const std::wstring& wstr)  
{  
	if (wstr.empty())  
		return std::string(); 

	UINT cp = GetACP();
	const wchar_t* u_str = wstr.c_str();  
	int u_len = (int)wstr.size();  

	int a_len = WideCharToMultiByte(cp, 0, u_str, u_len, NULL, 0, NULL, NULL);  
	if (a_len == 0)  
		return std::string();  

	std::vector<char> astr(a_len);  
	char* a_str = &astr.front();  

	WideCharToMultiByte(cp, 0, u_str, u_len, a_str, a_len, NULL, NULL);  

	return std::string(astr.begin(), astr.end());  
} 


CString unixTime2Localtime(const time_t& tickTime)
{
	if (tickTime == 0)
	{
		return _T("");
	}
	struct tm tme;
	localtime_s(&tme,&tickTime);
	CString strr;
	strr.Format(_T("%d/%02d/%02d %02d:%02d:%02d"),tme.tm_year+1900,tme.tm_mon+1,tme.tm_mday,tme.tm_hour,tme.tm_min,tme.tm_sec);
	return strr;
}


time_t localtime2unixTime(char timeStamp[])  
{  
	struct tm tm;  
	memset(&tm, 0, sizeof(tm));  

	sscanf_s(timeStamp, "%d/%d/%d %d:%d:%d",   
		&tm.tm_year, &tm.tm_mon, &tm.tm_mday,  
		&tm.tm_hour, &tm.tm_min, &tm.tm_sec);  

	tm.tm_year -= 1900;  
	tm.tm_mon--;  

	return mktime(&tm);  
} 

time_t localtime2unixTime(CString strTimeStamp)  
{  
	struct tm tm;  
	memset(&tm, 0, sizeof(tm));  

	string strtime = wstr2str(strTimeStamp.GetBuffer());
	sscanf_s(strtime.c_str(), "%d/%d/%d %d:%d:%d",   
		&tm.tm_year, &tm.tm_mon, &tm.tm_mday,  
		&tm.tm_hour, &tm.tm_min, &tm.tm_sec);  

	tm.tm_year -= 1900;  
	tm.tm_mon--;  

	return mktime(&tm);  
} 
*/
std::string wstring_to_string(const std::wstring& wstr)  
{  
	if (wstr.empty())  
		return std::string();  
	UINT cp = GetACP();
	const wchar_t* u_str = wstr.c_str();  
	int u_len = (int)wstr.size();  

	int a_len = WideCharToMultiByte(cp, 0, u_str, u_len, NULL, 0, NULL, NULL);  
	if (a_len == 0)  
		return std::string();  

	std::vector<char> astr(a_len);  
	char* a_str = &astr.front();  

	WideCharToMultiByte(cp, 0, u_str, u_len, a_str, a_len, NULL, NULL);  

	return std::string(astr.begin(), astr.end());  
}  

std::wstring string_to_wstring(const std::string& astr)  
{  
	if (astr.empty())  
		return std::wstring();  
	UINT cp = GetACP();
	const char* a_str = astr.c_str();  
	int a_len = (int)astr.size();  

	int u_len = MultiByteToWideChar(cp, 0, a_str, a_len, NULL, 0);  
	if (u_len == 0)  
		return std::wstring();  

	std::vector<wchar_t> wstr(u_len);  
	wchar_t* u_str = &wstr.front();  
	MultiByteToWideChar(cp, 0, a_str, a_len, u_str, u_len);  

	return std::wstring(wstr.begin(), wstr.end());  
}
/*
//单个字符异或运算
inline char MakecodeChar(char c,int key)
{
	return c=c^key;
}
//单个字符解密
inline char CutcodeChar(char c,int key)
{
	return c^key;
}

//加密
void Makecode(char *pstr,int *pkey,size_t len)
{
	for(int i=0;i<len;i++)
		*(pstr+i)=MakecodeChar(*(pstr+i),pkey[i%8]);
}

//解密
void Cutecode(char *pstr,int *pkey,size_t len)
{
	for(int i=0;i<len;i++)
		*(pstr+i)=CutcodeChar(*(pstr+i),pkey[i%8]);
}
*/
}

#endif