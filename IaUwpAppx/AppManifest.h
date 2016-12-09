// FILE: AppManifest.h
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

#define MAX_ELEMENTS	8

class CAppManifest
{
public:
	CAppManifest();
	~CAppManifest();

	enum Attribute
	{
		IdentityName,
		ProcessorArchitecture,
		Publisher,
		Version,
		DisplayName,
		PublisherDisplayName,
		Logo,
		ResourceLanguage
	};

	static UINT CheckElement(LPCWSTR lpwValue, Attribute attribute, CStdStringW& checkedOut);

	UINT SetElement(LPCWSTR lpwValue, Attribute attribute);

	UINT SetApplicationNode(CShortcut& shortcut, LPCWSTR lpwExecutable, LPCWSTR lpw150x150Logo, LPCWSTR lpw44x44Logo, CAssociation& association);

	void SetPath(CStdStringW sPath)
	{
		m_sPath = sPath;
	}

	UINT Do(CStdStringW sCertificate, CStdStringW sCertPassword);


protected:
	CStdStringW m_sPath;
	vector<wstring> m_vElem;
	vector<wstring> m_vApplicationNodes;

protected:
	void UpdateManifest(wistream& in, wostream& out);
	void FormatCertificateSubject(CStdString str, CStdString& result);
};

