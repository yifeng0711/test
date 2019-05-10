
#ifndef GM_STRUTIL_H
#define GM_STRUTIL_H

#include <sstream>
#include <algorithm>
#include <vector>
#include <string>

#include <iomanip>

#ifndef _MSWINDOWS_
#include <stdarg.h>
#include <string.h>
#endif


/*
*********************************************************************
*
*@brief : 字符串操作功能
*
*********************************************************************
*/

namespace  GMStrUtil
{


	// 删除字符串左边空白
	inline static std::string  TrimLeft(const std::string& str)
	{
		std::string t = str;
		t.erase(0, t.find_first_not_of(" \t\n\r"));
		return t;
	}

	// 删除字符串右边空白
	inline static std::string TrimRight(const std::string& str)
	{
		std::string t = str;
		t.erase(t.find_last_not_of(" \t\n\r") + 1);
		return t;
	}

	// 删除字符串左右两边空白
	inline static std::string Trim(const std::string& str)
	{
		std::string t = str;
		t.erase(0, t.find_first_not_of(" \t\n\r"));
		t.erase(t.find_last_not_of(" \t\n\r") + 1);
		return t;
	}

	// 转换为小写字符串
	inline static std::string ToLower(const std::string& str)
	{
		std::string t = str;
		std::transform(t.begin(), t.end(), t.begin(), ::tolower);
		return t;
	}

	// 转换为小写字符串
	inline static std::wstring ToLowerW(const std::wstring& str)
	{
		std::wstring t = str;
		std::transform(t.begin(), t.end(), t.begin(), ::tolower);
		return t;
	}

	// 转换为大写字符串
	inline static std::string ToUpper(const std::string& str)
	{
		std::string t = str;
		std::transform(t.begin(), t.end(), t.begin(), ::toupper);
		return t;
	}

	// 分解字符串
	inline static void Split(std::vector<std::string> &csResult, const std::string& str, const std::string& delimiters)
	{
		std::string::size_type iOffset = 0;
		std::string strToken;
		for(;;)
		{

			std::string::size_type i = str.find_first_not_of(delimiters, iOffset);
			if (i == std::string::npos)
			{
				iOffset = str.length();
				return;
			}

			// 查找标识结束位置
			std::string::size_type j = str.find_first_of(delimiters, i);
			if (j == std::string::npos)
			{
				strToken = str.substr(i);
				iOffset = str.length();
				csResult.push_back(strToken);
			}
			else
			{
				strToken = str.substr(i, j - i);
				iOffset = j;
				csResult.push_back(strToken);
			}
		}
	}

	//不分大小写的比较
	inline static bool EqualsIgnoreCase(const std::string& strSrc, const std::string& strDest)
	{
		return ToLower(strSrc) == ToLower(strDest);
	}


	// 字符串类型转换模板函数
	// 字符串类型转换模板函数
	template<class T> T ToNumber(const std::string& str);
	template<class T> T ToHexNumber(const std::string& str);
	template<class T> std::string ToString(const T value);
	template<class T> std::string ToString(const T value, int fixedPrecision);
	template<class T> std::string ToHexString(const T value);

	

	/*
	*********************************************
	Function : ToNumber
	DateTime : 2011/1/20 15:23
	Description : 将十进制字符串转换为数值
	Input :
	Output :
	Return :
	Note :
	*********************************************
	*/
	template<class T>
	T ToNumber(const std::string& str)
	{
		T value;
		std::istringstream iss(str.c_str());
		iss >> value;
		return value;
	}

	/*
	*********************************************
	Function :
	DateTime : 2011/1/20 15:23
	Description : 将十六进制字符串转换为数值
	Input :
	Output :
	Return :
	Note :
	*********************************************
	*/
	template<class T>
	T ToHexNumber(const std::string& str)
	{
		T value;
		std::istringstream iss(str.c_str());
		iss >> std::hex >> value;
		return value;
	}


	template<class T>
	std::string ToString(const T value)
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}

	template<class T>
	std::string ToString(const T value, int fixedPrecision)
	{
		std::ostringstream oss;
		oss <<std::fixed<<std::setprecision(fixedPrecision)<< value;
		return oss.str();
	}


	template<class T>
	std::string ToHexString(const T value)
	{
		std::ostringstream oss;
		oss << "0x" << std::hex << value;
		return oss.str();
	}



	template<class T>
	int CheckValueRange(T &vValue, const T vMin, const T vMax)
	{
		if( vValue<vMin )
		{
			vValue = vMin;
			return -1;
		}
		else if( vValue > vMax )
		{
			vValue = vMax;
			return 1;
		}
		return 0;
	}


