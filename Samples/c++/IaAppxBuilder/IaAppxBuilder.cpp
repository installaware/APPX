// IaAppxBuilder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static void g_Format(wstring& string, const WCHAR* fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	int size = _vscwprintf(fmt, vl);
	string.resize(++size);
	_vsnwprintf_s((WCHAR*)string.data(), size, _TRUNCATE, fmt, vl);
	va_end(vl);
}

static void WriteToConsole(wstring message)
{
	wcout << message;
	wcout << L"\n";
}

int _tmain(int argc, _TCHAR* argv[])
{
	wstring sError = L"";
	UINT uAppXResult = 0;

	bool b64BitPackage = false;

	wstring sPackageName = L"ApplicationX";
	wstring sPackageOutputName = L"ApplicationX";
	wstring sPackageOutDirectory = L"C:\\Users\\Public\\Documents\\IaAppxBuilder";
	wstring sManufacturer = L"InstallAware";
	wstring sPackageVersion = L"1.0.0.0";

	wstring sAuthenticodeCert = L""; //Path to certificate file
	wstring sAuthenticodePWD = L"";
	wstring sAuthenticodeTimestampURL = L"";

	// full path of the source EXE file eg. c:\\mypath\\WindowsFormsApplication.exe
	wstring sSourcePath = L"...fill with a full path to source file...";
	wstring sTargetPath = L"";

	g_Format(sTargetPath, L"C:\\Program Files(x86)\\%s\\%s", sPackageName.c_str(), L"WindowsFormsApplication.exe");

	wstring sShortcutName = L"ApplicationX";
	wstring sShortcutDescription = L"Appx Test package";

	do
	{
		WriteToConsole(L"Library initialization...");

		// Intialize APPX package
		uAppXResult = UAPPX_Initialize(sPackageOutputName.c_str(), sPackageOutDirectory.c_str(), b64BitPackage);
		if (0 != uAppXResult)
		{
			g_Format(sError, L"Failed to initialize APPX package!\n\nError code: %d", uAppXResult);
			break;
		}

		uAppXResult = UAPPX_SetDisplayName(sPackageName.c_str());
		if (0 != uAppXResult)
		{
			g_Format(sError, L"Failed to initialize APPX package!\n\nProduct Name might be not valid or APPX package not initialized!\nError code: %d", uAppXResult);
			break;
		}

		uAppXResult = UAPPX_SetLanguage(L"en-us");
		if (0 != uAppXResult)
		{
			g_Format(sError, L"Failed to initialize APPX package!\n\nLanguage identifier might be not valid or APPX package not initialized!\nError code: %d", uAppXResult);
			break;
		}

		if (!sAuthenticodeCert.empty())
			UAPPX_SetPublisherCertificate(sAuthenticodeCert.c_str(), sAuthenticodePWD.c_str(), sAuthenticodeTimestampURL.c_str());

		uAppXResult = UAPPX_SetPublisher(sManufacturer.c_str());
		if (0 != uAppXResult)
		{
			g_Format(sError, L"Failed to initialize APPX package!\n\nPublisher Name might be not valid or APPX package not initialized!\nError code: %d", uAppXResult);
			break;
		}

		uAppXResult = UAPPX_SetVersion(sPackageVersion.c_str());
		if (0 != uAppXResult)
		{
			g_Format(sError, L"Failed to initialize APPX package!\n\nProduct version might be not valid or APPX package not initialized!\nError code: %d", uAppXResult);
			break;
		}

		// Add File to package container
		size_t uFileidx = 0;
		uAppXResult = UAPPX_SetPackageFile(sSourcePath.c_str(), sTargetPath.c_str(), &uFileidx);
		if (0 != uAppXResult)
		{
			if (uAppXResult == 160)
				g_Format(sError, L"Failed to initialize APPX package!\n\nFailed to add file:\n %s \nFile not found!", sSourcePath.c_str());
			else
				g_Format(sError, L"Failed to initialize APPX package!\n\nFailed to add file:\n %s \nError code: %d", sSourcePath.c_str(), uAppXResult);

			break;
		}

		uAppXResult = UAPPX_SetShortcut(uFileidx, sShortcutName.c_str(), sShortcutDescription.c_str(), NULL, NULL, NULL, 0);
		if (0 != uAppXResult)
		{
			g_Format(sError, L"Failed to initialize APPX package!\n\nFailed to define shortuct:\n %s \nError code: %d", sSourcePath.c_str(), uAppXResult);
			break;
		}
		
		wstring sMessage = L"";
		g_Format(sMessage, L"Generating package: %s\\%s.appx", sPackageOutDirectory.c_str(), sPackageOutputName.c_str());
		WriteToConsole(sMessage);

		// Process package
		uAppXResult = UAPPX_Finalize();
		if (0 != uAppXResult)
		{
			if (uAppXResult == 185)
				g_Format(sError, L"Failed to initialize APPX package!\n\nAt least one shortcut has to be defined!\nError code: %d", uAppXResult);
			else
				g_Format(sError, L"Failed to create APPX package!\n\nError code: %d", uAppXResult);

			break;
		}

	} while (FALSE);

	UAPPX_Free();

	if (uAppXResult == 0)
		WriteToConsole(L"Process completed!");
	else
		WriteToConsole(sError);

    return (int)uAppXResult;
}

