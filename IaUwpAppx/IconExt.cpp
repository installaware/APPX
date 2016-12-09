// FILE: IconExt.cpp
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
#include "IconExt.h"


#include <shlwapi.h>
#include <comip.h>
#include <comdef.h>
#include <commoncontrols.h>

_COM_SMARTPTR_TYPEDEF(IImageList, __uuidof(IImageList));

struct BITMAP_AND_BYTES
{
	Gdiplus::Bitmap* bmp;
	int32_t* bytes;
};

static CLSID g_pngClsid = GUID_NULL;
/////////////////////////////////
static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

static BITMAP_AND_BYTES CreateAlphaChannelBitmapFromIcon(HICON hIcon)
{
	// Get the icon info
	ICONINFO iconInfo = { 0 };
	GetIconInfo(hIcon, &iconInfo);

	// Get the screen DC
	HDC dc = GetDC(NULL);

	// Get icon size info
	BITMAP bm = { 0 };
	GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bm);

	// Set up BITMAPINFO
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = bm.bmWidth;
	bmi.bmiHeader.biHeight = -bm.bmHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	// Extract the color bitmap
	int nBits = bm.bmWidth * bm.bmHeight;
	int32_t* colorBits = new int32_t[nBits];
	GetDIBits(dc, iconInfo.hbmColor, 0, bm.bmHeight, colorBits, &bmi, DIB_RGB_COLORS);

	// Check whether the color bitmap has an alpha channel.
	// (On my Windows 7, all file icons I tried have an alpha channel.)
	BOOL hasAlpha = FALSE;
	for (int i = 0; i < nBits; i++) 
	{
		if ((colorBits[i] & 0xff000000) != 0)
		{
			hasAlpha = TRUE;
			break;
		}
	}

	// If no alpha values available, apply the mask bitmap
	if (!hasAlpha)
	{
		// Extract the mask bitmap
		int32_t* maskBits = new int32_t[nBits];
		GetDIBits(dc, iconInfo.hbmMask, 0, bm.bmHeight, maskBits, &bmi, DIB_RGB_COLORS);
		// Copy the mask alphas into the color bits
		for (int i = 0; i < nBits; i++)
		{
			if (maskBits[i] == 0)
			{
				colorBits[i] |= 0xff000000;
			}
		}
		delete[] maskBits;
	}

	// Release DC and GDI bitmaps
	ReleaseDC(NULL, dc);
	::DeleteObject(iconInfo.hbmColor);
	::DeleteObject(iconInfo.hbmMask);

	// Create GDI+ Bitmap
	Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(bm.bmWidth, bm.bmHeight, bm.bmWidth * 4, PixelFormat32bppARGB, (BYTE*)colorBits);
	BITMAP_AND_BYTES ret = { bmp, colorBits };

	return ret;
}
/////////////////////////////////////////////////////

CIconExt::CIconExt()
{
	m_gdiplusToken = NULL;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	m_bmp = NULL;
}

CIconExt::~CIconExt()
{
	Free();

	GdiplusShutdown(m_gdiplusToken);
}

void CIconExt::Free()
{
	if (m_bmp)
		delete m_bmp;

	m_bmp = NULL;
}

bool CIconExt::Load(LPCWSTR lpwPath, int index, int nWidth, int nHeight)
{
	bool result = false;

	if (!lpwPath)
		return result;

	Free();

	HICON hIcon = NULL;

	do
	{
		CStdStringW sPath(lpwPath);
		CStdStringW sExt(_T(""));

		size_t nSize = 0;
		if (DF_GetFileExt(lpwPath, NULL, &nSize))
		{
			nSize += 1;
			LPWSTR p = sExt.GetBuffer((int)nSize);
			DF_GetFileExt(lpwPath, p, &nSize);
			sExt.ReleaseBuffer();
		}

		if (0 == sExt.CompareNoCase(_T("ico")))
		{
			hIcon = (HICON)LoadImage(0, lpwPath, IMAGE_ICON, nWidth, nHeight, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);

			if (hIcon)
			{
				break;
			}
		}
		
		hIcon = GetExtraLargeIcon(lpwPath, index);
		
	} while (FALSE);

	if (hIcon)
	{
		BITMAP_AND_BYTES bbs = CreateAlphaChannelBitmapFromIcon(hIcon);

		if (bbs.bmp && bbs.bytes)
		{
			//Resize
			int w = bbs.bmp->GetWidth();
			int h = bbs.bmp->GetHeight();

			if (w != nWidth || h != nHeight)
			{
				float wFactor = (float)nWidth / (float)w;
				float hFactor = (float)nHeight / (float)h;

				m_bmp = new Gdiplus::Bitmap((int)nWidth, (int)nHeight);
				
				Graphics g(m_bmp);
				g.ScaleTransform(wFactor, hFactor);
				g.DrawImage(bbs.bmp, 0, 0);
			}
			else
			{
				m_bmp = new Gdiplus::Bitmap((int)nWidth, (int)nHeight);
				Graphics g(m_bmp);
				g.DrawImage(bbs.bmp, 0, 0);
			}

			result = true;
		}
		
		if (bbs.bmp)
			delete bbs.bmp;
		if(bbs.bytes)
			delete[] bbs.bytes;
	}

	if (hIcon)
		::DestroyIcon(hIcon);

	return result;
}

HICON CIconExt::GetExtraLargeIcon(LPCWSTR lpwPath, int index)
{
	HICON hIcon = NULL;

	if (!lpwPath)
		return hIcon;

	do
	{
		SHFILEINFO sfi;

		if (!SHGetFileInfo(lpwPath, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX))
			break;

		// Get the extralarge image list
		IImageList *piml;
		if (FAILED(SHGetImageList(SHIL_JUMBO, IID_PPV_ARGS(&piml))))
			break;

		// Extract the icon
		if (index < 0)
			index = sfi.iIcon;

		piml->GetIcon(index, ILD_NORMAL, &hIcon);

		// Clean up
		piml->Release();

	} while (FALSE);

	return hIcon;
}

bool CIconExt::SaveAsPng(LPCWSTR lpwFile)
{
	bool result = false;

	if (!m_bmp)
		return result;
	if(!lpwFile)
		return result;

	do
	{
		IStream* fstrm = NULL;
		if (FAILED(SHCreateStreamOnFile(lpwFile, STGM_WRITE | STGM_CREATE, &fstrm)))
			break;
		
		GetEncoderClsid(L"image/png", &g_pngClsid);
		Status stat = m_bmp->Save(fstrm, &g_pngClsid, NULL);

		if (stat == Ok)
			result = true;

		fstrm->Release();

	} while (FALSE);

	return result;
}