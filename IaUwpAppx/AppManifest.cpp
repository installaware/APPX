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
#include "AppManifest.h"

static const LPCWSTR beginMap[]=
{
	_T(" "),
	_T("-"),
	_T("."),
	_T(".."),
	_T("con"),
	_T("prn"),
	_T("aux"),
	_T("nul"),
	_T("com1"),
	_T("com2"),
	_T("com3"),
	_T("com4"),
	_T("com5"),
	_T("com6"),
	_T("com7"),
	_T("com8"),
	_T("com9"),
	_T("lpt1"),
	_T("lpt2"),
	_T("lpt3"),
	_T("lpt4"),
	_T("lpt5"),
	_T("lpt6"),
	_T("lpt7"),
	_T("lpt8"),
	_T("lpt9")
};

static const LPCWSTR equalMap[] =
{
	_T("con."),
	_T("prn."),
	_T("aux."),
	_T("nul."),
	_T("com1."),
	_T("com2."),
	_T("com3."),
	_T("com4."),
	_T("com5."),
	_T("com6."),
	_T("com7."),
	_T("com8."),
	_T("com9."),
	_T("lpt1."),
	_T("lpt2."),
	_T("lpt3."),
	_T("lpt4."),
	_T("lpt5."),
	_T("lpt6."),
	_T("lpt7."),
	_T("lpt8."),
	_T("lpt9."),
	_T("xn--")
};


CAppManifest::CAppManifest()
{
	m_vElem.resize(MAX_ELEMENTS);
}

CAppManifest::~CAppManifest()
{
}

UINT CAppManifest::CheckElement(LPCWSTR lpwValue, Attribute attribute, CStdStringW& checkedOut)
{
	UINT result = UAPPX_SUCCESS;

	checkedOut.clear();

	if (!lpwValue)
		return UAPPX_PARAM_ERROR;

	CStdStringW sBuffer(lpwValue);

	if (sBuffer.IsEmpty())
		return UAPPX_INVALID_MANIFEST_VALUE;

	if (attribute == IdentityName)
	{
		size_t len = sBuffer.length();

		// Required a string between 3 and 50 characters in length 
		if (3 > len || 50 < len)
		{
			return UAPPX_INVALID_MANIFEST_VALUE;
		}

		// Remove firsts non alphanumeric char from string
		for (size_t i = 0; i < sBuffer.length(); i++)
		{
			wchar_t c = sBuffer.at(i);
			if (!(iswalpha(c) || iswdigit(c)) || (c == 0x20))
			{
				sBuffer.Delete(0, 1);
			}
			else
			{
				break;
			}
		}

		size_t size = sizeof(beginMap) / sizeof(beginMap[0]);

		for (size_t i = 0; i < size; i++)
		{
			CStdStringW s(beginMap[i]);

			if (g_StringBegins_with(sBuffer, s))
			{
				sBuffer.Delete(0, (int)s.length());
			}
		}

		size = sizeof(equalMap) / sizeof(equalMap[0]);

		for (size_t i = 0; i < size; i++)
		{
			if (sBuffer.CompareNoCase(equalMap[i]) == 0)
			{
				result = UAPPX_INVALID_MANIFEST_VALUE;
				break;
			}
		}

		for (CStdStringW::iterator it = sBuffer.begin(); it < sBuffer.end(); ++it)
		{
			// This replaces forbidden characters with Hyphen.
			switch (*it)
			{
			case 0x20:
			case 0x22:
			case 0x2f:
			case 0x3a:
			case 0x3b:
			case 0x3c:
			case 0x3e:
			case 0x3f:
			case 0x5c:
				(*it) = 0x2d;
				break;

			default:
				(*it) = *it;
				break;
			}
		}

		len = sBuffer.length();

		if (len > 1)
		{
			if (sBuffer.at(len - 1) == 0x2e) //('.')
			{
				sBuffer.Delete((int)len - 1);
			}
		}

		len = sBuffer.length();

		// Required a string between 3 and 50 characters in length 
		if (3 > len || 50 < len)
		{
			return UAPPX_INVALID_MANIFEST_VALUE;
		}
	}
	else if (attribute == DisplayName)
	{
		size_t len = sBuffer.length();

		// Required a string between 1 and 256 characters in length 
		if (1 > len || 256  < len)
		{
			return UAPPX_INVALID_MANIFEST_VALUE;
		}

		// Remove firsts non alphanumeric char from string
		for (size_t i = 0; i < sBuffer.length(); i++)
		{
			wchar_t c = sBuffer.at(i);
			if (!(iswalpha(c) || iswdigit(c)) || (c == 0x20))
			{
				sBuffer.Delete(0, 1);
			}
			else
			{
				break;
			}
		}

		len = sBuffer.length();

		// Required a string between 1 and 256 characters in length 
		if (1 > len || 256  < len)
		{
			return UAPPX_INVALID_MANIFEST_VALUE;
		}
	}
	else if (attribute == Version)
	{
		vector<CStdString> result;
		g_StdStrSplit(sBuffer, 0x2e, result);

		size_t size = result.size();

		if (size != 4)
		{
			//Version string has to be in quad notation, "Major.Minor.Build.Revision".

			sBuffer.clear();

			while (size > 4)
			{
				g_VectorRemoveElement(result, size - 1);
				size = result.size();
			}

			while (size < 4)
			{
				result.push_back(_T("0"));
				size = result.size();
			}

			for (size_t i = 0; (i < 4) && (i < size); i++)
			{
				sBuffer += result[i];

				if ((i + 1 < 4) && (i + 1 < size))
					sBuffer += _T(".");
			}
		}
	}
	else if (attribute == PublisherDisplayName)
	{
		sBuffer.Format(_T("CN=%s"), lpwValue);
	}

	if (result == UAPPX_SUCCESS)
		checkedOut = sBuffer;

	return result;
}

