// FILE: CAppx.h
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

class CAppx
{
public:
	CAppx(LPCWSTR lpPackagename, LPCWSTR lpPackagepath, bool b64BitPackage);
	~CAppx(void);

	UINT Initialize();

	void AddVRegKey(CVirtualRegKey& vKey);
	UINT AddFile(LPCWSTR lpwSource, LPCWSTR lpwTarget, size_t* pFileIdx);
	UINT AddDirectory(LPCWSTR lpwTarget, size_t* pFileIdx);

	UINT AddShortcut(size_t fileIdx, LPCWSTR lpwName, LPCWSTR lpwDescription, LPCWSTR lpwPath, LPCWSTR lpwCommandline, PCWSTR lpwIconfile, int iconIdx);
	UINT SetFileAssociation(size_t fileIdx, LPCWSTR lpwProgId, LPCWSTR lpwDescription, LPCWSTR** lpwExtensions, size_t extSize, LPCWSTR lpwIconfile, int iconIdx);
	UINT ProcessPackage(void);

	UINT SetDisplayName(LPCWSTR lpwValue);
	UINT SetLanguage(LPCWSTR lpwValue);
	UINT SetPublisherCertificate(LPCWSTR lpwCertificate, LPCWSTR lpwPassword, LPCWSTR lpwTimestamp);
	UINT SetPublisher(LPCWSTR lpwValue);
	UINT SetVersion(LPCWSTR lpwValue);

	bool SetWinSdkBinPath(LPCWSTR lpwBinPath);

protected:
	CStdStringW m_winSdkPath;
	CStdStringW m_packageName;
	CStdStringW m_PackagePath;
	CStdStringW m_TempPath;
	bool m_b64BitPackage;

	vector<CVfsPath> m_vVfsPaths;
	vector<CVirtualRegKey> m_vRegkeyList;
	vector<CPackageFile> m_vFileList;

	CAppManifest m_appManifest;

	// Authenticode
	CStdStringW m_sAuthenticodeCert;
	CStdStringW m_sAuthenticodeCertPwd;
	CStdStringW m_sAuthenticodeTimestamp;

	// Used to check if at least a shortcut has been defined
	bool m_packageHasShortcut;

protected:
	UINT FinalizeRegistry(void);
	UINT FinalizeVirtualFileSystem(void);
	UINT FinalizeAssets();
	UINT FinalizeAppManifest();
	UINT FinalizeAppXcontainer();
};
