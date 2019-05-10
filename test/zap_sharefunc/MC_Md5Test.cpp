#include "MC_Md5Test.h"
#include "md5.hpp"
//---------------------------------------------------------------------------
string MC_Md5Test::GetStrMD5(const string vString)
{
	MD5 tMD5;
	tMD5.update(vString);
	return tMD5.toString();
}
//---------------------------------------------------------------------------
string MC_Md5Test::GetFileMD5(const string vFileName)
{
	ifstream tIn(vFileName.c_str(), ios::binary);
	if(!tIn)
		return "";

	MD5 tMD5;
	std::streamsize tLength;
	char tBuffer[1024] = {0};

	while(!tIn.eof())
	{
		tIn.read(tBuffer, sizeof(tBuffer));
		tLength = tIn.gcount();
		if(tLength > 0)
			tMD5.update(tBuffer, tLength);
	}

	tIn.close();
	return tMD5.toString();
}
//---------------------------------------------------------------------------