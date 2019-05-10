//---------------------------------------------------------------------------
#include "WinMain.h"
#include "MC_Log.h"

#include <tchar.h>
#include <Windows.h>
#include <vector>

#include "MC_File.h"
//---------------------------------------------------------------------------
template <typename T>
void PrintfList(std::vector<T> vList)
{
	for(std::vector<T>::size_type i=0; i<vList.size(); i++)
	{
		cout << vList[i] <<endl;
	}
}
//---------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	MC_File::ContinueCopy("../3rdData/ContinueCopy.txt", "L:/1.txt");
	//MC_File::ContinueCopy("../3rdData/ContinueCopy.txt", "E:/2.txt");

	system("pause");
	return 0;
}
//---------------------------------------------------------------------------
