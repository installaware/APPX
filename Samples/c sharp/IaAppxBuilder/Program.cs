using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;


namespace IaAppxBuilder
{
    class Program
    {

#if (_WIN64)
        const string sIaUwpAppxDll = "IaUwpAppx64.dll";
#else
        const string sIaUwpAppxDll = "IaUwpAppx.dll";
#endif

        [DllImport(sIaUwpAppxDll, CharSet = CharSet.Unicode)]
        public static extern uint UAPPX_Initialize(string lpPackagename, string lpPath, bool b64BitPackage);

        [DllImport(sIaUwpAppxDll)]
        public static extern void UAPPX_Free();

        [DllImport(sIaUwpAppxDll, CharSet = CharSet.Unicode)]
        public static extern uint UAPPX_SetDisplayName(string lpwValue);

        [DllImport(sIaUwpAppxDll, CharSet = CharSet.Unicode)]
        public static extern uint UAPPX_SetLanguage(string lpwValue);

        [DllImport(sIaUwpAppxDll, CharSet = CharSet.Unicode)]
        public static extern uint UAPPX_SetPublisher(string lpwValue);

        [DllImport(sIaUwpAppxDll, CharSet = CharSet.Unicode)]
        public static extern uint UAPPX_SetPublisherCertificate(string lpwCertificate, string lpwPassword, string lpwTimestamp);

        [DllImport(sIaUwpAppxDll, CharSet = CharSet.Unicode)]
        public static extern uint UAPPX_SetVersion(string lpwValue);

        [DllImport(sIaUwpAppxDll, CharSet = CharSet.Unicode)]
        public static extern uint UAPPX_SetVirtualRegistryKey(UIntPtr hKey, string lpSubKey, Int32 dwType, string lpValueName, IntPtr lpValue, Int32 dwSize, bool bWin32);

        [DllImport(sIaUwpAppxDll, CharSet = CharSet.Unicode)]
        public static extern uint UAPPX_SetPackageFile(string lpwSource, string lpwTarget, out IntPtr pFileIdx);

        [DllImport(sIaUwpAppxDll, CharSet = CharSet.Unicode)]
        public static extern uint UAPPX_SetPackageDirectory(string lpwTarget, out IntPtr pFileIdx);

        [DllImport(sIaUwpAppxDll, CharSet = CharSet.Unicode)]
        public static extern uint UAPPX_SetFileAssociation(uint fileIdx, string lpwProgId, string lpwDescription, ref string[] lpwExtensions, int extSize, string lpwIconfile, int iconIdx);

        [DllImport(sIaUwpAppxDll, CharSet = CharSet.Unicode)]
        public static extern uint UAPPX_SetShortcut(uint fileIdx, string lpwName, string lpwDescription, string lpwPath, string lpwCommandline, string lpwIconfile, int iconIdx);

        [DllImport(sIaUwpAppxDll)]
        public static extern uint UAPPX_Finalize();

        static void WriteToConsole(string message)
        {
            Console.Write(message);
            Console.WriteLine();
        }
        static void Main(string[] args)
        {
            string sError = "";
            uint uAppXResult = 0;

            bool b64BitPackage = false;

            string sPackageName = "ApplicationX";
            string sPackageOutputName = "ApplicationX";
            string sPackageOutDirectory = "C:\\Users\\Public\\Documents\\IaAppxBuilder";
            string sManufacturer = "InstallAware";
            string sPackageVersion = "1.0.0.0";

            string sAuthenticodeCert = ""; //Path to certificate file
            string sAuthenticodePWD = "";
            string sAuthenticodeTimestampURL = "";

            // full path of the source EXE file eg. c:\\mypath\\WindowsFormsApplication.exe
            string sSourcePath = "...fill with a full path to source file...";
            string sTargetPath = "C:\\Program Files (x86)\\" + sPackageName + "\\WindowsFormsApplication.exe";

            string sShortcutName = "ApplicationX";
            string sShortcutDescription = "Appx Test package";

            try
            {
                WriteToConsole("Library initialization...");

                // Intialize APPX package
                uAppXResult = UAPPX_Initialize(sPackageOutputName, sPackageOutDirectory, b64BitPackage);
                if (0 != uAppXResult)
                {
                    sError = "Failed to initialize APPX package!\n\nError code: " + uAppXResult.ToString();
                    throw new Exception(sError);
                }

                uAppXResult = UAPPX_SetDisplayName(sPackageName);
                if (0 != uAppXResult)
                {
                    sError = "Cannot create APPX package!\n\nProduct Name might be not valid or APPX package not initialized!\nError code: " + uAppXResult.ToString();
                    throw new Exception(sError);
                }

                uAppXResult = UAPPX_SetLanguage("en-us");
                if (0 != uAppXResult)
                {
                    sError = "Cannot create APPX package!\n\nLanguage identifier might be not valid or APPX package not initialized!\nError code: " + uAppXResult.ToString();
                    throw new Exception(sError);
                }

                if (!string.IsNullOrEmpty(sAuthenticodeCert))
                    UAPPX_SetPublisherCertificate(sAuthenticodeCert, sAuthenticodePWD, sAuthenticodeTimestampURL);

                uAppXResult = UAPPX_SetPublisher(sManufacturer);
                if (0 != uAppXResult)
                {
                    sError = "Cannot create APPX package!\n\nPublisher Name might be not valid or APPX package not initialized!\nError code: " + uAppXResult.ToString();
                    throw new Exception(sError);
                }

                uAppXResult = UAPPX_SetVersion(sPackageVersion);
                if (0 != uAppXResult)
                {
                    sError = "Cannot create APPX package!\n\nProduct version might be not valid or APPX package not initialized!\nError code: " + uAppXResult.ToString();
                    throw new Exception(sError);
                }

                // Add File to package container
                IntPtr uPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(uint)));
                uAppXResult = UAPPX_SetPackageFile(sSourcePath, sTargetPath, out uPtr);
                uint uFileidx = (uint)uPtr;
                Marshal.FreeHGlobal(uPtr);

                if (0 != uAppXResult)
                {
                    if (uAppXResult == 160)
                        sError = "Cannot create APPX package!\n\nFailed to add file:\n" + sSourcePath + "\nFile not found!";
                    else
                        sError = "Cannot create APPX package!\n\nFailed to add file:\n" + sSourcePath + "\nError code: " + uAppXResult.ToString();

                    throw new Exception(sError);
                }

                uAppXResult = UAPPX_SetShortcut(uFileidx, sShortcutName, sShortcutDescription, null, null, null, 0);
                if (0 != uAppXResult)
                {
                    sError = "Cannot create APPX package!\n\nFailed to define shortuct:\n" + sSourcePath + "\nError code: " + uAppXResult.ToString();
                    throw new Exception(sError);
                }

                WriteToConsole("Generating package: " + sPackageOutDirectory + "\\" + sPackageOutputName + ".appx");

                // Process package
                uAppXResult = UAPPX_Finalize();
                if (0 != uAppXResult)
                {
                    if (uAppXResult == 185)
                        sError = "Cannot create APPX package!\n\nAt least one shortcut has to be defined!\nError code: " + uAppXResult.ToString();
                    else
                        sError = "Failed to create APPX package!\n\nError code: " + uAppXResult.ToString();

                    throw new Exception(sError);
                }

            }
            catch (Exception ex)
            {
                WriteToConsole(ex.Message);
            }

            UAPPX_Free();

            if(uAppXResult == 0)
                WriteToConsole("Process completed!");
        }
    }
}