#define MAX_STRING_LENGTH  (100<<20)


	inline static void FilenameFormat( std::string &strFilename )
	{
		//格式化文件名
#ifdef _MSWINDOWS_
		const char cDelimiters = '\\';
#else
		const char cDelimiters = '/';
#endif
		std::string strTemp = strFilename;
		char *pStr = (char*)strTemp.c_str();
		char *p = pStr;
		while(*p != '\0')
		{
			if((*p=='\\') || (*p=='/'))
			{
				*p = cDelimiters;
			}
			p++;
		}
		p = pStr;
		while((p[0]!='\0') && (p[1]!='\0'))
		{
			if((p[0]==cDelimiters) && (p[1]==cDelimiters))
			{
				for(int i=1; p[i]!='\0'; i++)
				{
					p[i] = p[i+1];
				}
			}
			p++;
		}
		strFilename = pStr;
	}

	inline static void FilenameFormatLinux(std::string &strFilename)
	{
		//格式化文件名
		const char cDelimiters =  '/';
		std::string strTemp = strFilename;
		char *pStr = (char*)strTemp.c_str();
		char *p = pStr;;
		while(*p != '\0')
		{
			if((*p=='\\') || (*p=='/'))
			{
				*p = cDelimiters;
			}
			p++;
		}
		p = pStr;
		while((p[0]!='\0') && (p[1] !='\0'))
		{
			if((p[0]==cDelimiters) && (p[1]==cDelimiters))
			{
				for(int i=1; p[i]!='\0'; i++)
				{
					p[i] = p[i+1];
				}
			}
			p++;
		}
		strFilename = pStr;
	}


	//从一个绝对路径中获取运行文件名
	inline static std::string GetFileName(std::string vFullPath)
	{
		std::string::size_type pos = 0;

		pos = vFullPath.find_last_of("/");
		if(pos == std::string::npos)
			pos = vFullPath.find_last_of("\\");

		if(pos == std::string::npos)
			return "";

		return vFullPath.substr(pos+1, vFullPath.length()-pos);
	}

	//从一个绝对路径中获取工作路径
	inline static std::string GetRunPath(std::string vFullPath)
	{
		std::string::size_type pos = 0;

		pos = vFullPath.find_last_of("/");
		if(pos == std::string::npos)
			pos = vFullPath.find_last_of("\\");

		if(pos == std::string::npos)
			return "";

		return vFullPath.substr(0, pos);
	}

	//从一个文件名获取后缀名
	inline static std::string GetExtension(std::string vFullPath)
	{
		std::string::size_type tPos, tDotPos = 0;	//tPos means the position of the lastest '/' or '\'
		tDotPos = vFullPath.find_last_of(".");
		if(std::string::npos == tDotPos)
			return "";

		tPos = vFullPath.find_last_of("/");
		if(tPos == std::string::npos)
			tPos = vFullPath.find_last_of("\\");

		if(tPos>tDotPos && tPos!=std::string::npos)
			return "";

		return vFullPath.substr(tDotPos+1, vFullPath.length()-tDotPos);
	}

	inline static bool IsAbsolutePath(const std::string& vPath)
	{
		return std::string::npos==vPath.find(":")?false:true;
	}

	// 字符串替换
	inline static bool StringReplace(std::string& strMain, const std::string& strFrom, const std::string& strTo)
	{
		if(true == strFrom.empty())
			return false;

		std::string::size_type pos = 0;
		std::string::size_type fromLen = strFrom.size();
		std::string::size_type toLen = strTo.size();

		pos = strMain.find(strFrom, pos);

		while((pos != std::string::npos))
		{
			strMain.replace(pos, fromLen, strTo);
			pos=strMain.find(strFrom, (pos+toLen));
		}

		return true;
	}
	
	inline static std::string WStringToString(std::wstring wstr)
	{
		std::string str;
		
		int nLen = (int)wstr.length();
		str.resize(nLen, ' ');

		WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);

		return str;
	}
	
	inline static std::wstring StringToWString(std::string str)
	{
		std::wstring wstr;
		
		int nLen = (int)str.length();
		wstr.resize(nLen, L' ');

		MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);

		return wstr;
	}
};


#endif //end _GS_H_GSSTRUTIL_H_
