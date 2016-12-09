// df-File-operation library.
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

#if (!defined __DF_FILEOP_H)
#define __DF_FILEOP_H

#include <accctrl.h> // For ACCESS_MODE declaration

//RETURN CODE DEFINES
#define DFFILEOP_SUCCESS			0
#define DFFILEOP_FAILURE			1
#define DFFILEOP_USER_ABORT			10

typedef void (CALLBACK DF_FILEOP_ENUM_NOTIFICATION) (LPARAM param, LPARAM data);
typedef DF_FILEOP_ENUM_NOTIFICATION* P_DF_FILEOP_ENUM_NOTIFICATION;

typedef struct tagDFIAVM_VFS_DATA {
	LPCWSTR pwName;
	LPCWSTR	pwSysPath;
} DF_FILEOP_VFS_DATA, *LPDF_FILEOP_VFS_DATA;

//EXPORT FUNCTION DECLARATIONs
/////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

	BOOL __stdcall DF_GetFileExtA(LPCSTR lpFile, LPSTR lpResult, size_t* numberOfElements);
	BOOL __stdcall DF_GetFileExtW(LPCWSTR lpwFile, LPWSTR lpwResult, size_t* numberOfElements);
	BOOL __stdcall DF_RemoveExtA(LPSTR lpFile, size_t numberOfElements);
	BOOL __stdcall DF_RemoveExtW(LPWSTR lpwFile, size_t numberOfElements);
	BOOL __stdcall DF_RemoveFileNameA(LPSTR lpPath, size_t numberOfElements);
	BOOL __stdcall DF_RemoveFileNameW(LPWSTR lpwPath, size_t numberOfElements);
	BOOL __stdcall DF_GetFileNameA(LPCSTR lpPath, LPSTR lpResult, size_t* numberOfElements);
	BOOL __stdcall DF_GetFileNameW(LPCWSTR lpwPath, LPWSTR lpwResult, size_t* numberOfElements);
	BOOL __stdcall DF_FileExistA(LPCSTR lpFile);
	BOOL __stdcall DF_FileExistW(LPCWSTR lpFile);
	BOOL __stdcall DF_DirExistA(LPCSTR lpPath);
	BOOL __stdcall DF_DirExistW(LPCWSTR lpPath);
	BOOL __stdcall DF_IsDirEmptyA(LPCSTR lpPath);
	BOOL __stdcall DF_IsDirEmptyW(LPCWSTR lpPath);
	BOOL __stdcall DF_CreateDirectoryA(LPCSTR folder, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
	BOOL __stdcall DF_CreateDirectoryW(LPCWSTR folder, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
	BOOL __stdcall DF_CopyFileA(LPCSTR source, LPCSTR destination, BOOL bFailIfExists);
	BOOL __stdcall DF_CopyFileW(LPCWSTR source, LPCWSTR destination, BOOL bFailIfExists);
	BOOL __stdcall DF_DeleteFileA(LPCSTR file);
	BOOL __stdcall DF_DeleteFileW(LPCWSTR file);
	BOOL __stdcall DF_DeleteDirectoryA(LPCSTR folder, BOOL forceIfNotEmpty, HWND hParent, LPCSTR progressTitle);
	BOOL __stdcall DF_DeleteDirectoryW(LPCWSTR folder, BOOL forceIfNotEmpty, HWND hParent, LPCWSTR progressTitle);

	UINT __stdcall DF_EnumerateKnownFoldersW(DF_FILEOP_ENUM_NOTIFICATION pEnumCallback, LPARAM pEnumCallbackParam);


	#if (defined _UNICODE)
		#define DF_GetFileExt DF_GetFileExtW
		#define DF_RemoveExt DF_RemoveExtW
		#define DF_RemoveFileName DF_RemoveFileNameW
		#define DF_GetFileName DF_GetFileNameW
		#define DF_FileExist DF_FileExistW
		#define DF_DirExist DF_DirExistW
		#define DF_IsDirEmpty DF_IsDirEmptyW
		#define DF_CreateDirectory DF_CreateDirectoryW
		#define DF_CopyFile DF_CopyFileW
		#define DF_DeleteFile DF_DeleteFileW
		#define DF_DeleteDirectory DF_DeleteDirectoryW
	#else
		#define DF_GetFileExt DF_GetFileExtA
		#define DF_RemoveExt DF_RemoveExtA
		#define DF_RemoveFileName DF_RemoveFileNameA
		#define DF_GetFileName DF_GetFileNameA
		#define DF_FileExist DF_FileExistA
		#define DF_DirExist DF_DirExistA
		#define DF_IsDirEmpty DF_IsDirEmptyA
		#define DF_CreateDirectory DF_CreateDirectoryA
		#define DF_CopyFile DF_CopyFileA
		#define DF_DeleteFile DF_DeleteFileA
		#define DF_DeleteDirectory DF_DeleteDirectoryA
	#endif

#if defined(__cplusplus)
}
#endif
/////////////////////////////////////////////////////////////////////////////

#endif // __DF_FILEOP_H