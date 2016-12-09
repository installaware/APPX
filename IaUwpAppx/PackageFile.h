// FILE: PackageFile.h
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

class CAssociation
{
public:
	CAssociation()
	{
		m_sId = _T("");
		m_sDescription = _T("");
		m_sIconfile = _T("");
		m_iconIdx = -1;
		m_sIconVFSpath = _T("");
	}

	~CAssociation() {};

	const CAssociation& CAssociation::operator=(const CAssociation& association)
	{
		m_sId = association.m_sId;
		m_sDescription = association.m_sDescription;
		m_sIconfile = association.m_sIconfile;
		m_iconIdx = association.m_iconIdx;
		m_sIconVFSpath = association.m_sIconVFSpath;

		m_vExt.clear();

		std::vector<CStdStringW>::const_iterator iter;
		for (iter = association.m_vExt.begin(); iter != association.m_vExt.end(); iter++)
		{
			m_vExt.push_back((CStdStringW)*iter);
		}

		return *this;
	}

	/*
	friend bool operator== (const CAssociation &v1, const CAssociation &v2)
	{
		return (v1.m_sProgId == v2.m_sProgId &&
			v1.m_sDescription == v2.m_sDescription &&
			v1.m_sIconfile == v2.m_sIconfile &&
			v1.m_iconIdx == v2.m_iconIdx);
	}

	friend bool operator!= (const CAssociation &v1, const CAssociation &v2)
	{
		return !(v1 == v2);
	}*/


	// Shortcut data
	CStdStringW m_sId;
	CStdStringW m_sDescription;
	vector<CStdStringW> m_vExt;
	CStdStringW m_sIconfile;
	int m_iconIdx;
	CStdStringW m_sIconVFSpath;
};

class CShortcut
{
public:
	CShortcut()
	{
		m_sName = _T("");
		m_sDescription = _T("");
		m_sPath = _T("");
		m_sIconfile = _T("");
		m_sCmdline = _T("");
		m_iconIdx = -1;
		m_bEntryPoint = false;
	}

	~CShortcut() {};

	const CShortcut& CShortcut::operator=(const CShortcut& shortcut)
	{
		m_sName = shortcut.m_sName;
		m_sDescription = shortcut.m_sDescription;
		m_sPath = shortcut.m_sPath;
		m_sCmdline = shortcut.m_sCmdline;
		m_sIconfile = shortcut.m_sIconfile;
		m_iconIdx = shortcut.m_iconIdx;
		m_bEntryPoint = shortcut.m_bEntryPoint;
		return *this;
	}

	friend bool operator== (const CShortcut &v1, const CShortcut &v2)
	{
		return (v1.m_sName == v2.m_sName &&
			v1.m_sDescription == v2.m_sDescription &&
			v1.m_sPath == v2.m_sPath &&
			v1.m_sCmdline == v2.m_sCmdline &&
			v1.m_sIconfile == v2.m_sIconfile &&
			v1.m_iconIdx == v2.m_iconIdx,
			v1.m_bEntryPoint == v2.m_bEntryPoint);
	}

	friend bool operator!= (const CShortcut &v1, const CShortcut &v2)
	{
		return !(v1 == v2);
	}


	// Shortcut data
	CStdStringW m_sName;
	CStdStringW m_sDescription;
	CStdStringW m_sPath;
	CStdStringW m_sCmdline;
	CStdStringW m_sIconfile;
	int m_iconIdx;
	bool m_bEntryPoint;
};

class CPackageFile
{
public:
	CPackageFile();
	~CPackageFile();

	friend bool operator== (const CPackageFile &v1, const CPackageFile &v2);
	friend bool operator!= (const CPackageFile &v1, const CPackageFile &v2);

	UINT Set(LPCWSTR lpwSource, LPCWSTR lpwTarget, vector<CVfsPath>& vVfs);
	UINT Set(LPCWSTR lpwTarget, vector<CVfsPath>& vVfs);

	void Set(CAssociation& association)
	{
		m_association = association;
	}

	void Set(CShortcut& shortcut)
	{
		m_shortcut = shortcut;
	}

	LPCWSTR GetVFSpath() const
	{
		return m_sVFSpath.c_str();
	}

	LPCWSTR GetSourcePath() const
	{
		return m_sSourcePath.c_str();
	}

	CAssociation GetAssociation() const
	{
		return m_association;
	}

	CShortcut GetShortcut() const
	{
		return m_shortcut;
	}

protected:
	CStdStringW m_sSourcePath;
	CStdStringW m_sTargetPath;
	CStdStringW m_sVFSpath;

	CAssociation m_association;
	CShortcut m_shortcut;

protected:
	CStdStringW ResolveVFS(LPCWSTR lpwPath, vector<CVfsPath>& vVfs);
};
