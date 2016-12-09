// FILE: CAppx.cpp
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

#include "IconExt.h"

void CALLBACK DF_VFS_enum(LPARAM param, LPARAM data)
{
	vector<CVfsPath>* pThis = (vector<CVfsPath>*)param;
	DF_FILEOP_VFS_DATA* pData = (DF_FILEOP_VFS_DATA*)data;

	if (!pThis || !pData)
		return;

	if (pData->pwName && pData->pwSysPath)
	{
		CVfsPath vfs;
		vfs.Set(pData->pwName, pData->pwSysPath);
		pThis->push_back(vfs);
	}
}

CAppx::CAppx(LPCWSTR lpPackagename, LPCWSTR lpPackagepath, bool b64BitPackage)
{
	m_packageName = lpPackagename;
	m_PackagePath = lpPackagepath;
	m_b64BitPackage = b64BitPackage;

	m_winSdkPath = _T("C:\\Program Files (x86)\\Windows Kits\\10\\bin\\x86");

	m_TempPath = _T("");

	m_sAuthenticodeCert = _T("");
	m_sAuthenticodeCertPwd = _T("");
	m_sAuthenticodeTimestamp = _T("");

	m_packageHasShortcut = false;
}

CAppx::~CAppx(void)
{
	std::vector<CVirtualRegKey>::const_iterator iter;
	for (iter = m_vRegkeyList.begin(); iter != m_vRegkeyList.end(); iter++)
	{
		CVirtualRegKey key = (CVirtualRegKey)*iter;
		key.Clear();
	}
}

UINT CAppx::Initialize()
{
	UINT result = UAPPX_SUCCESS;

	if (DF_EnumerateKnownFoldersW(DF_VFS_enum, (LPARAM)&m_vVfsPaths) != DFFILEOP_SUCCESS)
	{
		result = UAPPX_SYS_PATHS_ENUM_ERROR;
	}

	return result;
}

void CAppx::AddVRegKey(CVirtualRegKey& vKey)
{
	if (!(std::find(m_vRegkeyList.begin(), m_vRegkeyList.end(), vKey) != m_vRegkeyList.end()))
	{
		/* v does not contain x */
		m_vRegkeyList.push_back(vKey);
	}
}

UINT CAppx::AddFile(LPCWSTR lpwSource, LPCWSTR lpwTarget, size_t* pFileIdx)
{
	UINT result = UAPPX_SUCCESS;

	do
	{
		CPackageFile file;

		result = file.Set(lpwSource, lpwTarget, m_vVfsPaths);
		if (result != UAPPX_SUCCESS)
		{
			break;
		}

		if (!(std::find(m_vFileList.begin(), m_vFileList.end(), file) != m_vFileList.end()))
		{
			/* v does not contain file */
			m_vFileList.push_back(file);
			*pFileIdx = m_vFileList.size();
		}

	} while (FALSE);

	return result;
}

UINT CAppx::AddDirectory(LPCWSTR lpwTarget, size_t* pFileIdx)
{
	UINT result = UAPPX_SUCCESS;

	do
	{
		CPackageFile file;

		result = file.Set(lpwTarget, m_vVfsPaths);
		if (result != UAPPX_SUCCESS)
		{
			break;
		}

		if (!(std::find(m_vFileList.begin(), m_vFileList.end(), file) != m_vFileList.end()))
		{
			/* v does not contain file */
			m_vFileList.push_back(file);
			*pFileIdx = m_vFileList.size();
		}

	} while (FALSE);

	return result;
}

