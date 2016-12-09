// df-registry library.
// Copyright (C) 1997-2016 the Digital Farm® Arezzo - Italy.
// All rights reserved.
//

// This software is protected by copyright laws and international copyright treaties,
// as well as other intellectual property laws.
// The unauthorized copy of this software, the non-compliance with the above rules and
// of those described in the software license agreement, will be prosecuted by law.

// For conditions of use, license terms and distribution, please contact:
// www.digitalfarm.it email : info@digitalfarm.it
//
// The Digital Farm and logo are registered trademarks.
//
// Lib version: 4.0.0.1


#pragma once

#if (!defined __DF_REGISTRY_EXPORT)
#define __DF_REGISTRY_EXPORT

//EXPORT FUNCTION DECLARATIONs
/////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

	HKEY __stdcall DFREG_OpenW(HKEY hKey, LPCWSTR pszSubKey, DWORD dwRights);
	void __stdcall DFREG_CloseW(HKEY hKey);
	HKEY __stdcall DFREG_ConnectW(HKEY hKey, LPCWSTR pszRemote);
	HKEY __stdcall DFREG_CreateW(HKEY hKey, LPCWSTR pszSubKey, DWORD dwRights);

	bool __stdcall DFREG_LoadStringW(HKEY hKey, LPCWSTR pszSubKey, LPCWSTR pszValName, LPWSTR pszString, DWORD dwLength);
	bool __stdcall DFREG_LoadNewStringW(HKEY hKey, LPCWSTR pszSubKey, LPCWSTR pszValName, LPWSTR* pszString, DWORD *pdwLength);
	bool __stdcall DFREG_SaveStringW(HKEY hKey, LPCWSTR pszSubKey, LPCWSTR pszValName, LPCWSTR pszString);
	bool __stdcall DFREG_SaveExpandedStringW(HKEY hKey, LPCWSTR pszSubKey, LPCWSTR pszValName, LPCWSTR pszString);
	bool __stdcall DFREG_SaveMultiStringW(HKEY hKey, LPCWSTR pszSubKey, LPCWSTR pszValName, LPCWSTR pszString);

	bool __stdcall DFREG_LoadNumberW(HKEY hKey, LPCWSTR pszSubKey, LPCWSTR pszValName, DWORD *pdwNumber);
	bool __stdcall DFREG_SaveNumberW(HKEY hKey, LPCWSTR pszSubKey, LPCWSTR pszValName, DWORD dwNumber);

	bool __stdcall DFREG_LoadBinaryW(HKEY hKey, LPCWSTR pszSubKey, LPCWSTR pszValName, LPBYTE pbyteData, DWORD dwSize);
	bool __stdcall DFREG_LoadNewBinaryW(HKEY hKey, LPCWSTR pszSubKey, LPCWSTR pszValName, LPBYTE *pbyteData, DWORD *pdwSize);
	bool __stdcall DFREG_SaveBinaryW(HKEY hKey, LPCWSTR pszSubKey, LPCWSTR pszValName, const LPBYTE pbyteData, DWORD dwSize);

	bool __stdcall DFREG_DeletelKeyW(HKEY hKey, LPCWSTR pszSubKey, LPCWSTR pszValName);
	bool __stdcall DFREG_DeleteSubKeysW(HKEY hKey);

	LONG __stdcall DFREG_ExportKeyW(HKEY hKey, LPCWSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes);

	// ANSI implementation
	HKEY __stdcall DFREG_OpenA(HKEY hKey, LPCSTR pszSubKey, DWORD dwRights);
	void __stdcall DFREG_CloseA(HKEY hKey);
	HKEY __stdcall DFREG_ConnectA(HKEY hKey, LPCSTR pszRemote);
	HKEY __stdcall DFREG_CreateA(HKEY hKey, LPCSTR pszSubKey, DWORD dwRights);

	bool __stdcall DFREG_LoadStringA(HKEY hKey, LPCSTR pszSubKey, LPCSTR pszValName, LPSTR pszString, DWORD dwLength);
	bool __stdcall DFREG_LoadNewStringA(HKEY hKey, LPCSTR pszSubKey, LPCSTR pszValName, LPSTR* pszString, DWORD *pdwLength);
	bool __stdcall DFREG_SaveStringA(HKEY hKey, LPCSTR pszSubKey, LPCSTR pszValName, LPCSTR pszString);
	bool __stdcall DFREG_SaveExpandedStringA(HKEY hKey, LPCSTR pszSubKey, LPCSTR pszValName, LPCSTR pszString);
	bool __stdcall DFREG_SaveMultiStringA(HKEY hKey, LPCSTR pszSubKey, LPCSTR pszValName, LPCSTR pszString);

	bool __stdcall DFREG_LoadNumberA(HKEY hKey, LPCSTR pszSubKey, LPCSTR pszValName, DWORD *pdwNumber);
	bool __stdcall DFREG_SaveNumberA(HKEY hKey, LPCSTR pszSubKey, LPCSTR pszValName, DWORD dwNumber);

	bool __stdcall DFREG_LoadBinaryA(HKEY hKey, LPCSTR pszSubKey, LPCSTR pszValName, LPBYTE pbyteData, DWORD dwSize);
	bool __stdcall DFREG_LoadNewBinaryA(HKEY hKey, LPCSTR pszSubKey, LPCSTR pszValName, LPBYTE *pbyteData, DWORD *pdwSize);
	bool __stdcall DFREG_SaveBinaryA(HKEY hKey, LPCSTR pszSubKey, LPCSTR pszValName, const LPBYTE pbyteData, DWORD dwSize);

	bool __stdcall DFREG_DeletelKeyA(HKEY hKey, LPCSTR pszSubKey, LPCSTR pszValName);
	bool __stdcall DFREG_DeleteSubKeysA(HKEY hKey);

	LONG __stdcall DFREG_ExportKeyA(HKEY hKey, LPCSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes);

	#if (defined _UNICODE)
		#define DFREG_Open DFREG_OpenW
		#define DFREG_Close DFREG_CloseW
		#define DFREG_Connect DFREG_ConnectW
		#define DFREG_Create DFREG_CreateW

		#define DFREG_LoadString DFREG_LoadStringW
		#define DFREG_LoadNewString DFREG_LoadNewStringW
		#define DFREG_SaveString DFREG_SaveStringW
		#define DFREG_SaveExpandedString DFREG_SaveExpandedStringW
		#define DFREG_SaveMultiString DFREG_SaveMultiStringW

		#define DFREG_LoadNumber DFREG_LoadNumberW
		#define DFREG_SaveNumber DFREG_SaveNumberW

		#define DFREG_LoadBinary DFREG_LoadBinaryW
		#define DFREG_LoadNewBinary DFREG_LoadNewBinaryW
		#define DFREG_SaveBinary DFREG_SaveBinaryW

		#define DFREG_DeletelKey DFREG_DeletelKeyW
		#define DFREG_DeleteSubKeys DFREG_DeleteSubKeysW
		#define DFREG_ExportKey DFREG_ExportKeyW
	#else
		#define DFREG_Open DFREG_OpenA
		#define DFREG_Close DFREG_CloseA
		#define DFREG_Connect DFREG_ConnectA
		#define DFREG_Create DFREG_CreateA

		#define DFREG_LoadString DFREG_LoadStringA
		#define DFREG_LoadNewString DFREG_LoadNewStringA
		#define DFREG_SaveString DFREG_SaveStringA
		#define DFREG_SaveExpandedString DFREG_SaveExpandedStringA
		#define DFREG_SaveMultiString DFREG_SaveMultiStringA

		#define DFREG_LoadNumber DFREG_LoadNumberA
		#define DFREG_SaveNumber DFREG_SaveNumberA

		#define DFREG_LoadBinary DFREG_LoadBinaryA
		#define DFREG_LoadNewBinary DFREG_LoadNewBinaryA
		#define DFREG_SaveBinary DFREG_SaveBinaryA

		#define DFREG_DeletelKey DFREG_DeletelKeyA
		#define DFREG_DeleteSubKeys DFREG_DeleteSubKeysA
		#define DFREG_ExportKey DFREG_ExportKeyA
	#endif

#if defined(__cplusplus)
}
#endif
/////////////////////////////////////////////////////////////////////////////


#endif // __DF_REGISTRY_EXPORT
