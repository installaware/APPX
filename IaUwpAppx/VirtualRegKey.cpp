// FILE: VirtualRegKey.cpp
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
#include "VirtualRegKey.h"

bool operator== (const CVirtualRegKey &v1, const CVirtualRegKey &v2)
{
	return (v1.m_hKeyRoot == v2.m_hKeyRoot &&
		v1.m_subKey == v2.m_subKey &&
		v1.m_dwType == v2.m_dwType &&
		v1.m_sValueName == v2.m_sValueName &&
		v1.m_binarySize == v2.m_binarySize &&
		v1.m_dwValue == v2.m_dwValue &&
		v1.m_sValue == v2.m_sValue &&
		v1.m_bWin32 == v2.m_bWin32 &&
		v1.m_unSupported == v2.m_unSupported);
}

bool operator!= (CVirtualRegKey &v1, CVirtualRegKey &v2)
{
	return !(v1 == v2);
}

CVirtualRegKey::CVirtualRegKey()
{
	m_hKeyRoot = 0;
	m_subKey = _T("");
	m_dwType = 0;
	m_sValueName = _T("");
	m_pBinaryValue = NULL;
	m_binarySize = 0;
	m_dwValue = 0 ;
	m_sValue = _T("");
	m_bWin32 = false;
	m_unSupported = false;
}

CVirtualRegKey::~CVirtualRegKey()
{

}

UINT CVirtualRegKey::Set(HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpValueName, LPVOID lpValue, DWORD dwSize, bool bWin32)
{
	UINT result = UAPPX_SUCCESS;

	if (!lpSubKey || dwType == 0 || !lpValueName || !lpValue)
	{
		return UAPPX_PARAM_ERROR;
	}

	if (m_dwType == REG_BINARY && dwSize == 0)
	{
		return UAPPX_PARAM_ERROR;
	}

	m_hKeyRoot = hKey;
	m_subKey = lpSubKey;
	m_dwType = dwType;
	m_sValueName = lpValueName;
	m_bWin32 = bWin32;

	if(hKey == HKEY_CURRENT_USER || hKey == HKEY_CURRENT_CONFIG)
	{
		m_subKey.Format(_T("REGISTRY\\USER\\%s"), m_subKey);
	}
	else if (hKey == HKEY_LOCAL_MACHINE || HKEY_CLASSES_ROOT)
	{
		m_subKey.Format(_T("REGISTRY\\MACHINE\\%s"), m_subKey);
	}
	else
	{
		// Not handled!
		m_unSupported = true;
	}
	
	switch (m_dwType)
	{
		case REG_BINARY:
			m_pBinaryValue = new BYTE [dwSize];

			if (m_pBinaryValue)
			{
				errno_t err = memcpy_s(m_pBinaryValue, dwSize, lpValue, dwSize);

				if (err)
				{
					delete[] m_pBinaryValue;
					m_pBinaryValue = NULL;
					
					result = UAPPX_OUT_OF_MEMORY_ERROR;
				}
				else
				{
					m_binarySize = dwSize;
				}
			}
			else
			{
				result = UAPPX_OUT_OF_MEMORY_ERROR;
			}
			break;

		case REG_DWORD:
			m_dwValue = *(LPDWORD)lpValue;
			break;

		case REG_QWORD:
			// Not handled!
			result = UAPPX_UNSUPPORTED_REGISTRY_VALUE;
			break;

		case REG_SZ:
		case REG_EXPAND_SZ:
		case REG_LINK:
		case REG_MULTI_SZ:
			m_sValue = (LPCWSTR)lpValue;
			break;

		default:
			result = UAPPX_UNSUPPORTED_REGISTRY_VALUE;
			break;
	}

	return result;
}

LPVOID CVirtualRegKey::GetValue(DWORD& dwType)
{
	LPVOID lpValue = NULL;

	dwType = m_dwType;

	switch (m_dwType)
	{
		case REG_BINARY:
			lpValue = (LPVOID)&m_pBinaryValue;
			break;

		case REG_DWORD:
			lpValue = (LPVOID)&m_dwValue;
			break;

		//case REG_QWORD:
		//	// Not handled!
		//	result = false;
		//	break;

		case REG_SZ:
		case REG_EXPAND_SZ:
		case REG_LINK:
		case REG_MULTI_SZ:
			lpValue = (LPVOID)&m_sValue;
			break;

		default:
			break;
	}

	return lpValue;
}