UINT CAppx::AddShortcut(size_t fileIdx, LPCWSTR lpwName, LPCWSTR lpwDescription, LPCWSTR lpwPath, LPCWSTR lpwCommandline, PCWSTR lpwIconfile, int iconIdx)
{
	UINT result = UAPPX_SUCCESS;

	// NOTE
	// lpwPath, pwCommandline and lpwIconfile can be null
	if (fileIdx == 0 || !lpwName/* || !lpwPath*/)
	{
		result = UAPPX_PARAM_ERROR;
	}

	do
	{
		if (fileIdx > m_vFileList.size())
		{
			result = UAPPX_PARAM_ERROR;
			break;
		}

		CPackageFile* file = &m_vFileList[fileIdx-1];

		if(!file)
		{
			result = UAPPX_PARAM_ERROR;
			break;
		}

		CShortcut shortCut;

		if (!m_packageHasShortcut)
		{
			shortCut.m_bEntryPoint = true;
		}

		shortCut.m_sName = lpwName;
		if (lpwDescription)
			shortCut.m_sDescription = lpwDescription;
		if(lpwPath)
			shortCut.m_sPath = lpwPath;
		if (lpwCommandline)
			shortCut.m_sCmdline = lpwCommandline;
		shortCut.m_sIconfile = lpwIconfile;
		shortCut.m_iconIdx = iconIdx;

		file->Set(shortCut);

		m_packageHasShortcut = true;

	} while (FALSE);

	return result;
}

UINT CAppx::SetFileAssociation(size_t fileIdx, LPCWSTR lpwProgId, LPCWSTR lpwDescription, LPCWSTR** lpwExtensions, size_t extSize, LPCWSTR lpwIconfile, int iconIdx)
{
	UINT result = UAPPX_SUCCESS;

	// NOTE
	// lpwIconfile - Icon path can be null

	do
	{
		if (fileIdx > m_vFileList.size())
		{
			result = UAPPX_PARAM_ERROR;
			break;
		}

		CPackageFile* file = &m_vFileList[fileIdx - 1];

		if (!file)
		{
			result = UAPPX_PARAM_ERROR;
			break;
		}

		CAssociation association;

		if (lpwProgId)
			association.m_sId = lpwProgId;

		if (lpwDescription)
			association.m_sDescription = lpwDescription;
		
		if (lpwIconfile)
		{
			association.m_sIconfile = lpwIconfile;
			association.m_iconIdx = iconIdx;
		}

		if (extSize > 0 && *lpwExtensions)
		{
			for (size_t i = 0; i < extSize; i++)
			{
				LPCWSTR pw = (*lpwExtensions)[i];
				association.m_vExt.push_back(pw);
			}
		}

		file->Set(association);

	} while (FALSE);

	return result;
}

UINT CAppx::ProcessPackage(void)
{
	UINT result = UAPPX_SUCCESS;

	do
	{
		if (!m_packageHasShortcut)
		{
			result = UAPPX_NO_SHORTCUT_DEFINED;
			break;
		}

		// Create temporary package location
		m_TempPath = g_GetTempDirectory();

		if (m_TempPath.IsEmpty())
		{
			result = UAPPX_FAILURE;
			break;
		}

		m_TempPath.Format(_T("%s%s"), m_TempPath, g_GetGuId());

		if (DF_DirExist(m_TempPath.c_str()))
		{
			m_TempPath = g_GetTempDirectory();
			m_TempPath.Format(_T("%s%s"), m_TempPath, g_GetGuId());
		}

		if (!DF_CreateDirectory(m_TempPath.c_str(), NULL))
		{
			result = UAPPX_FILE_ACCESS_ERROR;
			break;
		}

		// Initialize AppManifest values
		m_appManifest.SetPath(m_TempPath);

		if(m_b64BitPackage)
			m_appManifest.SetElement(_T("x64"), CAppManifest::ProcessorArchitecture);
		else
			m_appManifest.SetElement(_T("x86"), CAppManifest::ProcessorArchitecture);

		// registry dat
		result = FinalizeRegistry();
		if (result != UAPPX_SUCCESS)
		{
			break;
		}

		// Virtual File System
		result = FinalizeVirtualFileSystem();
		if (result != UAPPX_SUCCESS)
		{
			break;
		}

		// Package ASSETS items
		result = FinalizeAssets();
		if (result != UAPPX_SUCCESS)
		{
			break;
		}

		// Package manifest
		result = FinalizeAppManifest();
		if (result != UAPPX_SUCCESS)
		{
			break;
		}

		// Generate APPX container
		result = FinalizeAppXcontainer();

	} while (FALSE);

	DF_DeleteDirectory(m_TempPath.c_str(), TRUE, NULL, NULL);

	return result;
}

