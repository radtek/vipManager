//类实现
#include "stdafx.h"
#include "ByteCode.h"
//#include <vld.h>
#ifdef NDEBUG
#include "windows.h"
#endif
using namespace std;

/*! 
 * 该函数用于将utf8数组转为Unicode格式! 
 * 目前该函数返回值为：转换后unicode数据占用的wchar_t的个数（切记不是总char的个数） ！ 
 * \param [out] sOut   转换结果存放区域指针 
 * \param [in] szU8   源串存放区域指针 
 * \return int 转换结果在目的串中的长度，转换失败则返回-1 
 */
int CByteCode::UTF_8ToUnicode(CString& sOut, char* szU8)
{
	//UTF8 to Unicode 


	//预转换，得到所需空间的大小 
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间 
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//转换 
	::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

	//最后加上'\0' 
	wszString[wcsLen] = '\0';
	sOut = wszString;
	delete[]wszString;

	return wcsLen;
}

void CByteCode::UnicodeToUTF_8(std::string& pOut, CString& pText)
{
	// unicode to UTF8 
	wchar_t* wszString = pText.GetBuffer();
	pText.ReleaseBuffer();
	int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);

	char* szU8 = new char[u8Len + 1];
	::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), szU8, u8Len, NULL, NULL);
	szU8[u8Len] = '\0';
	pOut = szU8;

	delete[] szU8;
}

// Convert a C string in UTF-8 format to UCS-2 format.
void CByteCode::ToUCS2(TCHAR* pcOut, int nOutLen, const char* kpcIn)
{
	MultiByteToWideChar(CP_UTF8, 0, kpcIn, -1, pcOut, nOutLen);
}

// Convert a UCS-2 string to C string in UTF-8 format.
void CByteCode::ToUTF8(char* pcOut, int nOutLen, const TCHAR* kpcIn)
{
	WideCharToMultiByte(CP_UTF8, 0, kpcIn, -1, pcOut, nOutLen, 0, 0);
}
