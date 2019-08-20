#include "Interface/Login/LoginDlg.h"
#include "Interface/Main/MainDlg.h"

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	// program entry point
	InitCommonControls();

	char szUserName[512] = { NULL };
	char szPassWord[512] = { NULL };

	// read login credentials from auth file
	GetPrivateProfileString(VMProtectDecryptString("RhinoCheats"), VMProtectDecryptString("USER"), "", szUserName, sizeof(szUserName), VMProtectDecryptString("C:\\RC\\Authorization.ini"));
	GetPrivateProfileString(VMProtectDecryptString("RhinoCheats"), VMProtectDecryptString("PASS"), "", szPassWord, sizeof(szPassWord), VMProtectDecryptString("C:\\RC\\Authorization.ini"));

	// construct login dialog
	cLoginDlg LoginDlg(hInstance);

	// check for empty login credentials
	if (!strcmp(szUserName, "") || !strcmp(szPassWord, ""))
	{
		// run the login dialog instance
		LoginDlg.RunModeless();

		// user most likely pressed cancel
		if (LoginDlg._loginData.empty() || LoginDlg._info_from_server.empty())
			exit(EXIT_FAILURE);
	}

	// check if stored credentials are valid
	else if (!LoginDlg.ValidateClientInfo(szUserName, szPassWord))
	{
		// run the login dialog instance
		LoginDlg.RunModeless();

		// user most likely pressed cancel
		if (LoginDlg._loginData.empty() || LoginDlg._info_from_server.empty())
			exit(EXIT_FAILURE);
	}

	// construct main dialog
	cMainDlg MainDlg(hInstance, LoginDlg._loginData, LoginDlg._info_from_server);

	// run the main dialog instance
	return (INT)MainDlg.RunModeless(NULL, IDR_ACCELERATOR);	
}