UINT CAppx::SetDisplayName(LPCWSTR lpwValue)
{
	UINT result = m_appManifest.SetElement(lpwValue, CAppManifest::IdentityName);

	if(result == UAPPX_SUCCESS)
		result = m_appManifest.SetElement(lpwValue, CAppManifest::DisplayName);

	return result;
}

UINT CAppx::SetLanguage(LPCWSTR lpwValue)
{
	return m_appManifest.SetElement(lpwValue, CAppManifest::ResourceLanguage);

	return UAPPX_SUCCESS;
}

UINT CAppx::SetPublisherCertificate(LPCWSTR lpwCertificate, LPCWSTR lpwPassword, LPCWSTR lpwTimestamp)
{
	if (!lpwCertificate || !lpwPassword)
		return UAPPX_PARAM_ERROR;

	m_sAuthenticodeCert = lpwCertificate;
	m_sAuthenticodeCertPwd = lpwPassword;

	if(lpwTimestamp)
		m_sAuthenticodeTimestamp = lpwTimestamp;

	return UAPPX_SUCCESS;
}

UINT CAppx::SetPublisher(LPCWSTR lpwValue)
{
	return m_appManifest.SetElement(lpwValue, CAppManifest::PublisherDisplayName);
}

UINT CAppx::SetVersion(LPCWSTR lpwValue)
{
	return m_appManifest.SetElement(lpwValue, CAppManifest::Version);
}

bool CAppx::SetWinSdkBinPath(LPCWSTR lpwBinPath)
{
	bool result = false;

	if (lpwBinPath)
	{
		if (DF_DirExist(lpwBinPath))
		{
			m_winSdkPath = lpwBinPath;
			result = true;
		}
	}

	return result;
}

