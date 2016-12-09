// FILE: VfsPath.cpp
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
#include "VfsPath.h"

bool operator== (const CVfsPath &v1, const CVfsPath &v2)
{
	return (v1.m_sPath == v2.m_sPath &&
		v1.m_sSystemPath == v2.m_sSystemPath);
}

bool operator!= (CVfsPath &v1, CVfsPath &v2)
{
	return !(v1 == v2);
}

CVfsPath::CVfsPath()
{
	m_sPath = _T("");
	m_sSystemPath = _T("");
}

CVfsPath::~CVfsPath()
{
}

bool CVfsPath::Set(LPCWSTR lpwPath, LPCWSTR lpwSystemPath)
{
	if (!lpwPath || !lpwSystemPath)
	{
		return false;
	}

	m_sPath = lpwPath;
	m_sPath.Replace(L" ", L"%20");
	m_sSystemPath = lpwSystemPath;

	return true;
}