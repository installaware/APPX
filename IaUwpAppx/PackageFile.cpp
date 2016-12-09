// FILE: PackageFile.cpp
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
#include "PackageFile.h"

bool operator== (const CPackageFile &v1, const CPackageFile &v2)
{
	return (v1.m_sSourcePath == v2.m_sSourcePath &&
		v1.m_sTargetPath == v2.m_sTargetPath &&
		v1.m_sVFSpath == v2.m_sVFSpath);
}

bool operator!= (CPackageFile &v1, CPackageFile &v2)
{
	return !(v1 == v2);
}

CPackageFile::CPackageFile()
{
	m_sSourcePath = _T("");
	m_sTargetPath = _T("");
	m_sVFSpath = _T("");
}

CPackageFile::~CPackageFile()
{
}

UINT CPackageFile::Set(LPCWSTR lpwSource, LPCWSTR lpwTarget, vector<CVfsPath>& vVfs)
{
	UINT result = UAPPX_SUCCESS;

	do
	{
		if (!lpwSource || !lpwTarget)
		{
			result = UAPPX_PARAM_ERROR;
			break;
		}

		if (!DF_FileExist(lpwSource))
		{
			result = UAPPX_SOURCE_FILE_NOT_FOUND;
			break;
		}

		m_sSourcePath = lpwSource;
		m_sTargetPath = lpwTarget;

		m_sVFSpath = ResolveVFS(lpwTarget, vVfs);

		if (m_sVFSpath.IsEmpty())
			result = UAPPX_VFS_PATH_NOT_RESOLVED;

	} while (FALSE);

	return result;
}

UINT CPackageFile::Set(LPCWSTR lpwTarget, vector<CVfsPath>& vVfs)
{
	UINT result = UAPPX_SUCCESS;

	do
	{
		if (!lpwTarget)
		{
			result = UAPPX_PARAM_ERROR;
			break;
		}

		m_sSourcePath.clear();
		m_sTargetPath = lpwTarget;

		m_sVFSpath = ResolveVFS(lpwTarget, vVfs);

		if (m_sVFSpath.IsEmpty())
			result = UAPPX_VFS_PATH_NOT_RESOLVED;

	} while (FALSE);

	return result;
}

CStdStringW CPackageFile::ResolveVFS(LPCWSTR lpwPath, vector<CVfsPath>& vVfs)
{
	CStdStringW sResult(_T(""));
	size_t bestLen = 0;

	do
	{
		CStdStringW sPath(lpwPath);

		vector<wchar_t> drive(8);
		errno_t err = _wsplitpath_s(m_sTargetPath.c_str(), drive.data(), drive.size(), NULL, 0, NULL, 0, NULL, 0);

		if (err != 0)
		{
			break;
		}

		sPath = sPath.Right((int)sPath.length() - lstrlenW(&drive[0]));

		size_t idx = 0;
		std::vector<CVfsPath>::const_iterator iter;
		for (iter = vVfs.begin(); iter != vVfs.end(); iter++)
		{
			CVfsPath vfs = (CVfsPath)*iter;
			CStdStringW sSysPath(vfs.GetSystemPath());

			err = _wsplitpath_s(sSysPath.c_str(), drive.data(), drive.size(), NULL, 0, NULL, 0, NULL, 0);
			if (err != 0)
			{
				break;
			}

			sSysPath = sSysPath.Right((int)sSysPath.length() - lstrlenW(&drive[0]));

			if (!g_StringBegins_with(sPath, sSysPath))
				continue;

			size_t len = sSysPath.length();
			idx++;

			if (bestLen < len)
			{
				bestLen = len;
				CStdStringW sBuffer(sPath.Right((int)(sPath.length() - sSysPath.length())));
				sResult.Format(_T("VFS\\%s%s"), vfs.GetPath(), sBuffer);
			}
		}

	} while (FALSE);

	return sResult;
}