UINT CAppx::FinalizeRegistry(void)
{
	UINT result = UAPPX_SUCCESS;
	bool bRet = true;

	CStdStringW sGuId = _T("");
	sGuId.Format(_T("{%s}"), g_GetGuId());

	if (!g_SetRegKeyPermission(sGuId.c_str()))
	{
		return UAPPX_REG_DAT_SECURITY_DESCRIPTOR_ERROR;
	}

	do
	{
		std::vector<CVirtualRegKey>::const_iterator iter;
		for (iter = m_vRegkeyList.begin(); iter != m_vRegkeyList.end(); iter++)
		{
			CVirtualRegKey key = (CVirtualRegKey)*iter;

			if (!key.IsUnSupported())
			{
				CStdStringW sSubKey = _T("");
				sSubKey.Format(_T("%s\\%s"), sGuId, key.GetSubKey());

				CStdStringW sValName = _T("");
				sValName = key.GetValueName();

				DWORD dwType = 0;
				LPVOID pValue = key.GetValue(dwType);

				switch (dwType)
				{
				case REG_BINARY:
				{
					DWORD size = key.GetBinaryValueSize();
					bRet = DFREG_SaveBinary(HKEY_CURRENT_USER, sSubKey.c_str(), sValName, (LPBYTE)pValue, size);
				}
				break;

				case REG_DWORD:
				{
					DWORD dwValue = *(LPDWORD)pValue;
					bRet = DFREG_SaveNumber(HKEY_CURRENT_USER, sSubKey.c_str(), sValName, dwValue);
				}
				break;

				//case REG_QWORD:
				//	// Not handled!
				//	result = false;
				//	break;

				case REG_SZ:
				case REG_EXPAND_SZ:
				case REG_LINK:
				{
					CStdStringW sValue = ((CStdStringW*)pValue)->c_str();

					if(dwType == REG_EXPAND_SZ)
						bRet = DFREG_SaveExpandedString(HKEY_CURRENT_USER, sSubKey.c_str(), sValName, sValue.c_str());
					else
						bRet = DFREG_SaveString(HKEY_CURRENT_USER, sSubKey.c_str(), sValName, sValue.c_str());
				}
				break;

				case REG_MULTI_SZ:
				{
					CStdStringW sValue = ((CStdStringW*)pValue)->c_str();

					int len = (int)sValue.length() + 2;
					LPWSTR p = sValue.GetBuffer(len);
					p[len - 1] = _T('\0');
					bRet = DFREG_SaveMultiString(HKEY_CURRENT_USER, sSubKey.c_str(), sValName, p);
					sValue.ReleaseBuffer();
				}
				break;

				default:
					//result = false;
					break;
				}
			}

			if (!bRet)
			{
				result = UAPPX_REG_DAT_ERROR;
				break;
			}
		}

		if (result != UAPPX_SUCCESS)
		{
			break;
		}

		// Export
		HKEY hKey = DFREG_Open(HKEY_CURRENT_USER, sGuId.c_str(), KEY_ALL_ACCESS);

		if (hKey)
		{
			CStdStringW sTargetFile(_T(""));
			sTargetFile.Format(_T("%s\\Registry.dat"), m_TempPath);

			if (DFREG_ExportKey(hKey, sTargetFile.c_str(), NULL) != ERROR_SUCCESS)
			{
				DF_DeleteFile(sTargetFile.c_str());
				result = UAPPX_REG_DAT_PRIVILEGE_ERROR;
			}

			DFREG_Close(hKey);
		}
		else
		{
			result = UAPPX_REG_DAT_ERROR;
			break;
		}

	} while (FALSE);

	// Do cleanup
	bRet = DFREG_DeletelKey(HKEY_CURRENT_USER, sGuId.c_str(), NULL);
	
	return result;
}

UINT CAppx::FinalizeVirtualFileSystem(void)
{
	UINT result = UAPPX_SUCCESS;

	do
	{
		// Copy source files to VFS temp directory
		std::vector<CPackageFile>::const_iterator fileIter;
		for (fileIter = m_vFileList.begin(); fileIter != m_vFileList.end(); fileIter++)
		{
			bool isVFS_File = true;

			CPackageFile file = (CPackageFile)*fileIter;

			if (lstrlen(file.GetSourcePath()) == 0)
			{
				isVFS_File = false;
			}

			CStdStringW sTargetFile(_T(""));
			sTargetFile.Format(_T("%s\\%s"), m_TempPath, file.GetVFSpath());

			CStdStringW sTargetDir(sTargetFile);

			if (isVFS_File)
			{
				size_t nSize = sTargetDir.GetLength() + 1;
				LPWSTR p = sTargetDir.GetBuffer((int)nSize);
				DF_RemoveFileName(p, nSize);
				sTargetDir.ReleaseBuffer();
			}

			if (!DF_DirExist(sTargetDir.c_str()))
			{
				if (!DF_CreateDirectory(sTargetDir.c_str(), NULL))
				{
					result = UAPPX_FILE_ACCESS_ERROR;
					break;
				}
			}

			if (!isVFS_File)
			{
				// Skip this, it's a directory not a file!
				continue;
			}

			if(!DF_CopyFile(file.GetSourcePath(), sTargetFile.c_str(), false))
			{
				result = UAPPX_VFSFILE_COPY_ERROR;
				break;
			}
		}

		if (result != UAPPX_SUCCESS)
		{
			break;
		}

	} while (FALSE);

	return result;
}