UINT CAppManifest::SetElement(LPCWSTR lpwValue, Attribute attribute)
{
	UINT result = UAPPX_SUCCESS;

	CStdStringW sBuffer(_T(""));
	result = CheckElement(lpwValue, attribute, sBuffer);

	if(result == UAPPX_SUCCESS)
		m_vElem[(size_t)attribute] = sBuffer;

	return result;
}

UINT CAppManifest::SetApplicationNode(CShortcut& shortcut, LPCWSTR lpwExecutable, LPCWSTR lpw150x150Logo, LPCWSTR lpw44x44Logo, CAssociation& association)
{
	UINT result = UAPPX_SUCCESS;

	if (shortcut.m_sName.IsEmpty() || !lpwExecutable || !lpw150x150Logo || !lpw44x44Logo)
		return UAPPX_PARAM_ERROR;

	do
	{
		CStdStringW sFmt(_T("<Application Id=\"%s\" Executable=\"%s\" EntryPoint=\"Windows.FullTrustApplication\">\n") \
		_T("\t<uap:VisualElements DisplayName=\"%s\" Description=\"%s\" BackgroundColor=\"#777777\" Square150x150Logo=\"%s\" Square44x44Logo=\"%s\"/>\n"));

		// Application ID
		CStdStringW sApplicationId(_T(""));

		size_t nSize = 0;
		if (DF_GetFileName(lpwExecutable, NULL, &nSize))
		{
			nSize += 1;
			LPWSTR p = sApplicationId.GetBuffer((int)nSize);
			DF_GetFileName(lpwExecutable, p, &nSize);
			DF_RemoveExt(p, nSize);
			sApplicationId.ReleaseBuffer();
		}
		
		if (shortcut.m_bEntryPoint)
		{
			sApplicationId.Format(_T("A%s"), sApplicationId);
		}

		CStdStringW sApplicationNode(_T(""));
		sApplicationNode.Format(sFmt.c_str(), sApplicationId, lpwExecutable, shortcut.m_sName, shortcut.m_sDescription, lpw150x150Logo, lpw44x44Logo);

		size_t size = m_vElem.size();
		m_vElem.resize(size + 1);

		if (size > MAX_ELEMENTS)
		{
			sApplicationNode.Format(_T("\n%s"), sApplicationNode);
		}

		if (association.m_vExt.size() > 0)
		{
			/*sFmt = _T("<Extensions>\n<uap:Extension Category=\"windows.fileTypeAssociation\">\n<uap:FileTypeAssociation Name=\"%s\">\n");
			sFmt.Format(sFmt, association.m_sId);
			sFmt.append(_T("<uap:SupportedFileTypes>\n%s</uap:SupportedFileTypes>\n"));*/

			sFmt = _T("<Extensions>\n<uap:Extension Category=\"windows.fileTypeAssociation\">\n<uap:FileTypeAssociation Name=\"filetypes\">\n") \
				_T("<uap:SupportedFileTypes>\n%s</uap:SupportedFileTypes>\n");

			CStdStringW sFileTypeNodes(_T(""));

			std::vector<CStdStringW>::const_iterator iter;
			for (iter = association.m_vExt.begin(); iter != association.m_vExt.end(); iter++)
			{
				CStdStringW sExt = (CStdStringW)*iter;

				CStdStringW sNode(_T(""));
				sNode.Format(_T("<uap:FileType>%s</uap:FileType>\n"), sExt);
				sFileTypeNodes.append(sNode);
			}

			CStdStringW sExtensionsNode(_T(""));
			sExtensionsNode.Format(sFmt, sFileTypeNodes);

			if (!association.m_sIconVFSpath.IsEmpty())
			{
				CStdStringW sNode(_T(""));
				sNode.Format(_T("<uap:Logo>%s</uap:Logo>\n"), association.m_sIconVFSpath);
				sExtensionsNode.append(sNode);
			}

			// Close FileTypeAssociation node
			sExtensionsNode.append(_T("</uap:FileTypeAssociation>\n</uap:Extension>\n</Extensions>\n"));

			sApplicationNode.append(sExtensionsNode);
		}

		// Close Application node
		sApplicationNode.append(_T("</Application>"));

		m_vElem[size] = sApplicationNode.c_str();

	} while (FALSE);

	return result;
}

