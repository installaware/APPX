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

static void g_WriteToConsole(wstring message)
{
	wcout << message;
	wcout << L"\n";
}

static void g_ShowUsage()
{
	wcout << L"Installaware (C) 2016\n";
	wcout << L"IaAppxBuilder /Option <Required Parameter> [Optional Parameter]\n";
	wcout << L"\n";
	wcout << L"</s> \tthe source Win32/Win64/.NET binary to convert into an APPX (full path)\n";
	wcout << L"</o> \tthe output folder for the APPX package\n";
	wcout << L"</n> \tthe human readable name of the APPX package to create\n";
	wcout << L"</v> \tthe human readable version number of the APPX package to create\n";
	wcout << L"\n";
	wcout << L"[/x] \t0 to indicate the i386 (32-bit) platform or 1 to indicate the AMD64 (64-bit) platform.\n\tThe default value is 0 (targeting 32-bit platforms).\n";
	wcout << L"[/c] \tcode signing certificate (full path)\n";
	wcout << L"[/p] \tsigning certificate password\n";
	wcout << L"[/t] \tauthenticode TimeStamp URL\n";
}

int _tmain(int argc, _TCHAR* argv[])
{
	wstring sError = L"";
	UINT uAppXResult = 0;

	bool b64BitPackage = false;

	wstring sPackageName = L"";
	wstring sPackageOutputName = L"";
	wstring sPackageOutDirectory = L"";
	wstring sManufacturer = L"InstallAware";
	wstring sPackageVersion = L"";

	wstring sAuthenticodeCert = L""; //Path to certificate file
	wstring sAuthenticodePWD = L"";
	wstring sAuthenticodeTimestampURL = L"";

	// full path of the source EXE file eg. c:\\mypath\\WindowsFormsApplication.exe
	wstring sSourcePath = L"";
	wstring sTargetPath = L"";


	wstring sShortcutName = L"";
	wstring sShortcutDescription = L"";

	for (int i = 0; argc > i; i++)
	{
		if (argv[i][0] == _T('/') && lstrlen(argv[i]) > 1)
		{
			if (argc > i + 1)
			{
				switch (argv[i][1])
				{
					case _T('s'):
						i++;
						sSourcePath = argv[i];
						break;

					case _T('o'):
						i++;
						sPackageOutDirectory = argv[i];
						break;

					case _T('n'):
						i++;
						sPackageName = argv[i];
						break;

					case _T('v'):
						i++;
						sPackageVersion = argv[i];
						break;

					case _T('x'):
						i++;
						if(argv[i][0] == _T('1'))
							b64BitPackage = true;
						else
							b64BitPackage = false;

						break;

					case _T('c'):
						i++;
						sAuthenticodeCert = argv[i];
						break;

					case _T('p'):
						i++;
						sAuthenticodePWD = argv[i];
						break;

					case _T('t'):
						i++;
						sAuthenticodeTimestampURL = argv[i];
						break;
				}
			}
		}
	}

	if (sSourcePath.empty() || sPackageOutDirectory.empty() || sPackageName.empty() || sPackageVersion.empty())
	{
		g_ShowUsage();
		return -1;
	}
	
	sPackageOutputName = sPackageName;
	sShortcutName = sPackageName;

	g_Format(sShortcutDescription, L"%s package", sShortcutName.c_str());

	std::wstring sSourceFilename = sSourcePath.substr(sSourcePath.find_last_of(L"/\\") + 1);

	if(b64BitPackage)
		g_Format(sTargetPath, L"C:\\Program Files\\%s\\%s", sPackageName.c_str(), sSourceFilename.c_str());
	else
		g_Format(sTargetPath, L"C:\\Program Files(x86)\\%s\\%s", sPackageName.c_str(), sSourceFilename.c_str());

	do
	{
		g_WriteToConsole(L"Library initialization...");

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
		g_WriteToConsole(sMessage);

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
		g_WriteToConsole(L"Process completed!");
	else
		g_WriteToConsole(sError);

    return (int)uAppXResult;
}