UINT CAppx::FinalizeAssets()
{
	UINT result = UAPPX_SUCCESS;

	do
	{
		// Create ASSETS directory
		CStdStringW sTargetDir(_T(""));
		sTargetDir.Format(_T("%s\\%s"), m_TempPath, _T("ASSETS"));
		if (!DF_CreateDirectory(sTargetDir.c_str(), NULL))
		{
			result = UAPPX_FILE_ACCESS_ERROR;
			break;
		}

		if (!g_ExtractModuleResource(IDR_ASSETS_50, _T("ASSETS"), CStdStringW(sTargetDir + _T("\\PackageLogo.50x50.png")).c_str()))
		{
			result = UAPPX_UNEXPECTED_MODULE_RESOURCE_ERROR;
			break;
		}

		result = m_appManifest.SetElement((CStdStringW(_T("Assets")) + _T("\\PackageLogo.50x50.png")).c_str(), CAppManifest::Logo);
		if (result != UAPPX_SUCCESS)
		{
			result = UAPPX_UNEXPECTED_MODULE_RESOURCE_ERROR;
			break;
		}

		vector<CPackageFile>::const_iterator fileIter;
		for (fileIter = m_vFileList.begin(); fileIter != m_vFileList.end(); fileIter++)
		{
			CPackageFile file = (CPackageFile)*fileIter;

			CShortcut shortCut = file.GetShortcut();

			if (shortCut.m_sName.IsEmpty())
				continue;

			int iDx = -1;
			CStdStringW sImageFile(_T(""));
			if (!shortCut.m_sIconfile.IsEmpty())
			{
				sImageFile = shortCut.m_sIconfile;
				iDx = shortCut.m_iconIdx;
			}
			else
			{
				sImageFile = file.GetSourcePath();
			}
			
			// Application 150x150 asset image
			CStdStringW s150x150img_VFSpath(_T(""));
			s150x150img_VFSpath.Format(_T("Assets\\%s.150x150.png"), shortCut.m_sName);
			
			bool bIconDone = false;
			CIconExt icon;
			if (icon.Load(sImageFile.c_str(), iDx, 150, 150))
			{
				bIconDone = icon.SaveAsPng(CStdStringW(m_TempPath + _T("\\") + s150x150img_VFSpath.c_str()).c_str());
			}
			
			if (!bIconDone)
			{
				if (!g_ExtractModuleResource(IDR_ASSETS_150, _T("ASSETS"), CStdStringW(m_TempPath + _T("\\") + s150x150img_VFSpath.c_str()).c_str()))
				{
					result = UAPPX_UNEXPECTED_MODULE_RESOURCE_ERROR;
					break;
				}
			}

			// Application 44x44 asset image
			CStdStringW s44x44img_VFSpath(_T(""));
			s44x44img_VFSpath.Format(_T("Assets\\%s.44x44.png"), shortCut.m_sName);

			if (icon.Load(sImageFile.c_str(), iDx, 44, 44))
			{
				bIconDone = icon.SaveAsPng(CStdStringW(m_TempPath + _T("\\") + s44x44img_VFSpath.c_str()).c_str());
			}

			if (!bIconDone)
			{
				if (!g_ExtractModuleResource(IDR_ASSETS_44, _T("ASSETS"), CStdStringW(m_TempPath + _T("\\") + s44x44img_VFSpath.c_str()).c_str()))
				{
					result = UAPPX_UNEXPECTED_MODULE_RESOURCE_ERROR;
					break;
				}
			}

			//Add Application node to AppManifest
			CAssociation association = file.GetAssociation();

			if (association.m_vExt.size() > 0)
			{
				// Create Extension logo asset image if an icon file has been provided. 
				if (!association.m_sIconfile.IsEmpty())
				{
					// Extension 150x150 asset image
					association.m_sIconVFSpath.Format(_T("Assets\\%s.ExtLogo150x150.png"), shortCut.m_sName);

					sImageFile = association.m_sIconfile;
					iDx = association.m_iconIdx;

					if (icon.Load(sImageFile.c_str(), iDx, 150, 150))
					{
						bIconDone = icon.SaveAsPng(CStdStringW(m_TempPath + _T("\\") + association.m_sIconVFSpath.c_str()).c_str());

						if (!bIconDone)
						{
							// Icon not found, delete the "m_sIconVFSpath".
							// This will cause the Extsion logo to be not adeded to the manifest.
							association.m_sIconVFSpath.clear();
						}
					}
				}
			}

			result = m_appManifest.SetApplicationNode(shortCut, file.GetVFSpath(), s150x150img_VFSpath.c_str(), s44x44img_VFSpath.c_str(), association);
		}

	} while (FALSE);

	return result;
}

