// FILE: AppManifest.cpp
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

#include "stdafx.h"
#include "IaUwpAppxApp.h"
#include "IaUwpAppx.h"
#include "CAppx.h"

CAppx* pAppx = NULL;

UINT __stdcall UAPPX_Initialize(LPCWSTR lpwPackagename, LPCWSTR lpwPackagepath, bool b64BitPackage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_FAILURE;

	do
	{
		if (!lpwPackagename || !lpwPackagepath)
		{
			result = UAPPX_PARAM_ERROR;
			break;
		}

		UAPPX_Free();
			
		pAppx = new CAppx(lpwPackagename, lpwPackagepath, b64BitPackage);
		if (!pAppx)
		{
			result = UAPPX_OUT_OF_MEMORY_ERROR;
			break;
		}

		result = pAppx->Initialize();

	} while (FALSE);

	return result;
}

void __stdcall UAPPX_Free()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pAppx)
	{
		delete pAppx;
		pAppx = NULL;
	}
}

//////////////////////////////////////////////////////////
UINT __stdcall UAPPX_SetDisplayName(LPCWSTR lpwValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_FAILURE;

	do
	{
		if (!pAppx)
		{
			result = UAPPX_NOT_INITIALIZED;
			break;
		}

		result = pAppx->SetDisplayName(lpwValue);

	} while (FALSE);

	return result;
}

UINT __stdcall UAPPX_SetLanguage(LPCWSTR lpwValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_FAILURE;

	do
	{
		if (!pAppx)
		{
			result = UAPPX_NOT_INITIALIZED;
			break;
		}

		result = pAppx->SetLanguage(lpwValue);

	} while (FALSE);

	return result;
}

UINT __stdcall UAPPX_SetPublisherCertificate(LPCWSTR lpwCertificate, LPCWSTR lpwPassword, LPCWSTR lpwTimestamp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_FAILURE;

	do
	{
		if (!pAppx)
		{
			result = UAPPX_NOT_INITIALIZED;
			break;
		}

		result = pAppx->SetPublisherCertificate(lpwCertificate, lpwPassword, lpwTimestamp);

	} while (FALSE);

	return result;
}

UINT __stdcall UAPPX_SetPublisher(LPCWSTR lpwValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_FAILURE;

	do
	{
		if (!pAppx)
		{
			result = UAPPX_NOT_INITIALIZED;
			break;
		}

		result = pAppx->SetPublisher(lpwValue);

	} while (FALSE);

	return result;
}

UINT __stdcall UAPPX_SetVersion(LPCWSTR lpwValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_FAILURE;

	do
	{
		if (!pAppx)
		{
			result = UAPPX_NOT_INITIALIZED;
			break;
		}

		result = pAppx->SetVersion(lpwValue);

	} while (FALSE);

	return result;
}

//////////////////////////////////////////////////////////
UINT __stdcall UAPPX_SetVirtualRegistryKey(HKEY hKey, LPCWSTR lpwSubKey, DWORD dwType, LPCWSTR lpwValueName, LPVOID lpwValue, DWORD dwSize, bool bWin32)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_SUCCESS;

	do
	{
		if (!pAppx)
		{
			result = UAPPX_NOT_INITIALIZED;
			break;
		}

		CVirtualRegKey key;
		result = key.Set(hKey, lpwSubKey, dwType, lpwValueName, lpwValue, dwSize, bWin32);
		
		if (result != UAPPX_SUCCESS)
			break;

		pAppx->AddVRegKey(key);

	} while (FALSE);

	return result;
}

UINT __stdcall UAPPX_SetPackageFile(LPCWSTR lpwSource, LPCWSTR lpwTarget, size_t* pFileIdx)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_FAILURE;

	do
	{
		if (!pAppx)
		{
			result = UAPPX_NOT_INITIALIZED;
			break;
		}

		result = pAppx->AddFile(lpwSource, lpwTarget, pFileIdx);

	} while (FALSE);

	return result;
}

UINT __stdcall UAPPX_SetPackageDirectory(LPCWSTR lpwTarget, size_t* pFileIdx)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_FAILURE;

	do
	{
		if (!pAppx)
		{
			result = UAPPX_NOT_INITIALIZED;
			break;
		}

		result = pAppx->AddDirectory(lpwTarget, pFileIdx);

	} while (FALSE);

	return result;
}

UINT __stdcall UAPPX_SetFileAssociation(size_t fileIdx, LPCWSTR lpwProgId, LPCWSTR lpwDescription, LPCWSTR** lpwExtensions, size_t extSize, LPCWSTR lpwIconfile, int iconIdx)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_FAILURE;

	do
	{
		if (!pAppx)
		{
			result = UAPPX_NOT_INITIALIZED;
			break;
		}

		result = pAppx->SetFileAssociation(fileIdx, lpwProgId, lpwDescription, lpwExtensions, extSize, lpwIconfile, iconIdx);

	} while (FALSE);

	return result;
}

UINT __stdcall UAPPX_SetShortcut(size_t fileIdx, LPCWSTR lpwName, LPCWSTR lpwDescription, LPCWSTR lpwPath, LPCWSTR lpwCommandline, PCWSTR lpwIconfile, int iconIdx)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_FAILURE;

	do
	{
		if (!pAppx)
		{
			result = UAPPX_NOT_INITIALIZED;
			break;
		}

		// NOTE
		// lpwIconfile - Icon path can be null
		// lpwDescription - Description can be null
		// lpwCommandline - CmdLine can be null

		result = pAppx->AddShortcut(fileIdx, lpwName, lpwDescription, lpwPath, lpwCommandline, lpwIconfile, iconIdx);

	} while (FALSE);

	return result;
}

UINT __stdcall UAPPX_Finalize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_FAILURE;

	do
	{
		if (!pAppx)
		{
			result = UAPPX_NOT_INITIALIZED;
			break;
		}

		result = pAppx->ProcessPackage();

	} while (FALSE);

	return result;
}

UINT __stdcall UAPPX_SetWinSdkBinPath(LPCWSTR lpwPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT result = UAPPX_FAILURE;

	do
	{
		if (!pAppx)
		{
			result = UAPPX_NOT_INITIALIZED;
			break;
		}

		if(pAppx->SetWinSdkBinPath(lpwPath))
		{
			result = UAPPX_SUCCESS;
		}

	} while (FALSE);

	return result;
}