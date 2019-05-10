#include "MC_GetCmdLine.h"
#include "MC_Log.h"
#include <Tlhelp32.h>
//------------------------------------------------------------------
std::string MC_GetCmdLine::GetCmdLineByProId(DWORD vProcessId)
{
	// open the process
	HANDLE tProHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, vProcessId);
	DWORD tError = 0;
	if (tProHandle == NULL)
	{
		LOG_ERROR("OpenProcess %u fail, errno: %d", vProcessId, GetLastError());
		return "";
	}

	// determine if 64 or 32-bit processor
	SYSTEM_INFO tSystemInfo;
	GetNativeSystemInfo(&tSystemInfo);

	// determine if this process is running on WOW64
	BOOL tWow64Flag;
	IsWow64Process(GetCurrentProcess(), &tWow64Flag);

	// use WinDbg "dt ntdll!_PEB" command and search for ProcessParameters offset to find the truth out
	DWORD tProcessParametersOffset	= tSystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ? 0x20 : 0x10;
	DWORD tCommandLineOffset		= tSystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ? 0x70 : 0x40;

	// func_Read basic info to get ProcessParameters address, we only need the beginning of PEB
	DWORD tPebSize	= tProcessParametersOffset + 8;
	PBYTE tPeb		= (PBYTE)malloc(tPebSize);
	ZeroMemory(tPeb, tPebSize);

	// func_Read basic info to get CommandLine address, we only need the beginning of ProcessParameters
	DWORD tProParameterSize	= tCommandLineOffset + 16;
	PBYTE tProParameter		= (PBYTE)malloc(tProParameterSize);
	ZeroMemory(tProParameter, tProParameterSize);

	PWSTR tCmdLine;
	if (tWow64Flag)
	{
		// we're running as a 32-bit process in a 64-bit OS
		PROCESS_BASIC_INFORMATION_WOW64 tProBasicInfo;
		ZeroMemory(&tProBasicInfo, sizeof(tProBasicInfo));

		// get process information from 64-bit world
		_NtQueryInformationProcess func_Query = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWow64QueryInformationProcess64");
		tError = func_Query(tProHandle, 0, &tProBasicInfo, sizeof(tProBasicInfo), NULL);
		if (0 != tError)
		{
			LOG_ERROR("NtWow64QueryInformationProcess64 fail, errno: %d", GetLastError());
			CloseHandle(tProHandle);
			return "";
		}

		// func_Read PEB from 64-bit address space
		_NtWow64ReadVirtualMemory64 func_Read = (_NtWow64ReadVirtualMemory64)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWow64ReadVirtualMemory64");
		tError = func_Read(tProHandle, tProBasicInfo.PebBaseAddress, tPeb, tPebSize, NULL);
		if (0 != tError)
		{
			LOG_ERROR("NtWow64ReadVirtualMemory64 PEB fail, errno: %d", GetLastError());
			CloseHandle(tProHandle);
			return "";
		}

		// func_Read ProcessParameters from 64-bit address space
		PBYTE* tParameters = (PBYTE*)*(LPVOID*)(tPeb + tProcessParametersOffset); // address in remote process adress space
		tError = func_Read(tProHandle, tParameters, tProParameter, tProParameterSize, NULL);
		if (0 != tError)
		{
			LOG_ERROR("NtWow64ReadVirtualMemory64 Parameters fail, errno: %d", GetLastError());
			CloseHandle(tProHandle);
			return "";
		}

		// func_Read CommandLine
		UNICODE_STRING_WOW64* tCommandLine = (UNICODE_STRING_WOW64*)(tProParameter + tCommandLineOffset);
		tCmdLine = (PWSTR)malloc(tCommandLine->MaximumLength);
		tError = func_Read(tProHandle, tCommandLine->Buffer, tCmdLine, tCommandLine->MaximumLength, NULL);
		if (0 != tError)
		{
			LOG_ERROR("NtWow64ReadVirtualMemory64 Parameters fail, errno: %d", GetLastError());
			CloseHandle(tProHandle);
			return "";
		}
	}
	else
	{
		// we're running as a 32-bit process in a 32-bit OS, or as a 64-bit process in a 64-bit OS
		PROCESS_BASIC_INFORMATION tProBasicInfo;
		ZeroMemory(&tProBasicInfo, sizeof(tProBasicInfo));

		// get process information
		_NtQueryInformationProcess func_Query = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
		tError = func_Query(tProHandle, 0, &tProBasicInfo, sizeof(tProBasicInfo), NULL);
		if (0 != tError)
		{
			LOG_ERROR("NtQueryInformationProcess fail, errno: %d", GetLastError());
			CloseHandle(tProHandle);
			return "";
		}

		// func_Read PEB
		if (!ReadProcessMemory(tProHandle, tProBasicInfo.PebBaseAddress, tPeb, tPebSize, NULL))
		{
			LOG_ERROR("ReadProcessMemory PEB fail, errno: %d", GetLastError());
			CloseHandle(tProHandle);
			return "";
		}

		// func_Read ProcessParameters
		PBYTE* tParameters = (PBYTE*)*(LPVOID*)(tPeb + tProcessParametersOffset); // address in remote process adress space
		if (!ReadProcessMemory(tProHandle, tParameters, tProParameter, tProParameterSize, NULL))
		{
			LOG_ERROR("ReadProcessMemory Parameters fail, errno: %d", GetLastError());
			CloseHandle(tProHandle);
			return "";
		}

		// func_Read CommandLine
		UNICODE_STRING* tCommandLine = (UNICODE_STRING*)(tProParameter + tCommandLineOffset);
		tCmdLine = (PWSTR)malloc(tCommandLine->MaximumLength);
		if (!ReadProcessMemory(tProHandle, tCommandLine->Buffer, tCmdLine, tCommandLine->MaximumLength, NULL))
		{
			LOG_ERROR("ReadProcessMemory Parameters fail, errno: %d", GetLastError());
			CloseHandle(tProHandle);
			return "";
		}
	}

	return UnicodeToAnsii(tCmdLine);
}
//------------------------------------------------------------------
std::string MC_GetCmdLine::UnicodeToAnsii(const std::wstring& ws)
{
	int nAnsiiLen=WideCharToMultiByte(CP_ACP,0,ws.c_str(),-1,NULL,0,NULL,NULL);
	char* pchAnsii=new char[nAnsiiLen+1];
	WideCharToMultiByte(CP_ACP,0,ws.c_str(),-1,pchAnsii,nAnsiiLen,NULL,NULL);
	std::string s=pchAnsii;
	delete [] pchAnsii;
	return s;
}
//---------------------------------------------------------------------------