UINT CAppManifest::Do(CStdStringW sCertificate, CStdStringW sCertPassword)
{
	UINT result = UAPPX_SUCCESS;

	do
	{
		// Process publisher certificate, if any.
		if (sCertificate.IsEmpty())
		{
			m_vElem[(size_t)Publisher] = m_vElem[(size_t)PublisherDisplayName];
		}
		else
		{
			// Get publisher "subject" from certificate file.
			LPWSTR pwSubject = NULL;

			DWORD dwRet = g_GetCertificateSubject(sCertificate.c_str(), sCertPassword.c_str(), pwSubject);

			if (dwRet != ERROR_SUCCESS)
			{
				switch (dwRet)
				{
					case ERROR_FILE_NOT_FOUND:
					case ERROR_PATH_NOT_FOUND:
						result = UAPPX_CERTIFICATE_FILE_NOT_FOUND;
					break;

					case ERROR_INVALID_HANDLE:
					case ERROR_ACCESS_DENIED:
						result = UAPPX_CERTIFICATE_FILE_ACCESS_ERROR;
					break;

					case ERROR_INVALID_PASSWORD:
						result = UAPPX_CERTIFICATE_INVALID_PASSWORD;
						break;

					default:
						result = UAPPX_CERTIFICATE_ERROR;
						break;
				}
				
				break;
			}

			CStdStringW sSubject(pwSubject);
			delete[] pwSubject;

			CStdString sResult(_T(""));
			FormatCertificateSubject(sSubject, sResult);

			if(sResult.IsEmpty())
			{
				result = UAPPX_CERTIFICATE_ERROR;
				break;
			}

			m_vElem[(size_t)Publisher] = sResult;
		}
		
		CStdStringW sSchema(m_sPath + _T("\\schema.xml"));
		CStdStringW sAppManifest(m_sPath + _T("\\AppxManifest.xml"));

		if (!g_ExtractModuleResource(IDR_XMLAPPMANIFEST1, _T("XMLAPPMANIFEST"), sSchema.c_str()))
		{
			result = UAPPX_UNEXPECTED_MODULE_RESOURCE_ERROR;
			break;
		}

		wfstream in(sSchema);
		wofstream out(sAppManifest);

		if (!in || !out)
		{
			result = UAPPX_CREATE_MANIFEST_ERROR;
			break;
		}

		UpdateManifest(in, out);

		in.close();
		DF_DeleteFile(sSchema.c_str());

	} while (FALSE);

	return result;
}

void CAppManifest::UpdateManifest(wistream& in, wostream& out)
{
	UINT uTokens = 0;
	size_t eSize = m_vElem.size();
	size_t iDx = 0;

	wchar_t c;
	while (in.get(c))
	{
		if (c != _T('$'))
		{
			uTokens = 0;
			out.put(c);
		}
		else
		{
			if (++uTokens == 3)
			{
				if (iDx < eSize)
				{
					if(iDx < MAX_ELEMENTS)
					{
						out << m_vElem[iDx++].c_str();
					}
					else
					{
						size_t nAppNode = eSize - MAX_ELEMENTS;

						for (; iDx < eSize; iDx++)
						{
							out << m_vElem[iDx].c_str();
						}
					}
				}
					
				uTokens = 0;
			}
		}
	}
}

void CAppManifest::FormatCertificateSubject(CStdString str, CStdString& result)
{
	std::vector<CStdString> v;

	size_t cutAt;
	while ((cutAt = str.find_first_of(0x2c)) != str.npos)
	{
		if (cutAt > 0)
		{
			CStdString sSubstring(str.substr(0, cutAt));

			if(sSubstring.at(0) != 0x20)// If not space, add one at zero index.
				sSubstring.Format(_T(" %s"), sSubstring);

			if (sSubstring.find_first_of(0x3d) == CStdString::npos)
			{
				sSubstring.Format(_T(",%s"), sSubstring);
			}

			v.push_back(sSubstring);
		}
		str = str.substr(cutAt + 1);
	}

	if (str.length() > 0)
	{
		v.push_back(str);
	}

	size_t vSize = v.size();

	if (vSize > 0)
	{
		CStdString sTempBuff(_T(""));
		for (int i = (int)(vSize-1); i >= 0; i--)
		{
			CStdStringW sSubstring(_T(""));
			sSubstring = v[(size_t)i];

			if (sSubstring.find_first_of(0x2c) != CStdString::npos)
			{
				sTempBuff.append(sSubstring);
			}
			else
			{
				if (!sTempBuff.IsEmpty())
				{
					sSubstring.append(sTempBuff);
					sTempBuff.clear();
				}

				if(!result.IsEmpty())
					sSubstring.Format(_T(",%s"), sSubstring);
				else
				{
					if (sSubstring.at(0) == 0x20)
						sSubstring.Delete(0);
				}

				sSubstring.Replace(_T("\""), _T("&quot;"));
				result.append(sSubstring);
			}
		}
	}
}