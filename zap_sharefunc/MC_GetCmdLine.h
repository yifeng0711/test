#pragma once
//------------------------------------------------------------------
#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;
//------------------------------------------------------------------
// NtQueryInformationProcess for pure 32 and 64-bit processes
typedef NTSTATUS (NTAPI *_NtQueryInformationProcess)(
	IN HANDLE ProcessHandle,
	ULONG ProcessInformationClass,
	OUT PVOID ProcessInformation,
	IN ULONG ProcessInformationLength,
	OUT PULONG ReturnLength OPTIONAL
	);

typedef NTSTATUS (NTAPI *_NtReadVirtualMemory)(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress,
	OUT PVOID Buffer,
	IN SIZE_T Size,
	OUT PSIZE_T NumberOfBytesRead);

// NtQueryInformationProcess for 32-bit process on WOW64
typedef NTSTATUS (NTAPI *_NtWow64ReadVirtualMemory64)(
	IN HANDLE ProcessHandle,
	IN PVOID64 BaseAddress,
	OUT PVOID Buffer,
	IN ULONG64 Size,
	OUT PULONG64 NumberOfBytesRead);

// PROCESS_BASIC_INFORMATION for pure 32 and 64-bit processes
typedef struct _PROCESS_BASIC_INFORMATION {
	PVOID Reserved1;
	PVOID PebBaseAddress;
	PVOID Reserved2[2];
	ULONG_PTR UniqueProcessId;
	PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;

// PROCESS_BASIC_INFORMATION for 32-bit process on WOW64
// The definition is quite funky, as we just lazily doubled sizes to match offsets...
typedef struct _PROCESS_BASIC_INFORMATION_WOW64 {
	PVOID Reserved1[2];
	PVOID64 PebBaseAddress;
	PVOID Reserved2[4];
	ULONG_PTR UniqueProcessId[2];
	PVOID Reserved3[2];
} PROCESS_BASIC_INFORMATION_WOW64;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;

typedef struct _UNICODE_STRING_WOW64 {
	USHORT Length;
	USHORT MaximumLength;
	PVOID64 Buffer;
} UNICODE_STRING_WOW64;
//------------------------------------------------------------------
class MC_GetCmdLine
{
public:
	static	bool		GetProIdByName		(std::string vProcessName, std::vector<DWORD>& vProIdList);

	/**
	 * @Method		:	GetCmdLineByProId
	 * @brief		:	获取进程在任务管理器中的【命令行】
	 * @param[in]	:	vProcessId	进程ID
	 * @return		:	
	 */
	static	std::string GetCmdLineByProId	(DWORD vProcessId);
	static	std::string UnicodeToAnsii		(const std::wstring& ws);
};
//------------------------------------------------------------------
