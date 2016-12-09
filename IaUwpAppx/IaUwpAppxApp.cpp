// FILE: IaUwpAppx.cpp : Defines the initialization routines for the DLL.
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CIaUwpAppxApp

BEGIN_MESSAGE_MAP(CIaUwpAppxApp, CWinApp)
END_MESSAGE_MAP()


// CIaUwpAppxApp construction

CIaUwpAppxApp::CIaUwpAppxApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CIaUwpAppxApp object
CIaUwpAppxApp theApp;


// CIaUwpAppxApp initialization

BOOL CIaUwpAppxApp::InitInstance()
{
	CWinApp::InitInstance();

	// Initialize COM
	//CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	return TRUE;
}

int CIaUwpAppxApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	//CoUninitialize();

	return CWinApp::ExitInstance();
}
