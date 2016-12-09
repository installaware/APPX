// FILE: VirtualRegKey.h
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

class CVirtualRegKey
{
public:
	CVirtualRegKey();
	
	~CVirtualRegKey();

	void Clear()
	{
		if (m_pBinaryValue)
		{
			delete[] m_pBinaryValue;
			m_pBinaryValue = NULL;

			m_binarySize = 0;
		}

		m_hKeyRoot = 0;
		m_subKey = _T("");
		m_dwType = 0;
		m_sValueName = _T("");
		m_binarySize = 0;
		m_dwValue = 0;
		m_sValue = _T("");
		m_bWin32 = false;
		m_unSupported = false;
	}

	friend bool operator== (const CVirtualRegKey &v1, const CVirtualRegKey &v2);
	friend bool operator!= (const CVirtualRegKey &v1, const CVirtualRegKey &v2);

	UINT Set(HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpValueName, LPVOID lpValue, DWORD dwSize, bool bWin32);

	bool IsUnSupported()
	{
		return m_unSupported;
	}

	CStdStringW GetSubKey()
	{
		return m_subKey;
	}

	CStdStringW GetValueName()
	{
		return m_sValueName;
	}

	LPVOID GetValue(DWORD& dwType);
	DWORD GetBinaryValueSize() const
	{
		return m_binarySize;
	}

protected:
	HKEY m_hKeyRoot;
	CStdStringW m_subKey;
	DWORD m_dwType;
	CStdStringW m_sValueName;
	LPBYTE m_pBinaryValue;
	DWORD m_binarySize;
	DWORD m_dwValue;
	CStdStringW m_sValue;
	bool m_bWin32;

	bool m_unSupported;
};