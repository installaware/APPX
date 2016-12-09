// FILE: IaUwpAppx.h main header file for the IaUwpAppx DLL
//
// InstallAware APPX Library for Microsoft's Desktop Bridge
// Copyright(C) 2016 InstallAware Software
//
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
// 
// Contact InstallAware Software at support@installaware.com.

//	REVISION HISTORY
//
//	  2016-DEC-08 - Version 1.0.2.301 - First Open Source release.
//					
//					
//

#pragma once

#if (!defined __IA_UWPAPPX_H)
#define __IA_UWPAPPX_H

//	RETURN CODES
#define UAPPX_SUCCESS								0
#define UAPPX_FAILURE								100
#define UAPPX_NOT_INITIALIZED						101
#define UAPPX_PARAM_ERROR							102
#define UAPPX_OUT_OF_MEMORY_ERROR					103
#define UAPPX_FILE_ACCESS_ERROR						105
#define UAPPX_UNEXPECTED_MODULE_RESOURCE_ERROR		106
#define UAPPX_CERTIFICATE_ERROR						107
#define UAPPX_CERTIFICATE_INVALID_PASSWORD			108
#define UAPPX_CERTIFICATE_FILE_ACCESS_ERROR			109
#define UAPPX_CERTIFICATE_FILE_NOT_FOUND			110

#define UAPPX_UNSUPPORTED_REGISTRY_VALUE			115

#define UAPPX_REG_DAT_ERROR							120
#define UAPPX_REG_DAT_SECURITY_DESCRIPTOR_ERROR		121
#define UAPPX_REG_DAT_PRIVILEGE_ERROR				122

#define UAPPX_SYS_PATHS_ENUM_ERROR					140

#define UAPPX_SOURCE_FILE_NOT_FOUND					160
#define UAPPX_VFS_PATH_NOT_RESOLVED					161
#define UAPPX_VFSFILE_COPY_ERROR					162
#define UAPPX_VFSASSETS_ERROR						163

#define UAPPX_CREATE_CONTAINER_ERROR				180

#define UAPPX_NO_SHORTCUT_DEFINED					185

#define UAPPX_CREATE_MANIFEST_ERROR					200
#define UAPPX_INVALID_MANIFEST_VALUE				201

#define UAPPX_SIGN_ERROR							210


//EXPORT FUNCTION DECLARATIONs
/////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

	UINT __stdcall UAPPX_Initialize(LPCWSTR lpwPackagename, LPCWSTR lpwPath, bool b64BitPackage);
	UINT __stdcall UAPPX_Finalize();
	void __stdcall UAPPX_Free();

	UINT __stdcall UAPPX_SetDisplayName(LPCWSTR lpwValue);

	// UAPPX_SetFileAssociation Notes!
	// With the current implementation only the prameters; "fileIdx", "lpwExtensions" and "extSize" are used.
	// The remaining parameters are not used by the library and these are present for "a possible" implementation in successive versions.
	UINT __stdcall UAPPX_SetFileAssociation(size_t fileIdx, LPCWSTR lpwProgId, LPCWSTR lpwDescription, LPCWSTR** lpwExtensions, size_t extSize, LPCWSTR lpwIconfile, int iconIdx);

	UINT __stdcall UAPPX_SetLanguage(LPCWSTR lpwValue);
	
	UINT __stdcall UAPPX_SetPackageFile(LPCWSTR lpwSource, LPCWSTR lpwTarget, size_t* pFileIdx);
	UINT __stdcall UAPPX_SetPackageDirectory(LPCWSTR lpwTarget, size_t* pFileIdx);

	UINT __stdcall UAPPX_SetPublisher(LPCWSTR lpwValue);
	UINT __stdcall UAPPX_SetPublisherCertificate(LPCWSTR lpwCertificate, LPCWSTR lpwPassword, LPCWSTR lpwTimestamp);

	// UAPPX_SetShortcut Notes!
	// Parameters; lpwDescription, lpwPath, pwCommandline and lpwIconfile can be NULL.
	//
	// With the current implementation only the prameters; "fileIdx", "lpwName" and "lpwDescription" are used.
	// The remaining parameters are not used by the library and these are present for "a possible" implementation in successive versions.
	UINT __stdcall UAPPX_SetShortcut(size_t fileIdx, LPCWSTR lpwName, LPCWSTR lpwDescription, LPCWSTR lpwPath, LPCWSTR lpwCommandline, PCWSTR lpwIconfile, int iconIdx);

	UINT __stdcall UAPPX_SetVersion(LPCWSTR lpwValue);
	UINT __stdcall UAPPX_SetVirtualRegistryKey(HKEY hKey, LPCWSTR lpwSubKey, DWORD dwType, LPCWSTR lpwValueName, LPVOID lpValue, DWORD dwSize, bool bWin32);
	
	UINT __stdcall UAPPX_SetWinSdkBinPath(LPCWSTR lpwPath);

#if defined(__cplusplus)
}
#endif
/////////////////////////////////////////////////////////////////////////////

#endif // __IA_UWPAPPX_H