UINT CAppx::FinalizeAppManifest()
{
	UINT result = UAPPX_SUCCESS;

	do
	{
		result = m_appManifest.Do(m_sAuthenticodeCert, m_sAuthenticodeCertPwd);

	} while (FALSE);

	return result;
}

UINT CAppx::FinalizeAppXcontainer()
{
	UINT result = UAPPX_SUCCESS;

	do
	{ 
		CStdStringW sExecutable(_T(""));
		CStdStringW sCmdLine(_T(""));
		CStdStringW sOutAppx(_T(""));

		// Create target directory
		if (!DF_DirExist(m_PackagePath.c_str()))
		{
			if (!DF_CreateDirectory(m_PackagePath.c_str(), NULL))
			{
				result = UAPPX_FILE_ACCESS_ERROR;
				break;
			}
		}

		sOutAppx.Format(_T("%s.appx"), m_PackagePath + _T("\\") + m_packageName);
		DF_DeleteFile(sOutAppx.c_str());

		sExecutable.Format(_T("%s\\%s"), m_winSdkPath, _T("makeappx.exe"));
		sCmdLine.Format(_T("\"%s\" pack /d \"%s\" /p \"%s\""), sExecutable, m_TempPath, sOutAppx);

		LPTSTR lpwCmdLine = sCmdLine.GetBuffer();
		DWORD dwRet = g_StartProcess(lpwCmdLine);
		sCmdLine.ReleaseBuffer();

		if (dwRet != ERROR_SUCCESS)
		{
			result = UAPPX_CREATE_CONTAINER_ERROR;
			break;
		}

		// Sign the Package, if a certificate file has been provided.
		if (!m_sAuthenticodeCert.IsEmpty())
		{
			sExecutable.Format(_T("%s\\%s"), m_winSdkPath, _T("signtool.exe"));

			if (!m_sAuthenticodeCertPwd.IsEmpty())
			{
				if(m_sAuthenticodeTimestamp.IsEmpty())
					sCmdLine.Format(_T("\"%s\" sign -f \"%s\" -fd SHA256 -v /p %s \"%s\""), sExecutable, m_sAuthenticodeCert, m_sAuthenticodeCertPwd, sOutAppx);
				else
					sCmdLine.Format(_T("\"%s\" sign -f \"%s\" /t \"%s\" -fd SHA256 -v /p %s \"%s\""), sExecutable, m_sAuthenticodeCert, m_sAuthenticodeTimestamp, m_sAuthenticodeCertPwd, sOutAppx);
			}	
			else
			{
				if (m_sAuthenticodeTimestamp.IsEmpty())
					sCmdLine.Format(_T("\"%s\" sign -f \"%s\" -fd SHA256 -v \"%s\""), sExecutable, m_sAuthenticodeCert, sOutAppx);
				else
					sCmdLine.Format(_T("\"%s\" sign -f \"%s\" /t \"%s\" -fd SHA256 -v \"%s\""), sExecutable, m_sAuthenticodeCert, m_sAuthenticodeTimestamp, sOutAppx);
			}

			lpwCmdLine = sCmdLine.GetBuffer();
			DWORD dwRet = g_StartProcess(lpwCmdLine);
			sCmdLine.ReleaseBuffer();

			if (dwRet != ERROR_SUCCESS)
			{
				result = UAPPX_SIGN_ERROR;
			}
		}

	} while (FALSE);

	return result;
}
