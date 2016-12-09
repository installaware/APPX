// FILE: utils.h
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

#include "VfsPath.h"
#include <Aclapi.h>

static CStdStringW g_GetGuId()
{
	CStdStringW sResult(_T(""));
	RPC_WSTR guidStr = NULL;

	GUID guid;
	HRESULT hr = CoCreateGuid(&guid);
	// Convert the GUID to a string 
	UuidToStringW(&guid, &guidStr);

	sResult.Format(_T("%s"), guidStr);
	RpcStringFreeW(&guidStr);

	return sResult;
}

static CStdStringW g_GetTempDirectory()
{
	CStdStringW result = _T("");

	wchar_t wcharPath[MAX_PATH];
	if (::GetTempPathW(MAX_PATH, wcharPath))
		result = wcharPath;

	return result;
}

static bool g_StringBegins_with(CStdString input, CStdString match)
{
	input = input.ToLower();
	match = match.ToLower();

	return input.size() >= match.size()
		&& equal(match.begin(), match.end(), input.begin());
}

static void g_StdStrSplit(CStdString str, WCHAR ch, std::vector<CStdString>& results)
{
	size_t cutAt;
	while ((cutAt = str.find_first_of(ch)) != str.npos)
	{
		if (cutAt > 0)
		{
			results.push_back(str.substr(0, cutAt));
		}
		str = str.substr(cutAt + 1);
	}

	if (str.length() > 0)
	{
		results.push_back(str);
	}
}

template <typename T>
static void g_VectorRemoveElement(std::vector<T>& vec, size_t pos)
{
	std::vector<T>::iterator it = vec.begin();
	std::advance(it, pos);
	vec.erase(it);
}

static BOOL g_SetRegKeyPermission(LPCTSTR lpszKey)
{
	BOOL result = TRUE;

	DWORD dwRes = 0;
	DWORD dwDisposition = 0;
	PSID pEveryoneSID = NULL, pAdminSID = NULL;
	PACL pACL = NULL;
	PSECURITY_DESCRIPTOR pSD = NULL;
	EXPLICIT_ACCESS ea[2];
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
	SECURITY_ATTRIBUTES sa;
	HKEY hkSub = NULL;

	// Create a well-known SID for the Everyone group.
	if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID))
	{
		result = FALSE;
		goto Cleanup;
	}

	// Initialize an EXPLICIT_ACCESS structure for an ACE.
	// The ACE will allow Everyone read access to the key.
	ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));
	ea[0].grfAccessPermissions = KEY_READ;
	ea[0].grfAccessMode = SET_ACCESS;
	ea[0].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
	ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ea[0].Trustee.ptstrName = (LPTSTR)pEveryoneSID;

	// Create a SID for the BUILTIN\Administrators group.
	if (!AllocateAndInitializeSid(&SIDAuthNT, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdminSID))
	{
		result = FALSE;
		goto Cleanup;
	}

	// Initialize an EXPLICIT_ACCESS structure for an ACE.
	// The ACE will allow the Administrators group full access to
	// the key.
	ea[1].grfAccessPermissions = KEY_ALL_ACCESS;
	ea[1].grfAccessMode = SET_ACCESS;
	ea[1].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
	ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
	ea[1].Trustee.ptstrName = (LPTSTR)pAdminSID;

	// Create a new ACL that contains the new ACEs.
	dwRes = SetEntriesInAcl(2, ea, NULL, &pACL);
	if (ERROR_SUCCESS != dwRes)
	{
		result = FALSE;
		goto Cleanup;
	}

	// Initialize a security descriptor.  
	pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (NULL == pSD)
	{
		result = FALSE;
		goto Cleanup;
	}

	if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
	{
		result = FALSE;
		goto Cleanup;
	}

	// Add the ACL to the security descriptor. 
	if (!SetSecurityDescriptorDacl(pSD,
		TRUE,     // bDaclPresent flag   
		pACL,
		FALSE))   // not a default DACL 
	{
		result = FALSE;
		goto Cleanup;
	}

	// Initialize a security attributes structure.
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = FALSE;

	// Use the security attributes to set the security descriptor 
	// when you create a key.
	LONG lRes = RegCreateKeyEx(HKEY_CURRENT_USER, lpszKey, 0, _T(""), 0, KEY_READ | KEY_WRITE, &sa, &hkSub, &dwDisposition);

	if(lRes == ERROR_SUCCESS)
	{
		result = TRUE;
	}
	
Cleanup:

	if (pEveryoneSID)
		FreeSid(pEveryoneSID);
	if (pAdminSID)
		FreeSid(pAdminSID);
	if (pACL)
		LocalFree(pACL);
	if (pSD)
		LocalFree(pSD);
	if (hkSub)
		RegCloseKey(hkSub);

	return result;
}

static bool g_ExtractModuleResource(UINT uResourceID, LPCWSTR lpType, LPCWSTR lpDestination)
{
	bool result = false;

	if (!lpDestination || !lpType)
		return result;

	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwResourceSize = 0;
	LPVOID lpResource = NULL;

	do
	{

#if (defined _WIN64)
		HMODULE hModule = ::GetModuleHandle(_T("IaUwpAppx64.dll"));
#else
		HMODULE hModule = ::GetModuleHandle(_T("IaUwpAppx.dll"));
#endif

		if (!hModule)
			break;

		// Find the binary file in resources
		HRSRC hRsrc = ::FindResource(hModule, MAKEINTRESOURCE(uResourceID), lpType);

		if (!hRsrc)
			break;

		HGLOBAL hResource = ::LoadResource(hModule, hRsrc);
		if (!hResource)
			break;

		lpResource = ::LockResource(hResource);

		if (lpResource)
		{
			dwResourceSize = ::SizeofResource(hModule, hRsrc);
		}

		if (!lpResource)
			break;

		// Copy extracted binary from resources to destination
		hFile = ::CreateFile(lpDestination, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
			return result;

		result = (::WriteFile(hFile, lpResource, dwResourceSize, NULL, NULL) != 0);
		DWORD dwError = GetLastError();

	} while (FALSE);

	
	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle(hFile);

	return result;
}

static DWORD g_StartProcess(LPWSTR szCmdLine)
{
	DWORD result = 0;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		szCmdLine,			// Command line
		NULL,				// Process handle not inheritable
		NULL,				// Thread handle not inheritable
		FALSE,				// Set handle inheritance to FALSE
		CREATE_NO_WINDOW,   // No creation flags
		NULL,				// Use parent's environment block
		NULL,				// Use parent's starting directory 
		&si,				// Pointer to STARTUPINFO structure
		&pi)				// Pointer to PROCESS_INFORMATION structure
		)
	{
		result = GetLastError();
		return result;
	}

	// Wait until child process exits.
	DWORD dwExitCode = -1;
	DWORD dwWaitResult = WaitForSingleObject(pi.hProcess, INFINITE);

	if (dwWaitResult == WAIT_OBJECT_0)
	{
		GetExitCodeProcess(pi.hProcess, &dwExitCode);
		result = dwExitCode;
	}
	else
	{
		result = GetLastError();
		//failure
	}

	// Close process and thread handles. 
	if (pi.hProcess)
		CloseHandle(pi.hProcess);
	if (pi.hThread)
		CloseHandle(pi.hThread);

	return result;
}

static DWORD g_GetCertificateSubject(LPCWSTR pwPfxFile, LPCWSTR pwPfxPassword, LPWSTR& pwSubject)
{
	if ((!pwPfxFile || !pwPfxPassword) && pwSubject)
		return ERROR_INVALID_PARAMETER;

	DWORD result = ERROR_SUCCESS;

	HANDLE hCertFile = INVALID_HANDLE_VALUE;
	CRYPT_DATA_BLOB pfxBlob = { 0 };
	HCERTSTORE hPfxStore = NULL;
	PCCERT_CONTEXT pcCert = NULL;

	do
	{
		hCertFile = CreateFile(pwPfxFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

		if (hCertFile == INVALID_HANDLE_VALUE)
		{
			result = GetLastError();
			break;
		}

		DWORD dwFileSize = GetFileSize(hCertFile, NULL);

		CRYPT_DATA_BLOB pfxBlob;
		pfxBlob.pbData = (PBYTE)CryptMemAlloc(dwFileSize * sizeof(BYTE));

		if (!pfxBlob.pbData)
		{
			result = ERROR_INVALID_HANDLE;
			break;
		}

		pfxBlob.cbData = dwFileSize;

		DWORD cbRead = NULL;
		ReadFile(hCertFile, pfxBlob.pbData, pfxBlob.cbData, &cbRead, NULL);

		CloseHandle(hCertFile);
		hCertFile = INVALID_HANDLE_VALUE;

		hPfxStore = PFXImportCertStore(&pfxBlob, pwPfxPassword, CRYPT_EXPORTABLE | CRYPT_USER_KEYSET);
		if (!hPfxStore)
		{
			result = GetLastError();
			break;
		}

		CryptMemFree(pfxBlob.pbData);
		pfxBlob.pbData = NULL;

		pcCert = CertFindCertificateInStore(hPfxStore, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, 0, CERT_FIND_ANY, NULL, NULL);

		if (!pcCert)
		{
			result = GetLastError();
			break;
		}

		// subject
		DWORD cbSize = CertNameToStr(X509_ASN_ENCODING, &pcCert->pCertInfo->Subject, CERT_X500_NAME_STR, NULL, 0);

		if (cbSize <= 1)
		{
			result = ERROR_INVALID_DATA;
			break;
		}

		pwSubject = new wchar_t[cbSize];

		if (!pwSubject)
		{
			result = ERROR_OUTOFMEMORY;
			break;
		}

		if (!CertNameToStr(X509_ASN_ENCODING, &pcCert->pCertInfo->Subject, CERT_X500_NAME_STR, pwSubject, cbSize))
		{
			delete[] pwSubject;
			pwSubject = NULL;
		}

	} while (FALSE);

	if (hCertFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hCertFile);
	}

	if (pfxBlob.pbData)
	{
		CryptMemFree(pfxBlob.pbData);
	}

	if (pcCert)
	{
		CertFreeCertificateContext(pcCert);
	}

	if (hPfxStore)
	{
		if (!CertCloseStore(hPfxStore, CERT_CLOSE_STORE_FORCE_FLAG))
		{
			DWORD dw = GetLastError();
		}
	}

	return result;
}